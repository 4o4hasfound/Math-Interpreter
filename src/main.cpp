#include "Token.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Map.hpp"
#include "Functions.hpp"

#include <iostream>
#include <string>
#include <stack>

int main() {
	//std::string str; std::cin >> str;
	std::string str("1+a*(4*2)^2-max(2, 1)+sqrt(4)");
	Lexer lexer(str);
	auto &result = lexer.getToken();
	Parser parser;
	parser.variable["a"] = 2;
	parser.functionMap["max"] = FunctionObj {
		2,
		[](const std::vector<double>& input) {
			return std::max(input[0], input[1]);
		}
	};
	parser.Evaluate(result);
	std::cout << parser.Calculate();
	return 0;
}