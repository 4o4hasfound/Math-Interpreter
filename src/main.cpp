#include "Token.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Map.hpp"
#include "Functions.hpp"
#include "Errors.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <stack>

Parser parser;

void parseCommand(const std::string& command);
void repl();
int parseArithmetic(int argc, char* argv[]);

int main(int argc, char* argv[]) {
	std::cout.setf(std::ios::fixed, std::ios::floatfield);
	std::cout.setf(std::ios::showpoint);

	parser = Parser();

	if (argc < 2) {
		repl();
		return 0;
	}

	const int ret = parseArithmetic(argc, argv);
	return ret;
}



void parseCommand(const std::string& command) {
	std::string operation = command.substr(1, command.find(' ') - 1);
	if (operation.empty()) {
		std::cout << "Empty command call";
		return;
	}
	for (auto& c : operation) {
		c = std::tolower(c);
	}

	// Load the arguments
	std::vector<std::string> result;
	std::stringstream ss(command);
	std::string tok;
	while (std::getline(ss, tok, ' ')) {
		result.push_back(tok);
	}

	if (operation == "set") {
		if (result.size() < 3) {
			std::cout << "Usage: /set variable value";
			return;
		}
		int value;
		try {
			value = std::stoi(result[2].c_str());
		}
		catch (std::invalid_argument) {
			std::cout << "Invalid value for variable: " << result[1];
			return;
		}
		parser.variable[result[1]] = value;
	}
	else if (operation == "clear") {
		parser = Parser();
	}
	else if (operation == "variables") {
		std::cout << "\n";
		for (const auto& [key, value] : parser.variable) {
			std::cout << key << ": " << value << "\n";
		}
	}
	else if (operation == "functions") {
		std::cout << "\n";
		for (const auto& [key, value] : parser.functionMap) {
			if (key.index() == 0) {
				std::cout << std::get<0>(key) << ": " << value.variableCount << "\n";
			}
			else {
				std::cout << std::get<1>(key) << ": " << value.variableCount << "\n";
			}
		}
	}
	else if (operation == "help" || operation == "h") {
		std::cout << "/set variable value\n\tset variable to value in the parser\n/clear\n\tclear the custom variables\n/variables\n\t/print all of the variables\n/functions\n\tprint all of the function name and its argument count\n/help or /h\n\tprint this message";
	}
}

void repl() {
	std::string str;
	std::cout << ">  ";
	for (std::getline(std::cin, str); !str.empty(); std::getline(std::cin, str)) {
		if (str[0] == '/') {
			parseCommand(str);
			std::cout << "\n>  ";
			continue;
		}
		try {
			Lexer lexer(str);
			parser.Evaluate(lexer.getToken());
			double out = parser.Calculate();
			if (std::fmod(out, 1.0) < std::numeric_limits<double>::epsilon()) {
				std::cout << static_cast<long long>(out) << "\n>  ";
			}
			else {
				std::cout << out << "\n>  ";
			}
		}
		catch (const std::exception& e) {
			std::cout << e.what() << "\n>  ";
		}
	}
}

int parseArithmetic(int argc, char* argv[]) {
	std::string expression(argv[1]);
	for (int i = 2; i < argc; ++i) {
		expression += " ";
		expression.append(argv[i]);
	}
	try {
		Lexer lexer(expression);
		Parser parser;
		parser.functionMap["haha"] = FunctionObj{
			1,
			{TokenType::NUMBER},
			[](const std::vector<Token>& input) {
				return input[0].number * 1000.0f;
}
		};
		parser.Evaluate(lexer.getToken());
		std::cout << parser.Calculate();
		return 0;
	}
	catch (const std::exception& e) {
		std::cout << e.what();
		return 1;
	}
}
