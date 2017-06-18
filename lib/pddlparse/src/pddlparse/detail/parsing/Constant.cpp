#include <pddlparse/detail/parsing/Constant.h>

#include <pddlparse/AST.h>
#include <pddlparse/Exception.h>

namespace pddl
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Constant
//
////////////////////////////////////////////////////////////////////////////////////////////////////

std::experimental::optional<ast::ConstantPointer> findConstant(const std::string &constantName, ast::ConstantDeclarations &constantDeclarations)
{
	const auto matchingConstant = std::find_if(constantDeclarations.begin(), constantDeclarations.end(),
		[&](const auto &constantDeclaration)
		{
			return constantDeclaration->name == constantName;
		});

	if (matchingConstant == constantDeclarations.end())
		return std::experimental::nullopt;

	return std::make_unique<ast::Constant>(matchingConstant->get());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

std::experimental::optional<ast::ConstantPointer> findConstant(const std::string &constantName, ASTContext &astContext)
{
	auto constant = findConstant(constantName, astContext.domain->constants);

	if (constant)
		return std::move(constant.value());

	if (astContext.problem)
	{
		constant = findConstant(constantName, astContext.problem.value()->objects);

		if (constant)
			return std::move(constant.value());
	}

	return std::experimental::nullopt;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

std::experimental::optional<ast::ConstantPointer> testParsingConstant(Context &context, ASTContext &astContext)
{
	auto &tokenizer = context.tokenizer;

	const auto constantName = tokenizer.getIdentifier();
	auto constant = findConstant(constantName, astContext);

	if (!constant)
		return std::experimental::nullopt;

	return std::move(constant.value());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ast::ConstantPointer parseConstant(Context &context, ASTContext &astContext)
{
	auto &tokenizer = context.tokenizer;

	const auto constantName = tokenizer.getIdentifier();
	auto constant = findConstant(constantName, astContext);

	if (!constant)
		throw ParserException(tokenizer, "undeclared constant “" + constantName + "”");

	return std::move(constant.value());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
