#include "Parser.hpp"

const Utils::Map<OperatorType, uint32_t, 11> Parser::s_precedenceMap{
	{OperatorType::ADDITION, 1},
	{OperatorType::SUBTRACTION, 1},
	{OperatorType::MULTIPLICATION, 2},
	{OperatorType::DIVISION, 2},
	{OperatorType::MODULUS, 2},
	{OperatorType::EXPONENTIATION, 3},
	{OperatorType::SQUAREROOT, 3},
	{OperatorType::NEGATIVE, 4},
	{OperatorType::LPARAN, 0},
	{OperatorType::RPARAN, 0},
	{OperatorType::COMMA, 0}
};

Parser::Parser(const Configuration& config) {
	initFuncVar(config);
}

void Parser::Evaluate(std::vector<Token> tokens) {
	m_output.clear();

	resolveIdentifier(tokens);
	implicitMultiplication(tokens);
	shuntingYard(tokens);
}

double Parser::Calculate() const {
	std::stack<Token> st;
	for (size_t i = 0; i < m_output.size(); ++i) {
		const Token& token = m_output[i];

		if (token.type == TokenType::NUMBER) {
			st.push(token);
		}
		else if (token.type == TokenType::OPERATOR || token.type == TokenType::IDENTIFIER) {
			// Get function from operator/function token
			const FunctionObj& func = getFunction(token);

			if (token.identifier.type == IdentifierType::FUNCTION && token.identifier.argumentCount != func.variableCount) {
				throw ArgumentError(token.identifier.name, token.identifier.argumentCount, func.variableCount);
			}
			// Construct the input list from back to front
			std::vector<Token> input(func.variableCount);
			for (int j = func.variableCount - 1; j >= 0; --j) {
				if (st.empty()) {
					throw ArgumentError(
						token.str(),
						func.variableCount - 1 - j,
						func.variableCount
					);
				}

				input[j] = st.top();
				st.pop();
			}

			st.push(func.function(input));
		}
	}
	if (st.empty()) {
		throw GrammarError("Didn't provide a valid string to calculate");
	}
	return st.top().number;
}

void Parser::initFuncVar(const Configuration& config) {
	functionMap = {
		   {OperatorType::ADDITION,		Functions::Operators::Addition},
		   {OperatorType::SUBTRACTION,		Functions::Operators::Subtraction},
		   {OperatorType::MULTIPLICATION,	Functions::Operators::Multiplication},
		   {OperatorType::DIVISION,		Functions::Operators::Division},
		   {OperatorType::MODULUS,			Functions::Operators::Modulo},
		   {OperatorType::EXPONENTIATION,	Functions::Operators::Exponentiation},
		   {OperatorType::SQUAREROOT,		Functions::Operators::SquareRoot},
		   {OperatorType::NEGATIVE,		Functions::Operators::Negative},

		   {"abs",		Functions::abs},
		   {"max",		Functions::max},
		   {"min",		Functions::min},
		   {"sqrt",		Functions::sqrt},
		   {"pow",		Functions::pow},
		   {"sin",		Functions::sin},
		   {"cos",		Functions::cos},
		   {"tan",		Functions::tan},
		   {"sinh",		Functions::sinh},
		   {"cosh",		Functions::cosh},
		   {"tanh",		Functions::tanh},
		   {"degree",	Functions::degree},
		   {"radian",	Functions::radian}
	};
	functionMap.insert(config.functions.begin(), config.functions.end());

	variable = {
		{"pi", 3.14159265},
		{"Pi", 3.14159265},
		{"e", 2.71827}
	};
	variable.insert(config.variables.begin(), config.variables.end());
}

const FunctionObj& Parser::getFunction(const Token& token) const {
	switch (token.type) {
	case TokenType::OPERATOR:
		if (functionMap.count(token.operatorType)) {
			return functionMap.at(token.operatorType);
		}
		break;
	case TokenType::IDENTIFIER:
		if (functionMap.count(token.identifier.name)) {
			return functionMap.at(token.identifier.name);
		}
		break;
	}

	throw FunctionNotFound(token.str());
}

void Parser::implicitMultiplication(std::vector<Token>& tokens) {
	for (size_t i = 0; i < tokens.size(); ++i) {
		const Token& token = tokens[i];

		if (
			(token.type == TokenType::IDENTIFIER && token.identifier.type == IdentifierType::FUNCTION) 
			|| (token.type != TokenType::IDENTIFIER && token.type != TokenType::NUMBER && (
				token.type == TokenType::OPERATOR && token.operatorType != OperatorType::RPARAN
				)) ) {
			continue;
		}

		if (i == tokens.size() - 1) {
			break;
		}

		if (tokens[i + 1].type == TokenType::OPERATOR && tokens[i + 1].operatorType == OperatorType::LPARAN) {
			tokens.insert(tokens.begin() + (i + 1), Token(OperatorType::MULTIPLICATION));
			continue;
		}

		if (tokens[i + 1].type != TokenType::OPERATOR || tokens[i+1].operatorType != OperatorType::LPARAN) {
			continue;
		}

		tokens.insert(tokens.begin() + (i + 1), Token(OperatorType::MULTIPLICATION));
	}
}

void Parser::resolveIdentifier(std::vector<Token>& tokens) {
	struct IdentifierData {
		Token& token;

		// Keeps track of the paranthesis count so that it identifies the correct closing paranthesis for the current identifier
		uint32_t paranthesisStack = 0;

		// To calculate the function argument count later
		uint32_t commaCount = 0;

		bool lparanEncountered = false;
	};

	bool inIdentifier = false;
	
	for (size_t i = 0; i < tokens.size(); ++i) {
		Token& t = tokens[i];

		// Skip until a identifier is encountered
		if (t.type != TokenType::IDENTIFIER) {
			continue;
		}

		// Token exist at the end of the tokens, must be a variable
		if (i == tokens.size() - 1) {
			t.identifier.type = IdentifierType::VARIABLE;
			break;
		}
		std::stack<IdentifierData> st;
		st.push({ t });

		while (!st.empty()) {
			// Expect a closing paranthesis
			if (i >= tokens.size()) {
				throw GrammarError("Paranthesises don't match");
			}

			IdentifierData& data = st.top();
			for (++i; i < tokens.size(); ++i) {
				Token& token = tokens[i];				

				// Haven't encounter the left paranthesis
				if (!data.lparanEncountered) {
					// Are not followed by a left paranthesis
					if (token.type != TokenType::OPERATOR || token.operatorType != OperatorType::LPARAN) {
						data.token.identifier.type = IdentifierType::VARIABLE;

						// Decrease it so it doesn't increases twice at the end of the outer for loop
						--i;

						st.pop();
						break;
					}
					data.lparanEncountered = true;

					// Are followed by a left paranthesis, could be both Function and Variable
					// Function has a higher priority, if it exists in the functionMap, then mark it as a function,
					// else mark it as a variable
					if (!functionMap.count(data.token.identifier.name)) {
						data.token.identifier.type = IdentifierType::VARIABLE;

						// Decrease it so it doesn't increases twice at the end of the outer for loop
						--i;

						st.pop();
						break;
					}
					// It is a function, but keep running this for loop to figure out its argument count
					data.token.identifier.type = IdentifierType::FUNCTION;
				}
			
				// Encounter another identifier, first process that identifier
				if (token.type == TokenType::IDENTIFIER) {
					st.push({ token });
					break;
				}

				// Continue until a comma or a right paranthesis is met
				if (token.type == TokenType::OPERATOR) {
					if (token.operatorType == OperatorType::COMMA) {
						++data.commaCount;
					}
					if (token.operatorType == OperatorType::LPARAN) {
						++data.paranthesisStack;
					}
					if (token.operatorType == OperatorType::RPARAN) {
						--data.paranthesisStack;
						if (data.paranthesisStack == 0) {
							// The function has 0/1 argument
							if (data.commaCount == 0) {
								// The token before the right paranthesis is a left paranthesis
								// meaning the function has 0 argument
								if (tokens[i - 1].type == TokenType::OPERATOR && tokens[i - 1].operatorType == OperatorType::LPARAN) {
									data.token.identifier.argumentCount = 0;
								}
								// The token has 1 argument
								else {
									data.token.identifier.argumentCount = 1;
								}
							}
							else {
								// The function has commaCount + 1 argument
								data.token.identifier.argumentCount = data.commaCount + 1;
							}
							st.pop();
							break;
						}
					}
				}
			}
		}
	}
}

void Parser::shuntingYard(const std::vector<Token>& tokens) {
	std::vector<Token> operatorStack;

	for (size_t i = 0; i < tokens.size(); ++i) {
		const Token& token = tokens[i];

		switch (token.type) {
		// Number or Function
		case TokenType::NUMBER:
			m_output.push_back(token);
			break;
		case TokenType::IDENTIFIER:
			// Is Variable
			if (token.identifier.type == IdentifierType::VARIABLE) {
				if (variable.count(token.identifier.name)) {
					m_output.push_back(Token(variable.at(token.identifier.name)));
					break;
				}
				else {
					throw VariableNotFound(token.identifier.name);
				}
			}

			// Is Function
			operatorStack.push_back(token);
			break;

		// Operator
		case TokenType::OPERATOR:
			// Comma
			if (token.operatorType == OperatorType::COMMA) {
				while (!operatorStack.empty()) {
					const Token& o2 = operatorStack.back();
					if (o2.operatorType != OperatorType::LPARAN) {
						m_output.push_back(o2);
						operatorStack.pop_back();
					}
					else {
						break;
					}
				}
				break;
			}


			// Left paranthesis
			if (token.operatorType == OperatorType::LPARAN) {
				operatorStack.push_back(token);
				break;
			}

			// Right paranthesis
			if (token.operatorType == OperatorType::RPARAN) {
				while (1) {
					if (operatorStack.empty()) {
						throw GrammarError("Mismatched parenthesis");
					}

					const Token o2 = operatorStack.back();
					if (o2.operatorType == OperatorType::LPARAN) {
						break;
					}
					m_output.push_back(o2);
					operatorStack.pop_back();
				}
				operatorStack.pop_back();
				if (!operatorStack.empty() && operatorStack.back().type == TokenType::IDENTIFIER) {
					const Token& o2 = operatorStack.back();
					m_output.push_back(o2);
					operatorStack.pop_back();
				}
				break;
			}

			// Other Operators
			while (!operatorStack.empty()) {
				const Token& o2 = operatorStack.back();
				if (o2.type == TokenType::OPERATOR && o2.operatorType != OperatorType::LPARAN) {
					uint32_t precedenceO2 = s_precedenceMap[o2.operatorType];
					uint32_t precedenceToken = s_precedenceMap[token.operatorType];

					if (precedenceO2 > precedenceToken ||
						(precedenceO2 == precedenceToken && (o2.operatorType == OperatorType::ADDITION ||
							o2.operatorType == OperatorType::SUBTRACTION ||
							o2.operatorType == OperatorType::MULTIPLICATION ||
							o2.operatorType == OperatorType::DIVISION ||
							o2.operatorType == OperatorType::MODULUS))) {
						m_output.push_back(o2);
						operatorStack.pop_back();
					}
					else {
						break;
					}
				}
				else {
					break;
				}
			}
			operatorStack.push_back(token);
		}
	}

	// Clear the rest tokens
	while (!operatorStack.empty()) {
		const Token& o2 = operatorStack.back();
		if (o2.type == TokenType::OPERATOR
			&& (o2.operatorType == OperatorType::LPARAN || o2.operatorType == OperatorType::RPARAN)) {
			throw GrammarError("Mismatched parenthesis");
		}
		m_output.push_back(o2);
		operatorStack.pop_back();
	}
}

