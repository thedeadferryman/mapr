//
// Created by kmeinkopf on 02.02.2022.
//

#include "enumMapper.hpp"

#include "transform/name/namespacedNameTransformer.hpp"
#include "transform/name/typeNameTransformer.hpp"
#include "transform/writers/enumWriter.hpp"
#include "transform/writers/externCWriter.hpp"
#include "transform/writers/sequentialWriter.hpp"
#include "transform/writers/textWriter.hpp"
#include "transform/writers/typedefWriter.hpp"

using kodgen::transform::EnumMapper;

EnumMapper::EnumMapper(std::shared_ptr<view::EnumDecl> enumDecl)
	: enumDecl(std::move(enumDecl)) {}

auto EnumMapper::checkDependencies() const
	-> std::vector<std::shared_ptr<DependencyRequest>> {
	return {std::make_shared<AuxRequest>(AuxDependencyId::ExternPrelude)};
}

void EnumMapper::write(kodgen::transform::WriterStream& writer) {
	auto externBlock = ExternCWriter::makeBlock();

	auto enumName = TypeNameTransformer::getTypeName(enumDecl->getType());

	auto enumBlock =
		std::make_unique<EnumWriter>(std::make_unique<TextWriter>(enumName));

	auto memberWriters = std::vector<std::unique_ptr<WriterBase>>();

	for (const auto& member : enumDecl->getMembers()) {
		(*enumBlock) << std::make_unique<TextWriter>(getMemberName(member));
	}

	externBlock << std::make_unique<TypedefWriter>(
		std::move(enumBlock), std::make_unique<TextWriter>(enumName));

	writer << externBlock;
}

auto EnumMapper::getMemberName(
	const std::shared_ptr<view::EnumMemberDecl>& member) const -> std::string {
	if (enumDecl->getType()->isScoped()) {
		return NamespacedNameTransformer::getName(
			view::NamespacedName(enumDecl->getType()->getName())
			/ member->getName());
	} else {
		return NamespacedNameTransformer::getName(
			enumDecl->getType()->getName().basename() / member->getName());
	}
}