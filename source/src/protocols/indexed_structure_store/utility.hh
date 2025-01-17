// -*- mode:c++;tab-width:2;indent-tabs-mode:t;show-trailing-whitespace:t;rm-trailing-spaces:t -*-
// vi: set ts=2 noet:
//
// (c) Copyright Rosetta Commons Member Institutions.
// (c) This file is part of the Rosetta software suite and is made available under license.
// (c) The Rosetta software is developed by the contributing members of the Rosetta Commons.
// (c) For more information, see http://www.rosettacommons.org. Questions about this can be
// (c) addressed to University of Washington CoMotion, email: license@uw.edu.

/// @file src/protocols/indexed_structure_store/utility.hh

#ifndef INCLUDED_protocols_indexed_structure_store_utility_HH
#define INCLUDED_protocols_indexed_structure_store_utility_HH

#include <core/types.hh>
#include <core/pose/Pose.fwd.hh>

namespace protocols { namespace indexed_structure_store {

core::pose::PoseOP
append_pose_by_bond(
	core::pose::Pose & upstream_pose,
	core::pose::Pose & downstream_pose
);

enum OverlapDeleteMode{
	DELETE_UPSTREAM,
	DELETE_DOWNSTREAM
};

core::pose::PoseOP
append_pose_with_overlap(
	core::pose::Pose & upstream_pose,
	core::pose::Pose & downstream_pose,
	core::Size overlap, OverlapDeleteMode delete_overlap
);

} }

#endif
