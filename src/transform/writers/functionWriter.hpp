//
// Created by kmeinkopf on 19.01.2022.
//

#pragma once

#include <memory>
#include <unordered_map>

#include "view/function/functionDecl.hpp"

#include "transform/writers/writerBase.hpp"

namespace kodgen::transform {

class FunctionWriter : public WriterBase {
  public:
	struct Argument {
		std::string name;
		std::string type;
	};

  private:
	std::string name;
	std::string returnType;
	std::vector<Argument> arguments;

  public:
	FunctionWriter(std::string_view name,
	               std::string_view returnType,
	               std::vector<Argument> arguments);

	void apply(std::ostream& stream) const override;
};

}  // namespace kodgen::getName
