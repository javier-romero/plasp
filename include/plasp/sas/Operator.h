#ifndef __SAS__OPERATOR_H
#define __SAS__OPERATOR_H

#include <string>
#include <vector>

#include <plasp/sas/Effect.h>
#include <plasp/sas/Predicate.h>
#include <plasp/sas/Variable.h>

namespace plasp
{
namespace sas
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Operator
//
////////////////////////////////////////////////////////////////////////////////////////////////////

struct Operator
{
	using Condition = AssignedVariable;
	using Conditions = std::vector<Condition>;
	using Effects = std::vector<Effect>;

	Predicate predicate;
	Conditions preconditions;
	Effects effects;
	size_t costs;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
