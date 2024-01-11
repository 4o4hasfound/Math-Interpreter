#include "Lexer.hpp"

const Utils::Map<int, Token, 10> Lexer::s_tokenMap{ {
	{'+', Token(TokenType::OPERATOR, OperatorType::ADDITION)},
	{'-', Token(TokenType::OPERATOR, OperatorType::SUBTRACTION)},
	{'*', Token(TokenType::OPERATOR, OperatorType::MULTIPLICATION)},
	{'/', Token(TokenType::OPERATOR, OperatorType::DIVISION)},
	{'%', Token(TokenType::OPERATOR, OperatorType::MODULUS)},
	{'^', Token(TokenType::OPERATOR, OperatorType::EXPONENTIATION)},
	{'กิ', Token(TokenType::OPERATOR, OperatorType::SQUAREROOT)},
	{'(', Token(TokenType::OPERATOR, OperatorType::LPARAN)},
	{')', Token(TokenType::OPERATOR, OperatorType::RPARAN)},
	{',', Token(TokenType::OPERATOR, OperatorType::COMMA)}
} };

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
		if (s_tokenMap.count(c)) {
			checkCurrentString();
			m_tokens.push_back(s_tokenMap[c]);
		}

		// Digit or String
		if (std::isalpha(c) || std::isdigit(c)) {
			if (!m_inString) {
				m_inString = true;
				m_lastPtr = m_ptr;
			}
		}
	}
	checkCurrentString();
}

bool Lexer::isNumber(std::string str) {
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
		m_tokens.push_back(Token(TokenType::NUMBER, std::stod(str)));
	}
	else {
		m_tokens.push_back(Token(TokenType::FUNCTION, str));
	}
	m_inString = false;
	m_lastPtr = m_ptr;
}
