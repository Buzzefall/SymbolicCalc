#pragma once

#include <memory>
#include <string>

using std::shared_ptr;
using std::make_shared;

class Expression {
public:
	Expression() = default;
	Expression(const Expression&) = delete;
	Expression& operator=(const Expression&) = delete;
	virtual ~Expression() = default;

	static shared_ptr<Expression> try_parse(std::string input_str);
	static shared_ptr<Expression> try_parse_recursive(std::string input_str);

	virtual double evaluate(double x) = 0;
	virtual shared_ptr<Expression> diff() = 0;
	virtual shared_ptr<Expression> taylor_series(size_t order, double at_point);
};


class Constant final: public Expression {
public:
	explicit Constant(double val) : value(val) { }
	~Constant() override = default;

	static shared_ptr<Expression> One();
	static shared_ptr<Expression> Zero();

	double evaluate(double x) override { return value; }
	shared_ptr<Expression> diff() override { return Zero(); }

private:
	double value;

	inline static shared_ptr<Expression> one = nullptr;
	inline static shared_ptr<Expression> zero = nullptr;
};


class Variable final: public Expression {
public:
	explicit Variable() = default;
	~Variable() override = default;

	double evaluate(double x) override { return x; }
	shared_ptr<Expression> diff() override { return Constant::One(); }
};
