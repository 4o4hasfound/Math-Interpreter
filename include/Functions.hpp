#pragma once
#include <cmath>
#include <functional>
#include <vector>

#include "Token.hpp"

struct FunctionObj {
	int variableCount;
	std::function<double(const std::vector<Token>&)> function;
};

namespace Functions {
	namespace Operators {
		const FunctionObj Addition{
			2,
			[](const std::vector<Token>& input) {
				return input[0].number + input[1].number;
			}
		};
		const FunctionObj Subtraction{
			2,
			[](const std::vector<Token>& input) {
				return input[0].number - input[1].number;
			}
		};
		const FunctionObj Multiplication{
			2,
			[](const std::vector<Token>& input) {
				return input[0].number * input[1].number;
			}
		};
		const FunctionObj Division{
			2,
			[](const std::vector<Token>& input) {
				return input[0].number / input[1].number;
			}
		};
		const FunctionObj Modulo{
			2,
			[](const std::vector<Token>& input) {
				return std::fmod(input[0].number, input[1].number);
			}
		};
		const FunctionObj Exponentiation{
			2,
			[](const std::vector<Token>& input) {
				return std::pow(input[0].number, input[1].number);
			}
		};
		const FunctionObj SquareRoot{
			1,
			[](const std::vector<Token>& input) {
				return std::sqrt(input[0].number);
			}
		};
		const FunctionObj Negative{
			1,
			[](const std::vector<Token>& input) {
				return -input[0].number;
			}
		};
	};

	const FunctionObj max{
		2,
		[](const std::vector<Token>& input) {
			return std::max(input[0].number, input[1].number);
		}
	};
	const FunctionObj min{
		2,
		[](const std::vector<Token>& input) {
			return std::min(input[0].number, input[1].number);
		}
	};
	const FunctionObj sqrt{
		1,
		[](const std::vector<Token>& input) {
			return std::sqrt(input[0].number);
		}
	};
	const FunctionObj pow{
		2,
		[](const std::vector<Token>& input) {
			return std::pow(input[0].number, input[1].number);
		}
	};
	const FunctionObj sin{
		1,
		[](const std::vector<Token>& input) {
			return std::sin(input[0].number);
		}
	};
	const FunctionObj cos{
		1,
		[](const std::vector<Token>& input) {
			return std::cos(input[0].number);
		}
	};
	const FunctionObj tan{
		1,
		[](const std::vector<Token>& input) {
			return std::tan(input[0].number);
		}
	};
	const FunctionObj sinh{
		1,
		[](const std::vector<Token>& input) {
			return std::sinh(input[0].number);
		}
	};
	const FunctionObj cosh{
		1,
		[](const std::vector<Token>& input) {
			return std::cosh(input[0].number);
		}
	};
	const FunctionObj tanh{
		1,
		[](const std::vector<Token>& input) {
			return std::tanh(input[0].number);
		}
	};
	const FunctionObj radian{
		1,
		[](const std::vector<Token>& input) {
			return 0.0174532925 * input[0].number;
		}
	};
	const FunctionObj degree{
		1,
		[](const std::vector<Token>& input) {
			return 57.2957795 * input[0].number;
		}
	};
};