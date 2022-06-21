//
// Created by karl on 31.03.22.
//

#include "recordDeclarationMapper.hpp"

#include "transform/dependencies/declRequest.hpp"
#include "transform/dependencies/typeRequest.hpp"
#include "transform/name/typeNameTransformer.hpp"
#include "transform/writers/textWriter.hpp"
#include "transform/writers/typedefWriter.hpp"

using mapr::transform::RecordDeclarationMapper;
using mapr::view::RecordType;

RecordDeclarationMapper::RecordDeclarationMapper(
	std::shared_ptr<const view::RecordDecl> decl,
	std::shared_ptr<config::PipelineContext> context)
	: context(std::move(context))
	, decl(std::move(decl)) {}

void RecordDeclarationMapper::write(WriterStream& writer) {
	auto type = RecordType::makePointer(decl->getQualifiedName())->getPointee();

	auto declName = TypeNameTransformer(context).getTypeName(type);

	writer << TypedefWriter(std::make_unique<TextWriter>("struct " + declName),
	                        std::make_unique<TextWriter>(declName));
}

auto RecordDeclarationMapper::checkDependencies() const
	-> std::vector<std::shared_ptr<DependencyRequest>> {
	auto request = std::vector<std::shared_ptr<DependencyRequest>>();

	std::transform(std::begin(decl->getMemberIds()),
	               std::end(decl->getMemberIds()),
	               std::back_inserter(request),
	               [](const std::string& id) {
					   auto request = std::make_shared<DeclRequest>(id);
					   request->setRetention(DependencyRetention::Deferred);
					   return request;
				   });

	return request;
}
