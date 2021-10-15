#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>

/* BNF 

::=             can be
<cat>           syntactic category
|               alternative, or
( )             parentheses can be used for grouping
[ ]             can be used to express that something is optional   
""              can be used to express that something is part of the language
...             zero or more

BNF for basic arithmetic operations
---------------------------------------
<expression> ::= [ "-" ] <term> [ ( "+" | "-" ) <term> ]
<term> ::= <factor> [ ( "*" | "/" ) <factor> ]...
<factor> ::= <number> | "(" <expression> ")"

*/

int read_expression(std::istream&);

int read_factor(std::istream& stream) {
    if(std::isdigit(stream.peek())) {
        return static_cast<int>(stream.get() - '0');
    } else if(stream.peek() == '(') {
        
        stream.get(); // eat (
        int value{read_expression(stream)};

        if(stream.peek() == ')')
            stream.get(); // eat )
        else
            throw std::runtime_error("Right parenthesis is missing.");
        
        return value;
    } else {
        throw std::runtime_error("Invalid expression.");
    }
}

int read_term(std::istream& stream) {
    int value{read_factor(stream)};
    
    while(stream.peek() == '*' or stream.peek() == '/') {
        int nextFactorValue{read_factor(stream)};
        char op{stream.get()};

        if(op  == '*')
            value *= nextFactorValue;
        else
            value /= nextFactorValue;
    }
    return value;
}

int read_expression(std::istream& stream) {
    bool isNegative{false};

    if(stream.peek() == '-') {
        stream.get(); // eat -
        isNegative = true;
    }

    int value{read_term(stream)};
    if(isNegative) {
        value *= -1;
    }

    while(stream.peek() == '+' or stream.peek() == '-') {
        char op{stream.get()};
        int nextValue{read_term(stream)};

        if(op == '+') 
            value += nextValue;
        else
            value -= nextValue;
    }
    
    return value;
}

void remove_whitespace(std::string& str) {
    auto endIter{std::remove_if(str.begin(), str.end(), [](char& c ){ return std::isspace(c);})};
    str.erase(endIter, str.end());
}

int main() {
    std::string line{};

    while(std::getline(std::cin, line)) {
        try {
            remove_whitespace(line);
            std::istringstream iss{line};
            int value{read_expression(iss)};
            std::cout << "Your expression " << line << " = " << value << "\n";
        } catch(std::runtime_error err) {
            std::cout << err.what() << "\n";
        }
    }
}