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

	// Resolve the identifier tokens and turn the tokens into Reversed Polish notation using shunting yard algorithm
	void Evaluate(std::vector<Token> tokens);

	// Calculate the result using the evaluated result
	double Calculate() const;

	std::unordered_map<std::variant<std::string, OperatorType>, FunctionObj> functionMap;
	std::unordered_map<std::string, double> variable;
private:
	std::vector<Token> m_output;

	static const Utils::Map<OperatorType, uint32_t, 11> s_precedenceMap;

	// Checks 
	//	1. Whether a identifier is a function or a variable
	//	2. How many arguments is a function given
	void resolveIdentifier(std::vector<Token>& tokens);

	// Insert a multiplication operator whenever an implicit multiplication occurs
	void implicitMultiplication(std::vector<Token>& tokens);

	// Performs shunting yard algorithm
	void shuntingYard(const std::vector<Token>& tokens);

	void initFuncVar(const Configuration& config);
	const FunctionObj& getFunction(const Token& token) const;
};
