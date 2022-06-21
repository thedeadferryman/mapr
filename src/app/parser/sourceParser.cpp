//
// Created by karl on 22.03.22.
//

#include "sourceParser.hpp"

#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/CompilerInvocation.h>

#include "match/matchers/enumMatcher.hpp"
#include "match/matchers/functionMatcher.hpp"
#include "match/matchers/operatorMatcher.hpp"
#include "match/matchers/recordMatcher.hpp"
#include "match/matchers/templateInstantiationMatcher.hpp"

using clang::CompilerInstance;
using clang::CompilerInvocation;
using clang::tooling::newFrontendActionFactory;
using mapr::app::SourceParser;

[[nodiscard]] auto createInvocation(const std::vector<const char*>& args)
	-> std::unique_ptr<CompilerInvocation>;

[[nodiscard]] auto allocInstance() -> std::shared_ptr<CompilerInstance>;

[[nodiscard]] auto createDiagnostics()
	-> llvm::IntrusiveRefCntPtr<clang::DiagnosticsEngine>;

SourceParser::SourceParser(config::ToolProperties properties,
                           const std::vector<const char*>& args)
	: config(std::move(properties))
	, declContext(std::make_shared<view::DeclContext>())
	, compiler(allocInstance()) {
	auto invocation = createInvocation(args);
	auto* fileManager = new clang::FileManager(invocation->getFileSystemOpts());

	compiler->setInvocation(std::move(invocation));
	compiler->setFileManager(fileManager);
	compiler->setDiagnostics(createDiagnostics().get());
}

#define ADD_MATCHER_WITH_ARGS(_Class, ...) \
	matchers.push_back(std::make_shared<_Class>(__VA_ARGS__))
#define ADD_MATCHER(_Class) ADD_MATCHER_WITH_ARGS(_Class, declContext)

void SourceParser::initialize() {
	ADD_MATCHER(match::FunctionMatcher);
	ADD_MATCHER(match::EnumMatcher);
	ADD_MATCHER(match::RecordMatcher);
	ADD_MATCHER(match::OperatorMatcher);
	ADD_MATCHER_WITH_ARGS(match::TemplateInstantiationMatcher,  //
	                      declContext,
	                      compiler);

	for (const auto& matcher : matchers) {
		matcher->bind(&matchFinder);
	}
}

auto SourceParser::parseAll() -> bool {
	auto actionFactory = newFrontendActionFactory(&matchFinder);

	auto action = actionFactory->create();

	return compiler->ExecuteAction(*action);
}

auto SourceParser::getParsedContext() const
	-> std::shared_ptr<view::DeclContext> {
	return declContext;
}

auto createInvocation(const std::vector<const char*>& args)
	-> std::unique_ptr<CompilerInvocation> {
	auto argsAry = llvm::makeArrayRef(args);

	return clang::createInvocationFromCommandLine(argsAry);
}

auto allocInstance() -> std::shared_ptr<CompilerInstance> {
	auto pch = std::make_shared<clang::PCHContainerOperations>();

	return std::make_shared<CompilerInstance>(pch);
}

auto createDiagnostics() -> llvm::IntrusiveRefCntPtr<clang::DiagnosticsEngine> {
	auto* diagOpts = new clang::DiagnosticOptions();

	return clang::CompilerInstance::createDiagnostics(diagOpts);
}
