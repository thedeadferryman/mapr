//
// Created by kmeinkopf on 27.01.2022.
//

#pragma once

#include <functional>
#include <memory>
#include <vector>

namespace kodgen::tree {

class NodeBase : public std::enable_shared_from_this<NodeBase> {
	std::vector<std::shared_ptr<NodeBase>> children;
	std::weak_ptr<NodeBase> parent;

  public:
	template<typename ValueT>
	static auto holdsValue(std::shared_ptr<NodeBase> node) -> bool {
		return node->holdsValue(typeid(ValueT));
	}

	[[nodiscard]] static auto empty() -> std::shared_ptr<NodeBase>;

	NodeBase() = default;

	NodeBase(const NodeBase&) = delete;
	NodeBase(NodeBase&&) = delete;

	auto operator=(const NodeBase&) -> NodeBase& = delete;
	auto operator=(NodeBase&&) -> NodeBase& = delete;

	virtual ~NodeBase() = default;

	[[nodiscard]] auto getParent() const -> std::shared_ptr<NodeBase>;

	[[nodiscard]] auto isRoot() const -> bool;
	[[nodiscard]] auto isLeaf() const -> bool;

	[[nodiscard]] auto begin() const -> decltype(children)::const_iterator;
	[[nodiscard]] auto end() const -> decltype(children)::const_iterator;

	[[nodiscard]] auto countChildren() const -> decltype(children)::size_type;

	[[nodiscard]] auto findChild(
		const std::function<bool(const std::shared_ptr<NodeBase>&)> pred) const
		-> std::shared_ptr<NodeBase>;

	[[nodiscard]] auto findOrAddChild(
		const std::function<bool(const std::shared_ptr<NodeBase>&)> pred,
		const std::function<std::shared_ptr<NodeBase>()> adder)
		-> std::shared_ptr<NodeBase>;

	void addChild(std::shared_ptr<NodeBase> child);

	void setParent(std::shared_ptr<NodeBase> parent);

	friend auto operator<<(std::ostream& os, std::shared_ptr<NodeBase> nodeBase)
		-> decltype(os) {
		return nodeBase->print(os, 0);
	}

  protected:
	[[nodiscard]] virtual auto holdsValue(const std::type_info& type)
		-> bool = 0;

	virtual auto printSelf(std::ostream& os) const -> decltype(os) = 0;

	virtual auto print(std::ostream& os, std::size_t depth) -> decltype(os);
};

}  // namespace kodgen::tree