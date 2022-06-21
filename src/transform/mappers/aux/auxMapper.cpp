//
// Created by kmeinkopf on 20.01.2022.
//

#include <utility>

#include "auxMapper.hpp"

#include "transform/aux/includeFile.hpp"
#include "transform/aux/templateFile.hpp"
#include "transform/writers/includeWriter.hpp"
#include "transform/writers/textWriter.hpp"

#include "util/matchType.hpp"

using mapr::transform::AuxMapper;
using mapr::util::MatchType;

AuxMapper::AuxMapper(std::shared_ptr<const AuxDecl> decl,
                     std::shared_ptr<config::PipelineContext> context)
	: decl(std::move(decl))
	, context(std::move(context)) {}

auto AuxMapper::checkDependencies() const
	-> std::vector<std::shared_ptr<DependencyRequest>> {
	return {};
}

void AuxMapper::write(mapr::transform::WriterStream& writer) {
	MatchType::switchType(
		decl,  //
		[&writer,
	     *this](const std::shared_ptr<const TemplateFile>& templateFile) {
			writer << TextWriter(
				templateFile->render(context->getConfig().variables)  //
			);
		},
		[&writer](const std::shared_ptr<const IncludeFile>& includeFile) {
			writer << IncludeWriter(includeFile->getFilename(),
		                            includeFile->getMode());
		});
}
