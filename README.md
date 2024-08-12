## First Section: The Power Function

### A Recursive Function Prototype

In this section, I write a recursive function (`slow_power`) that computes the power of a matrix. It is based on the following reasoning:
- If $n = 0$, then $M^n = I_d$.
- Otherwise, $M^n = M \cdot (M^{n-1})$.

In this function code, I use dense (full) matrixes.

### Optimization Perspectives
The matrix 𝐴 is small in size, so it is quick to compute its powers. However, to compute the n-th power of a matrix of size $N . N$,
𝑛 calls to matrix multiplication of $N . N$ are needed, which correspond to $N^2$ operations each, resulting in $n . N^2$ calculations. 
The underlying algorithm for the function has a complexity of $O(n . N^2)$ 

We can improve the algorithm by using the (`fast_power`) function, which takes the same input and output types as slow_power but operates on a different recurrence principle. 

- If $n = 0$, then $M^n = I_d$.
- Otherwise, if $n$ is even, then $M^n = (M^{n/2}) \cdot (M^{n/2})$.
- If $n$ is odd, then $M^n = M \cdot (M^{(n-1)/2}) \cdot (M^{(n-1)/2})$.

The choice of exponent is arbitrary and mainly depends on the machine:
- On an older and less powerful machine, differences appear even for small exponents.
- On a recent machine, time differences only become noticeable for large exponents.

I also test the computation times for different power values, for a $30 \times 30$ matrix given in the file `matrix.dat`, depending on whether we use the previously defined functions. I also test the effect of using or not using the `-O1`, `-O2`, `-O3` compiler options of `g++`, as well as the `-DNDEBUG` option (associated with Eigen).

Furthermore, the matrix in the file is a sparse matrix, meaning it has a limited number of non-zero coefficients. The Eigen library provides a way to encode this type of matrix that drastically reduces computation times. To do this, we need to include `<Eigen/Sparse>` at the beginning of our file and we use the type `Eigen::SparseMatrix<double>` instead of `Eigen::Matrix<double, N, M>`.

The operations `+`, `-`, and `*` are used in the same way for sparse matrices as for dense matrices, and it is also possible to perform operations between sparse and dense matrices:

```cpp
Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> A(N, N);
Eigen::SparseMatrix<double> B(N, N);
Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> C1(N, N);
Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> C2(N, N);

C1 = A * B;
C2 = A + B;
```

The `sparse_power` function computes the $n$-th power of a sparse matrix. I calculate the 1000th power and then compare the computation time to the previous ones. 

This comparison helps evaluate the performance improvements achieved by using sparse matrices for large exponentiation tasks.