#pragma once

#include "Map.hpp"
#include "Token.hpp"

class Conversions {
private:
	Conversions() = delete;
	~Conversions() = delete;

public:
	static const Utils::Map<int, OperatorType, 11> CharOperator;
	static const Utils::Map<TokenType, std::string, 5> TokenTypeToString;
};