// -*- mode:c++;tab-width:2;indent-tabs-mode:t;show-trailing-whitespace:t;rm-trailing-spaces:t -*-
// vi: set ts=2 noet:
//
// (c) Copyright Rosetta Commons Member Institutions.
// (c) This file is part of the Rosetta software suite and is made available under license.
// (c) The Rosetta software is developed by the contributing members of the Rosetta Commons.
// (c) For more information, see http://www.rosettacommons.org. Questions about this can be
// (c) addressed to University of Washington CoMotion, email: license@uw.edu.

/// @file protocols/protein_mpnn/ProteinMPNNProbabilitiesMetric.fwd.hh
/// @brief A metric for predicting amino acid probabilities using ProteinMPNN (Dauparas et al.).
/// @author Moritz Ertelt (moritz.ertelt@googlemail.com)


#ifndef INCLUDED_protocols_protein_mpnn_ProteinMPNNProbabilitiesMetric_fwd_hh
#define INCLUDED_protocols_protein_mpnn_ProteinMPNNProbabilitiesMetric_fwd_hh

// Utility headers
#include <utility/pointer/owning_ptr.hh>

// Forward
namespace protocols {
namespace protein_mpnn {

class ProteinMPNNProbabilitiesMetric;

using ProteinMPNNProbabilitiesMetricOP = utility::pointer::shared_ptr< ProteinMPNNProbabilitiesMetric >;
using ProteinMPNNProbabilitiesMetricCOP = utility::pointer::shared_ptr< ProteinMPNNProbabilitiesMetric const >;

} //protein_mpnn
} //protocols

#endif //INCLUDED_protocols_protein_mpnn_ProteinMPNNProbabilitiesMetric_fwd_hh
