#include <iostream>
#include <Eigen/Dense>
#include <Eigen/Eigenvalues>
#include <Eigen/Sparse>
#include <chrono>
#include <random>
#include <fstream>
#include <algorithm>

using namespace std;
using timer = std::chrono::system_clock;
using MatrixDouble = Eigen::Matrix <double, Eigen::Dynamic, Eigen::Dynamic>;
using SparseMatrix = Eigen::SparseMatrix<double>;
using MT = std::mt19937;
/*
Here, I am trying to do the same thing by using a function that generates random matrices, 
    along with a histogram class that helps us organize things and write clean code.
*/

auto generate_random_spectrum(MT & G,int matrixsize){
    MatrixDouble RandomMat(matrixsize,matrixsize);
    static normal_distribution<double> diagonal_distribution(0,1);
    static normal_distribution<double> off_diagonal_distribution(0,2);
    /*
    Using static for distributions is beneficial because it reduces the overhead of reinitializing 
            the distributions at each function call while ensuring that the distribution remains the same between calls.
    */
    for(int i = 0; i < matrixsize; i++){
        RandomMat(i,i) = diagonal_distribution(G);
        for(int j = i + 1; j < matrixsize; j++){
            double off_diag_value = off_diagonal_distribution(G); 
            RandomMat(i,j) = off_diag_value;
            RandomMat(j,i) = off_diag_value; 
        }
    }
    Eigen::EigenSolver<MatrixDouble> EigenVector(RandomMat);
    return EigenVector.eigenvalues();
}

class Histogram{
    private :
        double a;
        double b;
        double delta;
        vector<int> bars;
        int nb_out_of_box;
    public :
        Histogram(double a, double b, int K) : a(a), b(b), delta((b-a) / K), bars(K,0), nb_out_of_box(0) {}; // constructor
        double lower_bound() const {return this-> a;}
        double upper_bound() const {return this-> b;}
        int nb_boxes() const {return this->bars.size();}
        int nb_out_of_domain() const {return this-> nb_out_of_box;}
        bool operator +=(double x); // Add a data point by incrementing the correct slot in the histogram with h += x
        void print(ostream & out) const; // Display on the out stream
        void reset() {return fill(bars.begin(),bars.end(),0); nb_out_of_box = 0;}
        };

bool Histogram::operator +=(double x){
    int k = floor((x - a) / delta);
    if((k >= 0 ) && (k < bars.size())){
        bars[k]++;
        return true;
    }
    else{
        nb_out_of_box++;
        return false;
    }
}


/*
    We will write two numbers separated by a space on each line : 
        the center of the k-th bin and the height of the associated bar in the histogram.
*/
void Histogram::print(ostream & out) const{
    double middle_point;
    double normalized_value;
    int total_number_points = nb_out_of_box + accumulate(bars.begin(),bars.end(),0);
    for(int i = 0; i < bars.size(); i++){
        middle_point = a + (i * 0.5) * delta;
        normalized_value = bars[i] / double(total_number_points);
        out << middle_point << " " << normalized_value << "\n";
    }
}

int main(){
    // Parameters
    const int matrixsize = 150;
    const int nb_boxes = 20;
    double a=-3.;
    double b=3.;
    int simul = 50;
    MT G(time(nullptr));


    chrono::duration<double> process;
    auto start = timer::now();
    Histogram h(a,b,nb_boxes);
    for(int sampl = 0; sampl < simul; sampl++){
        auto spec = generate_random_spectrum(G,matrixsize);
        for(auto lambda : spec){
            h += lambda.real() / (2.*sqrt(matrixsize));
        }
    } 
    auto end = timer::now();
    process = end - start;
    cout << "End of the process : " << process.count() <<" s"<< endl;
    
    { 
        ofstream output("EigenValues_class.dat");
        h.print(output);
        output.close();
    }
    // CONCLUSION
    // 1/ This program is much more readable than the previous one.
    // 2/ Each step is independently testable from the others.
    // 3/ Each component is easily reusable.
    // 4/ The structure, as well as the names and types of variables, make it easy to follow.
    return 0;
}
