//
// Created by kmeinkopf on 02.02.2022.
//

#include "enumMapper.hpp"

#include "transform/name/qualifiedNameTransformer.hpp"
#include "transform/name/typeNameTransformer.hpp"
#include "transform/writers/assignmentWriter.hpp"
#include "transform/writers/enumWriter.hpp"
#include "transform/writers/externCWriter.hpp"
#include "transform/writers/textWriter.hpp"
#include "transform/writers/typedefWriter.hpp"

using mapr::transform::EnumMapper;

EnumMapper::EnumMapper(std::shared_ptr<const view::EnumDecl> enumDecl,
                       std::shared_ptr<config::PipelineContext> context)
	: enumDecl(std::move(enumDecl))
	, context(std::move(context)) {}

auto EnumMapper::checkDependencies() const
	-> std::vector<std::shared_ptr<DependencyRequest>> {
	return {std::make_shared<AuxRequest>(AuxDependencyId::ExternPrelude)};
}

void EnumMapper::write(mapr::transform::WriterStream& writer) {
	auto externBlock = ExternCWriter::makeBlock(context);

	auto enumName =
		TypeNameTransformer(context).getTypeName(enumDecl->getType());

	auto enumBlock =
		std::make_unique<EnumWriter>(std::make_unique<TextWriter>(enumName));

	auto memberWriters = std::vector<std::unique_ptr<WriterBase>>();

	for (const auto& member : enumDecl->getMembers()) {
		(*enumBlock) << std::make_unique<AssignmentWriter>(
			std::make_unique<TextWriter>(getMemberName(member)),
			std::make_unique<TextWriter>(member->getValue()));
	}

	externBlock << std::make_unique<TypedefWriter>(
		std::move(enumBlock), std::make_unique<TextWriter>(enumName));

	writer << externBlock << TextWriter::Newline;
}

auto EnumMapper::getMemberName(
	const std::shared_ptr<const view::EnumMemberDecl>& member) const -> std::string {
	auto nameTransformer = QualifiedNameTransformer(context);

	if (enumDecl->getType()->isScoped()) {
		return nameTransformer.getName(
			view::QualifiedName(enumDecl->getType()->getName())
			/ member->getName());
	}

	return nameTransformer.getName(enumDecl->getType()->getName().parent()
	                               / member->getName());
}