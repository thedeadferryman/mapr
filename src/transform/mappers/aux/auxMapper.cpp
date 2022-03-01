//
// Created by kmeinkopf on 20.01.2022.
//

#include "auxMapper.hpp"

#include "transform/aux/templateFile.hpp"
#include "transform/writers/textWriter.hpp"

#include "util/matchType.hpp"

using kodgen::transform::AuxMapper;
using kodgen::util::MatchType;

AuxMapper::AuxMapper(std::shared_ptr<AuxDecl> decl)
	: decl(decl) {}

auto AuxMapper::checkDependencies() const -> std::vector<std::shared_ptr<DependencyRequest>> {
	return {};
}

void AuxMapper::write(kodgen::transform::WriterStream& writer) {
	MatchType::switchType(
		decl,  //
		[&writer](const std::shared_ptr<TemplateFile>& templateFile) {
			writer << TextWriter(templateFile->replaceWith());
		});
}
