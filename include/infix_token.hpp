#pragma once
#include <operator_types.hpp>

enum InfixTokenType { Unknown, Integer, Float, Infix, Prefix, LeftParenthesis, RightParenthesis };

class InfixToken {
public:
	InfixTokenType type;
	double number;
	OperatorType oper;

	InfixToken(InfixTokenType type, OperatorType oper);

	InfixToken(InfixTokenType type, double number);

	InfixToken(InfixTokenType type);

	InfixToken();
};