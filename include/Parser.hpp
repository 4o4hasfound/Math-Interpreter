#pragma once
#include <vector>
#include <stack>
#include <any>
#include <variant>
#include <unordered_map>

#include "Token.hpp"
#include "Lexer.hpp"
#include "Map.hpp"
#include "Functions.hpp"

class Parser {
public:
	Parser();

	void Evaluate(const std::vector<Token>& tokens);
	double Calculate() const;

	std::unordered_map<std::variant<std::string, OperatorType>, FunctionObj> functionMap = {
		{OperatorType::ADDITION,		Functions::Operators::Addition},
		{OperatorType::SUBTRACTION,		Functions::Operators::Subtraction},
		{OperatorType::MULTIPLICATION,	Functions::Operators::Multiplication},
		{OperatorType::DIVISION,		Functions::Operators::Division},
		{OperatorType::MODULUS,			Functions::Operators::Modulo},
		{OperatorType::EXPONENTIATION,	Functions::Operators::Exponentiation},
		{OperatorType::SQUAREROOT,		Functions::Operators::SquareRoot},

		{"max",		Functions::max},
		{"min",		Functions::min},
		{"sqrt",	Functions::sqrt},
		{"pow",		Functions::pow}
	};

	std::unordered_map<std::string, double> variable = {
		{"pi", 3.14159265},
		{"Pi", 3.14159265},
		{"e", 2.71827}
	};
private:
	std::vector<Token> m_output;
	std::vector<Token> m_operator;

	static const Utils::Map<OperatorType, uint32_t, 10> s_precedenceMap;

	const FunctionObj& getFunction(const Token& token) const;
	void shuntingYard(const std::vector<Token>& tokens);
};

template<typename T, class... Types>
inline bool operator==(const T& t, const std::variant<Types...>& v) {
	const T* c = std::get_if<T>(&v);

	return c && *c == t;
}

template<typename T, class... Types>
inline bool operator==(const std::variant<Types...>& v, const T& t) {
	return t == v;
}