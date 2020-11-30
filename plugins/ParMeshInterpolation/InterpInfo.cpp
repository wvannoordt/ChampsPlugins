#include "InterpInfo.h"

InterpInfo::InterpInfo(std::string filename)
{
    sourceFilename = filename;
    dealloc = false;
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    MPI_Comm_size(MPI_COMM_WORLD, &pnum);
    for (int p = 0; p < pnum; p++)
    {
        if (p==pid)
        {
            if (!this->ReadFile(filename))
            {
                ThrowError("Unsuccessful file read: " + filename + " on rank " + std::to_string(pid));
            }
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }
}

bool InterpInfo::ReadFile(std::string filename)
{
    return true;
}

InterpInfo::~InterpInfo(void)
{
    if (dealloc)
    {
        dealloc = false;
        //dealloc
    }
}
