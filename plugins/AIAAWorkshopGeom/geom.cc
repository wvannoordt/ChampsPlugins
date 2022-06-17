#define PI 3.1415926535
#include <iostream>
#include "geolytical.h"
#include <cmath>
#include <string>
#include <cstdlib>
bbox bounds;
double h, Lx, Ly, Lz;
#define XBL -0.62
#define XSTART 0
#define XEND 1
#define LDEV 1.78
#define LOUT 5.0
#define YPLATE 0.22
#define YMAX 1.16
#define ZMAX 0.128
extern "C"
{
	void Initialize(int argc, char** argv);
}


void deform(double* x, double* y, double* z)
{
    double xp = *x;
    double yp = *y;
    double zp = *z;
    if (xp < XSTART && yp > -0.008)
    {
        *y = YPLATE;
    }
    if (xp >= XSTART && yp > -0.008 && xp <= XEND)
    {
        *y = YPLATE*(1.0 - 10*xp*xp*xp + 15*xp*xp*xp*xp - 6*xp*xp*xp*xp*xp);
    }
}

void Initialize(int argc, char** argv)
{
	double bufferSize = 0.5;
    if (argc < 2) {std::cout << "Not enough arguments to AIAA geom!" << std::endl; abort();}
    int nz = std::stoi(argv[1]);
    int nx = std::stoi(argv[0]);
    if (argc == 3)
    {
    	std::cout << "AAAA " << argv[2] << std::endl;
    	bufferSize = std::stod(argv[2]);
    	
    }
	double swshift = 0.0;
    h = 1.0;
    bounds.xmin = XBL-LDEV + swshift;
    bounds.xmax = XEND+LOUT + swshift;
    std::cout << "XMIN      : " << bounds.xmin << std::endl;
    std::cout << "XMAX      : " << bounds.xmax << std::endl;
    std::cout << "XMAX (DEV): " << XBL << std::endl;
	std::cout << "YMIN      : " << 0.0 << std::endl;
	std::cout << "YMAX      : " << "?" << std::endl;
	std::cout << "ZMIN      : " << 0.0 << std::endl;
	std::cout << "ZMAX      : " << ZMAX << std::endl;
    bounds.ymin = -0.1;
    bounds.ymax = 0.00001;
    bounds.zmin = 0.0;
    bounds.zmax = ZMAX;
    bounds.xmin = bounds.xmin - bufferSize;
    bounds.xmax = bounds.xmax + bufferSize;
    bounds.zmin = bounds.zmin - bufferSize;
    bounds.zmax = bounds.zmax + bufferSize;
    geolytical::FlatPlate plate3(nx, nz, bounds);
    geolytical::FlatLine plate2(nx, bounds);
    plate3.Deform(deform);
    plate2.Deform(deform);
    plate3.OutputToVtk("output3D.vtk");
    plate2.OutputToVtk("output2D.vtk");
}


