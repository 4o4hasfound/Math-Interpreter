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

	while (!m_operator.empty()) {
		const Token& o2 = m_operator.back();
		m_output.push_back(o2);
		m_operator.pop_back();
	}


	std::vector<bool> removed(m_output.size(), 0);
	std::stack<double> st;
	for (size_t i = 0; i < m_output.size(); ) {
		const Token& token = m_output[i];

		if (token.type == TokenType::NUMBER) {
			st.push(token.number);
		}
		else if (token.type == TokenType::OPERATOR) {
			switch (token.operatorType) {
			case OperatorType::ADDITION:
			{
				removed[i] = removed[i - 1] = 1;
				double d1 = st.top(); st.pop();
				double d2 = st.top(); st.pop();
				st.push(d1 + d2);
				break;
			}
			case OperatorType::SUBTRACTION:
			{
				removed[i] = removed[i - 1] = 1;
				double d1 = st.top(); st.pop();
				double d2 = st.top(); st.pop();
				st.push(d2 - d1);
				break;
			}
			case OperatorType::MULTIPLICATION:
			{
				removed[i] = removed[i - 1] = 1;
				double d1 = st.top(); st.pop();
				double d2 = st.top(); st.pop();
				st.push(d1 * d2);
				break;
			}
			case OperatorType::DIVISION:
			{
				removed[i] = removed[i - 1] = 1;
				double d1 = st.top(); st.pop();
				double d2 = st.top(); st.pop();
				st.push(d2 / d1);
				break;
			}
			case OperatorType::MODULUS:
			{
				removed[i] = removed[i - 1] = 1;
				double d1 = st.top(); st.pop();
				double d2 = st.top(); st.pop();
				st.push(static_cast<int>(d2) % static_cast<int>(d1));
				break;
			}
			case OperatorType::EXPONENTIATION:
			{
				removed[i] = removed[i - 1] = 1;
				double d1 = st.top(); st.pop();
				double d2 = st.top(); st.pop();
				st.push(std::pow(d2, d1));
				break;
			}
			case OperatorType::SQUAREROOT:
			{
				removed[i] = 1;
				double d1 = st.top(); st.pop();
				st.push(std::sqrt(d1));
				break;
			}
			}
		}

		while (++i < removed.size() && removed[i]);
	}
	m_result = st.top();
}

const double& Parser::getResult() const {
	return m_result;
}
