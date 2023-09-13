// Add libraries
#include <iostream>   // cout
#include <fstream>    // ofstream
#include <random>     // mt19937 and uniform_int_distribution
#include <algorithm>  // generate
#include <vector>     // vector
#include <iterator>   // begin, end, and ostream_iterator

// Define a function to generate random numbers
std::vector<int> create_random_vector(int n, int min, int max) {

    // Start the random device and seed it
    std::random_device r;
    std::seed_seq      seed{r(), r(), r(), r(), r(), r(), r(), r()};
    std::mt19937       eng(seed);

    // Specify the uniform distribution
    std::uniform_int_distribution<int> dist(min, max);

    // Fill the vector with values
    std::vector<int> v(n);
    generate(begin(v), end(v), bind(dist, eng));
    return v;

}

int main() {

    // Declare parameters
    int nx = 100;         // number of cells in x-direction
    int ny = 100;         // number of cells in y-direction
    int nt = 20;          // number of time steps
    int nw = 100;         // number of random walkers

    // Open the files for writing
    std::ofstream data_file("location.csv");
    data_file << "n,i,x,y" << std::endl;

    // Randomly generate x and y locations
    std::vector<int> x = create_random_vector(nw,0,nx-1);
    std::vector<int> y = create_random_vector(nw,0,ny-1);

    // Generate the random steps
    std::vector<int> step = create_random_vector(nt*nw,1,4);   

    // Loop over time
    for (int n=0; n<nt; n++) {

        // Loop through all points
        for (int i=0; i<nw; i++) {

            // Write location to file
            data_file << n << "," << i << "," << x[i] << "," << y[i] << std::endl;

            // Determine which direction to step
            // TODO: Change so we don't take a step if someone is in that spot
            int stepIndex = n*i + i;
            int stepChoice = step[stepIndex];
            if (stepChoice == 1) {
                x[i] = x[i] + 1;
            } else if (stepChoice == 2){
                y[i] = y[i] + 1;
            } else if (stepChoice == 3) {
                x[i] = x[i] - 1;
            } else {
                y[i] = y[i] - 1;
            }

            // Enforce periodic boundaries
            if (x[i] == nx) {
                x[i] = 0;
            } else if (x[i] == -1) {
                x[i] = nx-1;
            } else if (y[i] == ny) {
                y[i] = 0;
            } else if (y[i] == -1) {
                y[i] = ny-1;
            }

        }

    }

}