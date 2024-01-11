#pragma once
#include <vector>

#include "Token.hpp"
#include "Lexer.hpp"
#include "Map.hpp"

class Parser {
public:
	Parser(const std::vector<Token>& tokens);

private:
	std::vector<Token> m_output;
	std::vector<Token> m_operator;

	static const Utils::Map<OperatorType, uint32_t, 10> s_precedenceMap;
};