#pragma once
#include <array>
#include <string>
#include <vector>
#include <algorithm>
#include <initializer_list>

#include "Token.hpp"
#include "Map.hpp"
#include "Conversion.hpp"

class Lexer {
public:
	Lexer(const std::string& text);
	~Lexer();

	void generate_token();
	const std::vector<Token>& getToken() const;
private:
	std::string m_text;

	int m_ptr, m_lastPtr;

	bool m_inString;

	std::vector<Token> m_tokens;
	
	static const std::array<char, 3> s_whiteSpaces;

	void GenerateTokens();

	bool isNumber(std::string str);
	
	// Checks the string from m_lastPtr to m_ptr
	// The add a token representing the type of the "string"
	// whether it's a number or function
	void checkCurrentString();
};