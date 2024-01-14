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

Parser::Parser() {

}

void Parser::Evaluate(const std::vector<Token>& tokens) {
	shuntingYard(tokens);
}

double Parser::Calculate() const {
	std::stack<double> st;
	for (size_t i = 0; i < m_output.size(); ++i) {
		const Token& token = m_output[i];

		if (token.type == TokenType::NUMBER) {
			st.push(token.number);
		}
		else if (token.type == TokenType::OPERATOR || token.type == TokenType::FUNCTION) {
			// Get function from operator/function token
			const FunctionObj& func = getFunction(token);

			// Construct the input list
			std::vector<double> input(func.numVariable);
			for (int j = func.numVariable - 1; j >= 0; --j) {
				input[j] = st.top();
				st.pop();
			}

			st.push(func.function(input));
		}
	}
	return st.top();
}

const FunctionObj& Parser::getFunction(const Token& token) const {
	switch (token.type) {
	case TokenType::OPERATOR:
		if (functionMap.count(token.operatorType)) {
			return functionMap.at(token.operatorType);
		}
		goto noFunction;
	case TokenType::FUNCTION:
		if (functionMap.count(token.function)) {
			return functionMap.at(token.function);
		}
		goto noFunction;
	}

noFunction:
	return FunctionObj{};
}

void Parser::shuntingYard(const std::vector<Token>& tokens) {
	for (size_t i = 0; i < tokens.size(); ++i) {
		const Token& token = tokens[i];

		switch (token.type) {
			// Number or Function
		case TokenType::NUMBER:
			m_output.push_back(token);
			break;
		case TokenType::FUNCTION:
			// Is Variable
			if (variable.count(token.function)) {
				m_output.push_back(Token(TokenType::NUMBER, variable.at(token.function)));
				break;
			}

			// Is Function
			m_operator.push_back(token);
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
					else {
						break;
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
					const Token o2 = m_operator.back();
					if (o2.operatorType == OperatorType::LPARAN) {
						break;
					}
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

			// Other Operators
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
					else {
						break;
					}
				}
				else {
					break;
				}
			}
			m_operator.push_back(token);
		}
	}

	// Clear the rest tokens
	while (!m_operator.empty()) {
		const Token& o2 = m_operator.back();
		m_output.push_back(o2);
		m_operator.pop_back();
	}
}
