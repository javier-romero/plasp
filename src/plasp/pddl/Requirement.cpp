#include <plasp/pddl/Requirement.h>

#include <boost/assert.hpp>
#include <boost/assign.hpp>
#include <boost/bimap.hpp>

#include <plasp/pddl/Identifier.h>
#include <plasp/utils/ParserException.h>

namespace plasp
{
namespace pddl
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Requirement
//
////////////////////////////////////////////////////////////////////////////////////////////////////

using RequirementTypeNames = boost::bimap<Requirement::Type, std::string>;

////////////////////////////////////////////////////////////////////////////////////////////////////

const RequirementTypeNames requirementTypesToPDDL = boost::assign::list_of<RequirementTypeNames::relation>
	(Requirement::Type::STRIPS, "strips")
	(Requirement::Type::Typing, "typing")
	(Requirement::Type::NegativePreconditions, "negative-preconditions")
	(Requirement::Type::DisjunctivePreconditions, "disjunctive-preconditions")
	(Requirement::Type::Equality, "equality")
	(Requirement::Type::ExistentialPreconditions, "existential-preconditions")
	(Requirement::Type::UniversalPreconditions, "universal-preconditions")
	(Requirement::Type::QuantifiedPreconditions, "quantified-preconditions")
	(Requirement::Type::ConditionalEffects, "conditional-effects")
	(Requirement::Type::Fluents, "fluents")
	(Requirement::Type::NumericFluents, "numeric-fluents")
	(Requirement::Type::ObjectFluents, "object-fluents")
	(Requirement::Type::ADL, "adl")
	(Requirement::Type::DurativeActions, "durative-actions")
	(Requirement::Type::DurationInequalities, "duration-inequalities")
	(Requirement::Type::ContinuousEffects, "continuous-effects")
	(Requirement::Type::DerivedPredicates, "derived-predicates")
	(Requirement::Type::TimedInitialLiterals, "timed-initial-literals")
	(Requirement::Type::Preferences, "preferences")
	(Requirement::Type::Constraints, "constraints")
	(Requirement::Type::ActionCosts, "action-costs");

////////////////////////////////////////////////////////////////////////////////////////////////////

const RequirementTypeNames requirementTypesToASP = boost::assign::list_of<RequirementTypeNames::relation>
	(Requirement::Type::STRIPS, "strips")
	(Requirement::Type::Typing, "typing")
	(Requirement::Type::NegativePreconditions, "negativePreconditions")
	(Requirement::Type::DisjunctivePreconditions, "disjunctivePreconditions")
	(Requirement::Type::Equality, "equality")
	(Requirement::Type::ExistentialPreconditions, "existentialPreconditions")
	(Requirement::Type::UniversalPreconditions, "universalPreconditions")
	(Requirement::Type::QuantifiedPreconditions, "quantifiedPreconditions")
	(Requirement::Type::ConditionalEffects, "conditionalEffects")
	(Requirement::Type::Fluents, "fluents")
	(Requirement::Type::NumericFluents, "numericFluents")
	(Requirement::Type::ObjectFluents, "objectFluents")
	(Requirement::Type::ADL, "adl")
	(Requirement::Type::DurativeActions, "durativeActions")
	(Requirement::Type::DurationInequalities, "durationInequalities")
	(Requirement::Type::ContinuousEffects, "continuousEffects")
	(Requirement::Type::DerivedPredicates, "derivedPredicates")
	(Requirement::Type::TimedInitialLiterals, "timedInitialLiterals")
	(Requirement::Type::Preferences, "preferences")
	(Requirement::Type::Constraints, "constraints")
	(Requirement::Type::ActionCosts, "actionCosts");

////////////////////////////////////////////////////////////////////////////////////////////////////

Requirement::Type Requirement::fromPDDL(utils::Parser &parser)
{
	const auto requirementName = parser.parseIdentifier(isIdentifier);

	const auto match = requirementTypesToPDDL.right.find(requirementName);

	if (match == requirementTypesToPDDL.right.end())
		throw utils::ParserException(parser.row(), parser.column(), "Unknown PDDL requirement \"" + requirementName + "\"");

	return match->second;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Requirement::toPDDL(std::ostream &ostream, Requirement::Type requirementType)
{
	const auto match = requirementTypesToPDDL.left.find(requirementType);

	BOOST_ASSERT(match != requirementTypesToPDDL.left.end());

	ostream << match->second;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Requirement::toASP(std::ostream &ostream, Requirement::Type requirementType)
{
	const auto match = requirementTypesToASP.left.find(requirementType);

	BOOST_ASSERT(match != requirementTypesToASP.left.end());

	ostream << match->second;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}