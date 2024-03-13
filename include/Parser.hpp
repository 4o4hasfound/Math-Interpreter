#pragma once
#include <vector>
#include <stack>
#include <any>
#include <variant>
#include <unordered_map>
#include <math.h>

#include "Token.hpp"
#include "Lexer.hpp"
#include "Map.hpp"
#include "Functions.hpp"
#include "Errors.hpp"
#include "Conversion.hpp"
#include "varient_comparison.hpp"

class Configuration {
public:
	Configuration() {

	}
	Configuration(
		std::initializer_list<std::pair< std::variant<std::string, OperatorType>, FunctionObj> > functions,
		std::initializer_list<std::pair< std::string, double > > variables)
		: functions(functions.begin(), functions.end()), variables(variables.begin(), variables.end()) {

	}

	std::vector< std::pair< std::variant<std::string, OperatorType>, FunctionObj> > functions;
	std::vector< std::pair< std::string, double > > variables;
};

class Parser {
public:
	Parser(const Configuration& config = Configuration());

	void Evaluate(std::vector<Token> tokens);
	double Calculate() const;

	std::unordered_map<std::variant<std::string, OperatorType>, FunctionObj> functionMap;
	std::unordered_map<std::string, double> variable;
private:
	std::vector<Token> m_output;

	static const Utils::Map<OperatorType, uint32_t, 10> s_precedenceMap;

	// Checks 
	//	1. Whether a identifier is a function or a variable
	//	2. How many arguments is a function given
	void resolveIdentifier(std::vector<Token>& tokens);
	void explicitMultiplication(std::vector<Token>& tokens);
	void shuntingYard(const std::vector<Token>& tokens);

	void initFuncVar(const Configuration& config);
	const FunctionObj& getFunction(const Token& token) const;
};
