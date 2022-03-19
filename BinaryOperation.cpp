#include "BinaryOperation.h"

#include <utility>


Add::Add(shared_ptr<Expression> lex, shared_ptr<Expression> rex)
		: BinaryOperation(std::move(lex), std::move(rex)) { }

double Add::evaluate(double x) { return lexpression->evaluate(x) + rexpression->evaluate(x); }

shared_ptr<Expression> Add::diff() { return make_shared<Add>(lexpression->diff(), rexpression->diff()); }


Mult::Mult(shared_ptr<Expression> lex, shared_ptr<Expression> rex)
		: BinaryOperation(std::move(lex), std::move(rex)) { }

double Mult::evaluate(double x) { return lexpression->evaluate(x) * rexpression->evaluate(x); }


shared_ptr<Expression> Mult::diff() {
	return make_shared<Add>(
		make_shared<Mult>(lexpression, rexpression->diff()),
		make_shared<Mult>(lexpression->diff(), rexpression)
	);
}


Power::Power(shared_ptr<Expression> expr, shared_ptr<Expression> const_power)
		: BinaryOperation(std::move(expr), std::move(const_power)) { }

double Power::evaluate(double x) { return pow(lexpression->evaluate(x), rexpression->evaluate(x)); }


shared_ptr<Expression> Power::diff() {
	return make_shared<Mult>(
			rexpression, 
			make_shared<Mult>(
				make_shared<Power>(
					lexpression, 
					make_shared<Add>(rexpression, make_shared<Constant>(-1))
				), 
				lexpression->diff())
			);
}