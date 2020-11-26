#ifndef CPL_INTERP_INFO_H
#define CPL_INTERP_INFO_H
#include <string>
#include "mpi.h"
#include <cstdlib>
#include "IntError.h"
class InterpInfo
{
    public:
        InterpInfo(std::string filename);
        bool ReadFile(std::string filename);
        ~InterpInfo(void);
    private:
        std::string sourceFilename;
        int pid, pnum;
        bool dealloc;
};
#endif