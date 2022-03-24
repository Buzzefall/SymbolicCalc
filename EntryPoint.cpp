#include <iostream>

#include "BinaryOperation.h"
#include "Expression.h"
#include "UnaryOperation.h"

const auto TestInput = std::string("x^2 + sin(x*314/1000) + x^3");


void plot(const shared_ptr<Expression>& e, int count, double step = 1.0) {
	for (double x = 0; x <= step * count; x += step) { std::cout << e->evaluate(x) << " "; }
	std::cout << std::endl << "----" << std::endl;
}


int main()
{
	shared_ptr<Expression> x = make_shared<Variable>();
	shared_ptr<Expression> x_cube = make_shared<Power>(x, make_shared<Constant>(3));
	shared_ptr<Expression> mult_expr = make_shared<Mult>(x_cube, make_shared<Constant>(2));
	shared_ptr<Expression> cos_expr = make_shared<Cos>(x);
	shared_ptr<Expression> sin_expr = make_shared<Sin>(x);
	shared_ptr<Expression> monster_expr_0 = make_shared<Power>(make_shared<Mult>(mult_expr, make_shared<Sin>(cos_expr)), Constant::Zero());
	shared_ptr<Expression> monster_expr_1 = make_shared<Power>(make_shared<Mult>(mult_expr, make_shared<Sin>(cos_expr)), Constant::One());
	shared_ptr<Expression> monster_expr_3 = make_shared<Power>(make_shared<Mult>(mult_expr, make_shared<Sin>(cos_expr)), make_shared<Constant>(3));
	shared_ptr<Expression> not_allowed_expr = make_shared<Power>(x_cube, x);
	shared_ptr<Expression> diff_not_implemented = make_shared<Power>(make_shared<Constant>(2), x);

	constexpr auto pts = 10;
	constexpr auto angle_step = 0.628;

	plot(diff_not_implemented, 2*pts);
	plot(diff_not_implemented->diff(), 2*pts);

	plot(not_allowed_expr, pts);
	plot(not_allowed_expr->diff(), pts);

	plot(x_cube, pts);
	plot(x_cube->diff(), pts);

	plot(mult_expr, pts);
	plot(mult_expr->diff(), pts);

	plot(cos_expr, pts, angle_step);
	plot(cos_expr->diff(), pts, angle_step);

	plot(sin_expr, pts, angle_step);
	plot(sin_expr->diff(), pts, angle_step);

	plot(monster_expr_0, pts, angle_step);
	plot(monster_expr_0->diff(), pts, angle_step);

	plot(monster_expr_1, pts, angle_step);
	plot(monster_expr_1->diff(), pts, angle_step);

	// these passed on Wolfram Mathematica query "plot ((sin(cosx) * 2x^3)^3)' where x from 0 to pi"
	plot(monster_expr_3, pts, angle_step);
	plot(monster_expr_3->diff(), pts, angle_step);

	auto parsed = Expression::try_parse(TestInput);

	plot(parsed, pts);
	plot(parsed->diff(), pts);

	return EXIT_SUCCESS;
}
