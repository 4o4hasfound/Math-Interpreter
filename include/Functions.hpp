#pragma once
#include <cmath>
#include <functional>
#include <vector>

struct FunctionObj {
	int numVariable;
	std::function<double(const std::vector<double>&)> function;
};

namespace Functions {
	namespace Operators {
		const FunctionObj Addition { 
			2,
			[](const std::vector<double>& input) {
				return input[0] + input[1];
			} 
		};
		const FunctionObj Subtraction{
			2,
			[](const std::vector<double>& input) {
				return input[0] - input[1];
			}
		};
		const FunctionObj Multiplication {
			2,
			[](const std::vector<double>& input) {
				return input[0] * input[1];
			}
		};
		const FunctionObj Division {
			2,
			[](const std::vector<double>& input) {
				return input[0] / input[1];
			}
		};
		const FunctionObj Modulo {
			2,
			[](const std::vector<double>& input) {
				return std::fmod(input[0], input[1]);
			}
		};
		const FunctionObj Exponentiation {
			2,
			[](const std::vector<double>& input) {
				return std::pow(input[0], input[1]);
			}
		};
		const FunctionObj SquareRoot {
			1,
			[](const std::vector<double>& input) {
				return std::sqrt(input[0]);
			}
		};
	}

	const FunctionObj max{
		2,
		[](const std::vector<double>& input) {
			return std::max(input[0], input[1]);
		}
	};
	const FunctionObj min {
		2,
		[](const std::vector<double>& input) {
			return std::min(input[0], input[1]);
		}
	};
	const FunctionObj sqrt {
		1,
		[](const std::vector<double>& input) {
			return std::sqrt(input[0]);
		}
	};
	const FunctionObj pow {
		2,
		[](const std::vector<double>& input) {
			return std::pow(input[0], input[1]);
		}
	};
}