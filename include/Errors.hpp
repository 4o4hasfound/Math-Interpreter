#pragma once
#include <exception>
#include <string>

class FunctionNotFound: public std::exception {
public:
	explicit FunctionNotFound(const std::string& function): m_what("Function not found: " + function) {

	}

	virtual const char* what() const noexcept override {
		return m_what.c_str();
	}

private:
	std::string m_what;
};

class VariableTypeError : public std::exception {
public:
	explicit VariableTypeError(const std::string& function, int nthVariable, const std::string& getVariableType, const std::string& expectedVariableType) 
		: m_what(
			"Wrong type for " 
			+ nthVariable
			+ std::string("st variable for function: ")
			+ function
			+ std::string("\nGet: ")
			+ getVariableType
			+ std::string(" ; Expected: ")
			+ expectedVariableType
		) {

	}

	virtual const char* what() const noexcept override {
		return m_what.c_str();
	}

private:
	std::string m_what;
};

class GrammarError : public std::exception {
public:
	explicit GrammarError(const std::string& error) : m_what("Grammar error: " + error) {

	}

	virtual const char* what() const noexcept override {
		return m_what.c_str();
	}

private:
	std::string m_what;
};

class ArgumentError: public std::exception {
public:
	explicit ArgumentError(const std::string& function, int getArgumentCount, int expectedArgumentCount) 
		: m_what(
			"Wrong argument count for function: "
			+ function
			+ std::string("\nGet: ")
			+ std::to_string(getArgumentCount)
			+ std::string(" ; Expected: ")
			+ std::to_string(expectedArgumentCount)
			) {

	}

	virtual const char* what() const noexcept override {
		return m_what.c_str();
	}

private:
	std::string m_what;
};

class VariableNotFound : public std::exception {
public:
	explicit VariableNotFound(const std::string& variable) : m_what("Variable not found: " + variable) {

	}

	virtual const char* what() const noexcept override {
		return m_what.c_str();
	}

private:
	std::string m_what;
};