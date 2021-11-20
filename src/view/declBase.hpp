//
// Created by kmeinkopf on 20.11.2021.
//

#pragma once

#include <string>

namespace kodgen::view {

class DeclBase {
	const std::string name;

  public:
	explicit DeclBase(const std::string_view& name);

	[[nodiscard]] virtual std::string_view getName() const;

	virtual ~DeclBase();
};

}  // namespace kodgen::view