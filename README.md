# SymbolicCalc
Symbolic differentiator and evaluator for simple expressions parsed from text input. AST is formed by polymorhic class Expression hierarchy, where nodes are some derivative classes of Expression.

Memory is managed by std::shared_ptr so there should be no much cluttering as long as you access Expression object through std::shared_ptr<Expression>.

## Text input
**Binary operations**: +, -, \*, /, ^ (power);
	
**Unary operations**: none, except functions sin(expr), cos(expr);
	
**Variables**: x
	
**Constants**: any number [0-9]+
	
## Example expressions

**Note**: power calculates only as expr^const_expr, where const_expr is expression not containing `x` 

Not all possibilities are supported, but some examples are as follows:
* x^2 +x\*sin(x - 2\*x/1000 + 1)\*5 + 5/x + x/2 + x^2\*(5 - 1)
* x^x  (will evaluate, but differentiation is not implemented)
* (x^(sinx + 5\*x^2) + 5)\*x (will evaluate, but differentiation is not implemented)

## Usage

API of Expression class is plain simple:

```C++
static shared_ptr<Expression> tryParse(std::string input_str); // try to parse input string and get parsed expression

virtual double evaluate(double x); // evaluate expression for value 'x' provided
virtual shared_ptr<Expression> diff(); // get derivative of expression
```
	
Example 1:
```C++
auto x = 3.14;
try {
  auto parsed = Expression::tryParse(TestInput); // returns shared_ptr<Expression>
  auto value = parsed->evaluate(x);
  auto derivative = = parsed->diff();
  auto diff_value = derivative->evaluate(x);
} catch (std::exception& e) {
	std::cout << e.what() << std::endl;
}
```
	
Example 2:
```C++
auto x = make_shared<Variable>();
auto sinus = make_shared<Sin>(x);
auto base = make_shared<Mult>(x, sinus);
auto power = make_shared<Constant>(3.14);
	
auto expression = make_shared<Power>(base, power);
auto value = expression->evaluate(x);
auto diff_value = expression->diff()->evaluate(x);
```
