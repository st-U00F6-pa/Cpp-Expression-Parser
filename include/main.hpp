#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <stack>
#include <cmath>
#include <map>
#include <operator_types.hpp>
#include <infix_token.hpp>
#include <postfix_token.hpp>

int main();

std::vector<InfixToken>* read_expression(char* expression);
std::vector<PostfixToken>* convert_expression(std::vector<InfixToken>* expression);
double evaluate_expression(std::vector<PostfixToken>* expression);
bool isoper(char c);
