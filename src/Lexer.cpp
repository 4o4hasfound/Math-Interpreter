#include "Lexer.hpp"

const std::array<char, 3> Lexer::s_whiteSpaces{ { ' ', '\t', '\n' } };

Lexer::Lexer(const std::string& text) 
	: m_text(text), m_ptr(0), m_lastPtr(0), m_inString(false) {
	GenerateTokens();
}

Lexer::~Lexer() {

}

const std::vector<Token>& Lexer::getToken() const {
	return m_tokens;
}

void Lexer::GenerateTokens() {
	for (; m_ptr < m_text.size(); ++m_ptr) {
		const char& c = m_text[m_ptr];
		
		// White Space
		if (std::count(s_whiteSpaces.begin(), s_whiteSpaces.end(), c)) {
			checkCurrentString();
			continue;
		}
		
		// Operators
		if (Conversions::CharOperator.count(c)) {
			checkCurrentString();

			// Decide wether to use minus token or negative token
			if (c == '-') {
				resolveMinusSign();
				continue;
			}

			m_tokens.push_back(Conversions::CharOperator[c]);
		}

		// Digit or String
		if (c >= 0 && std::isalpha(c) || std::isdigit(c)) {
			if (!m_inString) {
				m_inString = true;
				m_lastPtr = m_ptr;
			}
		}
	}
	checkCurrentString();
}

bool Lexer::isNumber(const std::string& str) {
	std::string::const_iterator it = str.begin();
	int decimalPointCount = 0;
	while (it != str.end()) {
		if (std::isdigit(*it)) {
			++it;
		}
		else if (*it == '.' && decimalPointCount < 1) {
			++it;
			++decimalPointCount;
		}
		else {
			break;
		}
	}
	return !str.empty() && it == str.end();
}

void Lexer::checkCurrentString() {
	if (!m_inString || m_lastPtr == m_ptr) {
		return;
	}
	const std::string str = m_text.substr(m_lastPtr, m_ptr - m_lastPtr);
	if (isNumber(str)) {
		m_tokens.push_back(Token(std::stod(str)));
	}
	else {
		m_tokens.push_back(Token(str));
	}
	m_inString = false;
	m_lastPtr = m_ptr;
}

void Lexer::resolveMinusSign() {
	if (m_ptr + 1 >= m_text.size() || m_text[m_ptr + 1] == ' ') {
		m_tokens.emplace_back(OperatorType::SUBTRACTION);
		return;
	}
	if (m_ptr == 0) {
		m_tokens.emplace_back(OperatorType::NEGATIVE);
		return;
	}

	// If the last token is a number / an identifier / a lparan
	// then the token is a subtraction operator
	if (m_tokens.back().type == TokenType::NUMBER || 
		(m_tokens.back().type == TokenType::OPERATOR && m_tokens.back().operatorType == OperatorType::RPARAN)) {
		m_tokens.emplace_back(OperatorType::SUBTRACTION);
		return;
	}

	m_tokens.emplace_back(OperatorType::NEGATIVE);
}
