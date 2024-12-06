#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <stdexcept>
#include <cctype>
using namespace std;

vector<double> number(const string& chain) {
    int l = chain.length();
    double number = 0.0;
    int numberIndex = 0;
    bool isDecimal = false;
    double decimalFactor = 0.1;
    vector<double> vec;
    for (int i = 0; i < l; i++) {
        if (isdigit(chain[i]) || chain[i] == '.') {
            while (i < l && isdigit(chain[i]) || chain[i] == '.') {
                if (chain[i] == '.') {
                    isDecimal = true;
                }
                else if (isDecimal == false) {
                    number = number * 10 + (chain[i] - '0');
                }
                else {
                    number += (chain[i] - '0') * decimalFactor;
                    decimalFactor *= 0.1;
                }
                i++;
            }
            vec.push_back(number);
            number = 0.0;
            isDecimal = false;
            decimalFactor = 0.1;
        }
    }
    return vec;
}

vector<char> symbol(const string& chain) {
    int l = chain.length();
    char symbArr[4] = { '+', '-', '*', '/'};
    vector<char> vec;
    for (int i = 0; i < l; i++) {
        for (int n = 0; n < 4; n++) {
            if (chain[i] == symbArr[n]) {
                vec.push_back(symbArr[n]);
                break;
            }
        }
    }
    return vec;
}

double calculate(const vector<string>& tokens) {
    stack<double> stack;

    for (const auto& token : tokens) {
        if (token == "+" || token == "-" || token == "*" || token == "/") {
            if (stack.size() < 2) throw invalid_argument("There are not enough operands for the operation");
            double b = stack.top(); stack.pop();
            double a = stack.top(); stack.pop();

            if (token == "+") stack.push(a + b);
            else if (token == "-") stack.push(a - b);
            else if (token == "*") stack.push(a * b);
            else if (token == "/") stack.push(a / b); if (b == 0) throw invalid_argument("Can't divide by zero");
        }
        else {
            stack.push(stod(token));
        }
    }
    return stack.top();
}

int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

int main() {
    string example;
    cin >> example;
    vector<double> numbers = number(example);
    vector<char> symbols = symbol(example);

    vector<string> expression;
    stack<char> operators;
    size_t numIndex = 0, symIndex = 0;
    while (numIndex < numbers.size() || symIndex < symbols.size()) {
        if (numIndex < numbers.size()) {
            expression.push_back(to_string(numbers[numIndex]));
            numIndex++;
        }

        if (symIndex < symbols.size()) {
            char op = symbols[symIndex];
            
            while (!operators.empty() && precedence(operators.top()) >= precedence(op)) {
                expression.push_back(string(1, operators.top()));
                operators.pop();
            }
        
            operators.push(op);
            symIndex++;
        }
    }

    while (!operators.empty()) {
        expression.push_back(string(1, operators.top()));
        operators.pop();
    }

    for (const string& token : expression) {
        cout << token << " ";
    }
    cout << endl;

    try {
        double result = calculate(expression);
        cout << "Result: " << result << endl;
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }
    return 0;
}