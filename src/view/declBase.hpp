//
// Created by kmeinkopf on 20.11.2021.
//

#pragma once

#include <memory>
#include <string>

#include "view/loc/sourceLoc.hpp"

#include "util/macro.hpp"

namespace mapr::config {
class FilterBase;
enum class FilterResult;
enum class SparseFilteringMode;
}  // namespace mapr::config

namespace mapr::view {

class QualifiedName;

S_ENUM(DeclType,
       Base,
       Type,
       Typedef,
       Enum,
       EnumMember,
       Var,
       Function,
       Internal,
       Record,
       RecordMethod,
       RecordCtor,
       RecordDtor,
       SyntheticGetter,
       SyntheticSetter,
       OperatorOverload,
       RecordOperatorOverload
       //
);

class DeclBase : public std::enable_shared_from_this<DeclBase> {
	const std::string id;
	const DeclType declType;

  public:
	explicit DeclBase(std::string_view id, DeclType declType);

	[[nodiscard]] virtual auto getID() const -> const std::string&;

	[[nodiscard]] virtual auto getDeclType() const -> DeclType;

	[[nodiscard]] virtual auto getLocation() const
		-> std::shared_ptr<SourceLoc> = 0;

	[[nodiscard]] virtual auto acceptSparseFilter(
		const std::shared_ptr<const config::FilterBase>& filter,
		config::SparseFilteringMode filteringMode) const
		-> std::shared_ptr<const DeclBase>;

	[[nodiscard]] virtual auto acceptFilter(
		const std::shared_ptr<const config::FilterBase>& filter) const
		-> config::FilterResult;

	[[nodiscard]] virtual auto getQualifiedName() const -> QualifiedName = 0;

	DeclBase(const DeclBase&) = delete;
	DeclBase(DeclBase&&) = delete;
	auto operator=(const DeclBase&) -> DeclBase& = delete;
	auto operator=(DeclBase&&) -> DeclBase& = delete;
	virtual ~DeclBase() = default;
};

}  // namespace mapr::view