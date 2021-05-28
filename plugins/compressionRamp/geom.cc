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


double xRamp;
double rampAngle;
double yfp = 0.0;
#define PI180 0.01745329251
//compressionRamp = (is3d=@choose{${is3d},false,true},nx=768,nz=256,bufferSize=0.018,ymax=1e-7,zmin=${zmin},zmax=${zmax},xmin=${xmin},xmax=${xmax},angle=${rampAngle},output=output.vtk)

void ramp(double* x, double* y, double* z)
{
	if (*y < (yfp-0.01)) return;
	if (*x < xRamp) return;
	double deltaX = (*x) - xRamp;
	double deltaY = deltaX*sin(PI180*rampAngle);
	*y = *y + deltaY;
}

void Initialize(int argc, char** argv)
{
	NamedArgs args(argc, argv);
	rampAngle = args.Double("angle");
	
	bbox bounds;
	double bufferSize = 0.0;
	bool is3d = args.Bool("is3d");
	xRamp = args.Double("xRamp");
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
    bounds.ymax = 1e-7;
	if (args.HasArg("ymin")) bounds.ymin = args.Double("ymin");
	if (args.HasArg("ymax")) bounds.ymax = args.Double("ymax");
	
	yfp = bounds.ymax;
	
    bounds.zmin = args.Double("zmin") - bufferSize;
    bounds.zmax = args.Double("zmax") + bufferSize;
    bounds.xmin = args.Double("xmin") - bufferSize;
    bounds.xmax = args.Double("xmax") + bufferSize;
	
	if (is3d)
	{
		geolytical::FlatPlate plate3(nx, nz, bounds);
		plate3.Deform(ramp);
		plate3.OutputToVtk(output);
	}
	else
	{
		geolytical::FlatLine plate2(nx, bounds);
		plate2.Deform(ramp);
		plate2.OutputToVtk(output);
	}
	
}


