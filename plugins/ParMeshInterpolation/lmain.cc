#include <iostream>
#include "mpi.h"
#include "NamedArgs.h"
#include "InterpInfo.h"
extern "C"
{
    bool AllowParallel(void);
    void Initialize(int argc, char** argv);
}

bool AllowParallel(void)
{
    return true;
}

void Initialize(int argc, char** argv)
{
    NamedArgs args(argc, argv);
    std::cout << "Attempting to collect all ranks..." << std::endl;
    int pid, pnum;
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    MPI_Comm_size(MPI_COMM_WORLD, &pnum);
    bool isRoot = (pid==0);
    for (int p = 0; p < pnum; p++)
    {
        if (p==pid)
        {
            std::cout << "Found rank " << p << " of " << pnum << std::endl;
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }
    
    std::string sourceFile = args.String("source");
    std::string targetFile = args.String("target");
    if (isRoot)
    {
        std::cout << "Attempting to interpolate from source file " << sourceFile << " to target file " << targetFile << std::endl;
    }
    InterpInfo source(sourceFile);
    InterpInfo target(targetFile);
}