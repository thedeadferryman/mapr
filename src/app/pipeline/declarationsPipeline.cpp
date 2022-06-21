//
// Created by karl on 22.03.22.
//

#include "declarationsPipeline.hpp"

#include "transform/aux/includeFile.hpp"
#include "transform/sourceTransformer.hpp"

#include "transform/mappers/aux/auxMapperFactory.hpp"
#include "transform/mappers/aux/typeMapperFactory.hpp"
#include "transform/mappers/enum/enumMapperFactory.hpp"
#include "transform/mappers/function/functionMapperFactory.hpp"
#include "transform/mappers/record/recordMapperFactory.hpp"

using mapr::app::DeclarationsPipeline;

using mapr::transform::AuxMapperFactory;
using mapr::transform::EnumMapperFactory;
using mapr::transform::FunctionMapperFactory;
using mapr::transform::MapperFactoryMode;
using mapr::transform::RecordMapperFactory;
using mapr::transform::SourceTransformer;
using mapr::transform::TypeMapperFactory;

using mapr::transform::IncludeFile;

DeclarationsPipeline::DeclarationsPipeline(
	std::shared_ptr<config::PipelineContext> context,
	std::shared_ptr<SourceTransformer> transformer)
	: PipelineBase(
		std::move(context), std::move(transformer), "DeclarationsPipeline") {}

void DeclarationsPipeline::initialize() {
	auto transformer = getTransformer();

	transformer->registerMapper(  //
		std::make_shared<AuxMapperFactory>(getContext())  //
	);
	transformer->registerMapper(  //
		std::make_shared<FunctionMapperFactory>(  //
			MapperFactoryMode::Declaration,
			getContext())  //
	);
	transformer->registerMapper(  //
		std::make_shared<EnumMapperFactory>(getContext())  //
	);
	transformer->registerMapper(  //
		std::make_shared<TypeMapperFactory>(  //
			getContext(),
			MapperFactoryMode::Declaration  //
			)  //
	);
	transformer->registerMapper(  //
		std::make_shared<RecordMapperFactory>(  //
			MapperFactoryMode::Declaration,
			getContext())  //
	);
}
