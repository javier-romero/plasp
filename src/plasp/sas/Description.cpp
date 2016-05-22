#include <plasp/sas/Description.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include <boost/filesystem.hpp>

#include <plasp/utils/ParserException.h>
#include <plasp/utils/Parsing.h>
#include <plasp/sas/VariableTransition.h>

namespace plasp
{
namespace sas
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Description
//
////////////////////////////////////////////////////////////////////////////////////////////////////

Description::Description()
:	m_usesActionCosts{false}
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

Description Description::fromStream(std::istream &istream)
{
	Description description;

	std::setlocale(LC_NUMERIC, "C");

	istream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	description.parseVersionSection(istream);
	description.parseMetricSection(istream);
	description.parseVariablesSection(istream);
	description.parseMutexSection(istream);
	description.parseInitialStateSection(istream);
	description.parseGoalSection(istream);
	description.parseOperatorSection(istream);
	description.parseAxiomSection(istream);

	return description;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

Description Description::fromFile(const boost::filesystem::path &path)
{
	if (!boost::filesystem::is_regular_file(path))
		throw std::runtime_error("File does not exist: \"" + path.string() + "\"");

	std::ifstream fileStream(path.string(), std::ios::in);

	return Description::fromStream(fileStream);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool Description::usesActionCosts() const
{
	return m_usesActionCosts;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const Variables &Description::variables() const
{
	return m_variables;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const MutexGroups &Description::mutexGroups() const
{
	return m_mutexGroups;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const InitialState &Description::initialState() const
{
	return *m_initialState;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const Goal &Description::goal() const
{
	return *m_goal;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const Operators &Description::operators() const
{
	return m_operators;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const AxiomRules &Description::axiomRules() const
{
	return m_axiomRules;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Description::parseVersionSection(std::istream &istream) const
{
	// Version section
	utils::parseExpected<std::string>(istream, "begin_version");

	const auto formatVersion = utils::parse<size_t>(istream);

	if (formatVersion != 3)
		throw utils::ParserException("Unsupported SAS format version (" + std::to_string(formatVersion) + ")");

	utils::parseExpected<std::string>(istream, "end_version");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Description::parseMetricSection(std::istream &istream)
{
	utils::parseExpected<std::string>(istream, "begin_metric");

	m_usesActionCosts = utils::parse<bool>(istream);

	utils::parseExpected<std::string>(istream, "end_metric");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Description::parseVariablesSection(std::istream &istream)
{
	const auto numberOfVariables = utils::parse<size_t>(istream);
	m_variables.reserve(numberOfVariables);

	for (size_t i = 0; i < numberOfVariables; i++)
		m_variables.emplace_back(Variable::fromSAS(istream));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Description::parseMutexSection(std::istream &istream)
{
	const auto numberOfMutexGroups = utils::parse<size_t>(istream);
	m_mutexGroups.reserve(numberOfMutexGroups);

	for (size_t i = 0; i < numberOfMutexGroups; i++)
		m_mutexGroups.emplace_back(MutexGroup::fromSAS(istream, m_variables));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Description::parseInitialStateSection(std::istream &istream)
{
	m_initialState = std::make_unique<InitialState>(InitialState::fromSAS(istream, m_variables));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Description::parseGoalSection(std::istream &istream)
{
	m_goal = std::make_unique<Goal>(Goal::fromSAS(istream, m_variables));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Description::parseOperatorSection(std::istream &istream)
{
	const auto numberOfOperators = utils::parse<size_t>(istream);
	m_operators.reserve(numberOfOperators);

	for (size_t i = 0; i < numberOfOperators; i++)
		m_operators.emplace_back(Operator::fromSAS(istream, m_variables));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Description::parseAxiomSection(std::istream &istream)
{
	const auto numberOfAxiomRules = utils::parse<size_t>(istream);
	m_axiomRules.reserve(numberOfAxiomRules);

	for (size_t i = 0; i < numberOfAxiomRules; i++)
	{
		utils::parseExpected<std::string>(istream, "begin_rule");

		const auto numberOfConditions = utils::parse<size_t>(istream);

		AxiomRule::Conditions conditions;
		conditions.reserve(numberOfConditions);

		for (size_t j = 0; j < numberOfConditions; j++)
			conditions.emplace_back(AssignedVariable::fromSAS(istream, m_variables));

		const auto variableTransition = VariableTransition::fromSAS(istream, m_variables);

		if (&variableTransition.valueBefore() != &Value::Any)
			conditions.emplace_back(AssignedVariable(variableTransition.variable(), variableTransition.valueBefore()));

		const AxiomRule::Condition postcondition = {variableTransition.variable(), variableTransition.valueAfter()};
		const AxiomRule axiomRule = {std::move(conditions), std::move(postcondition)};
		m_axiomRules.push_back(std::move(axiomRule));

		utils::parseExpected<std::string>(istream, "end_rule");
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
