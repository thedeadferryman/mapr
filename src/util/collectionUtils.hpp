//
// Created by kmeinkopf on 02.02.2022.
//

#pragma once

#include <boost/algorithm/string/join.hpp>

namespace kodgen::util {

class CollectionUtils {
  private:
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
		if (std::begin(collection) == std::end(collection))
			return;

		std::for_each(std::begin(collection) + 1, std::end(collection), action);
	}
};

}  // namespace kodgen::util