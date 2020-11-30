#ifndef INT_ERR_H
#define INT_ERR_H
#include <string>
#include <cstdlib>
#define ThrowError(myarg) ThrowError_INTERNAL(myarg, __FILE__, __LINE__)

static void ThrowError_INTERNAL(std::string m, const char* f, const int l)
{
    std::cout << "----------------------------------------------------" << std::endl;
    std::cout << "Error thrown from file " << f << " line " << l << "." << std::endl;
    std::cout << "Message:" << std::endl;
    std::cout << m << std::endl;
    std::cout << "----------------------------------------------------" << std::endl;
    abort();
}

#endif