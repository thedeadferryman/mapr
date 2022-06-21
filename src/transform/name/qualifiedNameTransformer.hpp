//
// Created by kmeinkopf on 02.02.2022.
//

#pragma once

#include <memory>

#include <boost/algorithm/string/join.hpp>

#include "view/name/qualifiedName.hpp"

#include "config/pipelineContext.hpp"

namespace mapr::transform {

enum class NameStyle {
	Raw,
	Prefixed
};

class QualifiedNameTransformer {
	std::shared_ptr<config::PipelineContext> context;

  public:
	explicit QualifiedNameTransformer(std::shared_ptr<config::PipelineContext> context);

	[[nodiscard]] auto getName(const view::QualifiedName& name,
	                           NameStyle style = NameStyle::Prefixed) const
		-> std::string;

  private:
	[[nodiscard]] auto transformFragment(
		const view::NameFragment& fragment) const -> std::string;

	[[nodiscard]] auto transformTemplateArgument(
		const std::shared_ptr<view::TemplateArgumentBase>& argument) const
		-> std::string;
};

}  // namespace mapr::transform