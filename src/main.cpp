#include "Token.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Map.hpp"
#include <iostream>
#include <string>

int main() {
	Lexer lexer("1 + 12345.678*(2%50)^2+ std::max(1, 2, 34+5)");
	auto &result = lexer.getToken();
	for (auto i : result) {
		std::cout << i << "\n";
	}
	Parser parser(result);
	return 0;
}