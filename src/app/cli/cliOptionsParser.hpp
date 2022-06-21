///
/// Credited from the original Clang libTooling code: (https://clang.llvm.org/doxygen/CommonOptionsParser_8h_source.html)
///

#pragma once

#include <clang/Tooling/ArgumentsAdjusters.h>
#include <clang/Tooling/CompilationDatabase.h>
#include <llvm/Support/CommandLine.h>
#include <llvm/Support/Error.h>

namespace mapr::app {

class CliOptionsParser {
  protected:
	CliOptionsParser(
		int& argc,
		const char** argv,
		llvm::cl::OptionCategory& Category,
		llvm::cl::NumOccurrencesFlag OccurrencesFlag = llvm::cl::OneOrMore,
		const char* Overview = nullptr);

  public:
	static llvm::Expected<CliOptionsParser> create(
		int& argc,
		const char** argv,
		llvm::cl::OptionCategory& Category,
		llvm::cl::NumOccurrencesFlag OccurrencesFlag = llvm::cl::OneOrMore,
		const char* Overview = nullptr);

	clang::tooling::CompilationDatabase& getCompilations() { return *Compilations; }

	const std::vector<std::string>& getSourcePathList() const {
		return SourcePathList;
	}

	clang::tooling::ArgumentsAdjuster getArgumentsAdjuster() { return Adjuster; }

	static const char* const HelpMessage;

  private:
	CliOptionsParser() = default;

	llvm::Error init(int& argc,
	                 const char** argv,
	                 llvm::cl::OptionCategory& Category,
	                 llvm::cl::NumOccurrencesFlag OccurrencesFlag,
	                 const char* Overview);

	std::unique_ptr<clang::tooling::CompilationDatabase> Compilations;
	std::vector<std::string> SourcePathList;
	clang::tooling::ArgumentsAdjuster Adjuster;
};

}  // namespace mapr::app