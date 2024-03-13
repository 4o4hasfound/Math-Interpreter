#include "Token.hpp"

Token::Token()
	: type(TokenType::NONTYPE) {

}

Token::Token(const double& num) 
	: type(TokenType::NUMBER), number(num) {


}

Token::Token(const std::string& function)
	: type(TokenType::IDENTIFIER), identifier({ function }) {

}

Token::Token(OperatorType operatorType)
	: type(TokenType::OPERATOR), operatorType(operatorType) {

}

Token::Token(const Token& token) : type(token.type), identifier({""}) {
	switch (token.type) {
	case TokenType::NUMBER:
		number = token.number;
		break;
	case TokenType::IDENTIFIER:
		identifier = token.identifier;
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
	case TokenType::IDENTIFIER:
		identifier = token.identifier;
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
	if (token.type == TokenType::ANY || type == TokenType::ANY) {
		return true;
	}

	if (token.type != type) {
		return false;
	}

	switch(type) {
	case TokenType::NUMBER:
		return token.number == number;
	case TokenType::OPERATOR:
		return token.operatorType == operatorType;
	case TokenType::IDENTIFIER:
		return token.identifier == identifier;
	default:
		return false;
	}
}

std::string Token::str() const {
	switch (type) {
	case TokenType::NUMBER:
		return std::to_string(number);
	case TokenType::OPERATOR:
		return std::string(1, Conversions::CharOperator.get(operatorType, ' '));
	case TokenType::IDENTIFIER:
		return identifier.name;
	}
}

std::ostream& operator<<(std::ostream& cout, const Token& token)  {
	cout << token.str();
	return cout;
}