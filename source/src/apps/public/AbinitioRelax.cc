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

#ifdef BOINC
#include <utility/boinc/boinc_util.hh>
#include <protocols/boinc/boinc.hh>
#include "boinc_zip.h"
#endif // BOINC

/// Must have this after BOINC stuff to avoid windows build error
#include <basic/options/option.hh>
#include <devel/init.hh>
#include <protocols/abinitio/AbrelaxApplication.hh>
#include <protocols/checkpoint/Checkpoint.hh>
#include <utility/excn/Exceptions.hh>

// Citation manager:
#include <basic/citation_manager/CitationManager.hh>

// option key includes
#include <basic/options/keys/run.OptionKeys.gen.hh>

#include <utility/vector1.hh>


int
main( int argc, char * argv [] )
{
	using namespace basic::options;
	using namespace basic::options::OptionKeys;
	using std::string;
	using utility::vector1;

	try {
		//YL, move the options register functions out of the boinc section
		// has to be called before devel::init. Which is really stupid.
		protocols::abinitio::AbrelaxApplication::register_options();

		// options, random initialization
		devel::init( argc, argv );
		if ( option[ run::checkpoint ] || option[ run::checkpoint_interval ].user() ) {
			protocols::checkpoint::checkpoint_with_interval( option[ run::checkpoint_interval ] );
		}

		//YL, create abrelax application then run it.
		protocols::abinitio::AbrelaxApplication abrelax;

		// Call fun before citation manager, since settings are set in run call:
		abrelax.run();

		{
			//Citation manager registration:
			basic::citation_manager::CitationCollectionList citations;
			abrelax.provide_citation_info( citations );
			basic::citation_manager::CitationManager::get_instance()->add_citations( citations );
		}

		basic::citation_manager::CitationManager::get_instance()->write_all_citations_and_unpublished_author_info();
	} catch (utility::excn::Exception& excn ) {
		excn.display();
		return -1;
	}
	return 0;
}
