//
// Created by kmeinkopf on 26.12.2021.
//

#include <iostream>
#include <utility>

#include "functionMapper.hpp"

#include <boost/assert.hpp>

#include "view/types/typeBase.hpp"

#include "transform/dependencies/typeRequest.hpp"
#include "transform/name/functionNameTransformer.hpp"
#include "transform/name/namespacedNameTransformer.hpp"
#include "transform/name/typeNameTransformer.hpp"
#include "transform/writers/c11GenericWriter.hpp"
#include "transform/writers/defineWriter.hpp"
#include "transform/writers/functionWriter.hpp"
#include "transform/writers/ieeWriter.hpp"
#include "transform/writers/textWriter.hpp"
#include "transform/writerStream.hpp"

#include "util/stringBuilder.hpp"
#include "util/tree/valueNode.hpp"

using kodgen::transform::FunctionMapper;
using kodgen::tree::NodeBase;
using kodgen::tree::ValueNode;
using kodgen::view::TypeBase;

FunctionMapper::FunctionMapper(std::shared_ptr<view::FunctionDecl> functionDecl)
	: functionDecl(std::move(functionDecl))
	, overloadTree(NodeBase::empty()) {
	computeArgMap();

	for (auto overload : this->functionDecl->getOverloads()) {
		buildOverloadTree(overloadTree, overload);
	}
}

auto FunctionMapper::checkDependencies() const
	-> std::vector<std::shared_ptr<DependencyRequest>> {
	auto dependencies = std::vector<std::shared_ptr<DependencyRequest>> {
		std::make_shared<AuxRequest>(AuxDependencyId::ExternPrelude)};

	if (argCounts.size() > 1) {
		dependencies.push_back(
			std::make_shared<AuxRequest>(AuxDependencyId::OverloadPrelude));
	}

	for (auto overload : functionDecl->getOverloads()) {
		dependencies.push_back(
			std::make_shared<TypeRequest>(overload.getReturnType()));

		for (auto arg : overload.getArguments()) {
			dependencies.push_back(
				std::make_shared<TypeRequest>(arg->getType()));
		}
	}

	return dependencies;
}

void FunctionMapper::writeDeclaration(WriterStream& writer) {
	for (const auto& overload : functionDecl->getOverloads()) {
		const auto& overloadName = FunctionNameTransformer::getOverloadName(
			functionDecl->getQualifiedName(),
			overload,
			getSlugType(overload.countArguments()));

		const auto& returnType =
			TypeNameTransformer::getTypeName(overload.getReturnType());

		auto arguments = std::vector<FunctionWriter::Argument>();

		for (auto arg : overload.getArguments()) {
			const auto& type = TypeNameTransformer::getTypeName(arg->getType());
			arguments.push_back(
				FunctionWriter::Argument {.name = arg->getID(), .type = type});
		}

		writer << FunctionWriter(overloadName, returnType, arguments);
	}
	typedOverloadDeclarationWriter(writer);
}

void FunctionMapper::writeDefinition(WriterStream& writer) {}

auto FunctionMapper::getSlugType(std::size_t argCount) const
	-> FunctionNameTransformer::SlugType {
	if (functionDecl->getOverloads().size() == 1) {
		return FunctionNameTransformer::SlugType::NoSlug;
	}

	if (argCounts.at(argCount) <= 1) {
		return FunctionNameTransformer::SlugType::CountSlug;
	}

	return FunctionNameTransformer::SlugType::FullSlug;
}

void FunctionMapper::computeArgMap() {
	BOOST_ASSERT_MSG(functionDecl, "Should not be null!");

	if (argCounts.empty()) {
		for (const auto& overload : functionDecl->getOverloads()) {
			argCounts[overload.countArguments()] += 1;
		}
	}
}

void FunctionMapper::buildOverloadTree(
	std::shared_ptr<NodeBase> node,
	const view::FunctionOverload& overload) const {
	if (node->isRoot()) {
		auto argCount = overload.countArguments();

		auto newNode = node->findOrAddChild(
			[argCount](const std::shared_ptr<NodeBase>& child) {
				if (auto valueChild =
			            std::dynamic_pointer_cast<ValueNode<std::size_t>>(
							child)  //
			    ) {
					return valueChild->getValue() == argCount;
				}

				return false;
			},
			[argCount]() {
				return (std::make_shared<ValueNode<std::size_t>>(argCount));
			});

		buildOverloadTree(newNode, overload);
	} else {
		if (overload.countArguments() <= 0)
			return;

		auto arg0 = overload.getArguments()[0]->getType();
		auto argSlug = TypeNameTransformer::getOverloadSlug(arg0);

		auto newNode = node->findOrAddChild(
			[argSlug](const std::shared_ptr<NodeBase>& child) {
				if (auto valueChild = std::dynamic_pointer_cast<
						ValueNode<std::shared_ptr<view::TypeBase>>>(child)) {
					return argSlug
						== TypeNameTransformer::getOverloadSlug(
							   valueChild->getValue());
				}

				return false;
			},
			[arg0]() {
				return std::make_shared<
					ValueNode<std::shared_ptr<view::TypeBase>>>(arg0);
			});

		const auto bound = overload.bindArgument();

		if (!bound.has_value()) {
			UNREACHABLE();
		}

		buildOverloadTree(newNode, bound.value());
	}
}

auto FunctionMapper::typedOverloadDeclarationWriterI(
	std::shared_ptr<NodeBase> node, view::FunctionOverload path) const
	-> std::unique_ptr<WriterBase> {
	if (node->isLeaf()) {
		return std::make_unique<TextWriter>(
			FunctionNameTransformer::getOverloadName(
				functionDecl->getQualifiedName(),
				path,
				FunctionNameTransformer::SlugType::FullSlug));
	}

	auto vec = std::vector<C11GenericWriter::Case>();

	if (node->countChildren() <= 1) {
		auto child = *node->begin();
		if (auto valueChild =
		        std::dynamic_pointer_cast<ValueNode<std::shared_ptr<TypeBase>>>(
					child)) {
			return typedOverloadDeclarationWriterI(
				child,
				path.appendArgument(std::make_shared<view::VarDecl>(  //
					"_",
					valueChild->getValue())));
		}
	}

	for (auto child : *node) {
		if (auto valueChild =
		        std::dynamic_pointer_cast<ValueNode<std::shared_ptr<TypeBase>>>(
					child)) {
			const view::FunctionOverload& newPath = path.appendArgument(  //
				std::make_shared<view::VarDecl>(  //
					"_",
					valueChild->getValue()));

			auto childWriter = typedOverloadDeclarationWriterI(child, newPath);

			vec.push_back(C11GenericWriter::Case {
				.type = std::make_unique<TextWriter>(
					TypeNameTransformer::getTypeName(valueChild->getValue())),
				.value = std::move(childWriter)  //
			});
		}
	}

	std::string condName = util::StringBuilder()
		<< "("
		<< "__" << std::to_string(path.countArguments()) << ")";

	return std::make_unique<C11GenericWriter>(
		nullptr, std::make_unique<TextWriter>(condName), std::move(vec));
}

void FunctionMapper::typedOverloadDeclarationWriter(
	WriterStream& stream) const {
	for (auto child : *overloadTree) {
		if (auto valueChild =
		        std::dynamic_pointer_cast<ValueNode<std::size_t>>(child)) {
			auto macroArgs = std::vector<std::unique_ptr<WriterBase>>();
			auto iieArgs = std::vector<std::unique_ptr<WriterBase>>();

			for (std::size_t i = 0; i < valueChild->getValue(); i++) {
				macroArgs.push_back(
					std::make_unique<TextWriter>("__" + std::to_string(i)));
				iieArgs.push_back(
					std::make_unique<TextWriter>("__" + std::to_string(i)));
			}

			auto bodyWriter = typedOverloadDeclarationWriterI(
				child, view::FunctionOverload(nullptr, {}));

			stream << DefineWriter(
				std::make_unique<TextWriter>(  //
					NamespacedNameTransformer::getName(
						functionDecl->getQualifiedName())),
				std::move(macroArgs),
				std::make_unique<IEEWriter>(std::move(bodyWriter),
			                                std::move(iieArgs)));
		}
	}
}
