#include <fstream>
#include <sstream>
#include <string>

#include <boost/algorithm/string.hpp>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/Frontend/FrontendActions.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>
#include <llvm/Support/CommandLine.h>

#include "match/matchContext.hpp"
#include "match/matchers/primitiveFunctionMatcher.hpp"
#include "view/function/primitiveFunction.hpp"

using std::int32_t;

using boost::algorithm::join;

using llvm::cl::NumOccurrencesFlag;
using llvm::cl::OptionCategory;

using clang::tooling::ClangTool;
using clang::tooling::CommonOptionsParser;
using clang::tooling::newFrontendActionFactory;

using clang::ast_matchers::MatchFinder;

using kodgen::match::MatchContext;
using kodgen::match::PrimitiveFunctionMatcher;

using kodgen::view::PrimitiveFunction;
using kodgen::view::PrimitiveType;
using kodgen::view::VarDecl;

auto main(int32_t argc, const char** argv) -> int32_t {
	static auto category = OptionCategory("kodgen");

	auto parserOr = CommonOptionsParser::create(
		argc, argv, category, NumOccurrencesFlag::Optional);

	if (!parserOr) {
		llvm::errs() << parserOr.takeError();
		return errorToErrorCode(parserOr.takeError()).value();
	}

	auto& parser = parserOr.get();

	auto tool = ClangTool(parser.getCompilations(), parser.getSourcePathList());

	auto ctx = std::make_shared<MatchContext>();
	auto matcher = MatchFinder();
	auto callback = PrimitiveFunctionMatcher(ctx);

	matcher.addMatcher(callback.getMatcher(), &callback);

	auto runResult = tool.run(newFrontendActionFactory(&matcher).get());

	auto out = std::ofstream("sample.c.cpp");

	for (const auto& decl : ctx->getDeclarations()) {
		auto* funcDecl = dynamic_cast<PrimitiveFunction*>(
			decl.get());  // Damn unsafe, but is for test, so ok

		if (funcDecl == nullptr) {
			continue;
		}

		auto parmsView = funcDecl->getArguments();
		auto parmDefs = std::vector<std::string>();

		std::transform(parmsView.begin(),
		               parmsView.end(),
		               std::back_inserter(parmDefs),
		               [](const VarDecl<PrimitiveType>& parm) {
						   auto ss2 = std::stringstream();
						   ss2 << parm.getType().getName() << ' '
							   << parm.getName();

						   return ss2.str();
					   });

		out << "\n\n"
			<< "extern \"C\" " << funcDecl->getReturnType().getName() << " CC_"
			<< funcDecl->getName() << "(" << join(parmDefs, ", ") << ")"
			<< ";\n\n";
	}

	out.close();

	return runResult;
}
