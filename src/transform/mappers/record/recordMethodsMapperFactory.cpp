//
// Created by karl on 31.03.22.
//

#include "recordMethodsMapperFactory.hpp"

#include "view/record/recordCtorDecl.hpp"
#include "view/record/recordDtorDecl.hpp"
#include "view/record/recordMethodDecl.hpp"
#include "view/record/syntheticGetterDecl.hpp"
#include "view/record/syntheticSetterDecl.hpp"

#include "util/matchType.hpp"

#include "transform/mappers/record/recordCtorDefinitionMapper.hpp"
#include "transform/mappers/record/recordDtorDefinitionMapper.hpp"
#include "transform/mappers/record/recordMethodDefinitionMapper.hpp"
#include "transform/mappers/record/syntheticGetterDefinitionMapper.hpp"
#include "transform/mappers/record/syntheticSetterDefinitionMapper.hpp"

using mapr::transform::RecordMethodsMapperFactory;
using mapr::util::MatchType;
using mapr::view::DeclType;

constexpr auto AcceptedTypes = {
	DeclType::RecordMethod,
	DeclType::RecordCtor,
	DeclType::RecordDtor,
	DeclType::RecordOperatorOverload,
	DeclType::SyntheticGetter,
	DeclType::SyntheticSetter,
};

RecordMethodsMapperFactory::RecordMethodsMapperFactory(
	MapperFactoryMode mode, std::shared_ptr<config::PipelineContext> context)
	: context(std::move(context))
	, mode(mode) {}

auto RecordMethodsMapperFactory::acceptsDecl(
	const std::shared_ptr<const view::DeclBase>& decl) const -> bool {
	if (mode == MapperFactoryMode::Declaration) {
		return false;
	}

	return std::find(  //
			   std::begin(AcceptedTypes),
			   std::end(AcceptedTypes),
			   decl->getDeclType()  //
			   )
		!= std::end(AcceptedTypes);
}

auto RecordMethodsMapperFactory::mapperForDecl(
	std::shared_ptr<const view::DeclBase> decl) const -> std::unique_ptr<MapperBase> {
	return MatchType::matchType<std::unique_ptr<MapperBase>>(  //
		std::move(decl),
		[this](const std::shared_ptr<const view::RecordMethodDecl>& methodDecl)
			-> std::unique_ptr<MapperBase> {
			return std::make_unique<RecordMethodDefinitionMapper>(methodDecl,
		                                                          context);
		},
		[this](const std::shared_ptr<const view::RecordCtorDecl>& methodDecl)
			-> std::unique_ptr<MapperBase> {
			return std::make_unique<RecordCtorDefinitionMapper>(methodDecl,
		                                                        context);
		},
		[this](const std::shared_ptr<const view::RecordDtorDecl>& methodDecl)
			-> std::unique_ptr<MapperBase> {
			return std::make_unique<RecordDtorDefinitionMapper>(methodDecl,
		                                                        context);
		},
		[this](const std::shared_ptr<const view::SyntheticGetterDecl>& getterDecl)
			-> std::unique_ptr<MapperBase> {
			return std::make_unique<SyntheticGetterDefinitionMapper>(getterDecl,
		                                                             context);
		},
		[this](const std::shared_ptr<const view::SyntheticSetterDecl>& setterDecl)
			-> std::unique_ptr<MapperBase> {
			return std::make_unique<SyntheticSetterDefinitionMapper>(setterDecl,
		                                                             context);
		},
		[](const std::shared_ptr<const view::DeclBase>& /* unused */) {  // Fallback
			return std::unique_ptr<MapperBase>(nullptr);
		});
}
