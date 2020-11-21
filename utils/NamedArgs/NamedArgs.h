#ifndef NA_ARG_H
#define NA_ARG_H
#include <string>
#include <map>
#include <iostream>
class NamedArgs
{
    public:
        NamedArgs(int& argc, char**& argv);
        ~NamedArgs(void);
        bool HasArg(std::string name);
        double Double(std::string name);
        bool Bool(std::string name);
        std::string String(std::string name);
        int Int(std::string name);
    private:
        std::map<std::string, std::string> vals;
};

#endif