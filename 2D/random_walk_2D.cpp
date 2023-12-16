// Add libraries
#include <iostream>   // cout
#include <fstream>    // ofstream
#include <random>     // mt19937 and uniform_int_distribution
#include <algorithm>  // generate
#include <vector>     // vector
#include <iterator>   // begin, end, and ostream_iterator
#include <cmath>      // pow, abs

// Define a function to generate one random number
int create_random_integer(int min, int max) {

    //the random device that will seed the generator
    std::random_device seeder;

    //then make a mersenne twister engine
    std::mt19937 engine(seeder());

    //then the easy part... the distribution
    std::uniform_int_distribution<int> dist(min, max);

    //then just generate the integer like this:
    return dist(engine);

}

// Define a function to generate random numbers
std::vector<int> create_random_vector(int n, int min, int max) {

    // Start the random device and seed it
    std::random_device r;
    std::seed_seq seed{r(), r(), r(), r(), r(), r(), r(), r()};
    std::mt19937 eng(seed);

    // Specify the uniform distribution
    std::uniform_int_distribution<int> dist(min, max);

    // Fill the vector with values
    std::vector<int> v(n);
    generate(begin(v), end(v), bind(dist, eng));
    return v;

}

bool check_step_preference(int stepChoice, int verticalPreference, int horizontalPreference) {

    // Initalize reroll as false
    bool reroll = false;

    // Check if step choice voilates preferred direction
    if ((stepChoice == 1) && (horizontalPreference != 1)) {
            reroll == true;
    } else if ((stepChoice == 2) && (verticalPreference != 1)) {
            reroll == true;
    // Step left
    } else if ((stepChoice == 3) && (horizontalPreference != -1)) {
            reroll == true;
    // Step down
    } else if ((stepChoice == 4) && (verticalPreference != -1)) {
            reroll == true;
    }

    return reroll;

}

int main() {

    // Declare parameters
    int nx = 300;         // number of cells in x-direction
    int ny = 300;         // number of cells in y-direction
    int nt = 2000;        // number of time steps
    int nw = 900;         // number of random walkers

    // Declare cluster reroll, the number of times to regenerate a random 
    // number until we go towards or away from another point. Positive 
    // number means roll until we go towards, negative for away.
    int clusterReroll = 10;

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
            int stepIndex = n*i + i;
            int stepChoice = step[stepIndex];

            // Initialize step check as true
            bool canStepRight = true;
            bool canStepUp = true;
            bool canStepLeft = true;
            bool canStepDown = true;

            // Initialize closest relative x and y integers and the difference norm
            int closest_relative_x;
            int closest_relative_y;
            int diff_norm_min = pow(nx,2) + pow(ny,2);

            // Loop through all points to see if we can make that step
            for (int ii=0; ii<nw; ii++) {
                
                // Check if we can step right
                if ((x[ii] == x[i]+1) && (y[ii] == y[i])) {

                    canStepRight = false;

                // Check if we can step right over the periodic boundary
                } else if ((x[i]+1 == nx) && (x[ii] == 0) && (y[ii] == y[i])) {

                    canStepRight = false;

                // Check if we can step up
                } else if ((x[ii] == x[i]) && (y[ii] == y[i]+1)) {
                    
                    canStepUp = false;

                // Check if we can step up over the periodic boundary
                } else if ((y[i]+1 == ny) && (x[ii] == x[i]) && (y[ii] == 0)) {
                    
                    canStepUp = false;

                // Check if we can step left
                } else if ((x[ii] == x[i]-1) && (y[ii] == y[i])) {
                    
                    canStepLeft = false;

                // Check if we can step left over the periodic boundary
                } else if ((x[i]-1 == -1) && (x[ii] == nx-1) && (y[ii] == y[i])) {

                    canStepLeft = false;

                // Check if we can step down
                } else if ((x[ii] == x[i]) && (y[ii] == y[i]-1)) {
                    
                    canStepDown = false;

                // Check if we can step down over the periodic boundary
                } else if ((y[i]-1 == -1) && (x[ii] == x[i]) && (y[ii] == ny-1)) {
                    
                    canStepDown = false;

                }

                // Get the difference away from each point in the array
                int x_diff = x[i] - x[ii];
                int y_diff = y[i] - y[ii]; 

                // Set the previous difference norm and get the current 
                // difference norm
                int diff_norm = pow(x_diff,2) + pow(y_diff,2);

                // If the difference norm isn't too small (the closest point 
                // isn't too close) and is smaller than the current minimum,
                // set the new minimum and different vector values
                // TODO: This won't work if the closest point is across the boundary
                if ((diff_norm > 2) && (diff_norm < diff_norm_min)) {
                    diff_norm_min = diff_norm;
                    closest_relative_x = x_diff;
                    closest_relative_y = y_diff;        
                }

            }

            // Initialize cluster preference
            int verticalPreference;
            int horizontalPreference;

            // Specify the preffered vertical direction
            if (closest_relative_y < 0) {
                verticalPreference = -1;
            } else if (closest_relative_y > 0) {
                verticalPreference = 1;
            } else {
                verticalPreference = 0;
            }

            // Specify the preffered horizontal direction
            if (closest_relative_x < 0) {
                horizontalPreference = -1;
            } else if (closest_relative_x > 0) {
                horizontalPreference = 1;
            } else {
                horizontalPreference = 0;
            }

            // If our cluster reroll is negative, take the 
            // opposite preference
            if (clusterReroll < 0) {
                verticalPreference = -verticalPreference;
                horizontalPreference = -horizontalPreference;
            }

            // If the current step choice doesn't align with the 
            // preference, reroll up to clusterReroll times
            int roll = 0;
            bool reroll = check_step_preference(stepChoice,verticalPreference,horizontalPreference);
            while (reroll) {
                roll += 1;
                stepChoice = create_random_integer(1,4);
                reroll = check_step_preference(stepChoice,verticalPreference,horizontalPreference);
                if (roll > abs(clusterReroll)) {
                    reroll = false;
                }
            }

            // Step right
            if ((stepChoice == 1) && (canStepRight)) {
                
                    x[i] = x[i] + 1;

            // Step up
            } else if ((stepChoice == 2) && (canStepUp)) {

                    y[i] = y[i] + 1;

            // Step left
            } else if ((stepChoice == 3) && (canStepLeft)) {

                    x[i] = x[i] - 1;

            // Step down
            } else if ((stepChoice == 4) && (canStepDown)) {

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
