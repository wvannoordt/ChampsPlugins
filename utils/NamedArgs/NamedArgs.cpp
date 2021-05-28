#include <vector>
#include "NamedArgs.h"
#include "strunformat.h"

NamedArgs::NamedArgs(int& argc, char**& argv)
{
    for (int i = 0; i < argc; i++)
    {
        std::string argstring(argv[i]);
        if (argstring.find("=")!=std::string::npos)
        {
            size_t eqPos = argstring.find("=");
            std::string name = argstring.substr(0, eqPos);
            std::string value = argstring.substr(eqPos+1, argstring.length()-1-eqPos);
            vals.insert({name,value});
        }
    }
}

NamedArgs::~NamedArgs(void)
{
    
}

bool NamedArgs::HasArg(std::string name)
{
    return (vals.find(name) != vals.end());
}

double NamedArgs::Double(std::string name)
{
    if (!HasArg(name)) {std::cout << "Plugin is requesting argument called \"" << name << "\" of type double, but couldn't find it!" << std::endl; abort();}
    try
    {
        return std::stod(vals[name]);
    }
    catch (...)
    {
        std::cout << "Argument \"" << name << "\" with value \"" << vals[name] << "\" could not be interpreted as double." << std::endl; abort();
    }
}

bool NamedArgs::Bool(std::string name)
{
    if (!HasArg(name)) {std::cout << "Plugin is requesting argument called \"" << name << "\" of type bool, but couldn't find it!" << std::endl; abort();}
    if (vals[name]=="true") return true;
    if (vals[name]=="false") return false;
    std::cout << "Argument \"" << name << "\" with value \"" << vals[name] << "\" could not be interpreted as bool." << std::endl; abort();
    return false;
}

std::string NamedArgs::String(std::string name)
{
    if (!HasArg(name)) {std::cout << "Plugin is requesting argument called \"" << name << "\" of type string, but couldn't find it!" << std::endl; abort();}
    return vals[name];
}

int NamedArgs::Int(std::string name)
{
    if (!HasArg(name)) {std::cout << "Plugin is requesting argument called \"" << name << "\" of type int, but couldn't find it!" << std::endl; abort();}
    try
    {
        return std::stod(vals[name]);
    }
    catch (...)
    {
        std::cout << "Argument \"" << name << "\" with value \"" << vals[name] << "\" could not be interpreted as int." << std::endl; abort();
    }
}

std::vector<int> NamedArgs::IntVec(std::string name)
{
  std::vector<int> output;

  if (!HasArg(name)) {std::cout << "Plugin is requesting argument called \"" << name << "\" of type int, but couldn't find it!" << std::endl; abort();}
    try
    {
      std::string elements = "abc";
      strunformat(vals[name],"[{}]", elements);
      std::vector<std::string> temp = StringSplit(elements,",");
      for (auto &s:temp)
        {
          output.push_back(std::stoi(s));
        }
      return output;
      
    }
    catch (...)
    {
        std::cout << "Argument \"" << name << "\" with value \"" << vals[name] << "\" could not be interpreted as int." << std::endl; abort();
    }
    return output;
}

std::vector<double> NamedArgs::DoubleVec(std::string name)
{
  std::vector<double> output;

  if (!HasArg(name)) {std::cout << "Plugin is requesting argument called \"" << name << "\" of type int, but couldn't find it!" << std::endl; abort();}
    try
    {
      std::string elements = "abc";
      strunformat(vals[name],"[{}]", elements);
      std::vector<std::string> temp = StringSplit(elements,",");
      for (auto &s:temp)
        {
          output.push_back(std::stod(s));
        }
      return output;
      
    }
    catch (...)
    {
        std::cout << "Argument \"" << name << "\" with value \"" << vals[name] << "\" could not be interpreted as int." << std::endl; abort();
    }
    return output;
}

void NamedArgs::print(void)
{
  for (auto &p:vals)
    {
      std::cout << p.first << " = " << p.second  << std::endl;
    }
}
