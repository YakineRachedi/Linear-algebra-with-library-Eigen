#include <iostream>
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <chrono>
#include <fstream>

using namespace std;
using timer = std::chrono::system_clock;
using MatrixDouble = Eigen::Matrix <double, Eigen::Dynamic, Eigen::Dynamic>;
using SparseMatrix = Eigen::SparseMatrix<double>;

template <class MatrixType> 
    MatrixType fast_power(const MatrixType & M, int n){
        if (n == 0) {
            MatrixType Id(M.rows(), M.cols());
            Id.setIdentity();
            return Id;
            } 
        else if (n == 1) {
            return M;
            } 
        else if (n % 2 == 0) {
            MatrixType N = fast_power(M, n / 2);
            return N * N;
            } 
        else {
            MatrixType N = fast_power(M, n / 2);
            return M * N * N;
            }
    }   
    
template <class MatrixType,class FUNC> 
    double measure_time(FUNC func, const MatrixType & M,long unsigned power) {
        chrono::duration<double> computation_time;
        auto start = timer::now();
        auto result = func(M, power);
        auto end = timer::now();
        computation_time = end - start;
        return computation_time.count();
    }

/*
    Note : 
    In templates, the auto type cannot be used as a function parameter because parameter types must be explicitly specified.
    That's why i wrote 
            template <class MatrixType,class FUNC>  double measure_time(FUNC func, const MatrixType & M,long unsigned power)
    instead of 
            template <class MatrixType> double measure_time(auto func, const MatrixType & M,long unsigned power) !
*/
int main(){
    const int size_of_matrix = 30;
    
    //Dense
    MatrixDouble B(size_of_matrix,size_of_matrix);
    ifstream input_1("matrice.txt");
    for(int i = 0; i < size_of_matrix; i++){
        for(int j = 0; j < size_of_matrix; j++){
            input_1 >> B(i,j);
        }
    }
    input_1.close();
    
    // Sparse
    SparseMatrix C(size_of_matrix,size_of_matrix);
    ifstream input_2("matrice.txt");
    for (int i = 0; i < size_of_matrix ; i++){
       for (int j = 0; j < size_of_matrix ; j++){
        double v;
        input_2 >> v;
        if (std::abs(v)> 1e-12)
            C.coeffRef(i,j) = v;
        }
    }
    input_2.close();

    double time_fast_power = measure_time(fast_power<MatrixDouble>,B,1000);
    cout << "Time taken to compute B^1000 using the second method = " << time_fast_power << " s \n";
    double time_sparse_power = measure_time(fast_power<SparseMatrix>,C,1000);
    cout << "Time taken to compute B^1000 in sparse format = " << time_sparse_power << " s" << std::endl;

    return 0;
}