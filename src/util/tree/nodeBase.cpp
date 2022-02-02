//
// Created by kmeinkopf on 27.01.2022.
//

#include "nodeBase.hpp"

#include "util/tree/valueNode.hpp"

using kodgen::tree::NodeBase;

auto NodeBase::begin() const -> decltype(children)::const_iterator {
	return children.begin();
}

auto NodeBase::end() const -> decltype(children)::const_iterator {
	return children.end();
}

auto NodeBase::countChildren() const -> decltype(children)::size_type {
	return children.size();
}

auto NodeBase::getParent() const -> std::shared_ptr<NodeBase> {
	if (parent.expired()) {
		return nullptr;
	}

	return parent.lock();
}

auto NodeBase::isRoot() const -> bool {
	if (parent.expired()) {
		return true;
	}

	return (parent.lock() == nullptr);
}

auto NodeBase::isLeaf() const -> bool {
	return children.empty();
}

void NodeBase::addChild(std::shared_ptr<NodeBase> child) {
	child->setParent(shared_from_this());
	children.push_back(std::move(child));
}
void NodeBase::setParent(std::shared_ptr<NodeBase> newParent) {
	parent = std::move(newParent);
}

auto NodeBase::findChild(
	const std::function<bool(const std::shared_ptr<NodeBase>&)> pred  //
) const -> std::shared_ptr<NodeBase> {
	for (auto child : children) {
		bool predResult = pred(child);
		if (predResult) {
			return child;
		}
	}

	return nullptr;
}

auto NodeBase::findOrAddChild(
	const std::function<bool(const std::shared_ptr<NodeBase>&)> pred,
	const std::function<std::shared_ptr<NodeBase>()> adder)
	-> std::shared_ptr<NodeBase> {
	auto exNode = findChild(std::move(pred));

	if (exNode) {
		return exNode;
	}

	auto newNode = adder();
	addChild(newNode);

	return newNode;
}

auto NodeBase::print(std::ostream& os, std::size_t depth) -> decltype(os) {
	os << std::string(depth, '\t');
	printSelf(os);
	os << '\n';

	for (auto child : children) {
		child->print(os, depth + 1);
	}

	return os;
}
auto NodeBase::empty() -> std::shared_ptr<NodeBase> {
	return std::make_shared<ValueNode<std::nullptr_t>>(nullptr);
}
