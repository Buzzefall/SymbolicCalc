# SymbolicCalc
Symbolic differentiator and evaluator for simple expressions parsed from text input.

Memory is managed by std::shared_ptr so there should be no much cluttering as long as you access Expression object through std::shared_ptr<Expression>.

## Text input
Binary operations allowed: +, -, \*, /, ^ (power)
Unary operations allowed: none, except functions sin(expr), cos(expr)

## Example expressions

**Note**: power calculates only as expr^const_expr, where const_expr is expression not containing `x` 

Not all possibilities are supported, but some examples are as follows:
* x^2 +x\*sin(x - 2\*x/1000 + 1)\*5 + 5/x + x/2 + x^2\*(5 - 1)
* x^x  (will evaluate, but differentiation is not implemented)
* (x^(sinx + 5\*x^2) + 5)\*x (will evaluate, but differentiation is not implemented)

## Usage example

```C++
auto x = 3.14;
try {
  auto parsed = Expression::tryParse(TestInput);
  auto value = parsed->evaluate(x);
  
  auto derivative = = parsed->diff();
  auto diff_value = derivative->evaluate(x); // or parsed->diff()->evaluate(x);
} catch (std::exception& e) {
	std::cout << e.what() << std::endl;
}
```
