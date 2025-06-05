#include <iostream>
#include <stack>
#include <sstream>
#include <cctype>
#include <map>
using namespace std;

int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

double applyOp(double a, double b, char op) {
    switch(op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
    }
    return 0;
}

double evaluate(const string& expr) {
    stack<double> values;
    stack<char> ops;
    int i = 0;
    
    while(i < expr.length()) {
        if (isspace(expr[i])) {
            i++;
            continue;
        }

        if (isdigit(expr[i])) {
            double val = 0;
            while(i < expr.length() && (isdigit(expr[i]) || expr[i] == '.')) {
                if (expr[i] == '.') {
                    i++;
                    double frac = 0.1;
                    while(i < expr.length() && isdigit(expr[i])) {
                        val += (expr[i] - '0') * frac;
                        frac *= 0.1;
                        i++;
                    }
                    break;
                }
                val = (val * 10) + (expr[i++] - '0');
            }
            values.push(val);
        }
        else if (expr[i] == '(') {
            ops.push(expr[i]);
            i++;
        }
        else if (expr[i] == ')') {
            while(!ops.empty() && ops.top() != '(') {
                double b = values.top(); values.pop();
                double a = values.top(); values.pop();
                char op = ops.top(); ops.pop();
                values.push(applyOp(a, b, op));
            }
            ops.pop(); // remove '('
            i++;
        }
        else {
            while(!ops.empty() && precedence(ops.top()) >= precedence(expr[i])) {
                double b = values.top(); values.pop();
                double a = values.top(); values.pop();
                char op = ops.top(); ops.pop();
                values.push(applyOp(a, b, op));
            }
            ops.push(expr[i]);
            i++;
        }
    }

    while(!ops.empty()) {
        double b = values.top(); values.pop();
        double a = values.top(); values.pop();
        char op = ops.top(); ops.pop();
        values.push(applyOp(a, b, op));
    }

    return values.top();
}

int main() {
    string expr;
    cout << "Enter arithmetic expression: ";
    getline(cin, expr);

    try {
        cout << "Result = " << evaluate(expr) << endl;
    } catch (...) {
        cout << "Error evaluating expression." << endl;
    }

    return 0;
}
