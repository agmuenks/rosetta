// -*- mode:c++;tab-width:2;indent-tabs-mode:t;show-trailing-whitespace:t;rm-trailing-spaces:t -*-
// vi: set ts=2 noet:
//
// (c) Copyright Rosetta Commons Member Institutions.
// (c) This file is part of the Rosetta software suite and is made available under license.
// (c) The Rosetta software is developed by the contributing members of the Rosetta Commons.
// (c) For more information, see http://www.rosettacommons.org. Questions about this can be
// (c) addressed to University of Washington CoMotion, email: license@uw.edu.

/// @file   protocols/loops/loops_definers/LoopsFileDefinerCreator.hh
/// @brief  Header for LoopsFileDefinerCreator for the LoosFileDefiner load-time factory registration scheme
/// @author Matthew O'Meara (mattjomeara@gmail.com)

#ifndef INCLUDED_protocols_loops_loops_definers_LoopsFileDefinerCreator_HH
#define INCLUDED_protocols_loops_loops_definers_LoopsFileDefinerCreator_HH

// Unit Headers
#include <protocols/loops/loops_definers/LoopsDefinerCreator.hh>
#include <protocols/loops/loops_definers/LoopsFileDefinerCreator.fwd.hh>


#include <string>

namespace protocols {
namespace loops {
namespace loops_definers {

/// @brief creator for the LoopsFileDefiner class
class LoopsFileDefinerCreator : public LoopsDefinerCreator {
public:
	LoopsFileDefinerCreator();
	~LoopsFileDefinerCreator() override;

	LoopsDefinerOP create_loops_definer() const override;
	std::string type_name() const override;
	void provide_xml_schema( utility::tag::XMLSchemaDefinition & xsd ) const override;
};

} //namespace
} //namespace
} //namespace

#endif // include guard
