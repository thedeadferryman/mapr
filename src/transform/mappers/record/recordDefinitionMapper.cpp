//
// Created by karl on 14.05.22.
//

#include "recordDefinitionMapper.hpp"

#include "transform/dependencies/declRequest.hpp"
#include "transform/writers/textWriter.hpp"

using mapr::transform::RecordDefinitionMapper;

RecordDefinitionMapper::RecordDefinitionMapper(
	std::shared_ptr<const view::RecordDecl> decl)
	: decl(std::move(decl)) {}

void RecordDefinitionMapper::write(WriterStream& writer) {
	writer << TextWriter("/* class ")
		   << TextWriter(decl->getQualifiedName().str()) //
		   << TextWriter(" */\n");
}

auto RecordDefinitionMapper::checkDependencies() const
	-> std::vector<std::shared_ptr<DependencyRequest>> {
	auto request = std::vector<std::shared_ptr<DependencyRequest>>();

	auto memberIds = decl->getMemberIds();

	std::transform(std::begin(memberIds),
	               std::end(memberIds),
	               std::back_inserter(request),
	               [](const std::string& id) {
					   auto request = std::make_shared<DeclRequest>(id);
					   request->setRetention(DependencyRetention::Deferred);
					   return request;
				   });

	return request;
}
