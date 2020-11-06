#define PI 3.1415926535
#include <iostream>
#include "geolytical.h"
#include <cmath>
#include <string>
#include <cstdlib>
extern "C"
{
	void Initialize(int argc, char** argv);
}

void Initialize(int argc, char** argv)
{
    if (argc < 3) {std::cout << "Not enough arguments to NACA geom!" << std::endl; abort();}
    int nz = std::stoi(argv[0]);
    double zmin = std::stod(argv[1]);
	double zmax = std::stod(argv[2]);
	double data [1024];
#include "data.hpp"
    geolytical::ExtrudedCurve2D foil3D(nz, zmin, zmax, data, 512);
	geolytical::Curve2D foil2D(data, 512);
	foil2D.OutputToVtk("output2D.vtk");
    foil3D.OutputToVtk("output3D.vtk");
}


