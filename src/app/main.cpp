#include <concepts>

#include <clang/Frontend/CompilerInstance.h>

#include "app/cli/cliOptionsParser.hpp"
#include "app/cli/utils.hpp"
#include "app/parser/sourceParser.hpp"
#include "app/pipeline/declarationsPipeline.hpp"
#include "app/pipeline/definitionsPipeline.hpp"
#include "app/sanitizer/codeSanitizer.hpp"

#ifndef RESOURCES_DIR
#	define RESOURCES_DIR \
		(FS::path(argv[0]).parent_path() / ".." / "share" / "mapr" \
		 / "resources")
#endif

namespace cl = llvm::cl;
using mapr::app::CliOptionsParser;
using mapr::app::getConfigFile;

[[maybe_unused]] cl::OptionCategory parserOptions("Parser Options");
[[maybe_unused]] cl::OptionCategory toolOptions("mapr Options");

[[maybe_unused]] cl::extrahelp commonHelp(CliOptionsParser::HelpMessage);

[[maybe_unused]] cl::opt<std::string> configFile(
	"config-file",
	cl::desc("Configuration file to use"),
	cl::value_desc("filename"),
	cl::cat(toolOptions),
	cl::init(".maprrc.yml"));
[[maybe_unused]] cl::alias configFileShort(
	"c", cl::desc("Alias for '--config-file'"), cl::aliasopt(configFile));

using mapr::app::CodeSanitizer;
using mapr::app::DeclarationsPipeline;
using mapr::app::DefinitionsPipeline;
using mapr::app::PipelineBase;
using mapr::app::SourceParser;

using mapr::view::DeclContext;

using mapr::transform::DependencyResolver;
using mapr::transform::SourceTransformer;
using mapr::transform::WriterStream;

using mapr::util::ResourceLoader;

using mapr::config::PipelineContext;

namespace FS = std::filesystem;

template<class Pipeline>
	requires(std::derived_from<Pipeline, PipelineBase>)
void executePipeline(std::ostream& stream,
                     const std::shared_ptr<PipelineContext>& pipelineContext,
                     DependencyResolver& dependencyResolver,
                     std::shared_ptr<DeclContext> declContext) {
	auto wStream = WriterStream(stream);
	auto transformer =
		std::make_shared<SourceTransformer>(wStream, dependencyResolver);
	auto pipeline = Pipeline(pipelineContext, transformer);
	pipeline.initialize();
	pipeline.execute(declContext);
}

auto main(int32_t argc, const char** argv) -> int32_t {
	auto parserOr = CliOptionsParser::create(
		argc, argv, parserOptions, cl::NumOccurrencesFlag::Required);

	if (!parserOr) {
		auto err = parserOr.takeError();

		llvm::errs() << err << "\n";
		return errorToErrorCode(std::move(err)).value();
	}

	auto inputFile = parserOr->getSourcePathList().front();
	auto config = getConfigFile(configFile.c_str(), inputFile);
	auto pipelineContext = std::make_shared<PipelineContext>(config, inputFile);
	auto command =
		parserOr->getCompilations().getCompileCommands(inputFile).front();

	auto adjustedArgs = std::vector<const char*>();

	std::transform(
		std::begin(command.CommandLine) + 1,  // skip the program name
		std::end(command.CommandLine),
		std::back_inserter(adjustedArgs),
		[](const std::string& str) { return str.data(); });

	auto sourceParser = SourceParser(config, adjustedArgs);

	sourceParser.initialize();

	auto success = sourceParser.parseAll();

	if (!success) {
		llvm::errs() << "Failed to parse the source code!\n";
		return -1;
	}

	const auto& declContext = sourceParser.getParsedContext();

	auto resourceManager = std::make_shared<ResourceLoader>(  //
		FS::path(RESOURCES_DIR)  //
	);

	auto dependencyResolver = DependencyResolver(  //
		declContext,
		pipelineContext,
		resourceManager  //
	);

	auto headerOutputPath =
		FS::current_path() / config.outPath / pipelineContext->getHeaderName();
	auto implOutputPath =
		FS::current_path() / config.outPath / pipelineContext->getImplName();

	auto headerOutputStream = std::ofstream(headerOutputPath);
	auto implOutputStream = std::ofstream(implOutputPath);

	if (config.formatOptions.has_value()) {
		auto sanitizer = CodeSanitizer(config.formatOptions.value());

		auto headerRawStream = std::ostringstream();
		auto implRawStream = std::ostringstream();

		executePipeline<DeclarationsPipeline>(  //
			headerRawStream,
			pipelineContext,
			dependencyResolver,
			declContext  //
		);
		sanitizer.sanitize(headerRawStream.str(), headerOutputStream);

		executePipeline<DefinitionsPipeline>(  //
			implRawStream,
			pipelineContext,
			dependencyResolver,
			declContext  //
		);
		sanitizer.sanitize(implRawStream.str(), implOutputStream);
	} else {
		executePipeline<DeclarationsPipeline>(  //
			headerOutputStream,
			pipelineContext,
			dependencyResolver,
			declContext  //
		);

		executePipeline<DefinitionsPipeline>(  //
			implOutputStream,
			pipelineContext,
			dependencyResolver,
			declContext  //
		);
	}

	headerOutputStream.close();
	implOutputStream.close();

	return 0;
}
