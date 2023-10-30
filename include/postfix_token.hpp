#pragma once
#include <operator_types.hpp>

enum PostfixTokenType { Number, BinaryOp, UnaryOp};

class PostfixToken {
public:
	PostfixTokenType type;
	double number;
	OperatorType oper;

	PostfixToken(PostfixTokenType type, OperatorType oper);

	PostfixToken(PostfixTokenType type, double number);
};