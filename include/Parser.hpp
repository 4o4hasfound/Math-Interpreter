#pragma once
#include <vector>
#include <stack>

#include "Token.hpp"
#include "Lexer.hpp"
#include "Map.hpp"

class Parser {
public:
	Parser(const std::vector<Token>& tokens);

	const double& getResult() const;
private:
	std::vector<Token> m_output;
	std::vector<Token> m_operator;

	double m_result;

	static const Utils::Map<OperatorType, uint32_t, 10> s_precedenceMap;
};