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

MatrixDouble slow_power_without_ampersand(MatrixDouble M, int n){
    if(n==0){
        return MatrixDouble::Identity(M.rows(),M.cols());
    }
    else if (n == 1){
        return M;
    }
    else{
        return M * slow_power_without_ampersand(M,n-1);
    }

}

/*
    The ampersand '&' is used to pass the argument by reference. Without the ampersand, 
        all the coefficients of the matrix and its powers from 1 to n are copied. In the case of A^100, this would result in 900 copies.
*/

MatrixDouble fast_power(const MatrixDouble & M, int n){
    if(n == 0){
        return MatrixDouble::Identity(M.rows(),M.cols());
    }
    else if (n == 1) {
        return M;
    }
    else if(n % 2 == 0){  // Case when n is an even number (n = 2k, k integer)
        MatrixDouble N(M.rows(),M.cols());
        N = fast_power(M, n / 2);
        /*
        * Be careful here :
        *   return puissance_rapide(M, n/2) * puissance_rapide(M, n/2);
        * the program calculates the half power twice, and there is no time gain.
        */
        return N * N;
    }
    else{ // Case where n is an odd number (n = 2k + 1, k integer)
        MatrixDouble N(M.rows(),M.cols());
        N = fast_power(M, n / 2); // Same as (n-1) / 2 : This is Euclidean (integer) division, so if n = 2k + 1, then n / 2 equals to k
        // It gives the same result directly, so even if I use (n-1) / 2, it is also correct.
        return M * N * N;
    }
}

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

    // Time measurement for slow_power (by reference)
    double time_with_ampersand_toThePower100 = measure_time(slow_power, A, 100);
    cout << "The computation time for A^10000 (using the ampersand) is : " << time_with_ampersand_toThePower100 << "s\n";

    // Time measurement for slow_power_without_ampersand (by value)
    double time_without_ampersand_toThePower100 = measure_time(slow_power_without_ampersand, A, 100);
    cout << "The computation time for A^10000 (without the ampersand) is : " << time_without_ampersand_toThePower100 << "s\n";
    
    cout << "\n**************************************\n";
    /*************************************************************************************************************************************/
    
    double time_with_ampersand_toThePower1000 = measure_time(slow_power, A, 1000);
    cout << "The computation time for A^10000 (using the ampersand) is : " << time_with_ampersand_toThePower1000 << "s\n";

    double time_without_ampersand_toThePower1000 = measure_time(slow_power_without_ampersand, A, 1000);
    cout << "The computation time for A^10000 (without the ampersand) is : " << time_without_ampersand_toThePower1000 << "s\n";
    
    cout << "\n**************************************\n";
    /*************************************************************************************************************************************/
    
    double time_with_ampersand_toThePower10000 = measure_time(slow_power, A, 10000);
    cout << "The computation time for A^10000 (using the ampersand) is : " << time_with_ampersand_toThePower10000 << "s\n";

    double time_without_ampersand_toThePower10000 = measure_time(slow_power_without_ampersand, A, 10000);
    cout << "The computation time for A^10000 (without the ampersand) is : " << time_without_ampersand_toThePower10000 << "s\n";
    
    cout << "\n**************************************\n";
    /*************************************************************************************************************************************/
    /*
    On my machine, this is the maximum power choice; in this case, without the ampersand, 
            it does not work for this power value due to several reasons (memory blockage or overflow, recursion issues...)

    double time_with_ampersand_toThePowerTest = measure_time(slow_power, A, 30000);
    cout << "The computation time for A^Test (using the ampersand) is : " << time_with_ampersand_toThePowerTest << "s\n";

    double time_without_ampersand_toThePowerTest = measure_time(slow_power_without_ampersand, A, Test);
    cout << "The computation time for A^Test (without the ampersand) is : " << time_without_ampersand_toThePowerTest << "s\n";
    */
    cout << "\n**************************************\n";
    /*************************************************************************************************************************************/

    const int size_of_matrix = 30;
    MatrixDouble B(size_of_matrix,size_of_matrix);
    ifstream input_1("matrice.txt");
    for(int i = 0; i < size_of_matrix; i++){
        for(int j = 0; j < size_of_matrix; j++){
            input_1 >> B(i,j);
        }
    }
    input_1.close();

    double time_slow_power = measure_time(slow_power,B,1000);
    cout << "Time taken to compute B^1000 using the first method = " << time_slow_power << " s \n";
    double time_fast_power = measure_time(fast_power,B,1000);
    cout << "Time taken to compute B^1000 using the second method = " << time_fast_power << " s \n";
    
    return 0;
}