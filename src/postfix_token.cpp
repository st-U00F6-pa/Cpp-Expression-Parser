#include <postfix_token.hpp>

PostfixToken::PostfixToken(PostfixTokenType type, OperatorType oper) {
	this->type = type;
	this->oper = oper;
	this->number = 0.0f;
}
PostfixToken::PostfixToken(PostfixTokenType type, float number) {
	this->type = type;
	this->oper = OperatorType::Undefined;
	this->number = number;
}