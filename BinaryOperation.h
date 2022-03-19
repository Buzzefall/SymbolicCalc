#pragma once

#include <utility>

#include "Expression.h"


class BinaryOperation : public Expression {
protected:
	shared_ptr<Expression> lexpression = nullptr;
	shared_ptr<Expression> rexpression = nullptr;

public:
	explicit BinaryOperation(shared_ptr<Expression> lex, shared_ptr<Expression> rex)
			: lexpression(std::move(lex)), rexpression(std::move(rex)) { }
	//~BinaryOperation() override { delete lexpression; delete rexpression; }
};


class Add final : public BinaryOperation {
public:
	explicit Add(shared_ptr<Expression> lex, shared_ptr<Expression> rex);

	double evaluate(double x) override;
	shared_ptr<Expression> diff() override;
};


class Mult final : public BinaryOperation {
public:
	explicit Mult(shared_ptr<Expression> lex, shared_ptr<Expression> rex);

	double evaluate(double x) override;
	shared_ptr<Expression> diff() override;
};


// Power restricted to accept args:
// left: shared_ptr<Expression> -> shared_ptr<Expression>,
// right: shared_ptr<Expression> -> shared_ptr<Constant>
class Power final : public BinaryOperation {
public:
	explicit Power(shared_ptr<Expression> expr, shared_ptr<Expression> const_power);

	double evaluate(double x) override;
	shared_ptr<Expression> diff() override;
};