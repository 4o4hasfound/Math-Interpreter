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
		m_tokens.push_back(Token(std::stod(str)));
	}
	else {
		m_tokens.push_back(Token(str));
	}
	m_inString = false;
	m_lastPtr = m_ptr;
}
