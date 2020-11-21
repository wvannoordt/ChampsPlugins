#define PI 3.1415926535
#include <iostream>
#include "geolytical.h"
#include <cmath>
#include <string>
#include <cstdlib>
#include "NamedArgs.h"
extern "C"
{
	void Initialize(int argc, char** argv);
}

void sharp(double* x, double* y, double* z)
{
	double xprev = *x;
	double yprev = *y;
	double zprev = *z;
	if (xprev > 0.05 && xprev < 0.1)
	{
		*y = yprev + 0.1*(xprev - 0.05) / 0.05;
	}
	else if (xprev > 0.099 && xprev < 0.15)
	{
		*y = yprev + 0.1 + 0.1*(0.1 - xprev) / 0.05;
	}
	if (yprev < 0)
	{
		*y = -1.0;
	}
}

void Initialize(int argc, char** argv)
{
	NamedArgs args(argc, argv);
	bbox bounds;
	double bufferSize = 0.0;
	bool is3d = args.Bool("is3d");
	int nx = args.Int("nx");
	int nz = 1;
	if(is3d)
	{
		nz = args.Int("nz");
	}
	std::string output = "output.vtk";
	if (args.HasArg("output")) output = args.String("output");
	if (args.HasArg("bufferSize")) bufferSize = args.Double("bufferSize");
    bounds.ymin = -0.1;
    bounds.ymax = 0.00001;
	if (args.HasArg("ymin")) bounds.ymin = args.Double("ymin");
	if (args.HasArg("ymax")) bounds.ymax = args.Double("ymax");
    bounds.zmin = args.Double("zmin") - bufferSize;
    bounds.zmax = args.Double("zmax") + bufferSize;
    bounds.xmin = args.Double("xmin") - bufferSize;
    bounds.xmax = args.Double("xmax") + bufferSize;
	bool doSharp = false;
	if (args.HasArg("doSharp")) doSharp = args.Bool("doSharp");
	
	if (is3d)
	{
		geolytical::FlatPlate plate3(nx, nz, bounds);
		if (doSharp)
		{
			plate3.Deform(sharp);
		}
		plate3.OutputToVtk(output);
	}
	else
	{
		geolytical::FlatLine plate2(nx, bounds);
		if (doSharp)
		{
			plate2.Deform(sharp);
		}
		plate2.OutputToVtk(output);
	}
	
}


