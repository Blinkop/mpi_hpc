#include <iostream>
#include <chrono>
#include <limits>
#include <thread>
#include <fstream>

#include <mpi.h>

namespace
{
    constexpr int NUM_CYCLES = 100;
    constexpr int MESSAGE_SIZE = 100000;
    constexpr int MESSAGE_SIZE_MAX = 1000000;

    void log_time(int message_size, long long time, const std::string& filename)
    {
        std::ofstream outfile;

        outfile.open(filename, std::ios::app);
        outfile << std::to_string(message_size) << "," << std::to_string(time) << std::endl;
    }

    void task_send(int world_rank)
    {
        char message[MESSAGE_SIZE_MAX];
        for (uint64_t i = 0; i < MESSAGE_SIZE_MAX; i++)
            message[i] = i % __SCHAR_MAX__;

        for (auto s = MESSAGE_SIZE; s <= MESSAGE_SIZE_MAX; s += MESSAGE_SIZE)
        {
            bool zero_send = true;
            auto start = std::chrono::system_clock::now();
            for (auto i = 0; i < NUM_CYCLES; i++)
            {
                if (world_rank == 0)
                {
                    if (zero_send)
                        MPI_Send(message, s, MPI_BYTE, 1, 0, MPI_COMM_WORLD);
                    else
                        MPI_Recv(message, s, MPI_BYTE, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                }
                else if (world_rank == 1)
                {
                    if (!zero_send)
                        MPI_Send(message, s, MPI_BYTE, 0, 0, MPI_COMM_WORLD);
                    else
                        MPI_Recv(message, s, MPI_BYTE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                }

                zero_send = !zero_send;
            }
            auto end = std::chrono::system_clock::now();
            auto t = (end - start).count();

            if (world_rank == 0)
                log_time(s, t, "send.csv");

            if (world_rank == 0)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                std::cout << "Excecuted " << NUM_CYCLES << " cycles with n = " << s
                        << ". System clock: " << t << std::endl;
            }
        }
    }

    void task_ssend(int world_rank)
    {
        char message[MESSAGE_SIZE_MAX];
        for (uint64_t i = 0; i < MESSAGE_SIZE_MAX; i++)
            message[i] = i % __SCHAR_MAX__;

        for (auto s = MESSAGE_SIZE; s <= MESSAGE_SIZE_MAX; s += MESSAGE_SIZE)
        {
            bool zero_send = true;
            auto start = std::chrono::system_clock::now();
            for (auto i = 0; i < NUM_CYCLES; i++)
            {
                if (world_rank == 0)
                {
                    if (zero_send)
                        MPI_Ssend(message, s, MPI_BYTE, 1, 0, MPI_COMM_WORLD);
                    else
                        MPI_Recv(message, s, MPI_BYTE, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                }
                else if (world_rank == 1)
                {
                    if (!zero_send)
                        MPI_Ssend(message, s, MPI_BYTE, 0, 0, MPI_COMM_WORLD);
                    else
                        MPI_Recv(message, s, MPI_BYTE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                }

                zero_send = !zero_send;
            }
            auto end = std::chrono::system_clock::now();
            auto t = (end - start).count();

            if (world_rank == 0)
                log_time(s, t, "ssend.csv");

            if (world_rank == 0)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                std::cout << "Excecuted " << NUM_CYCLES << " cycles with n = " << s
                        << ". System clock: " << t << std::endl;
            }
        }
    }

    void task_bsend(int world_rank)
    {
        auto buf_size = 10 * MESSAGE_SIZE_MAX * NUM_CYCLES;
        auto buffer = new char[buf_size];

        MPI_Buffer_attach(buffer, buf_size);

        char message[MESSAGE_SIZE_MAX];
        for (uint64_t i = 0; i < MESSAGE_SIZE_MAX; i++)
            message[i] = i % __SCHAR_MAX__;

        for (auto s = MESSAGE_SIZE; s <= MESSAGE_SIZE_MAX; s += MESSAGE_SIZE)
        {
            bool zero_send = true;
            auto start = std::chrono::system_clock::now();
            for (auto i = 0; i < NUM_CYCLES; i++)
            {
                if (world_rank == 0)
                {
                    if (zero_send)
                        MPI_Bsend(message, s, MPI_BYTE, 1, 0, MPI_COMM_WORLD);
                    else
                        MPI_Recv(message, s, MPI_BYTE, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                }
                else if (world_rank == 1)
                {
                    if (!zero_send)
                        MPI_Bsend(message, s, MPI_BYTE, 0, 0, MPI_COMM_WORLD);
                    else
                        MPI_Recv(message, s, MPI_BYTE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                }

                zero_send = !zero_send;
            }
            auto end = std::chrono::system_clock::now();
            auto t = (end - start).count();

            if (world_rank == 0)
                log_time(s, t, "bsend.csv");

            if (world_rank == 0)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                std::cout << "Excecuted " << NUM_CYCLES << " cycles with n = " << s
                        << ". System clock: " << t << std::endl;
            }
        }

        char* bbuf;
        int bsize;
        MPI_Buffer_detach(&bbuf, &bsize);
    }

    void task_rsend(int world_rank)
    {
        char message[MESSAGE_SIZE_MAX];
        for (uint64_t i = 0; i < MESSAGE_SIZE_MAX; i++)
            message[i] = i % __SCHAR_MAX__;

        for (auto s = MESSAGE_SIZE; s <= MESSAGE_SIZE_MAX; s += MESSAGE_SIZE)
        {
            bool zero_send = true;
            auto start = std::chrono::system_clock::now();
            for (auto i = 0; i < NUM_CYCLES; i++)
            {
                if (world_rank == 0)
                {
                    if (zero_send)
                        MPI_Rsend(message, s, MPI_BYTE, 1, 0, MPI_COMM_WORLD);
                    else
                        MPI_Recv(message, s, MPI_BYTE, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                }
                else if (world_rank == 1)
                {
                    if (!zero_send)
                        MPI_Rsend(message, s, MPI_BYTE, 0, 0, MPI_COMM_WORLD);
                    else
                        MPI_Recv(message, s, MPI_BYTE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                }

                zero_send = !zero_send;
            }
            auto end = std::chrono::system_clock::now();
            auto t = (end - start).count();

            if (world_rank == 0)
                log_time(s, t, "rsend.csv");

            if (world_rank == 0)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                std::cout << "Excecuted " << NUM_CYCLES << " cycles with n = " << s
                        << ". System clock: " << t << std::endl;
            }
        }
    }

    void task_isend(int world_rank)
    {
        char message[MESSAGE_SIZE_MAX];
        for (uint64_t i = 0; i < MESSAGE_SIZE_MAX; i++)
            message[i] = i % __SCHAR_MAX__;

        for (auto s = MESSAGE_SIZE; s <= MESSAGE_SIZE_MAX; s += MESSAGE_SIZE)
        {
            bool zero_send = true;
            auto start = std::chrono::system_clock::now();
            for (auto i = 0; i < NUM_CYCLES; i++)
            {
                MPI_Request request;
                MPI_Status status;
                int request_complete = 0;

                if (world_rank == 0)
                {
                    if (zero_send)
                        MPI_Isend(message, s, MPI_BYTE, 1, i, MPI_COMM_WORLD, &request);
                    else
                    {
                        MPI_Irecv(message, s, MPI_BYTE, 1, i, MPI_COMM_WORLD, &request);
                        MPI_Wait(&request, &status);
                    }
                }
                else if (world_rank == 1)
                {
                    if (!zero_send)
                        MPI_Isend(message, s, MPI_BYTE, 0, i, MPI_COMM_WORLD, &request);
                    else
                    {
                        MPI_Irecv(message, s, MPI_BYTE, 0, i, MPI_COMM_WORLD, &request);
                        MPI_Wait(&request, &status);
                    }
                }

                zero_send = !zero_send;
            }
            auto end = std::chrono::system_clock::now();
            auto t = (end - start).count();

            if (world_rank == 0)
                log_time(s, t, "isend.csv");

            if (world_rank == 0)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                std::cout << "Excecuted " << NUM_CYCLES << " cycles with n = " << s
                        << ". System clock: " << t << std::endl;
            }
        }
    }

    void task_send_recv(int world_rank)
    {
        char message[MESSAGE_SIZE_MAX];
        for (uint64_t i = 0; i < MESSAGE_SIZE_MAX; i++)
            message[i] = i % __SCHAR_MAX__;

        for (auto s = MESSAGE_SIZE; s <= MESSAGE_SIZE_MAX; s += MESSAGE_SIZE)
        {
            auto start = std::chrono::system_clock::now();
            for (auto i = 0; i < NUM_CYCLES / 2; i++)
            {
                if (world_rank == 0)
                    MPI_Sendrecv(message, s, MPI_BYTE, 1, 0, message, s, MPI_BYTE, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                else if (world_rank == 1)
                    MPI_Sendrecv(message, s, MPI_BYTE, 0, 0, message, s, MPI_BYTE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
            auto end = std::chrono::system_clock::now();
            auto t = (end - start).count();

            if (world_rank == 0)
                log_time(s, t, "sendrecv.csv");

            if (world_rank == 0)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                std::cout << "Excecuted " << NUM_CYCLES << " cycles with n = " << s
                        << ". System clock: " << t << std::endl;
            }
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

    if (world_rank == 0)
        std::cout << "----------Send----------" << std::endl;
    task_send(world_rank);

    if (world_rank == 0)
        std::cout << "----------Ssend----------" << std::endl;
    task_ssend(world_rank);

    if (world_rank == 0)
        std::cout << "----------Bsend----------" << std::endl;
    task_bsend(world_rank);

    if (world_rank == 0)
        std::cout << "----------Rsend----------" << std::endl;
    task_rsend(world_rank);

    if (world_rank == 0)
        std::cout << "----------Non-block----------" << std::endl;
    task_isend(world_rank);

    if (world_rank == 0)
        std::cout << "----------Send-Receive----------" << std::endl;
    task_send_recv(world_rank);

    MPI_Finalize();

    return 0;
}
