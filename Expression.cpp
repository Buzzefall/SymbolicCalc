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


std::string toStackMachineForm(const std::string& input_str) {
	std::string output_str;
	std::stack<char> stack;

	for (size_t i = 0; i < input_str.size(); i++) {
		while (numbers.find(input_str[i]) != string::npos) {
			output_str.push_back(input_str[i]);
			++i;
		}

		if (i > 1 && numbers.find(input_str[i-1]) != string::npos) {
			output_str.push_back('#');
		}

		auto ch = input_str[i];

		if (ch == 'x') {
			output_str.push_back(ch);
			continue;
		}

		if (ch == '(') {
			stack.push(ch);
			continue;
		}

		if (ch == ')') {
			while (stack.top() != '(') {
				output_str.push_back(stack.top());
				stack.pop();
			}
			stack.pop();
			continue;
		}

		if (i + 2 < input_str.size()) {
			auto sub = input_str.substr(i, 3);
			if (sub == "sin" || sub == "cos") {
				stack.push(sub == "sin" ? 'S' : 'C');
				continue;
			}
		}

		if (operations.find(ch) != string::npos) {
			while (!stack.empty() && (stack.top() == 'S' || stack.top() == 'C' || operations.find(ch) >= operations.find(stack.top()))) {
				output_str.push_back(stack.top());
				stack.pop();
			}
			stack.push(ch);
		}
	}

	while (!stack.empty()) {
		output_str.push_back(stack.top());
		stack.pop();
	}

	return output_str;
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
shared_ptr<Expression> Expression::try_parse(std::string input_str) {
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
			case '#':
				continue;

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
						expression_stack.push(make_shared<Div>(op1, op2));
						break;
					case '+': 
						expression_stack.push(make_shared<Add>(op1, op2));
						break;
					case '-': 
						expression_stack.push(make_shared<Sub>(op1, op2));
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


shared_ptr<Expression> Expression::taylor_series(size_t order, double at_point) {
	shared_ptr<Expression> arg_difference = make_shared<Add>(
		make_shared<Variable>(), 
		make_shared<Constant>(-at_point)
	);

	shared_ptr<Expression> derivative = this->diff();
	shared_ptr<Expression> taylor = make_shared<Mult>(arg_difference, make_shared<Constant>(derivative->evaluate(at_point)));

	size_t factorial = 1;
	size_t member_degree = 2;
	while (member_degree <= order) {
		factorial *= member_degree;
		derivative = derivative->diff();
		auto arg_diff_nth = make_shared<Power>(arg_difference, make_shared<Constant>(member_degree));
		taylor = make_shared<Add>(
			taylor,
			make_shared<Mult>(
				make_shared<Mult>(arg_diff_nth, make_shared<Constant>(derivative->evaluate(at_point))), 
				make_shared<Power>(
					make_shared<Constant>(factorial), make_shared<Constant>(-1))
				)
		);
		++member_degree;
	}


	return make_shared<Add>(make_shared<Constant>(this->evaluate(at_point)), taylor);
}


// TODO
size_t find_lowest(std::string input_str) {
	return 0;
}


// TODO
shared_ptr<Expression> Expression::try_parse_recursive(std::string input_str) {
	assert(brackets_correspond(input_str));

	auto l_bracket = input_str.find_first_of('(', 0);
	auto r_bracket = input_str.find_last_of(')', 0);

	if (r_bracket - l_bracket > 1) {
		shared_ptr<Expression> m_subexpr = try_parse_recursive(input_str.substr(l_bracket + 1, r_bracket - 1));
		assert(m_subexpr != nullptr);
	}

	//if ()

	// auto split_by_lowest(input_str, 0, l_bracket - 1);

	// assert: no brackets outside of [l_bracket, r_bracket] (that is middle expression)
	// search for lowest priority operator at left side, split left side of expr by it, parse two pieces and construct Expr
	// same for right side
	// return Expression{
	//			Expression{try_parse_recursive(left_piece1), try_parse_recursive(left_piece2)},
	//			Expression(m_subexpr)},
	//			Expression{try_parse_recursive(right_piece1), try_parse_recursive(right_piece2)}}

	//find_lowest(input_str);

	return try_parse_recursive(input_str);
}


shared_ptr<Expression> Constant::One() {
	if (one == nullptr) { one = make_shared<Constant>(1); }
	return one;
}


shared_ptr<Expression> Constant::Zero() {
	if (zero == nullptr) { zero = make_shared<Constant>(0); }
	return zero;
}