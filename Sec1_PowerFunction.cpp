#include <iostream>
#include <Eigen/Dense>
#include <chrono>
#include <fstream>



using namespace std;
using timer = std::chrono::system_clock;

/*
A matrix with real coefficients and a fixed size of N × M at the time of writing the program can be declared as :
    
    Eigen::Matrix<double, N, M> A;

If the size is not fixed at the time of writing the program but at its execution, we need to use :
    Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> A(N,M);
*/
// To simplify the code, we can save this type in the form
using MatrixDouble = Eigen::Matrix <double, Eigen::Dynamic, Eigen::Dynamic>;

/*
    This recursive function allows calculating the power of a matrix.
*/

MatrixDouble slow_power(const MatrixDouble & M, int n){
    if(n==0){
        return MatrixDouble::Identity(M.rows(),M.cols());
    }
    else if (n == 1){
        return M;
    }
    else{
        return M * slow_power(M,n-1);
    }

}
/*
The total number of multiplications performed by the slow_power function to raise a matrix to the power of n is n - 1. 
The n == 1 test is added for efficiency, to prevent unnecessary recursion (which simply returns M), and to simplify the code.
*/

int main(){
    MatrixDouble A(3,3);
    A << 0.4, 0.6, 0,
        0.75, 0.25, 0,
        0, 0, 1;    

    auto measure_time = [](auto func, const MatrixDouble &A,long unsigned power) {
        chrono::duration<double> computation_time;
        auto start = timer::now();
        auto result = func(A, power);
        auto end = timer::now();
        computation_time = end - start;
        return computation_time.count();
    };


    cout << "A ->> \n" << A << "\n    \n";
    cout << "\n**************************************\n";
    /*************************************************************************************************************************************/

    // Time measurement for slow_power
    double time_toThePower100 = measure_time(slow_power, A, 100);
    cout << "The computation time for A^10000 (using the ampersand) is : " << time_toThePower100 << "s\n";