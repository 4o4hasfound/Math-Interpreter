#include "Parser.hpp"

const Utils::Map<OperatorType, uint32_t, 10> Parser::s_precedenceMap{
	{OperatorType::ADDITION, 1},
	{OperatorType::SUBTRACTION, 1},
	{OperatorType::MULTIPLICATION, 2},
	{OperatorType::DIVISION, 2},
	{OperatorType::MODULUS, 2},
	{OperatorType::EXPONENTIATION, 3},
	{OperatorType::SQUAREROOT, 3},
	{OperatorType::LPARAN, 0},
	{OperatorType::RPARAN, 0},
	{OperatorType::COMMA, 0}
};

Parser::Parser(const std::vector<Token>& tokens) {
	for (size_t i = 0; i < tokens.size(); ++i) {
		const Token& token = tokens[i];

		switch (token.type) {
			// Number or Function
			case TokenType::NUMBER:
			case TokenType::FUNCTION:
				if (token.function == ",") {

				}
				m_output.push_back(token);
				break;
			
			// Operator
			case TokenType::OPERATOR:
				// Comma
				if (token.operatorType == OperatorType::COMMA) {
					while (!m_operator.empty()) {
						const Token& o2 = m_operator.back();
						if (o2.operatorType != OperatorType::LPARAN) {
							m_output.push_back(o2);
							m_operator.pop_back();
						}
					}
					break;
				}

				// Left paranthesis
				if (token.operatorType == OperatorType::LPARAN) {
					m_operator.push_back(token);
					break;
				}

				// Right paranthesis
				if (token.operatorType == OperatorType::RPARAN) {
					while (!m_operator.empty()) {
						const Token& o2 = m_operator.back();
						m_output.push_back(o2);
						m_operator.pop_back();
					}
					m_operator.pop_back();
					if (!m_operator.empty() && m_operator.back().type == TokenType::FUNCTION) {
						const Token& o2 = m_operator.back();
						m_output.push_back(o2);
						m_operator.pop_back();
					}
					break;
				}
				while (!m_operator.empty()) {
					const Token& o2 = m_operator.back(); 
					if (o2.type == TokenType::OPERATOR && o2.operatorType != OperatorType::LPARAN) {
						uint32_t precedenceO2 = s_precedenceMap[o2.operatorType];
						uint32_t precedenceToken = s_precedenceMap[token.operatorType];

						if (precedenceO2 > precedenceToken ||
							(precedenceO2 == precedenceToken && (o2.operatorType == OperatorType::ADDITION ||
								o2.operatorType == OperatorType::SUBTRACTION ||
								o2.operatorType == OperatorType::MULTIPLICATION ||
								o2.operatorType == OperatorType::DIVISION ||
								o2.operatorType == OperatorType::MODULUS))) {
							m_output.push_back(o2);
							m_operator.pop_back();
						}
					}
				}
				m_operator.push_back(token);
		}
	}
	int i = 0;

	while (!m_operator.empty()) {
		const Token& o2 = m_operator.back();
		m_output.push_back(o2);
		m_operator.pop_back();
	}

}