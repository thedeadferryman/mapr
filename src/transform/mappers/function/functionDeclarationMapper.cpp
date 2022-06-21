//
// Created by kmeinkopf on 26.12.2021.
//

#include <utility>

#include "functionDeclarationMapper.hpp"

#include <boost/assert.hpp>

#include "view/types/typeBase.hpp"

#include "transform/dependencies/typeRequest.hpp"
#include "transform/name/functionNameTransformer.hpp"
#include "transform/name/typeNameTransformer.hpp"
#include "transform/writers/c11GenericWriter.hpp"
#include "transform/writers/defineWriter.hpp"
#include "transform/writers/expressionCallWriter.hpp"
#include "transform/writers/function/functionHeadWriter.hpp"
#include "transform/writers/ieeWriter.hpp"
#include "transform/writers/sequentialWriter.hpp"
#include "transform/writers/textWriter.hpp"
#include "transform/writerStream.hpp"

#include "util/stringBuilder.hpp"
#include "util/tree/valueNode.hpp"

using mapr::transform::FunctionDeclarationMapper;
using mapr::tree::NodeBase;
using mapr::tree::ValueNode;
using mapr::view::TypeBase;

FunctionDeclarationMapper::FunctionDeclarationMapper(
	std::shared_ptr<const view::FunctionDecl> functionDecl,
	std::shared_ptr<config::PipelineContext> context)
	: functionDecl(std::move(functionDecl))
	, overloadTree(NodeBase::empty())
	, context(std::move(context)) {
	computeArgMap();

	for (const auto& overload : this->functionDecl->getOverloads()) {
		buildOverloadTree(overloadTree, overload);
	}
}

auto FunctionDeclarationMapper::checkDependencies() const
	-> std::vector<std::shared_ptr<DependencyRequest>> {
	auto dependencies = std::vector<std::shared_ptr<DependencyRequest>> {
		std::make_shared<AuxRequest>(AuxDependencyId::ExternPrelude)};

	if (argCounts.size() > 1) {
		dependencies.push_back(
			std::make_shared<AuxRequest>(AuxDependencyId::OverloadPrelude));
	}

	for (const auto& overload : functionDecl->getOverloads()) {
		dependencies.push_back(
			std::make_shared<TypeRequest>(overload.getReturnType()));

		for (const auto& arg : overload.getArguments()) {
			dependencies.push_back(
				std::make_shared<TypeRequest>(arg->getType()));
		}
	}

	return dependencies;
}

void FunctionDeclarationMapper::write(WriterStream& writer) {
	if (functionDecl->getOverloads().size() == 1) {
		writeOverload(writer, functionDecl->getOverloads().front());
	} else {
		writeOverloadTree(writer);
	}
}

void FunctionDeclarationMapper::writeOverloadTree(
	mapr::transform::WriterStream& writer) const {
	for (const auto& overload : functionDecl->getOverloads()) {
		writeOverload(writer, overload);
	}

	writer << TextWriter::Newline;

	writeTypedOverloadDeclarations(writer);

	if (argCounts.size() > 1) {
		writeCountSwitchDeclaration(writer);
	}
}

void FunctionDeclarationMapper::writeOverload(
	WriterStream& writer, const mapr::view::FunctionOverload& target) const {
	auto shortName =
		FunctionNameTransformer::getOverloadName(context, functionDecl);
	auto fullName =
		FunctionNameTransformer::getOverloadName(context, functionDecl, target);
	auto functionName =
		(functionDecl->getOverloads().size() == 1) ? shortName : fullName;

	auto typeTransformer = TypeNameTransformer(context);

	auto returnType = typeTransformer.buildMappedType(target.getReturnType());
	auto args = std::vector<FunctionHeadWriter::Argument>();

	for (const auto& arg : target.getArguments()) {
		const auto& type = typeTransformer.buildMappedType(arg->getType());
		args.push_back(FunctionHeadWriter::Argument {
			.name = arg->getID(),
			.type = type,
		});
	}

	writer << FunctionHeadWriter(context,
	                             std::make_unique<TextWriter>(functionName),
	                             std::make_unique<TextWriter>(returnType),
	                             std::move(args));
}

void FunctionDeclarationMapper::computeArgMap() {
	BOOST_ASSERT_MSG(functionDecl, "Should not be null!");

	if (argCounts.empty()) {
		for (const auto& overload : functionDecl->getOverloads()) {
			argCounts[overload.countArguments()] += 1;
		}
	}
}

void FunctionDeclarationMapper::buildOverloadTree(
	const std::shared_ptr<NodeBase>& node,
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
		if (overload.countArguments() <= 0) {
			return;
		}

		auto typeTransformer = TypeNameTransformer(context);

		auto arg0 = overload.getArguments()[0]->getType();
		auto argSlug = typeTransformer.getOverloadSlug(arg0);

		auto newNode = node->findOrAddChild(
			[argSlug, typeTransformer](const std::shared_ptr<NodeBase>& child) {
				if (auto valueChild = std::dynamic_pointer_cast<
						ValueNode<std::shared_ptr<view::TypeBase>>>(child)) {
					return argSlug
						== typeTransformer.getOverloadSlug(
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

auto FunctionDeclarationMapper::writeTypedOverloadDeclarationBody(
	const std::shared_ptr<NodeBase>& node,
	const view::FunctionOverload& path) const -> std::unique_ptr<WriterBase> {
	if (node->isLeaf()) {
		auto seq = std::vector<std::unique_ptr<WriterBase>>();

		seq.push_back(std::make_unique<TextWriter>("/* "));
		seq.push_back(std::make_unique<TextWriter>(
			functionDecl->getQualifiedName().str()));
		seq.push_back(std::make_unique<TextWriter>(" */"));
		seq.push_back(std::make_unique<TextWriter>(
			FunctionNameTransformer::getOverloadName(
				context, functionDecl, path)));

		return std::make_unique<SequentialWriter>(std::move(seq));
	}

	if (node->countChildren() <= 1) {
		auto child = *node->begin();
		if (auto valueChild =
		        std::dynamic_pointer_cast<ValueNode<std::shared_ptr<TypeBase>>>(
					child)) {
			return writeTypedOverloadDeclarationBody(
				child,
				path.appendArgument(std::make_shared<view::VarDecl>(  //
					"_",
					valueChild->getValue(),
					std::make_shared<view::SparseSourceLoc>()  //
					))  //
			);
		}
	}

	auto vec = std::vector<C11GenericWriter::Case>();

	for (const auto& child : *node) {
		if (auto valueChild =
		        std::dynamic_pointer_cast<ValueNode<std::shared_ptr<TypeBase>>>(
					child)) {
			const view::FunctionOverload& newPath = path.appendArgument(  //
				std::make_shared<view::VarDecl>(  //
					"_",
					valueChild->getValue(),
					std::make_shared<view::SparseSourceLoc>()  //
					));

			auto childWriter =
				writeTypedOverloadDeclarationBody(child, newPath);

			vec.push_back(C11GenericWriter::Case {
				.type = std::make_unique<TextWriter>(
					TypeNameTransformer(context).buildMappedType(
						valueChild->getValue())),
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

void FunctionDeclarationMapper::writeTypedOverloadDeclarations(
	WriterStream& stream) const {
	for (const auto& child : *overloadTree) {
		if (auto valueChild =
		        std::dynamic_pointer_cast<ValueNode<std::size_t>>(child)) {
			auto macroArgs = std::vector<std::unique_ptr<WriterBase>>();
			auto iieArgs = std::vector<std::unique_ptr<WriterBase>>();

			auto argCount = valueChild->getValue();
			for (std::size_t i = 0; i < argCount; i++) {
				macroArgs.push_back(
					std::make_unique<TextWriter>("__" + std::to_string(i)));
				iieArgs.push_back(
					std::make_unique<TextWriter>("__" + std::to_string(i)));
			}

			auto bodyWriter = writeTypedOverloadDeclarationBody(
				child, view::FunctionOverload(nullptr, {}, nullptr));

			stream << DefineWriter(
				getTypedOverloadHandlerName(argCount),
				std::move(macroArgs),
				std::make_unique<IEEWriter>(std::move(bodyWriter),
			                                std::move(iieArgs)));
		}
	}
}

auto FunctionDeclarationMapper::getTypedOverloadHandlerName(
	std::size_t argCount) const -> std::unique_ptr<WriterBase> {
	if (argCounts.size() <= 1) {
		return std::make_unique<TextWriter>(
			FunctionNameTransformer::getOverloadName(context, functionDecl)  //
		);
	}

	return std::make_unique<TextWriter>(
		FunctionNameTransformer::getOverloadName(
			context, functionDecl, argCount)  //
	);
}

void FunctionDeclarationMapper::writeCountSwitchDeclaration(
	WriterStream& stream) const {
	auto functionName =
		FunctionNameTransformer::getOverloadName(context, functionDecl);

	auto declArgs = std::vector<std::unique_ptr<WriterBase>>();
	declArgs.emplace_back(std::make_unique<TextWriter>("..."));

	auto bodyArgs = std::vector<std::unique_ptr<WriterBase>>();
	bodyArgs.emplace_back(std::make_unique<TextWriter>(functionName));
	bodyArgs.emplace_back(std::make_unique<TextWriter>("__VA_ARGS__"));

	auto maybePrefix = context->readConfigVariable("preludePrefix");

	if (!maybePrefix.has_value()) {
		UNREACHABLE();
	}

	auto body = std::make_unique<ExpressionCallWriter>(
		std::make_unique<TextWriter>(maybePrefix.value() + "_CALL_OVERLOAD"),
		std::move(bodyArgs));

	stream << DefineWriter(std::make_unique<TextWriter>(functionName),
	                       std::move(declArgs),
	                       std::move(body))
		   << TextWriter::Newline;
}
