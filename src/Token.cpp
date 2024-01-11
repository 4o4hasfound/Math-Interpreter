#include "Token.hpp"

Token::Token()
	: type(TokenType::NONTYPE) {

}

Token::Token(TokenType type, const double& num) 
	: type(type), number(num) {

}

Token::Token(TokenType type, const std::string& function)
	: type(type), function(function) {

}

Token::Token(TokenType type, OperatorType operatorType)
	: type(type), operatorType(operatorType) {

}

Token::Token(const Token& token) : type(token.type), function("") {
	switch (token.type) {
	case TokenType::NUMBER:
		number = token.number;
		break;
	case TokenType::FUNCTION:
		function = token.function;
		break;
	case TokenType::OPERATOR:
		operatorType = token.operatorType;
		break;
	}
}

Token& Token::operator=(const Token& token) {
	type = token.type;
	switch (token.type) {
	case TokenType::NUMBER:
		number = token.number;
		break;
	case TokenType::FUNCTION:
		function = token.function;
		break;
	case TokenType::OPERATOR:
		operatorType = token.operatorType;
		break;
	}
	return *this;
}

Token::~Token() {

}

bool Token::operator==(const Token& token) const {
	if (token.type != type) {
		return false;
	}

	switch(type) {
	case TokenType::NUMBER:
		return token.number == number;
	case TokenType::OPERATOR:
		return token.operatorType == operatorType;
	case TokenType::FUNCTION:
		return token.function == function;
	default:
		return false;
	}
}

std::ostream& operator<<(std::ostream& cout, const Token& token) {
	switch (token.type) {
	case TokenType::NUMBER:
		cout << token.number;
		break;
	case TokenType::OPERATOR:
		switch (token.operatorType) {
		case OperatorType::ADDITION:
			cout << '+';
			break;
		case OperatorType::SUBTRACTION:
			cout << '-';
			break;
		case OperatorType::MULTIPLICATION:
			cout << '*';
			break;
		case OperatorType::DIVISION:
			cout << '/';
			break;
		case OperatorType::MODULUS:
			cout << '%';
			break;
		case OperatorType::EXPONENTIATION:
			cout << '^';
			break;
		case OperatorType::SQUAREROOT:
			cout << 'กิ';
			break;
		case OperatorType::LPARAN:
			cout << '(';
			break;
		case OperatorType::RPARAN:
			cout << ')';
			break;
		case OperatorType::COMMA:
			cout << ',';
			break;
		}
		break;
	case TokenType::FUNCTION:
		cout << token.function;
		break;
	}
	return cout;
}