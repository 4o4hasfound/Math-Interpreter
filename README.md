# Math Interpreter
## Description
This calculator first uses a lexer to tokenize the input, then uses Shunting Yard algorithm to turn the tokenized token into Reversed Polish notation, then finaly calculate the result.

## Code Examples
Basic usage:  
```c++
std::string expression("1 + max(5, 6)");

Lexer lexer(expression);
Parser parser;

parser.Evaluate(lexer.getToken());
std::cout<<parser.Calculate();
```

Custom functions and variables:  
```c++
std::string expression("1 + test(a, 6)");

Lexer lexer(expression);
Parser parser;

// Create the function object for 'test'
parser.functionMap["test"] = FunctionObj {
    2, // 'test' function takes 2 arguments
    [](const std::vector<Token>& input) {
        return input[0] + input[1]
    } // The actuall function thats being called
};

// Create the variable for 'a'
parser.function["a"] = 1;

parser.Evaluate(lexer.getToken());
std::cout<<parser.Calculate();
```

## Explanation
The parser seperate the Evaluate function and Calculate function because by doing this, you can change variables and functions AFTER the Evaluate and it will effect the result of the Calculate function. This can be achieved if the types of identifier doesn't change. Because the type of the identifiers are set in the Evaluate(variable or function), if you change a variable into a function or a function into a variable after the Evaluate, something will go wrong.

## Error checking
This parser checks for tje following errors:
* Argument count for functions / operators
* Mismatched paranthesis
* Function / Variables not found
  
This simple parser still lack the ability to correctly check for grammar, so if you input something like "1 2", the output will be '2'.