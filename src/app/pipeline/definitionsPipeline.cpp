//
// Created by karl on 22.03.22.
//

#include "definitionsPipeline.hpp"

#include "transform/aux/includeFile.hpp"

#include "transform/mappers/aux/auxMapperFactory.hpp"
#include "transform/mappers/aux/typeMapperFactory.hpp"
#include "transform/mappers/function/functionMapperFactory.hpp"
#include "transform/mappers/record/recordMapperFactory.hpp"
#include "transform/mappers/record/recordMethodsMapperFactory.hpp"

using mapr::app::DefinitionsPipeline;

using mapr::transform::AuxMapperFactory;
using mapr::transform::FunctionMapperFactory;
using mapr::transform::MapperFactoryMode;
using mapr::transform::RecordMapperFactory;
using mapr::transform::RecordMethodsMapperFactory;
using mapr::transform::TypeMapperFactory;

using mapr::transform::IncludeFile;

DefinitionsPipeline::DefinitionsPipeline(
	std::shared_ptr<config::PipelineContext> context,
	std::shared_ptr<transform::SourceTransformer> transformer)
	: PipelineBase(
		std::move(context), std::move(transformer), "DefinitionsPipeline") {}

void DefinitionsPipeline::initialize() {
	auto transformer = getTransformer();

	transformer->registerMapper(  //
		std::make_shared<AuxMapperFactory>(getContext())  //
	);
	transformer->registerMapper(  //
		std::make_shared<TypeMapperFactory>(  //
			getContext(),
			MapperFactoryMode::Definition  //
			)  //
	);
	transformer->registerMapper(
		std::make_shared<RecordMethodsMapperFactory>(  //
			MapperFactoryMode::Definition,
			getContext()));
	transformer->registerMapper(  //
		std::make_shared<FunctionMapperFactory>(  //
			MapperFactoryMode::Definition,
			getContext()  //
			)  //
	);

	transformer->registerMapper(  //
		std::make_shared<RecordMapperFactory>(  //
			MapperFactoryMode::Definition,
			getContext())  //
	);
}

void DefinitionsPipeline::execute(
	const std::shared_ptr<view::DeclContext>& declContext) {
	getTransformer()->writeDecl(
		std::make_shared<IncludeFile>(getContext()->getHeaderName()),
		getName());
	getTransformer()->writeDecl(
		std::make_shared<IncludeFile>(getContext()->getInputFileRelative()),
		getName());

	PipelineBase::execute(declContext);
}
