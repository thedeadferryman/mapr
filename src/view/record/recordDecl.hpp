//
// Created by kmeinkopf on 05.03.22.
//

#pragma once

#include <vector>

#include "view/declBase.hpp"
#include "view/function/functionDecl.hpp"
#include "view/varDecl.hpp"

namespace mapr::view {

S_ENUM(RecordKind, Class, Struct, Union);

class RecordDecl : public DeclBase {
	QualifiedName qualifiedName;
	RecordKind kind;
	std::vector<std::string> members;
	std::shared_ptr<SourceLoc> location;

  public:
	RecordDecl(std::string_view id,
	           QualifiedName qualifiedName,
	           RecordKind kind,
	           std::shared_ptr<SourceLoc> location
	           );

	[[nodiscard]] auto getQualifiedName() const -> QualifiedName override;

	[[nodiscard]] auto getMemberIds() const -> const std::vector<std::string>&;

	void addMember(std::string_view member);

	auto getLocation() const -> std::shared_ptr<SourceLoc> override;
};

}  // namespace mapr::view
