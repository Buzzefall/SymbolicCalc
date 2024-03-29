1. **Class operator overloading** for operations: **\*, /, +, -, ^**. 
      * Will make a lot easier to generate expressions.
2. **Unary operations**: **exp(x), log(x), sqrt(x)**
3. **Binary operations**: **div(x, y)** (that is x/y), **Sub(x, y)** (that is x - y). 
    * Less code bloat for the cost of more classes (**Sub(x, y)** currently is **Add(x, -y)** and **div(x, y)** is **Mult(x, Power(y, -1))**
4. **~~Make Constant(0), Constant(1)~~ and Variable() singletons**
     * Will save memory from unnecessary duplicates
6. **AST pruning** while differentiating (omit branches which evaluate to zero)
     * Get rid of redundant information and unnecessary computations 
     * Must have for Taylor series differentiation as expression tree grows exponentially
7. **Abstract FunctionExpression type**
     * This will (hopefully) allow to implement composition of abstract functions
     * And probably helpful to implement equation solver
8. **Equation Expression type**
9. **Multiple expression variables support**
