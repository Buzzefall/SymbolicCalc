#pragma once

#include <utility>

#include "Expression.h"


class UnaryOperation : public Expression {
protected:
	shared_ptr<Expression> expression = nullptr;

public:
	explicit UnaryOperation(shared_ptr<Expression> expr) : expression(std::move(expr)) { }
};


class Negation final : public UnaryOperation {
public:
	explicit Negation(shared_ptr<Expression> expr);

	double evaluate(double x) override;
	shared_ptr<Expression> diff() override;
};


class Sin final : public UnaryOperation {
public:
	explicit Sin(shared_ptr<Expression> expr);

	double evaluate(double x) override;
	shared_ptr<Expression> diff() override;
};



class Cos final : public UnaryOperation {
public:
	explicit Cos(shared_ptr<Expression> expr);

	double evaluate(double x) override;
	shared_ptr<Expression> diff() override;
};