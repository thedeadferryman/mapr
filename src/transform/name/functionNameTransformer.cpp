//
// Created by kmeinkopf on 19.01.2022.
//

#include "functionNameTransformer.hpp"

#include <clang/Basic/OperatorKinds.h>
#include <llvm/Support/raw_ostream.h>

#include "view/function/operatorDecl.hpp"
#include "view/record/recordOperatorDecl.hpp"

#include "transform/name/qualifiedNameTransformer.hpp"
#include "transform/name/typeNameTransformer.hpp"

#include "util/stringBuilder.hpp"

#include <fmt/format.h>

using mapr::transform::FunctionNameTransformer;
using mapr::view::OperatorDecl;

auto FunctionNameTransformer::getOverloadName(
	const std::shared_ptr<config::PipelineContext>& context,
	const std::shared_ptr<const view::FunctionDecl>& functionDecl,
	const view::FunctionOverload& overload,
	SlugType slugType) -> std::string {
	switch (slugType) {
		case SlugType::NoSlug:
			return getOverloadName(context, functionDecl);
		case SlugType::CountSlug:
			return getOverloadName(
				context, functionDecl, overload.countArguments());
		case SlugType::FullSlug:
			return getOverloadName(context, functionDecl, overload);
		default:
			UNREACHABLE();
	}
}

auto FunctionNameTransformer::getOverloadName(
	const std::shared_ptr<config::PipelineContext>& context,
	const std::shared_ptr<const view::FunctionDecl>& functionDecl,
	const view::FunctionOverload& overload) -> std::string {
	return getOverloadName(context, functionDecl)
		+ getOverloadTypedSuffix(context, overload);
}

auto FunctionNameTransformer::getOverloadName(
	const std::shared_ptr<config::PipelineContext>& context,
	const std::shared_ptr<const view::FunctionDecl>& functionDecl) -> std::string {
	auto functionName = functionDecl->getQualifiedName();

	auto nameTransformer = QualifiedNameTransformer(context);

	switch (functionDecl->getDeclType()) {
		case view::DeclType::Function:
		case view::DeclType::RecordMethod:
			return nameTransformer.getName(functionName);
		case view::DeclType::RecordCtor:
			return nameTransformer.getName(functionName.parent()
			                               / getCtorName(context));
		case view::DeclType::RecordDtor:
			return nameTransformer.getName(functionName.parent()
			                               / getDtorName(context));
		case view::DeclType::SyntheticGetter:
			return nameTransformer.getName(
				functionName.parent()
				/ getSyntheticGetterName(context,
			                             functionName.basename().str()));
		case view::DeclType::SyntheticSetter:
			return nameTransformer.getName(functionName.parent()
			                               / getSyntheticSetterName(  //
											   context,
											   functionName.basename().str()  //
											   )  //
			);
		case view::DeclType::OperatorOverload:
		case view::DeclType::RecordOperatorOverload:
			return nameTransformer.getName(
				functionName.parent() / getOperatorName(context, functionDecl));
		default:
			UNREACHABLE_M("Unsupported decl type!");
	}
}

auto FunctionNameTransformer::getOverloadName(
	const std::shared_ptr<config::PipelineContext>& context,
	const std::shared_ptr<const view::FunctionDecl>& functionDecl,
	std::size_t argCount) -> std::string {
	return getOverloadName(context, functionDecl)
		+ getOverloadCountSuffix(argCount);
}

auto FunctionNameTransformer::getOverloadTypedSuffix(
	const std::shared_ptr<config::PipelineContext>& context,
	const view::FunctionOverload& overload) -> std::string {
	auto typeSlugs = std::vector<std::string>();

	std::transform(std::begin(overload.getArguments()),
	               std::end(overload.getArguments()),
	               std::back_inserter(typeSlugs),
	               [context](const std::shared_ptr<const view::VarDecl>& decl) {
					   return TypeNameTransformer(context).getOverloadSlug(
						   decl->getType());
				   });

	return "_" + boost::algorithm::join(typeSlugs, "$");
}

auto FunctionNameTransformer::getOverloadCountSuffix(std::size_t argCount)
	-> std::string {
	switch (argCount) {
		case 0:
			return "$1$$01";
		case 1:
			return "$1$$00";
		default:
			return util::StringBuilder("$") << argCount << "$$11";
	}
}

auto FunctionNameTransformer::getCtorName(
	const std::shared_ptr<config::PipelineContext>& context) -> std::string {
	auto maybeName = context->readConfigVariable("constructorName");

	if (maybeName.has_value()) {
		return maybeName.value();
	}

	UNREACHABLE();
}

auto FunctionNameTransformer::getDtorName(
	const std::shared_ptr<config::PipelineContext>& context) -> std::string {
	auto maybeName = context->readConfigVariable("destructorName");

	if (maybeName.has_value()) {
		return maybeName.value();
	}

	UNREACHABLE();
}

auto FunctionNameTransformer::getSyntheticGetterName(
	const std::shared_ptr<config::PipelineContext>& context,
	std::string_view fieldName) -> std::string {
	auto maybePattern = context->readConfigVariable("getterPattern");
	if (maybePattern.has_value()) {
		return fmt::vformat(maybePattern.value(),
		                    fmt::make_format_args(fieldName));
	}

	UNREACHABLE();
}

auto FunctionNameTransformer::getSyntheticSetterName(
	const std::shared_ptr<config::PipelineContext>& context,
	std::string_view fieldName) -> std::string {
	auto maybePattern = context->readConfigVariable("setterPattern");
	if (maybePattern.has_value()) {
		return fmt::vformat(maybePattern.value(),
		                    fmt::make_format_args(fieldName));
	}

	UNREACHABLE();
}

auto FunctionNameTransformer::getOperatorName(
	const std::shared_ptr<config::PipelineContext>& context,
	const std::shared_ptr<const view::FunctionDecl>& functionDecl) -> std::string {
	if (auto opDecl =
	        std::dynamic_pointer_cast<const view::OperatorDecl>(functionDecl)) {
		return getOperatorNameForKind(context, opDecl->getOperatorKind());
	}

	if (auto opDecl =
	        std::dynamic_pointer_cast<const view::RecordOperatorDecl>(functionDecl)) {
		return getOperatorNameForKind(context, opDecl->getOperatorKind());
	}

	UNREACHABLE();
}

auto FunctionNameTransformer::getOperatorNameForKind(
	const std::shared_ptr<config::PipelineContext>& context,
	clang::OverloadedOperatorKind operatorKind) -> std::string {
	auto maybePattern = context->readConfigVariable("operatorPattern");

	if (maybePattern.has_value()) {
		return fmt::vformat(
			maybePattern.value(),
			fmt::make_format_args(getOperatorAlias(operatorKind)));
	}

	UNREACHABLE();
}

auto FunctionNameTransformer::getOperatorAlias(OperatorDecl::Kind operatorKind)
	-> std::string {
	switch (operatorKind) {
		case clang::OO_Plus:
			return "add";
		case clang::OO_Minus:
			return "sub";
		case clang::OO_Star:
			return "mul";
		case clang::OO_Slash:
			return "div";
		case clang::OO_Percent:
			return "mod";
		case clang::OO_Caret:
			return "pow";
		case clang::OO_Amp:
			return "amp";
		case clang::OO_Pipe:
			return "pipe";
		case clang::OO_Tilde:
			return "tld";
		case clang::OO_Exclaim:
			return "exc";
		case clang::OO_Equal:
			return "assign";
		case clang::OO_Less:
			return "ltt";
		case clang::OO_Greater:
			return "gtt";
		case clang::OO_PlusEqual:
			return "addeq";
		case clang::OO_MinusEqual:
			return "subeq";
		case clang::OO_StarEqual:
			return "muleq";
		case clang::OO_SlashEqual:
			return "diveq";
		case clang::OO_PercentEqual:
			return "modeq";
		case clang::OO_CaretEqual:
			return "poweq";
		case clang::OO_AmpEqual:
			return "ampeq";
		case clang::OO_PipeEqual:
			return "pipeq";
		case clang::OO_LessLess:
			return "llt";
		case clang::OO_GreaterGreater:
			return "ggt";
		case clang::OO_LessLessEqual:
			return "llteq";
		case clang::OO_GreaterGreaterEqual:
			return "ggteq";
		case clang::OO_EqualEqual:
			return "eq";
		case clang::OO_ExclaimEqual:
			return "neq";
		case clang::OO_LessEqual:
			return "lte";
		case clang::OO_GreaterEqual:
			return "gte";
		case clang::OO_Spaceship:
			return "cmp";
		case clang::OO_AmpAmp:
			return "and";
		case clang::OO_PipePipe:
			return "or";
		case clang::OO_PlusPlus:
			return "aadd";
		case clang::OO_MinusMinus:
			return "ssub";
		case clang::OO_Comma:
			return "comma";
		case clang::OO_ArrowStar:
			return "arstar";
		case clang::OO_Arrow:
			return "ptr";
		case clang::OO_Call:
			return "call";
		case clang::OO_Subscript:
			return "sbs";
		case clang::OO_Conditional:
			return "cond";
		case clang::OO_Coawait:
			return "await";
		default:
			UNREACHABLE_M("Unsupported operator!");
	}
}

#pragma clang diagnostic pop
