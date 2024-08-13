// Random matrices and their spectrum :

#include <iostream>
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <Eigen/Eigenvalues>
#include <chrono>
#include <fstream>
#include <random>


using namespace std;
using timer = std::chrono::system_clock;
using MatrixDouble = Eigen::Matrix <double, Eigen::Dynamic, Eigen::Dynamic>;
using SparseMatrix = Eigen::SparseMatrix<double>;


int main(){

    // Calculation of eigenvalues for a dense matrix :
    MatrixDouble M(4,4);
    M << 5, 0, 0, 0,
         0, 3, 0, 0,
         0, 0, 2, 0,
         0, 0, 0, 1;

    bool bo = true;
    Eigen::EigenSolver<MatrixDouble> Solver(M,bo);  // eigenvalues vector
    // If b is true, the eigenvectors are also calculated.
    // If b is false, only the eigenvalues are calculated.

    cout << "Eigenvalues:\n";
    for(std::complex<double> lambda : Solver.eigenvalues()){
        cout << lambda.real() << " ";
        // lambda.imag() for imaginary part
    }

    cout << "\n\nEigenvectors:\n";
    MatrixDouble eigenvectors = Solver.eigenvectors().real(); // getting the real part
    cout << eigenvectors << endl;


    std::mt19937_64 G(time(nullptr)); // pseudo-random number generator
    cout << "\n * * * * * * * * * * * \n";
    // * * * Histogram part * * * //

    // Parameters
    const int matrixsize = 150;
    const int nb_boxes = 20;
    double a=-3.;
    double b=3.;
    int out_of_box = 0;
    int simul = 50;

    /* We can replace simul with a larger number,
    * such as 40 or 50, but not too large,
    * otherwise the computation time becomes very long.
    */

    vector<double> hist(nb_boxes,0); // This is the vector that will contain the number of eigenvalues that fall into each subinterval of [a, b].
    chrono::duration<double> process;
    auto start = timer::now();
    for(int sample = 0; sample < simul; sample++){
        MatrixDouble RandomMat(matrixsize,matrixsize);
        normal_distribution<double> diagonal_distribution(0,1); // Diagonal elements distribution
        normal_distribution<double> off_diagonal_distribution(0,2); // Off-diagonal elements distribution
        for(int i = 0; i < matrixsize; i++){
            RandomMat(i,i) = diagonal_distribution(G);
            for(int j = 0; j < matrixsize; j++){
                double off_diag_value = off_diagonal_distribution(G); // Be careful here ! 
                RandomMat(i,j) = off_diag_value;
                RandomMat(j,i) = off_diag_value; 
                /*
                * if we write directly : 
                * RandomMat(j,i) = off_diagonal_distribution(G);
                * RandomMat(j,i) = off_diagonal_distribution(G);
                * we will get a new value, which gives an incorrect result. 
                * We must always store the result of a random variable in another variable.
                */
            }
        }
        Eigen::EigenSolver<MatrixDouble> Solver(RandomMat); // Eigen vector
        for(int i = 0; i < matrixsize; i++){
            double lambda_normalized = Solver.eigenvalues()[i].real() / (2.*sqrt(matrixsize));
            /* The documentation states that diagonalization is performed during the construction of Solver:
            * eigenvalues() is just an accessor to the computation, and the eigenvalues are not calculated each time.
            */
            int index = std::floor( (lambda_normalized-a)/(b-a)*nb_boxes);
            /*
            * This formula, in the case of subdividing an interval, exactly determines if an eigenvalue is within the correct segment. 
            * With a small modification, we can obtain an index that will represent a slot in the hist vector, 
            * where each slot represents a segment of the subdivision
            */
            if( (index >=  0) && (index < nb_boxes) ){
                hist[index] += 1.;
            }
            else{
                out_of_box++;
            }
        }
    }

    // Normalization of histograms to obtain a density of a probability
    for(auto & x : hist){
        x /= double(simul * matrixsize);
    }
    auto end = timer::now();
    process = end - start;
    cout << "End of the process : " << process.count() <<" s"<< endl;
    cout << "Number of eigenvalues outside the interval is: " << out_of_box << endl;

    {
		ofstream output("eigenvalues.dat");
		for(size_t i = 0; i< hist.size(); i++){
			output << a + (2 * i + 1) * (b - a) / (2. * nb_boxes) << "\t" << hist[i] << endl;
                // The formula calculates the center of each interval in the histogram.
		}
		output.close();
	}

    /* The conclusion is that the above program is completely unmanageable for the following reasons:
    *  1/ We nest for loops within each other, making it difficult to keep track of what is happening.
    *  2/ Nothing is easily reusable because everything is mixed together: random matrix generation algorithm,
    *     histogram creation, steps are not clearly identified, normalizations are scattered around, etc.
    *  Hence, the other program with a histogram class and an additional function.
    */
    return 0;
}