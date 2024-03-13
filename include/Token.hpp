#pragma once
#include <string>
#include <iostream>
#include <variant>

enum class TokenType {
	NUMBER,
	OPERATOR,
	IDENTIFIER,
	ANY,
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

enum class IdentifierType {
	VARIABLE,
	FUNCTION,
	UNKNOWN
};

#include "Conversion.hpp"

class Token {
public:
	Token();
	Token(const double& num);
	Token(const std::string& function);
	Token(OperatorType operatorType);
	Token(const Token& token);
	Token& operator=(const Token& token);

	~Token();

	bool operator==(const Token& token) const;

	std::string str() const;
public:
	TokenType type = TokenType::NONTYPE;

	union {
		double number;

		struct IdentifierData {
			std::string name;
			IdentifierType type = IdentifierType::UNKNOWN;

			// Store the argument count for functions
			uint32_t argumentCount = 0;

			bool operator==(const IdentifierData& other) const {
				return other.type == type && other.name == name;
			}
		} identifier;

		OperatorType operatorType;
	};
};

std::ostream& operator<<(std::ostream& cout, const Token& token);