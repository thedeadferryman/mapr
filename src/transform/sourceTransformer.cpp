//
// Created by kmeinkopf on 19.01.2022.
//

#include <utility>

#include "sourceTransformer.hpp"

#include <llvm/Support/raw_ostream.h>

#include "transform/exceptions/undefinedDeclException.hpp"

#include "util/matchType.hpp"

using mapr::transform::SourceTransformer;

SourceTransformer::SourceTransformer(WriterStream& wStream,
                                     DependencyResolver declResolver)
	: writerStream(wStream)
	, dependencyResolver(std::move(declResolver)) {}

void SourceTransformer::writeDecl(const std::shared_ptr<const view::DeclBase>& decl,
                                  std::string_view pipelineName) {
	auto declId = decl->getID();

	if (writtenDecls[declId]) {
		return;
	}

	auto mapper = mapperForDecl(decl);

	if (!mapper) {
		DEBUG {
			llvm::errs() << "warn: declaration of type '"
						 << view::stringOfDeclType(decl->getDeclType())
						 << "' falls down in pipeline '" << pipelineName
						 << "'\n";
			return;
		}
	}

	auto deferredDependencies =
		std::vector<std::shared_ptr<DependencyRequest>>();

	for (const auto& request : mapper->checkDependencies()) {
		if (request->getRetention() == DependencyRetention::Deferred) {
			deferredDependencies.push_back(request);
		} else {
			writeDependency(request, pipelineName);
		}
	}

	mapper->write(writerStream);

	writtenDecls[declId] = true;

	for (const auto& request : deferredDependencies) {
		writeDependency(request, pipelineName);
	}
}
void SourceTransformer::writeDependency(
	const std::shared_ptr<DependencyRequest>& request,
	std::string_view& pipelineName) {
	auto dependency = resolveDependency(request);

	if (!dependency) {
		throw UndefinedDeclException::fromRequest(request);
	}

	if (!writtenDecls[dependency->getID()]) {
		writeDecl(dependency, pipelineName);
	}
}

auto SourceTransformer::resolveDependency(
	std::shared_ptr<DependencyRequest> dependencyId) const
	-> std::shared_ptr<const view::DeclBase> {
	return dependencyResolver.resolve(std::move(dependencyId));
}

auto SourceTransformer::mapperForDecl(
	std::shared_ptr<const view::DeclBase> decl)
	-> std::unique_ptr<MapperBase> {
	for (const auto& factory : factories) {
		if (factory->acceptsDecl(decl)) {
			return factory->mapperForDecl(decl);
		}
	}

	return nullptr;
}

void mapr::transform::SourceTransformer::registerMapper(
	std::shared_ptr<MapperFactoryBase> factory) {
	factories.emplace_back(std::move(factory));
}