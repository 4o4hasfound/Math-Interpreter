#include "Conversion.hpp"

const Utils::Map<int, OperatorType, 11> Conversions::CharOperator{ {
	{'+', OperatorType::ADDITION},
	{'-', OperatorType::SUBTRACTION},
	{'*', OperatorType::MULTIPLICATION},
	{'/', OperatorType::DIVISION},
	{'%', OperatorType::MODULUS},
	{'^', OperatorType::EXPONENTIATION},
	{'กิ', OperatorType::SQUAREROOT},
	{'(', OperatorType::LPARAN},
	{')', OperatorType::RPARAN},
	{',', OperatorType::COMMA},
	{'-', OperatorType::NEGATIVE}
} };

const Utils::Map<TokenType, std::string, 5> Conversions::TokenTypeToString{ {
	{TokenType::ANY, "Any"},
	{TokenType::IDENTIFIER, "Function"},
	{TokenType::NUMBER, "Number"},
	{TokenType::OPERATOR, "Operator"},
	{TokenType::NONTYPE, "None Type"}
} };
