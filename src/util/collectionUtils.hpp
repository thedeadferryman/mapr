//
// Created by kmeinkopf on 02.02.2022.
//

#pragma once

#include <boost/algorithm/string/join.hpp>
#include <functional>
#include <vector>

namespace mapr::util {

class CollectionUtils {
	template<typename Collection>
	using Element = decltype(*std::begin(Collection()));

  public:
	CollectionUtils() = delete;
	CollectionUtils(const CollectionUtils&) = delete;
	CollectionUtils(CollectionUtils&&) = delete;

	auto operator=(const CollectionUtils&) -> CollectionUtils& = delete;
	auto operator=(CollectionUtils&&) -> CollectionUtils& = delete;

	virtual ~CollectionUtils() = delete;

	template<typename Collection>
	static void forEachSkipFirst(
		const Collection& collection,
		std::function<void(const Element<Collection>&)> action) {
		if (std::begin(collection) == std::end(collection)) {
			return;
		}

		std::for_each(std::begin(collection) + 1, std::end(collection), action);
	}

	template<typename Collection>
	static auto mapJoin(
		const Collection& collection,
		const std::string& glue,
		std::function<std::string(const Element<Collection>&)> action) {
		auto strings = std::vector<std::string>();

		std::transform(std::begin(collection),
		               std::end(collection),
		               std::back_inserter(strings),
		               action);

		return boost::algorithm::join(strings, glue);
	}

	template<size_t Count, typename T>
	static auto decompose(const T& container) {
		return decompImpl(container, std::make_index_sequence<Count>());
	}

  private:
	template<typename T, size_t... S>
	static auto decompImpl(const T& container,
	                       std::index_sequence<S...> /* unused */) {
		return std::forward_as_tuple(*std::next(std::begin(container), S)...);
	}
};

}  // namespace mapr::util