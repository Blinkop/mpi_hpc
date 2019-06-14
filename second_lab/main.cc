#include <iostream>
#include <chrono>
#include <limits>
#include <thread>
#include <fstream>

#include <mpi.h>

namespace
{
    constexpr int NUM_CYCLES = 100;

    void log_time(int message_size, long long time, const std::string& filename)
    {
        std::ofstream outfile;

        outfile.open(filename, std::ios::app);
        outfile << std::to_string(message_size) << "," << std::to_string(time) << std::endl;
    }

    
            
}

int main(int argc, char* argv[])
{
    MPI_Init(NULL, NULL);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    

    MPI_Finalize();

    return 0;
}
