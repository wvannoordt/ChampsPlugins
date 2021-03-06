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

double rotationAngle;
double rotationX;
double rotationY;
#define PI180 0.01745329251
void rotate(double* x, double* y, double* z)
{
	double xprev = *x;
	double yprev = *y;
	double zprev = *z;
	*x = (xprev-rotationX)*cos(PI180*rotationAngle) - (yprev-rotationY)*sin(PI180*rotationAngle) + rotationX;
	*y = (xprev-rotationX)*sin(PI180*rotationAngle) + (yprev-rotationY)*cos(PI180*rotationAngle) + rotationY;
}
double componentId2Max;
int componentFunc(double x, double y, double z)
{
	if (x < componentId2Max) return 2;
	return 1;
}

void Initialize(int argc, char** argv)
{
	NamedArgs args(argc, argv);
	bool doRotation = args.HasArg("rotationAngle");
	if (doRotation)
	{
		rotationAngle = args.Double("rotationAngle");
		rotationX = args.Double("rotationX");
		rotationY = args.Double("rotationY");
	}
	bbox bounds;
	double bufferSize = 0.0;
	bool is3d = args.Bool("is3d");
	componentId2Max = -1e20;
	if (args.HasArg("componentLim"))
	{
		componentId2Max = args.Double("componentLim");
	}
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
    bounds.zmin = args.Double("zmin") - bufferSize;
    bounds.zmax = args.Double("zmax") + bufferSize;
    bounds.xmin = args.Double("xmin") - bufferSize;
    bounds.xmax = args.Double("xmax") + bufferSize;
	bool doSharp = false;
	bool altTri = false;
	if (args.HasArg("alignCentroids")) altTri = args.Bool("alignCentroids");
	if (args.HasArg("doSharp")) doSharp = args.Bool("doSharp");
	
	if (is3d)
	{
		geolytical::FlatPlate plate3(nx, nz, bounds);
		plate3.SetEnableAlignedCenters(altTri);
		plate3.AddIntegerScalar("Components", componentFunc);
		if (doSharp)
		{
			plate3.Deform(sharp);
		}
		if (doRotation)
		{
			plate3.Deform(rotate);
		}
		plate3.OutputToVtk(output);
	}
	else
	{
		geolytical::FlatLine plate2(nx, bounds);
		plate2.AddIntegerScalar("Components", componentFunc);
		if (doSharp)
		{
			plate2.Deform(sharp);
		}
		if (doRotation)
		{
			plate2.Deform(rotate);
		}
		plate2.OutputToVtk(output);
	}
	
}


