#ifndef __PLASP__PDDL__TRANSLATION__PRECONDITION_H
#define __PLASP__PDDL__TRANSLATION__PRECONDITION_H

#include <pddlparse/AST.h>

#include <plasp/output/Formatting.h>
#include <plasp/output/TranslatorException.h>

#include <plasp/pddl/translation/Predicate.h>
#include <plasp/pddl/translation/Primitives.h>

namespace plasp
{
namespace pddl
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Precondition
//
////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename PrintObjectName>
inline void translatePrecondition(output::ColorStream &outputStream, const ::pddl::ast::Precondition &precondition, const std::string &objectType, PrintObjectName printObjectName)
{
	const auto handleUnsupported =
		[](const auto &)
		{
			throw output::TranslatorException("only “and” expressions and (negated) predicates supported as action preconditions currently");
		};

	const auto handlePredicate =
		[&](const ::pddl::ast::PredicatePointer &predicate, bool isPositive = true)
		{
			outputStream << std::endl << output::Function("precondition") << "(";
			printObjectName();
			outputStream << ", ";
			translatePredicateToVariable(outputStream, *predicate, isPositive);
			outputStream << ") :- " << output::Function(objectType.c_str()) << "(";
			printObjectName();
			outputStream << ").";
		};

	const auto handleAtomicFormula =
		[&](const ::pddl::ast::AtomicFormula &atomicFormula)
		{
			atomicFormula.match(handlePredicate, handleUnsupported);
		};

	const auto handleNot =
		[&](const ::pddl::ast::NotPointer<::pddl::ast::Precondition> &not_)
		{
			if (!not_->argument.is<::pddl::ast::AtomicFormula>() || !not_->argument.get<::pddl::ast::AtomicFormula>().is<::pddl::ast::PredicatePointer>())
				handleUnsupported(not_);

			const auto &predicate = not_->argument.get<::pddl::ast::AtomicFormula>().get<::pddl::ast::PredicatePointer>();

			handlePredicate(predicate, false);
		};

	const auto handleAnd =
		[&](const ::pddl::ast::AndPointer<::pddl::ast::Precondition> &and_)
		{
			for (const auto &argument : and_->arguments)
				translatePrecondition(outputStream, argument, objectType, printObjectName);
		};

	precondition.match(handleAtomicFormula, handleNot, handleAnd, handleUnsupported);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
