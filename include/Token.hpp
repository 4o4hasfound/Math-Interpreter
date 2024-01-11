#pragma once
#include <string>
#include <iostream>
#include <variant>

enum class TokenType {
	NUMBER,
	OPERATOR,
	FUNCTION,
	NONTYPE
};

enum class OperatorType {
	ADDITION,
	SUBTRACTION,
	MULTIPLICATION,
	DIVISION,
	MODULUS,
	EXPONENTIATION,
	SQUAREROOT,
	LPARAN,
	RPARAN,
	COMMA
};

class Token {
public:
	Token();
	Token(TokenType type, const double& num);
	Token(TokenType type, const std::string& function);
	Token(TokenType type, OperatorType operatorType);
	Token(const Token& token);
	Token& operator=(const Token& token);

	~Token();

	bool operator==(const Token& token) const;
public:
	TokenType type;

	union {
		double number;
		std::string function;
		OperatorType operatorType;
	};
};

std::ostream& operator<<(std::ostream& cout, const Token& token);