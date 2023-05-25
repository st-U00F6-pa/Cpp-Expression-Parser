#pragma once
#include <operator_types.hpp>

enum InfixTokenType { Unknown, Integer, Float, Infix, Prefix, LeftParenthesis, RightParenthesis };

class InfixToken {
public:
	InfixTokenType type;
	float number;
	OperatorType oper;

	InfixToken(InfixTokenType type, OperatorType oper);

	InfixToken(InfixTokenType type, float number);

	InfixToken(InfixTokenType type);

	InfixToken();
};