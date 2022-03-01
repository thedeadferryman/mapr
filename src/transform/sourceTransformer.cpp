//
// Created by kmeinkopf on 19.01.2022.
//

#include <utility>

#include "sourceTransformer.hpp"

#include "transform/exceptions/undefinedDeclException.hpp"

#include "util/matchType.hpp"

using kodgen::transform::SourceTransformer;
using kodgen::util::MatchType;

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

	mapper->write(writerStream);

	writtenDecls[declId] = true;
}

auto SourceTransformer::resolveDependency(
	std::shared_ptr<DependencyRequest> dependencyId) const
	-> std::shared_ptr<view::DeclBase> {
	return dependencyResolver.resolve(std::move(dependencyId));
}

auto SourceTransformer::mapperForDecl(std::shared_ptr<view::DeclBase> decl)
	-> std::unique_ptr<MapperBase> {
	for (const auto& factory : factories) {
		if (factory->acceptsDecl(decl)) {
			return factory->mapperForDecl(decl);
		}
	}

	return nullptr;
}

void kodgen::transform::SourceTransformer::registerMapper(
	std::shared_ptr<MapperFactoryBase> factory) {
	factories.emplace_back(std::move(factory));
}