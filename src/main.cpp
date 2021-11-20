#include <string>

#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/Frontend/FrontendActions.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>
#include <llvm/Support/CommandLine.h>

#include "match/matchContext.hpp"
#include "match/matchers/primitiveFunctionMatcher.hpp"

using std::int32_t;

using llvm::cl::NumOccurrencesFlag;
using llvm::cl::OptionCategory;

using clang::tooling::ClangTool;
using clang::tooling::CommonOptionsParser;
using clang::tooling::newFrontendActionFactory;

using clang::ast_matchers::functionDecl;
using clang::ast_matchers::isExpansionInFileMatching;

using clang::ast_matchers::MatchFinder;

using clang::FunctionDecl;

using kodgen::match::MatchContext;
using kodgen::match::PrimitiveFunctionMatcher;

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

	for(auto decl : ctx->getDeclarations()) {
		llvm::errs() << decl.getName() << "\n";
	}

	return runResult;
}
