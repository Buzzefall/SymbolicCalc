1. Operator overload for operations: **\*, /, +, -, ^**. 
      * Will make a lot easier to generate expressions.
2. Unary operations: **exp(x), log(x), sqrt(x)**
3. Binary operations: **div(x, y)** (that is x/y), **Sub(x, y)** (that is x - y). 
    * Less code bloat for the cost of more classes (**Sub(x, y)** currently is **Add(x, -y)** and **div(x, y)** is **Mult(x, Power(y, -1))**
