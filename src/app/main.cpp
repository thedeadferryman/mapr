#include <filesystem>
#include <iostream>
#include <string>

#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/Format/Format.h>
#include <clang/Frontend/FrontendActions.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>
#include <llvm/Support/CommandLine.h>

#include <boost/algorithm/string.hpp>

#include "match/matchers/classMatcher.hpp"
#include "match/matchers/enumMatcher.hpp"
#include "match/matchers/functionMatcher.hpp"
#include "match/matchers/structMatcher.hpp"
#include "match/matchers/typedefMatcher.hpp"

#include "view/declContext.hpp"
#include "view/function/functionDecl.hpp"
#include "view/function/functionOverload.hpp"
#include "view/types/aliasType.hpp"

#include "transform/aux/templateFile.hpp"
#include "transform/dependencies/dependencyResolver.hpp"
#include "transform/sourceTransformer.hpp"
#include "transform/writerStream.hpp"

#include "transform/mappers/aux/auxMapperFactory.hpp"
#include "transform/mappers/enum/enumMapperFactory.hpp"
#include "transform/mappers/function/functionDeclarationMapperFactory.hpp"

#include "util/resourceLoader.hpp"

namespace FS = std::filesystem;

using std::int32_t;

using llvm::cl::NumOccurrencesFlag;
using llvm::cl::OptionCategory;

using clang::tooling::ClangTool;
using clang::tooling::CommonOptionsParser;
using clang::tooling::newFrontendActionFactory;

using clang::ast_matchers::MatchFinder;

using kodgen::match::ClassMatcher;
using kodgen::match::EnumMatcher;
using kodgen::match::FunctionMatcher;
using kodgen::match::MatcherBase;
using kodgen::match::StructMatcher;
using kodgen::match::TypedefMatcher;

using kodgen::view::AliasType;
using kodgen::view::DeclContext;
using kodgen::view::DeclType;
using kodgen::view::FunctionDecl;
using kodgen::view::FunctionOverload;
using kodgen::view::VarDecl;

using kodgen::transform::DependencyResolver;
using kodgen::transform::SourceTransformer;
using kodgen::transform::TemplateFile;
using kodgen::transform::WriterStream;

using kodgen::transform::AuxMapperFactory;
using kodgen::transform::EnumMapperFactory;
using kodgen::transform::FunctionDeclarationMapperFactory;
using kodgen::transform::MapperFactoryMode;

using kodgen::util::ResourceLoader;

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

	auto ctx = std::make_shared<DeclContext>();
	auto matchFinder = MatchFinder();

	auto functionMatcher = FunctionMatcher(ctx);
	auto enumMatcher = EnumMatcher(ctx);
	auto classMatcher = ClassMatcher(ctx);

	functionMatcher.bind(&matchFinder);
	enumMatcher.bind(&matchFinder);
	classMatcher.bind(&matchFinder);

	auto runResult = tool.run(newFrontendActionFactory(&matchFinder).get());

	auto sout = std::stringstream();

	auto writer = WriterStream(sout);

	auto resourceLoader = ResourceLoader(FS::current_path() / "resources");

	auto resolver = DependencyResolver(ctx, resourceLoader);

	auto transformer = SourceTransformer(writer, resolver);

	auto funcMapper = std::make_shared<FunctionDeclarationMapperFactory>(
		MapperFactoryMode::DECLARATION);
	auto auxMapper = std::make_shared<AuxMapperFactory>();
	auto enumMapper = std::make_shared<EnumMapperFactory>();

	transformer.registerMapper(funcMapper);
	transformer.registerMapper(auxMapper);
	transformer.registerMapper(enumMapper);

	for (auto [_, decl] : ctx->getDeclarations()) {
		transformer.writeDecl(decl);
	}

	auto code = sout.str();

	auto style = clang::format::getStyle(
		"file", (FS::current_path() / ".clang-format").string(), "LLVM");

	if (style) {
		auto range = clang::tooling::Range(0, code.length());
		auto replacements = clang::format::reformat(
			style.get(), sout.str(), llvm::ArrayRef(range));
		auto maybeCode =
			clang::tooling::applyAllReplacements(code, replacements);

		if (maybeCode) {
			std::cerr << "Formatted!" << std::endl;

			code = maybeCode.get();
		}
	}

	auto ofs = std::ofstream(FS::current_path() / "out" / "sample.h");

	ofs << code;

	ofs.close();

	return runResult;
}
