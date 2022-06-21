//
// Created by karl on 31.03.22.
//

#include "memberAccessWriter.hpp"

#include "util/macro.hpp"

using mapr::transform::MemberAccessWriter;

MemberAccessWriter::MemberAccessWriter(std::unique_ptr<WriterBase> owner,
                                       std::unique_ptr<WriterBase> target,
                                       MemberAccessMode accessMode)
	: owner(std::move(owner))
	, target(std::move(target))
	, accessMode(accessMode) {}

void MemberAccessWriter::apply(std::ostream& stream) const {
	stream << '(' << owner;

	switch (accessMode) {
		case MemberAccessMode::Regular:
			stream << '.';
			break;
		case MemberAccessMode::PointerLike:
			stream << "->";
			break;
		default:
			UNREACHABLE();
	}

	stream << target << ')';
}
