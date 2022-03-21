# SymbolicCalc
Symbolic differentiator and evaluator for simple expressions parsed from text input. AST is formed by polymorhic class Expression hierarchy, where nodes are some derivative classes of Expression.

Memory is managed by `std::shared_ptr` so there should be no much cluttering as long as you access Expression object through `std::shared_ptr<Expression>`.

## Text input
**Binary operations**: +, -, \*, /, ^ (power);
	
**Unary operations**: none, except functions sin(expr), cos(expr);
	
**Variables**: x
	
**Constants**: any number [0-9]+
	
## Example expressions

**Note**: power calculates only as expr^const_expr, where const_expr is expression not containing `x` 

Not all possibilities are supported, but here some examples:
* **x^2 +x\*sin(x - 2\*x/1000 + 1)\*5 + 5/x + x/2 + x^2\*(5 - 1)**
* **x^x**  (will be parsed and evaluated, but differentiation is not implemented)
* **(x^(sinx + 5\*x^2) + 5)\*x** (will be parsed and evaluated, but differentiation is not implemented)

## Usage

API of Expression class is plain simple:

```C++
// try to parse input string and get parsed expression
static shared_ptr<Expression> try_parse(std::string input_str); 

// evaluate expression for value 'x' provided
virtual double evaluate(double x); 
// get derivative of expression
virtual shared_ptr<Expression> diff(); 
// get taylor series expression
virtual shared_ptr<Expression> taylor_series(size_t order, double at_point); 
```
	
**Example 1**:
```C++
auto x = 3.14;
try {
  auto parsed = Expression::try_parse(TestInput);
  auto value = parsed->evaluate(x);
  auto derivative = = parsed->diff();
  auto diff_value = derivative->evaluate(x);
  auto taylor = parsed->taylor_series(3, 0); // needs expression to be 3 times differentiable 
} catch (std::exception& e) {
	std::cout << e.what() << std::endl;
}
```
	
**Example 2**:
```C++
auto x = make_shared<Variable>();
auto sinx = make_shared<Sin>(x);
auto mult = make_shared<Mult>(x, sinx);
auto power = make_shared<Power>(x, make_shared<Constant>(3.14));
auto expression = make_shared<Add>(power, mult); // represents x^(3.14) + x*sinx
	
auto x_value = 5;
auto value = expression->evaluate(x_value); // 151.796 for x = 5
auto diff_value = expression->diff()->evaluate(x_value); // 98.7983 for x = 5
```
