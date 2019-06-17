#include <iostream>
#include <chrono>
#include <limits>
#include <thread>
#include <fstream>
#include <random>
#include <tuple>

#include <mpi.h>

typedef std::tuple<int*, int, int> mtx;

namespace
{
    constexpr int NUM_CYCLES = 100;

    constexpr int MATRIX_HEIGHT_1 = 500;
    constexpr int MATRIX_WIDTH_1 = 1000;
    constexpr int MATRIX_SIZE_1 = MATRIX_HEIGHT_1 * MATRIX_WIDTH_1;

    constexpr int MATRIX_HEIGHT_2 = 1000;
    constexpr int MATRIX_WIDTH_2 = 500;
    constexpr int MATRIX_SIZE_2 = MATRIX_HEIGHT_2 * MATRIX_WIDTH_2;

    constexpr int MATRIX_SIZE_3 = MATRIX_HEIGHT_1 * MATRIX_WIDTH_2;

    enum method
    {
        SEQUENTIAL = 0,
        PARALLEL = 1
    };

    void log_time(int method, long long time, const std::string& filename)
    {
        std::ofstream outfile;

        outfile.open(filename, std::ios::app);
        outfile << std::to_string(method) << "," << std::to_string(time) << std::endl;
    }

    void print_matrix(int* m, int h, int w)
    {
        for (auto i = 0; i < h; i++)
        {
            for (auto j = 0; j < w; j++)
            {
                std::cout << m[i * w + j] << " ";
            }
            std::cout << std::endl;
        }
    }

    void seq_mult(int* result, int* m1, int h1, int w1, int* m2, int h2, int w2)
    {
        if (w1 != h2)
            return;

        for (auto i = 0; i < h1; i++)
        {
            for (auto j = 0; j < w2; j++)
            {
                for (auto k = 0; k < w1; k++)
                    result[i * w2 + j] += m1[i * w1 + k] * m2[k * w2 + j];
            }
        }
    }

    void mpi_mult(int* result, int* m1, int h1, int w1, int* m2, int h2, int w2, int rank)
    {
        if (h2 != h2)
            return;

        if (rank == 0)
        {
            for (auto i = 0; i < 3; i++)
            {
                auto start = (h1 * i) / 3;
                auto end = (h1 * (i + 1)) / 3;
                auto offset = (end - start) * w1;

                MPI_Send(m1 + offset, offset, MPI_INT, i * 3 + 1, 0, MPI_COMM_WORLD);
            }
        }
        if (rank > 0)
        {
            
        }
    }
}

int main(int argc, char* argv[])
{
    MPI_Init(NULL, NULL);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    //----------creating random matrix----------

    std::mt19937 gen(1337);
    std::uniform_int_distribution<int> distr(0, 10);

    int m1[MATRIX_SIZE_1];
    int m2[MATRIX_SIZE_2];
    int m3[MATRIX_SIZE_3];

    int* mtxs[2] = {m1, m2};
    int m_sizes[2] = {MATRIX_SIZE_1, MATRIX_SIZE_2};

    for (auto j = 0; j < 2; j++)
        for (auto i = 0; i < m_sizes[j]; i++)
            mtxs[j][i] = distr(gen);

    //------------------------------------------

    auto start = std::chrono::high_resolution_clock::now();
    seq_mult(m3, m1, MATRIX_HEIGHT_1, MATRIX_WIDTH_1, m2, MATRIX_HEIGHT_2, MATRIX_WIDTH_2);
    auto end = std::chrono::high_resolution_clock::now();
    auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "Sequential time: " << time << " ms" << std::endl;

    MPI_Finalize();

    return 0;
}
