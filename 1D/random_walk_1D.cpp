// Load libraries
#include <iostream>   // cout
#include <fstream>    // ofstream

// Define a function to return the step direction
int random_step() {

    // Generate a random number between 0 and 1
    int random = rand() % 2;

    // Assign step direction (left or right) based on the random number
    int step;
    if (random==0) {
        step = -1;
    } else {
        step = 1;
    }

    return step;

}

int main()
{

	// Provide a seed value so that the random numbers change each time
	srand((unsigned) time(NULL));

    // Declare parameters
    int n_steps = 1000;

    // Declare variables
    int location = 0;
    int step;

    // Open the files for writing
    std::ofstream data_file("location.csv");

    // Write the initial iteration and location to file
    data_file << "i" << "," << "location" << std::endl;
    data_file << 0 << "," << location << std::endl;

    // Loop n_steps times 
    for (int i = 1; i <= n_steps; i++) {

        // Generate a random step direction
        step = random_step();

        // Update the location
        location = location + step;

        // Write the iteration and location to file
        data_file << i << "," << location << std::endl;

    }

    //data_file << std::endl;
    data_file.close();

}