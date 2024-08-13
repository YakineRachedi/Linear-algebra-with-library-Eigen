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

# Section 2 : Template function
In this file i create a template function that merge the `fast_power` and `sparse_power` functions into a single template function puissance_rapide<MatrixType> compatible with both of their respective types.

# Section 3 : Random matrices and their spectrum

In random matrix theory, the Gaussian Orthogonal Ensemble (GOE) is the set of symmetric matrices $A \in M_N(\mathbb{R})$ whose diagonal and above-diagonal entries are independent, such that $a_{ii} \sim \mathcal{N}(0, 1)$ and $a_{ij} \sim \mathcal{N}(0, 2)$ for all $1 \leq i < j \leq N$. As real symmetric matrices, they are diagonalizable with real eigenvalues. It can be shown that almost surely (with respect to the Lebesgue measure on the matrices) these eigenvalues $(\lambda_1, \ldots, \lambda_N)$ are all distinct. 

We can then define the empirical spectral measure of the eigenvalues as:

$$
\mu_N = \frac{1}{N} \sum_{i=1}^{N} \delta_{\frac{\lambda_i}{\sqrt{N}}}
$$

A classical result is that this measure converges narrowly, as $N$ tends to infinity, to the semicircle measure:

$$
d\sigma(x) = \frac{1}{\pi} \sqrt{4 - x^2} \, 1_{[-2,2]}(x) \, dx
$$

where the density is a semicircle centered at 0 with radius 2. In particular, the limiting measure is compactly supported. The aim of the following questions is to illustrate this phenomenon.

This section aims to accomplish the following tasks:
1. Generate large independent random matrices of size $N$.
2. Compute their spectra.
3. Create a histogram of their eigenvalues, i.e., divide the interval $[a, b]$ into $K$ segments of equal length and count how many eigenvalues fall into each segment.

The `<Eigen/Eigenvalues>` sub-library allows for the calculation of eigenvalues of a matrix. Given a matrix `MatrixDouble M(N, N)`, you declare the eigenvalue computation algorithm as follows:

```cpp
Eigen::EigenSolver<MatrixDouble> Solver(M, b);
// If b is true, the eigenvectors are also computed.
// If b is false, only the eigenvalues are computed.
```
### Histogram part :
Consider an interval $[a, b[$ which is divided into $K$ successive segments of equal size $B_k = [a + k\delta, a + (k + 1)\delta[$ with $0 \leq k < K$ and $\delta = \frac{b - a}{K}$: these will constitute the "bins" of the histogram.

Next, consider a sequence of real values $(\lambda_n)_{0 \leq n < N}$. Creating the histogram of these values involves calculating the numbers $(h_k)_{0 \leq k < K}$ such that, for each $k$, $h_k$ is an integer indicating the number of values $\lambda_n$ that fall into the bin $B_k$. You can then optionally normalize $h_k$ by $N$ to obtain the fraction of $\lambda_n$ values that fall into the bin $B_k$.

To do this, you simply need to iterate through all values $(\lambda_n)_{0 \leq n < N}$, determine which $B_k$ each $\lambda_n$ belongs to, and increment the corresponding $h_k$ value.

# Section 4 : Histogram class (The right approach : breaking it down into basic tasks)

For the fourth section, to write readable code and break down complexity into elementary tasks, I believe it is better to create a class that will help organize everything. 

In this section, I do the same as in the previous section, but using a `Histogram` class and a separate function to generate the spectrum of a GOE matrix of size $N$. At the end, I test the performance of this code and compare it with the other code.

I also made a Python script that uses the `matplotlib.pyplot` library to visualize this histogram (we could also use Gnuplot, but I don't have it on my machine).

* Note : Since the code takes a bit of time to run, I have included the result files as well.
