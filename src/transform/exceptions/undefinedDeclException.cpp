//
// Created by kmeinkopf on 19.01.2022.
//

#include "undefinedDeclException.hpp"

#include "transform/dependencies/declRequest.hpp"
#include "transform/dependencies/typeRequest.hpp"

#include "util/matchType.hpp"
#include "util/stringBuilder.hpp"

using kodgen::transform::UndefinedDeclException;
using kodgen::util::MatchType;

auto UndefinedDeclException::fromRequest(
	const std::shared_ptr<DependencyRequest>& request)
	-> UndefinedDeclException {
	auto message = MatchType::matchType<std::string>(
		request,  //
		[=](const std::shared_ptr<DeclRequest>& declRequest) -> std::string {
			return util::StringBuilder()
				<< "Trying to reference undefined declaration '"
				<< declRequest->getDeclarationId() << "'";
		},
		[=](const std::shared_ptr<AuxRequest>& auxRequest) -> std::string {
			return util::StringBuilder()
				<< "Trying to reference undefined internal resource '"
				<< stringOfAuxDependencyId(auxRequest->getDependencyId())
				<< "'";
		},
		[=](const std::shared_ptr<TypeRequest>& typeRequest) -> std::string {
			return util::StringBuilder()
				<< "Trying to reference undefined type '"
				<< typeRequest->getType()->getPrettyName() << "'";
		},
		[](const std::shared_ptr<DependencyRequest>& /*unused*/)
			-> std::string { UNREACHABLE(); });

	return UndefinedDeclException(message);
}

UndefinedDeclException::UndefinedDeclException(std::string_view message)
	: BaseException(message) {}