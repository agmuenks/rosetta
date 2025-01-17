// -*- mode:c++;tab-width:2;indent-tabs-mode:t;show-trailing-whitespace:t;rm-trailing-spaces:t -*-
// vi: set ts=2 noet:
//
// (c) Copyright Rosetta Commons Member Institutions.
// (c) This file is part of the Rosetta software suite and is made available under license.
// (c) The Rosetta software is developed by the contributing members of the Rosetta Commons.
// (c) For more information, see http://www.rosettacommons.org. Questions about this can be
// (c) addressed to University of Washington CoMotion, email: license@uw.edu.

/// @file   core/pack/rotamer_set/RotamerSet_.hh
/// @brief  rotamer set implementation class
/// @author Andrew Leaver-Fay (leaverfa@email.unc.edu)


#ifndef INCLUDED_core_pack_rotamer_set_RotamerSubset_hh
#define INCLUDED_core_pack_rotamer_set_RotamerSubset_hh

//Unit headers
#include <core/pack/rotamer_set/RotamerSubset.fwd.hh>

// Package headers
#include <core/pack/rotamer_set/RotamerSet.hh>
#include <core/pack/rotamer_set/RotamerSets.fwd.hh>
#include <core/pack/task/PackerTask.fwd.hh>

//Project headers
#include <core/conformation/Residue.fwd.hh>
#include <core/chemical/ResidueType.fwd.hh>
#include <core/scoring/EnergyMap.fwd.hh>
#include <core/scoring/ScoreFunction.fwd.hh>
#include <core/scoring/trie/RotamerTrieBase.fwd.hh>
#include <core/pose/Pose.fwd.hh>


// Utility headers
#include <utility/graph/Graph.fwd.hh>
#include <utility/vector1.hh>

// C++ headers
#include <list>

#ifdef WIN32
#include <core/pack/rotamer_set/RotamerSet.fwd.hh>
#include <core/pack/rotamer_set/RotamerSet.hh>
#include <utility/graph/Graph.hh>
#endif

#ifdef    SERIALIZATION
// Cereal headers
#include <cereal/access.fwd.hpp>
#include <cereal/types/polymorphic.fwd.hpp>
#endif // SERIALIZATION

namespace core {
namespace pack {
namespace rotamer_set {

/// @brief Container for a subset of rotamers that have been created
/// by another rotamer set.  This subset object copies pointers to
/// the rotamers contained in another set, as opposed to cloning the
/// rotamers.  It's main purpose is to manage the bookkeeping involved
/// in packing with a subset of rotamers (when it might be faster
/// to use a subset and to create an interaction graph specifically
/// for that subset than to simply pass an abreviated list of rotamers
/// to the SimAnnealer with the "rot_to_pack" vector).
class RotamerSubset : public RotamerSet
{
public:
	typedef conformation::ResidueOP ResidueOP;
	typedef conformation::ResidueCOP ResidueCOP;
	typedef scoring::trie::RotamerTrieBaseOP RotamerTrieBaseOP;

public:
	RotamerSubset(
		RotamerSet & rotset,
		utility::vector1< Size > const & rotamer_subset
	);

	~RotamerSubset() override;

	void
	add_rotamer(
		conformation::Residue const & rotamer
	) override;

	void
	add_rotamer_into_existing_group(
		conformation::Residue const & rotamer
	) override;

	Size
	get_n_residue_types() const override;

	Size
	get_n_residue_groups() const override;

	Size
	get_residue_type_begin( Size which_restype ) const override;

	Size
	get_residue_group_begin( Size which_resgroup ) const override;

	Size
	get_n_rotamers_for_residue_type( Size which_restype ) const override;

	Size
	get_n_rotamers_for_residue_group( Size which_resgroup ) const override;

	/// @brief given a rotamer id, return an int which represents a type for this rotamer.
	Size
	get_residue_type_index_for_rotamer( Size which_rotamer ) const override ;

	Size
	get_residue_group_index_for_rotamer( Size which_rotamer ) const override;

	Size
	num_rotamers() const override;

	Size
	id_for_current_rotamer() const override;

	conformation::ResidueCOP
	rotamer( Size rot_id ) const override;

	basic::datacache::BasicDataCache &
	rotamer_data_cache( Size rot_id ) const override;

	conformation::Residue const &
	rotamer_ref( Size rot_id ) const override;

	Rotamers::const_iterator begin() const override { return rotamers_.begin(); }
	Rotamers::const_iterator end() const override { return rotamers_.end(); }

	conformation::ResidueOP
	nonconst_rotamer( Size rot_id ) override;

	void
	store_trie( Size method_enum_id, conformation::AbstractRotamerTrieOP trie ) override;

	conformation::AbstractRotamerTrieCOP
	get_trie( Size method_enum_id ) const override;

	/// @brief removes a single rotamer and causes a rotamer index update
	void
	drop_rotamer( Size rot_id ) override;

	/// @brief rotamers_to_delete must be of size nrotmaers -- each position
	/// in the array that's "true" is removed from the set of rotamers
	void
	drop_rotamers( utility::vector1< bool > const & rotamers_to_delete ) override;

	/// @brief deletes the rotamers in the list with the given indices.
	/// The indices of these rotamers is presumed to be those before any delete operation.
	/// e.g. if there are four rotamers, and rotamer_indices_to_delete includes 1 & 3,
	/// then the rotamers that will remain are the rotamers originally indexed as 2 and 4,
	/// even though their new indices will be 1 & 2.
	void
	drop_rotamers_by_index( utility::vector1< Size > const & rotamer_indices_to_delete ) override;

	/// @brief Give the pose a chance to stash any data needed by the _rotset_
	///        need nonconst access to pose
	void
	initialize_pose_for_rotset_creation(
		pose::Pose & /*pose*/
	) const override {}

	void
	show( std::ostream & out ) const override;

private:
	/// @brief (private) No copy-constructor
	RotamerSubset( RotamerSubset const & );

	/// @brief declare that a new block of residue types has begun, and that new residues
	/// are about to be pushed back. NOT IMPLEMENTED.
	//void
	//declare_new_residue_type();

	/// @brief appends a rotamer to the list of rotamers, and increments the count
	/// for the number of rotamers for the current value of n_residue_types. NOT IMPLEMENTED.
	//void
	//push_back_rotamer( conformation::ResidueOP );

	/// @brief Borrow (steal) a rotamer held by another RotamerSet
	/// without cloning that rotamer.  That is, both sets will now point
	/// at the same rotamer object, so if that rotamer changes for one set,
	/// it changes for both.
	void
	steal_rotamer( conformation::ResidueOP rotamer );


	/// @brief declare that a new block of residue types has begun, and that new residues
	/// are about to be pushed back.
	void
	prepare_for_new_residue_type( core::chemical::ResidueType const & restype );

	/// @brief should two residue types be considered the same residue type?
	bool
	different_restype( core::chemical::ResidueType const & rt1, core::chemical::ResidueType const & rt2 ) const;

	/// @brief should two residue types be considered to belong to the same residue-type group?
	bool
	different_resgroup( core::chemical::ResidueType const & rt1, core::chemical::ResidueType const & rt2 ) const;

	/// @brief This function should not be called directly -- it ought to be called only from prepare_for_new_residue_type
	void
	new_residue_type();

	/// @brief This function should not be called directly -- it ought to be called only from prepare_for_new_residue_type
	void
	new_residue_group();

	/// @brief appends a rotamer to the list of rotamers, and increments the count
	/// for the number of rotamers for the current value of n_residue_types.
	void
	push_back_rotamer( conformation::ResidueOP );

	void
	update_rotamer_offsets() const override;


public: // noop functions:

	void build_rotamers(
		pose::Pose const & the_pose,
		scoring::ScoreFunction const & scorefxn,
		task::PackerTask const & task,
		utility::graph::GraphCOP packer_neighbor_graph,
		bool use_neighbor_context = true
	) override;

	void build_dependent_rotamers(
		RotamerSets const & rotamer_sets,
		pose::Pose const & pose,
		scoring::ScoreFunction const & scorefxn,
		task::PackerTask const & task,
		utility::graph::GraphCOP packer_neighbor_graph
	) override;

	virtual
	void build_pwat_rotamers(
		pose::Pose const & pose,
		core::Size resid,
		utility::vector1< Vector > const & new_pwat_rotset
	);

	void
	compute_one_body_energies(
		pose::Pose const & pose,
		scoring::ScoreFunction const & scorefxn,
		task::PackerTask const & task,
		utility::graph::GraphCOP packer_neighbor_graph,
		utility::vector1< core::PackerEnergy > & energies,
		bool const no_update = false
	) const override;

	void
	compute_one_and_two_body_energies(
		pose::Pose const & pose,
		scoring::ScoreFunction const & scorefxn,
		task::PackerTask const & task,
		utility::graph::GraphCOP packer_neighbor_graph,
		utility::vector1< core::PackerEnergy > & one_body_energies,
		utility::vector1< utility::vector1< core::PackerEnergy > > & two_body_energies,
		utility::vector1< core::Size > & packable_neighbors ) const override;

	/// for OptE
	void
	compute_one_body_energy_maps(
		pose::Pose const & pose,
		scoring::ScoreFunction const & scorefxn,
		task::PackerTask const & task,
		utility::graph::GraphCOP packer_neighbor_graph,
		utility::vector1< scoring::EnergyMap > & energies ) const override;


	// DATA
private:

	mutable Rotamers rotamers_;
	mutable std::list< ResidueOP > rotamers_waiting_for_sort_;

	mutable Size n_residue_types_;
	mutable Size n_residue_groups_;
	mutable utility::vector1< Size > residue_type_rotamers_begin_;
	mutable utility::vector1< Size > residue_group_rotamers_begin_;
	mutable utility::vector1< Size > n_rotamers_for_restype_;
	mutable utility::vector1< Size > n_rotamers_for_resgroup_;

	mutable utility::vector1< Size > residue_type_for_rotamers_;
	mutable utility::vector1< Size > residue_group_for_rotamers_;

	utility::vector1< conformation::AbstractRotamerTrieOP > cached_tries_;

	mutable Size id_for_current_rotamer_;

	ResidueOP current_rotamer_copy_;
	mutable bool rotamer_offsets_require_update_;
#ifdef    SERIALIZATION
protected:
	friend class cereal::access;
	RotamerSubset();

public:
	template< class Archive > void save( Archive & arc ) const;
	template< class Archive > void load( Archive & arc );
#endif // SERIALIZATION

};

} // namespace rotamer_set
} // namespace pack
} // namespace core


#ifdef    SERIALIZATION
CEREAL_FORCE_DYNAMIC_INIT( core_pack_rotamer_set_RotamerSubset )
#endif // SERIALIZATION


#endif // INCLUDED_core_pack_RotamerSet_RotamerSet__HH

