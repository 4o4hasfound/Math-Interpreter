#pragma once
#include <array>
#include <string>
#include <vector>
#include <algorithm>
#include <initializer_list>

#include "Token.hpp"
#include "Map.hpp"
#include "Conversion.hpp"

// Convert the input string into tokens
class Lexer {
public:
	Lexer(const std::string& text);
	~Lexer();

	const std::vector<Token>& getToken() const;
private:
	std::string m_text;

	// Points to the token that the lexer is currently processing
	int m_ptr;

	// Points to the last token that is the start of an identifier
	int m_lastPtr;

	bool m_inString;

	std::vector<Token> m_tokens;
	
	static const std::array<char, 3> s_whiteSpaces;

	void GenerateTokens();

	bool isNumber(const std::string& str);
	
	// Checks the string from m_lastPtr to m_ptr
	// The add a token representing the type of the "string"
	// whether it's a number or function
	void checkCurrentString();

	// Check if the '-' sign is a subtraction operator or a negative sign
	void resolveMinusSign();
};