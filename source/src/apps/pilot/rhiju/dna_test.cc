// -*- mode:c++;tab-width:2;indent-tabs-mode:t;show-trailing-whitespace:t;rm-trailing-spaces:t -*-
// vi: set ts=2 noet:
//
// (c) Copyright Rosetta Commons Member Institutions.
// (c) This file is part of the Rosetta software suite and is made available under license.
// (c) The Rosetta software is developed by the contributing members of the Rosetta Commons.
// (c) For more information, see http://www.rosettacommons.org. Questions about this can be
// (c) addressed to University of Washington CoMotion, email: license@uw.edu.

/// @file
/// @brief


// libRosetta headers
#include <core/types.hh>
#include <core/chemical/AA.hh>
#include <core/conformation/Residue.fwd.hh>
#include <core/chemical/ResidueTypeSet.hh>
#include <core/conformation/ResidueFactory.hh>
#include <core/chemical/ChemicalManager.hh>
#include <core/id/TorsionID.hh>

//#include <core/scoring/ScoringManager.hh>



#include <protocols/idealize/idealize.hh>

#include <protocols/viewer/viewers.hh>

#include <core/pose/Pose.hh>



#include <devel/init.hh>


#include <utility/vector1.hh>


#include <ObjexxFCL/format.hh>


// C++ headers
//#include <cstdlib>
#include <iostream>
#include <string>

//silly using/typedef


// option key includes



#include <basic/options/keys/OptionKeys.hh> // AUTO IWYU For options, OptionKeys

using namespace core;
using namespace protocols;
using namespace basic::options::OptionKeys;

using utility::vector1;

using ObjexxFCL::format::A;
using ObjexxFCL::format::I;
using ObjexxFCL::format::F;



using Matrix = numeric::xyzMatrix<Real>;


///////////////////////////////////////////////////////////////
void
dna_test(){

	using namespace core::chemical;
	using namespace core::conformation;
	using namespace core::pose;
	using namespace core::id;

	ResidueTypeSetCOP rsd_set( ChemicalManager::get_instance()->residue_type_set( "fa_standard" ) );

	// Create an extended pose from scratch.
	std::string sequence = "acgt";
	std::string outfile = "extended.pdb";
	std::cout << "ABOUT TO MAKE EXTENDED POSE in " << outfile << std::endl;

	Pose pose;

	Real const ALPHA_A_FORM( -64.11), BETA_A_FORM( 176.33), GAMMA_A_FORM( 53.08), EPSILON_A_FORM( -150.17), ZETA_A_FORM( -71.45), CHI_A_FORM( 79.43);

	for ( Size n = 1; n <= sequence.size(); n++ ) {

		char const new_seq = sequence[ n-1 ];
		// The representative type should have no/minimal variants
		ResidueTypeCOP new_rsd_type( rsd_set->get_representative_type_name1( new_seq ) );
		ResidueOP new_rsd( ResidueFactory::create_residue( *new_rsd_type ) );
		if ( n == 1 ) {
			pose.append_residue_by_bond( *new_rsd );
		} else {
			pose.append_polymer_residue_after_seqpos( *new_rsd, n-1, true /*build ideal geometry*/ );
		}
	}

	for ( Size n = 1; n <= sequence.size(); n++ ) {
		pose.set_torsion( TorsionID( n, BB, 1),  ALPHA_A_FORM);
		pose.set_torsion( TorsionID( n, BB, 2),   BETA_A_FORM);
		pose.set_torsion( TorsionID( n, BB, 3),  GAMMA_A_FORM);
		//pose.set_torsion( TorsionID( n, BB, 4),  DELTA_A_FORM);
		pose.set_torsion( TorsionID( n, BB, 5),  EPSILON_A_FORM);
		pose.set_torsion( TorsionID( n, BB, 6),  ZETA_A_FORM);
		pose.set_torsion( TorsionID( n, CHI, 1), CHI_A_FORM);
	}

	std::cout << pose.sequence() << std::endl;

	pose.dump_pdb( outfile );

}

///////////////////////////////////////////////////////////////
void*
my_main( void* )
{

	using namespace basic::options;

	dna_test();
	protocols::viewer::clear_conformation_viewers();
	exit( 0 );

}


///////////////////////////////////////////////////////////////////////////////
int
main( int argc, char * argv [] )
{

	try {

		using namespace basic::options;


		////////////////////////////////////////////////////////////////////////////
		// setup
		////////////////////////////////////////////////////////////////////////////
		devel::init(argc, argv);


		////////////////////////////////////////////////////////////////////////////
		// end of setup
		////////////////////////////////////////////////////////////////////////////

		protocols::viewer::viewer_main( my_main );


	} catch (utility::excn::Exception const & e ) {
		e.display();
		return -1;
	}

}
