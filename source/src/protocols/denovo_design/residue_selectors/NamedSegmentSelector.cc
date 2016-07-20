// -*- mode:c++;tab-width:2;indent-tabs-mode:t;show-trailing-whitespace:t;rm-trailing-spaces:t -*-
// vi: set ts=2 noet:
//
// (c) Copyright Rosetta Commons Member Institutions.
// (c) This file is part of the Rosetta software suite and is made available under license.
// (c) The Rosetta software is developed by the contributing members of the Rosetta Commons.
// (c) For more information, see http://www.rosettacommons.org. Questions about this can be
// (c) addressed to University of Washington UW TechTransfer, email: license@u.washington.edu.

/// @file   core/pack/task/residue_selector/NamedSegmentSelector.hh
/// @brief  Selects residues from a named segment generated by StructureArchitects
/// @author Tom Linsky (tlinsky@uw.edu)

// Unit headers
#include <protocols/denovo_design/residue_selectors/NamedSegmentSelector.hh>
#include <protocols/denovo_design/residue_selectors/NamedSegmentSelectorCreator.hh>

// Protocol headers
#include <protocols/denovo_design/components/Segment.hh>
#include <protocols/denovo_design/components/StructureData.hh>
#include <protocols/denovo_design/components/StructureDataFactory.hh>
#include <protocols/denovo_design/util.hh>

// Core headers
#include <core/conformation/Residue.hh>
#include <core/select/residue_selector/ResidueSelectorFactory.hh>
#include <core/select/residue_selector/util.hh> // for xml schema utility functions
#include <core/pose/Pose.hh>

// Basic Headers
#include <basic/datacache/DataMap.hh>
#include <basic/Tracer.hh>

// Utility Headers
#include <utility/string_util.hh>
#include <utility/tag/Tag.hh>
#include <utility/tag/XMLSchemaGeneration.hh>

// C++ headers
#include <utility/assert.hh>

static THREAD_LOCAL basic::Tracer TR( "protocols.denovo_design.residue_selectors.NamedSegmentSelector" );

namespace protocols {
namespace denovo_design {
namespace residue_selectors {

using namespace core::select::residue_selector;

/// @brief Constructor.
///
NamedSegmentSelector::NamedSegmentSelector():
	ResidueSelector(),
	segment_( "" ),
	residues_( "" )
{}

NamedSegmentSelector::NamedSegmentSelector( std::string const & segment_name, std::string const & residues_str ):
	ResidueSelector(),
	segment_( segment_name ),
	residues_( residues_str )
{}

/// @brief Destructor.
///
NamedSegmentSelector::~NamedSegmentSelector() {}

/// @brief Clone function.
/// @details Copy this object and return owning pointer to the copy (created on the heap).
ResidueSelectorOP
NamedSegmentSelector::clone() const
{
	return ResidueSelectorOP( new NamedSegmentSelector( *this ) );
}

/// @brief "Apply" function.
/// @details Given the pose, generate a vector of bools with entries for every residue in the pose
/// indicating whether each residue is selected ("true") or not ("false").
ResidueSubset
NamedSegmentSelector::apply( core::pose::Pose const & pose ) const
{
	// get StructureData object from pose
	components::StructureDataFactory const & factory = *components::StructureDataFactory::get_instance();
	components::StructureData const sd = *factory.create_from_pose( pose );

	SignedResidSet const resids = resid_set();

	// subset to work on
	ResidueSubset subset( pose.total_residue(), false );

	// if residues are specified, the segment name provided must be an actual segment
	// rather than an alias or segment group
	if ( !resids.empty() ) {
		if ( !sd.has_segment( segment_ ) ) {
			std::stringstream msg;
			msg << class_name() << "::apply(): Specific residues are provided (" << residues_
				<< "), but the name provided (" << segment_
				<< ") does not match any named segments found in the pose ("
				<< SegmentNameList( sd.segments_begin(), sd.segments_end() )
				<< ").  Alias or segment groups cannot be specified with residues" << std::endl;
			msg << "SD=" << sd << std::endl;
			utility_exit_with_message( msg.str() );
		}
		components::Segment const & res = sd.segment( segment_ );
		for ( SignedResidSet::const_iterator r=resids.begin(); r!=resids.end(); ++r ) {
			core::Size const pose_resid = res.segment_to_pose( *r );
			TR.Debug << "Pose resid = " << pose_resid << " Segment resid = " << *r << std::endl;
			subset[ pose_resid ] = true;
		}
		return subset;
	}

	// compute intervals and use the to modify the subset
	Intervals const intervals = compute_residue_intervals( sd );
	for ( Intervals::const_iterator n=intervals.begin(); n!=intervals.end(); ++n ) {
		core::Size start = n->first;
		core::Size stop = n->second;
		debug_assert( start <= pose.total_residue() );
		debug_assert( stop <= pose.total_residue() );
		debug_assert( start >= 1 );
		debug_assert( stop >= 1 );
		if ( start > stop ) {
			core::Size tmp = stop;
			stop = start;
			start = tmp;
		}
		debug_assert( start <= stop );
		for ( core::Size resid=start; resid<=stop; ++resid ) {
			subset[ resid ] = true;
			TR.Debug << " Selected res " << pose.residue(resid).name() << " " << resid << " : " << subset[resid] << std::endl;
		}
	}

	return subset;
}

NamedSegmentSelector::Intervals
NamedSegmentSelector::compute_residue_intervals( components::StructureData const & sd ) const
{
	Intervals intervals;

	if ( sd.has_segment( segment_ ) ) {
		// Case 1: single segment specified
		components::Segment const & res = sd.segment( segment_ );
		intervals.push_back( std::make_pair( res.lower(), res.upper() ) );
	} else if ( sd.has_alias( segment_ ) ) {
		// Case 2: alias specified
		TR.Debug << segment_ << " is an alias." << std::endl;
		core::Size const start = sd.alias( segment_ );
		intervals.push_back( std::make_pair( start, start ) );
	} else if ( sd.has_segment_group( segment_ ) ) {
		// Case 3: segment group specified
		SegmentNames const segments = sd.segment_group( segment_ );
		TR.Debug << segment_ << " is a multi-segment group composed of " << segments << "." << std::endl;
		for ( SegmentNames::const_iterator c=segments.begin(); c!=segments.end(); ++c ) {
			TR.Debug << "Adding interval for segment " << *c << std::endl;
			components::Segment const & seg = sd.segment( *c );
			intervals.push_back( std::make_pair( seg.lower(), seg.upper() ) );
		}
	} else {
		std::stringstream error_msg;
		error_msg << class_name() << ": The segment name (" << segment_ << ") given was not found." << std::endl;
		error_msg << "SD = " << sd << std::endl;
		utility_exit_with_message( error_msg.str() );
	}
	return intervals;
}

NamedSegmentSelector::SignedResidSet
NamedSegmentSelector::resid_set() const
{
	SignedResidSet residueset;
	if ( residues_.empty() ) return residueset;

	utility::vector1< std::string > const residue_blocks = utility::string_split( residues_, ',' );
	for ( utility::vector1< std::string >::const_iterator b=residue_blocks.begin(); b!=residue_blocks.end(); ++b ) {
		utility::vector1< std::string > const ranges = utility::string_split( *b, ':' );
		if ( ( ! ranges.size() ) || ( ranges.size() > 2 ) ) {
			throw utility::excn::EXCN_Msg_Exception( "Bad residue range specified to NamedSegmentSelector: " + *b );
		}
		SignedResid start = boost::lexical_cast< SignedResid >( ranges[ 1 ] );
		SignedResid stop = start;
		if ( ranges.size() == 2 ) {
			stop = boost::lexical_cast< SignedResid >( ranges[ 2 ] );
		}
		if ( start > stop ) {
			SignedResid const tmp = start;
			start = stop;
			stop = tmp;
		}
		for ( SignedResid i=start; i<=stop; ++i ) {
			residueset.insert( i );
		}
	}
	return residueset;
}

/// @brief XML parse.
/// @details Parse RosettaScripts tags and set up this mover.
void
NamedSegmentSelector::parse_my_tag(
	utility::tag::TagCOP tag,
	basic::datacache::DataMap & )
{
	set_segment( tag->getOption< std::string >( "segment", segment_ ) );
	set_residues( tag->getOption< std::string >( "residues", residues_ ) );

	if ( segment_.empty() ) {
		std::stringstream error_message;
		error_message << "TomponentSelector::parse_my_tag Missing required option 'segment' in the input Tag" << std::endl;
		throw utility::excn::EXCN_Msg_Exception( error_message.str() );
	}
}

std::string
NamedSegmentSelector::get_name() const
{
	return NamedSegmentSelector::class_name();
}

std::string
NamedSegmentSelector::class_name()
{
	return "NamedSegment";
}

void
NamedSegmentSelector::provide_xml_schema( utility::tag::XMLSchemaDefinition & xsd )
{
	using namespace utility::tag;
	AttributeList attributes;
	attributes
		+ XMLSchemaAttribute( "segment", xs_string )
		+ XMLSchemaAttribute( "residues", xs_string );
	xsd_type_definition_w_attributes( xsd, class_name(), attributes );
}

void
NamedSegmentSelector::set_segment( std::string const & segment_name )
{
	segment_ = segment_name;
}

void
NamedSegmentSelector::set_residues( std::string const & residues_str )
{
	residues_ = residues_str;
}

ResidueSelectorOP
NamedSegmentSelectorCreator::create_residue_selector() const
{
	return ResidueSelectorOP( new NamedSegmentSelector );
}

std::string
NamedSegmentSelectorCreator::keyname() const
{
	return NamedSegmentSelector::class_name();
}

/// @brief Provide XSD information, allowing automatic evaluation of bad XML.
void
NamedSegmentSelectorCreator::provide_xml_schema( utility::tag::XMLSchemaDefinition & xsd ) const
{
	NamedSegmentSelector::provide_xml_schema( xsd );
}

} //protocols
} //denovo_design
} //residue_selectors