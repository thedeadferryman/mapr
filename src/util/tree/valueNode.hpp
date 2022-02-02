//
// Created by kmeinkopf on 27.01.2022.
//

#pragma once

#include "util/tree/nodeBase.hpp"

namespace kodgen::tree {

template<typename ValueT>
class ValueNode : public NodeBase {
  public:
	using ValueType = ValueT;

  private:
	ValueT value;

  public:
	explicit ValueNode(ValueT value)
		: value(std::move(value)) {}

	[[nodiscard]] auto getValue() const -> ValueT { return value; }

	auto holdsValue(const std::type_info& type) -> bool override {
		return typeid(ValueT) == type;
	}

  protected:
	auto printSelf(std::ostream& os) const -> decltype(os) override {
		return os << "ValueNode(" << value << ")";
	}
};
}  // namespace kodgen::tree