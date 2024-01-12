#include "Token.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Map.hpp"
#include <iostream>
#include <string>
#include <stack>

int main() {
	Lexer lexer("3 + 4* 2 / ( 1 - 5 ) ^ 2");
	auto &result = lexer.getToken();
	Parser parser(result);
	std::cout << parser.getResult();
	return 0;
}