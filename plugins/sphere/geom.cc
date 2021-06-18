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


void Initialize(int argc, char** argv)
{
	NamedArgs args(argc, argv);
	double radius = args.Double("radius");
	double cx = args.Double("cx");
	double cy = args.Double("cy");
	double cz = args.Double("cz");
	int    nequat = args.Int("numEquator");
	
	bbox bounds;
	std::string output = "output.vtk";
	if (args.HasArg("output")) output = args.String("output");
	
	
	bounds.xmin = cx-radius;
	bounds.xmax = cx+radius;
	bounds.ymin = cy-radius;
	bounds.ymax = cy+radius;
	bounds.zmin = cz-radius;
	bounds.zmax = cz+radius;
	geolytical::Sphere spr(nequat, bounds);
	spr.OutputToVtk(output);
}


