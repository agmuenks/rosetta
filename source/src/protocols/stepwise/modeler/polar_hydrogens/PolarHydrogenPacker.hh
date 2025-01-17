// -*- mode:c++;tab-width:2;indent-tabs-mode:t;show-trailing-whitespace:t;rm-trailing-spaces:t -*-
// vi: set ts=2 noet:
//
// (c) Copyright Rosetta Commons Member Institutions.
// (c) This file is part of the Rosetta software suite and is made available under license.
// (c) The Rosetta software is developed by the contributing members of the Rosetta Commons.
// (c) For more information, see http://www.rosettacommons.org. Questions about this can be
// (c) addressed to University of Washington CoMotion, email: license@uw.edu.

/// @file protocols/stepwise/modeler/polar_hydrogens/PolarHydrogenPacker.hh
/// @brief
/// @details
/// @author Rhiju Das, rhiju@stanford.edu


#ifndef INCLUDED_protocols_stepwise_modeler_polar_hydrogens_PolarHydrogenPacker_HH
#define INCLUDED_protocols_stepwise_modeler_polar_hydrogens_PolarHydrogenPacker_HH

#include <protocols/moves/Mover.hh>
#include <protocols/stepwise/modeler/polar_hydrogens/PolarHydrogenPacker.fwd.hh>
#include <core/scoring/hbonds/HBondOptions.fwd.hh>
#include <core/scoring/hbonds/HBondDatabase.fwd.hh>
#include <core/kinematics/Stub.fwd.hh>
#include <core/types.hh>
#include <core/scoring/hbonds/HBEvalTuple.hh>

#include <utility/vector1.hh> // AUTO IWYU For vector1

namespace protocols {
namespace stepwise {
namespace modeler {
namespace polar_hydrogens {

class PolarHydrogenPacker: public moves::Mover {

public:

	//constructor
	PolarHydrogenPacker();

	//destructor
	~PolarHydrogenPacker() override;

public:

	/// @brief  Apply the corresponding move to <pose>.
	void apply( core::pose::Pose & pose ) override;

	/// @brief  Return the name of the Mover.
	std::string get_name() const override { return "PolaryHydrogenPacker"; }

	void set_allow_virtual_o2prime_hydrogens( bool const setting ) { allow_virtual_o2prime_hydrogens_ = setting; }

private:

	void init();

	void check_hbond_score( core::Vector const & H_xyz,
		core::Vector const & D_xyz,
		core::Real & best_score,
		core::Vector & best_hydrogen_xyz );

	void
	get_best_hxyz( core::conformation::Residue const & residue,
		utility::vector1< core::Vector > const & ideal_hydrogen_xyz_positions,
		core::Size const abase,
		core::Size const abase2,
		core::Vector const & donor_xyz,
		core::Real & best_score,
		core::Vector & best_hydrogen_xyz );

	utility::vector1< core::Vector >
	get_ideal_hxyz_positions(
		core::conformation::Residue const & residue,
		core::conformation::Residue const & ideal_res,
		core::Size const j,
		core::kinematics::Stub const & current_input_stub,
		core::kinematics::Stub const & ideal_input_stub );

	void
	virtualize_poor_scoring_o2prime_hydrogens(
		core::pose::Pose & pose,
		core::Size const i,
		core::Size const j,
		core::Real const best_score );

	void
	get_possible_hbond_acceptors( core::pose::Pose const & pose, core::Size const moving_res, core::Size const atomno );

private:

	core::scoring::hbonds::HBondOptionsOP hbond_options_;
	core::scoring::hbonds::HBondDatabaseCOP hbond_database_;

	utility::vector1< utility::vector1< core::Vector > > possible_hbond_acceptors_;
	utility::vector1< core::scoring::hbonds::HBEvalTuple > hb_eval_tuples_;

	bool allow_virtual_o2prime_hydrogens_;
	bool disallow_vary_geometry_proton_chi_;
};

} //polar_hydrogens
} //modeler
} //stepwise
} //protocols

#endif
