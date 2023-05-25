#include <infix_token.hpp>

InfixToken::InfixToken(InfixTokenType type, OperatorType oper) {
	this->type = type;
	this->oper = oper;
	this->number = 0.0f;
}
InfixToken::InfixToken(InfixTokenType type, float number) {
	this->type = type;
	this->oper = OperatorType::Undefined;
	this->number = number;
}
InfixToken::InfixToken(InfixTokenType type) {
	this->type = type;
	this->oper = OperatorType::Undefined;
	this->number = 0.0f;
}
InfixToken::InfixToken() {
	this->type = InfixTokenType::Unknown;
	this->oper = OperatorType::Undefined;
	this->number = 0.0f;
}