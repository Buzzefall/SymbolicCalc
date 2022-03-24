#include <cmath>
#include <utility>

#include "Expression.h"
#include "BinaryOperation.h"
#include "UnaryOperation.h"


Negation::Negation(shared_ptr<Expression> expr): UnaryOperation(std::move(expr)) { }

double Negation::evaluate(double x) { return -1 * expression->evaluate(x); }

shared_ptr<Expression> Negation::diff() { return make_shared<Negation>(expression->diff()); }


Sin::Sin(shared_ptr<Expression> expr): UnaryOperation(std::move(expr)) { }

double Sin::evaluate(double x) { return sin(expression->evaluate(x)); }


shared_ptr<Expression> Sin::diff() { return make_shared<Mult>(make_shared<Cos>(expression), expression->diff()); }


Cos::Cos(shared_ptr<Expression> expr): UnaryOperation(std::move(expr)) { }

double Cos::evaluate(double x) { return cos(expression->evaluate(x)); }


shared_ptr<Expression> Cos::diff() {
	return make_shared<Negation>(make_shared<Mult>(make_shared<Sin>(expression), expression->diff()));
}
