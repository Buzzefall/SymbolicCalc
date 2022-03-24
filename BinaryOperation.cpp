#include "BinaryOperation.h"

#include <utility>


Add::Add(shared_ptr<Expression> lex, shared_ptr<Expression> rex)
		: BinaryOperation(std::move(lex), std::move(rex)) { }

double Add::evaluate(double x) { return lexpression->evaluate(x) + rexpression->evaluate(x); }

shared_ptr<Expression> Add::diff() { return make_shared<Add>(lexpression->diff(), rexpression->diff()); }



Sub::Sub(shared_ptr<Expression> lex, shared_ptr<Expression> rex) 
		: BinaryOperation(std::move(lex), std::move(rex)) { }

double Sub::evaluate(double x) { return lexpression->evaluate(x) - rexpression->evaluate(x); }

shared_ptr<Expression> Sub::diff() { return make_shared<Sub>(lexpression->diff(), rexpression->diff()); }



Mult::Mult(shared_ptr<Expression> lex, shared_ptr<Expression> rex)
		: BinaryOperation(std::move(lex), std::move(rex)) { }

double Mult::evaluate(double x) { return lexpression->evaluate(x) * rexpression->evaluate(x); }

shared_ptr<Expression> Mult::diff() {
	return make_shared<Add>(
		make_shared<Mult>(lexpression, rexpression->diff()),
		make_shared<Mult>(lexpression->diff(), rexpression)
	);
}



Div::Div(shared_ptr<Expression> lex, shared_ptr<Expression> rex)
		: BinaryOperation(std::move(lex), std::move(rex)) { }

double Div::evaluate(double x) { return lexpression->evaluate(x) / rexpression->evaluate(x); } // for now

shared_ptr<Expression> Div::diff() {
	return make_shared<Div>(
		make_shared<Sub>(make_shared<Mult>(lexpression->diff(), rexpression), lexpression), 
		make_shared<Power>(rexpression, make_shared<Constant>(2))
	);
}



Power::Power(shared_ptr<Expression> expr, shared_ptr<Expression> const_power)
		: BinaryOperation(std::move(expr), std::move(const_power)) { }

double Power::evaluate(double x) { return pow(lexpression->evaluate(x), rexpression->evaluate(x)); }

shared_ptr<Expression> Power::diff() {
	return make_shared<Mult>(
			make_shared<Mult>(
				rexpression,
				make_shared<Power>(
					lexpression, 
					make_shared<Sub>(rexpression, Constant::One())
				) 
			),
			lexpression->diff()
	);
}