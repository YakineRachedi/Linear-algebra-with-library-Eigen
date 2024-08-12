## First Section: The Power Function

### A Recursive Function Prototype

In this section, I write a recursive function (`slow_power`) that computes the power of a matrix. It is based on the following reasoning:
- If $n = 0$, then $M^n = I_d$.
- Otherwise, $M^n = M \cdot (M^{n-1})$.

In this function code, I use dense (full) matrices.

### Optimization Perspectives

I write another function based on a different reasoning:
- If $n = 0$, then $M^n = I_d$.
- Otherwise, if $n$ is even, then $M^n = (M^{n/2}) \cdot (M^{n/2})$.
- If $n$ is odd, then $M^n = M \cdot (M^{(n-1)/2}) \cdot (M^{(n-1)/2})$.

The choice of exponent is arbitrary and mainly depends on the machine:
- On an older and less powerful machine, differences appear even for small exponents.
- On a recent machine, time differences only become noticeable for large exponents.

I also test the computation times for different power values, for a $30 \times 30$ matrix given in the file `matrix.dat`, depending on whether we use the previously defined functions. I also test the effect of using or not using the `-O1`, `-O2`, `-O3` compiler options of `g++`, as well as the `-DNDEBUG` option (associated with Eigen).
