//
// Created by kmeinkopf on 05.03.22.
//

#include "recordDecl.hpp"

using mapr::view::RecordDecl;

template<typename T>
auto lockWeakPtrVector(std::vector<std::weak_ptr<T>> vect)
	-> std::vector<std::shared_ptr<T>> {
	auto result = std::vector<std::shared_ptr<T>>();

	std::transform(std::begin(vect),
	               std::end(vect),
	               std::back_inserter(result),
	               [](const std::weak_ptr<T>& ptr) { return ptr.lock(); });

	return result;
}

RecordDecl::RecordDecl(std::string_view id,
                       QualifiedName qualifiedName,
                       RecordKind kind,
                       std::shared_ptr<SourceLoc> location)
	: DeclBase(id, DeclType::Record)
	, qualifiedName(std::move(qualifiedName))
	, kind(kind)
	, location(std::move(location)) {}

auto RecordDecl::getQualifiedName() const -> QualifiedName {
	return qualifiedName;
}

auto RecordDecl::getMemberIds() const -> const std::vector<std::string>& {
	return members;
}

void RecordDecl::addMember(std::string_view member) {
	members.emplace_back(member);
}

auto RecordDecl::getLocation() const -> std::shared_ptr<SourceLoc> {
	return location;
};