#include "Expression.h"

#include <algorithm>
#include <cassert>
#include <regex>
#include <stack>

#include "BinaryOperation.h"
#include "UnaryOperation.h"

using std::string;

const string operations = "^*/+-";
const string numbers = "0123456789";


std::string toStackMachineForm(const std::string& expr_str) {
	std::string output;
	std::stack<char> stack;

	for (size_t i = 0; i < expr_str.size(); i++) {
		auto ch = expr_str[i];

		if (ch == 'x' || numbers.find(ch) != string::npos) {
			output.push_back(ch);
			continue;
		}

		if (ch == '(') {
			stack.push(ch);
			continue;
		}

		if (ch == ')') {
			while (stack.top() != '(') {
				output.push_back(stack.top());
				stack.pop();
			}
			stack.pop();
			continue;
		}

		if (i + 2 < expr_str.size()) {
			auto sub = expr_str.substr(i, 3);
			if (sub == "sin" || sub == "cos") {
				stack.push(sub == "sin" ? 'S' : 'C');
				continue;
			}
		}

		if (operations.find(ch) != string::npos) {
			while (!stack.empty() && (stack.top() == 'S' || stack.top() == 'C' || operations.find(ch) >= operations.find(stack.top()))) {
				output.push_back(stack.top());
				stack.pop();
			}
			stack.push(ch);
		}
	}

	while (!stack.empty()) {
		output.push_back(stack.top());
		stack.pop();
	}

	return output;
}


bool brackets_correspond(const std::string& input_str) {
	auto sum = 0;
	for (char c : input_str) {
		sum += (c == '(') ? 1 : (c == ')' ? -1 : 0);
		if (sum < 0) break;
	}
	return sum == 0;
}


// Allowed: (, ), 0-9, x, (expr)^[0-9], sinx, cosx, binary +, -, *, /
shared_ptr<Expression> Expression::tryParse(std::string input_str) {
	assert(brackets_correspond(input_str));

	auto is_space = [] (char c) { return std::isspace(c); };
	input_str.erase(
		std::remove_if(input_str.begin(), input_str.end(), is_space), 
		input_str.end()
	);

	auto expr_str = toStackMachineForm(input_str);

	std::stack<shared_ptr<Expression>> expression_stack;
	for (size_t i = 0; i < expr_str.size(); ++i) {

		string number;
		while (numbers.find_first_of(expr_str[i]) != string::npos) {
			number.push_back(expr_str[i]); ++i;
		}
		if (!number.empty()) {
			expression_stack.push(make_shared<Constant>(std::stoi(number)));
			number.clear();
		}

		auto ch = expr_str[i];
		shared_ptr<Expression> op1, op2;
		switch (ch) {
			case 'x': 
				expression_stack.push(make_shared<Variable>());
				break;
			
			case 'S': case 'C': {
				op1 = expression_stack.top();
				expression_stack.pop();
				switch (ch) {
					case 'S':
						expression_stack.push(make_shared<Sin>(op1));
						break;
					case 'C':
						expression_stack.push(make_shared<Cos>(op1));
						break;
				}
				break;
			}

			case '^': case '*': case '/': case '+': case '-': {
				op2 = expression_stack.top();
				expression_stack.pop();
				op1 = expression_stack.top();
				expression_stack.pop();
				switch (ch) {
					case '^': 
						expression_stack.push(make_shared<Power>(op1, op2));
						break;
					case '*': 
						expression_stack.push(make_shared<Mult>(op1, op2));
						break;
					case '/': 
						expression_stack.push(make_shared<Mult>(op1, make_shared<Power>(op2, make_shared<Constant>(-1))));
						break;
					case '+': 
						expression_stack.push(make_shared<Add>(op1, op2));
						break;
					case '-': 
						expression_stack.push(make_shared<Add>(op1, make_shared<Negation>(op2)));
						break;
				}
				break;
			}

			default: {
				throw std::invalid_argument("Invalid input string.");
			}
		}	
	}

	assert(expression_stack.size() == 1);
	return expression_stack.top();
}


size_t find_lowest(std::string input_str) {
	return 0;
}


shared_ptr<Expression> Expression::Parse(std::string input_str) {
	assert(brackets_correspond(input_str));

	auto l_bracket = input_str.find_first_of('(', 0);
	auto r_bracket = input_str.find_last_of(')', 0);

	if (r_bracket - l_bracket > 1) {
		shared_ptr<Expression> m_subexpr = Parse(input_str.substr(l_bracket + 1, r_bracket - 1));
		assert(m_subexpr != nullptr);
	}

	//if ()

	// auto split_by_lowest(input_str, 0, l_bracket - 1);

	// assert: no brackets outside of [l_bracket, r_bracket] (that is middle expression)
	// search for lowest priority operator at left side, split left side of expr by it, parse two pieces and construct Expr
	// same for right side
	// return Expression{
	//			Expression{Parse(left_piece1), Parse(left_piece2)},
	//			Expression(m_subexpr)},
	//			Expression{Parse(right_piece1), Parse(right_piece2)}}

	//find_lowest(input_str);

	return Parse(input_str);
}



