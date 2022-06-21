///
/// Credited from the original Clang libTooling code:
/// (https://clang.llvm.org/doxygen/CommonOptionsParser_8cpp_source.html)
///
/// The changes made to the original code consist of changing the class name
/// and removing the cl::HideUnrelatedOptions invocation on line 107 of the original file.

#include "cliOptionsParser.hpp"

#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>
#include <llvm/Support/CommandLine.h>

using clang::tooling::ArgumentInsertPosition;
using clang::tooling::ArgumentsAdjustingCompilations;
using clang::tooling::combineAdjusters;
using clang::tooling::CompilationDatabase;
using clang::tooling::FixedCompilationDatabase;

using llvm::Twine;

using mapr::app::CliOptionsParser;

namespace cl = llvm::cl;

const char* const CliOptionsParser::HelpMessage =
	"\n"
	"-p <build-path> is used to read a compile command database.\n"
	"\n"
	"\tFor example, it can be a CMake build directory in which a file named\n"
	"\tcompile_commands.json exists (use -DCMAKE_EXPORT_COMPILE_COMMANDS=ON\n"
	"\tCMake option to get this output). When no build path is specified,\n"
	"\ta search for compile_commands.json will be attempted through all\n"
	"\tparent paths of the first input file . See:\n"
	"\thttps://clang.llvm.org/docs/HowToSetupToolingForLLVM.html for an\n"
	"\texample of setting up Clang Tooling on a source tree.\n"
	"\n"
	"<source0> ... specify the paths of source files. These paths are\n"
	"\tlooked up in the compile command database. If the path of a file is\n"
	"\tabsolute, it needs to point into CMake's source tree. If the path is\n"
	"\trelative, the current working directory needs to be in the CMake\n"
	"\tsource tree and the file must be in a subdirectory of the current\n"
	"\tworking directory. \"./\" prefixes in the relative files will be\n"
	"\tautomatically removed, but the rest of a relative path must be a\n"
	"\tsuffix of a path in the compile command database.\n"
	"\n";

llvm::Error CliOptionsParser::init(int& argc,
                                   const char** argv,
                                   cl::OptionCategory& Category,
                                   llvm::cl::NumOccurrencesFlag OccurrencesFlag,
                                   const char* Overview) {
	static cl::opt<std::string> BuildPath("p",
	                                      cl::desc("Build path"),
	                                      cl::Optional,
	                                      cl::cat(Category),
	                                      cl::sub(*cl::AllSubCommands));

	static cl::list<std::string> SourcePaths(
		cl::Positional,
		cl::desc("<source0> [... <sourceN>]"),
		OccurrencesFlag,
		cl::cat(Category),
		cl::sub(*cl::AllSubCommands));

	static cl::list<std::string> ArgsAfter(
		"extra-arg",
		cl::desc("Additional argument to append to the compiler command line"),
		cl::cat(Category),
		cl::sub(*cl::AllSubCommands));

	static cl::list<std::string> ArgsBefore(
		"extra-arg-before",
		cl::desc("Additional argument to prepend to the compiler command line"),
		cl::cat(Category),
		cl::sub(*cl::AllSubCommands));

	cl::ResetAllOptionOccurrences();

	std::string ErrorMessage;
	Compilations =
		FixedCompilationDatabase::loadFromCommandLine(argc, argv, ErrorMessage);
	if (!ErrorMessage.empty())
		ErrorMessage.append("\n");
	llvm::raw_string_ostream OS(ErrorMessage);
	// Stop initializing if command-line option parsing failed.
	if (!cl::ParseCommandLineOptions(argc, argv, Overview, &OS)) {
		OS.flush();
		return llvm::make_error<llvm::StringError>(
			ErrorMessage, llvm::inconvertibleErrorCode());
	}

	cl::PrintOptionValues();

	SourcePathList = SourcePaths;
	if ((OccurrencesFlag == cl::ZeroOrMore || OccurrencesFlag == cl::Optional)
	    && SourcePathList.empty())
		return llvm::Error::success();
	if (!Compilations) {
		if (!BuildPath.empty()) {
			Compilations = CompilationDatabase::autoDetectFromDirectory(
				BuildPath, ErrorMessage);
		} else {
			Compilations = CompilationDatabase::autoDetectFromSource(
				SourcePaths[0], ErrorMessage);
		}
		if (!Compilations) {
			llvm::errs()
				<< "Error while trying to load a compilation database:\n"
				<< ErrorMessage << "Running without flags.\n";
			Compilations.reset(
				new FixedCompilationDatabase(".", std::vector<std::string>()));
		}
	}
	auto AdjustingCompilations =
		std::make_unique<ArgumentsAdjustingCompilations>(
			std::move(Compilations));
	Adjuster =
		getInsertArgumentAdjuster(ArgsBefore, ArgumentInsertPosition::BEGIN);
	Adjuster = combineAdjusters(
		std::move(Adjuster),
		getInsertArgumentAdjuster(ArgsAfter, ArgumentInsertPosition::END));
	AdjustingCompilations->appendArgumentsAdjuster(Adjuster);
	Compilations = std::move(AdjustingCompilations);
	return llvm::Error::success();
}

llvm::Expected<CliOptionsParser> CliOptionsParser::create(
	int& argc,
	const char** argv,
	llvm::cl::OptionCategory& Category,
	llvm::cl::NumOccurrencesFlag OccurrencesFlag,
	const char* Overview) {
	CliOptionsParser Parser;
	llvm::Error Err =
		Parser.init(argc, argv, Category, OccurrencesFlag, Overview);
	if (Err)
		return std::move(Err);
	return std::move(Parser);
}

CliOptionsParser::CliOptionsParser(int& argc,
                                   const char** argv,
                                   cl::OptionCategory& Category,
                                   llvm::cl::NumOccurrencesFlag OccurrencesFlag,
                                   const char* Overview) {
	llvm::Error Err = init(argc, argv, Category, OccurrencesFlag, Overview);
	if (Err) {
		llvm::report_fatal_error(
			Twine(
				"CommonOptionsParser: failed to parse command-line arguments. ")
			+ llvm::toString(std::move(Err)));
	}
}
