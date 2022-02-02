//
// Created by kmeinkopf on 19.01.2022.
//

#include <iostream>
#include <utility>

#include "sourceTransformer.hpp"

#include "view/enum/enumDecl.hpp"

#include "transform/aux/typeDecl.hpp"
#include "transform/exceptions/undefinedDeclException.hpp"
#include "transform/mappers.hpp"

#include "util/matchType.hpp"

using kodgen::transform::SourceTransformer;
using kodgen::util::MatchType;

#define REGISTER_MAPPER3(_DeclType, _DeclName, _Capture) \
	[_Capture](const std::shared_ptr<_DeclType>& _DeclName) \
		-> std::unique_ptr<MapperBase>

#define REGISTER_MAPPER2(_DeclType, _DeclName) \
	REGISTER_MAPPER3(_DeclType, _DeclName, )

#define REGISTER_MAPPER_VALUE(_DeclType, _Value) \
	REGISTER_MAPPER2(_DeclType, ) { return (_Value); }

#define REGISTER_MAPPER_INIT(_DeclType, _DeclName, _MapperType, _Initializer) \
	REGISTER_MAPPER2(_DeclType, _DeclName) { \
		return std::make_unique<_MapperType>(_Initializer); \
	}

#define REGISTER_MAPPER(_DeclType, _MapperType) \
	REGISTER_MAPPER_INIT( \
		_DeclType, _DeclName, _MapperType, std::move(_DeclName))

SourceTransformer::SourceTransformer(WriterStream& wStream,
                                     DependencyResolver declResolver)
	: writerStream(wStream)
	, dependencyResolver(std::move(declResolver)) {}

void SourceTransformer::writeDecl(std::shared_ptr<view::DeclBase> decl) {
	auto declId = decl->getID();

	if (writtenDecls[declId]) {
		return;
	}

	auto mapper = mapperForDecl(std::move(decl));

	if (!mapper) {
		return;
	}

	for (const auto& dependencyId : mapper->checkDependencies()) {
		auto dependency = resolveDependency(dependencyId);

		if (!dependency) {
			throw UndefinedDeclException::fromRequest(dependencyId);
		}

		if (!writtenDecls[dependency->getID()]) {
			writeDecl(dependency);
		}
	}

	mapper->writeDeclaration(writerStream);

	writtenDecls[declId] = true;
}

auto SourceTransformer::resolveDependency(
	std::shared_ptr<DependencyRequest> dependencyId) const
	-> std::shared_ptr<view::DeclBase> {
	return dependencyResolver.resolve(dependencyId);
}

auto SourceTransformer::mapperForDecl(std::shared_ptr<view::DeclBase> decl)
	-> std::unique_ptr<MapperBase> {
	return MatchType::matchType<std::unique_ptr<MapperBase>>(
		std::move(decl),  //
		REGISTER_MAPPER(view::FunctionDecl, FunctionMapper),
		REGISTER_MAPPER(transform::AuxDecl, AuxMapper),
		REGISTER_MAPPER(transform::TypeDecl, TypeMapper),
		REGISTER_MAPPER_VALUE(view::DeclBase, nullptr));
}
