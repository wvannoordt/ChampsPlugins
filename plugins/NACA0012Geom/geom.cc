#define PI 3.1415926535
#include <iostream>
#include "geolytical.h"
#include <cmath>
#include <string>
#include <cstdlib>
#include <vector>
extern "C"
{
	void Initialize(int argc, char** argv);
}

void Initialize(int argc, char** argv)
{
    if (argc < 4) {std::cout << "Not enough arguments to NACA geom!" << std::endl; abort();}
    int npts = 1000;
    int nz = std::stoi(argv[0]);
    double zmin = std::stod(argv[1]);
    double zmax = std::stod(argv[2]);
    double drmin = std::stod(argv[3]);
    //double data [1024];
    double data [2*npts];
#include "data_fine.hpp"
    
    double* newPoints;
    int numNewPoints;
    int lasti=0;
    std::vector<int> idxes;
    for (int i = 0; i < npts; i++)
    {
        double x1 = data[2*i];
        double x2 = data[2*lasti];
        double y1 = data[2*i+1];
        double y2 = data[2*lasti+1];
        double dr = sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
        if (dr>drmin || i==0 || i==256)
        {
            lasti = i;
            idxes.push_back(i);
        }
    }
    
    newPoints = new double[2*idxes.size()];
    
    for (int i = 0; i < idxes.size(); i++)
    {
        newPoints[2*i] = data[2*idxes[i]];
        newPoints[2*i+1] = data[2*idxes[i]+1];
    }
    std::cout << "Reduced " << npts-idxes.size() << " points" << std::endl;
    geolytical::ExtrudedCurve2D foil3DLite(nz, zmin, zmax, newPoints, idxes.size());
    geolytical::Curve2D foil2D(data, npts);
    foil2D.AddIntegerScalar("Components",[](double x, double y, double z)
                            {
                              if (x < 0.10  && y > 0.0) return 2;
                              if (x < 0.005) return 2;
                              return 1;                                
                            });
    foil3DLite.AddIntegerScalar("Components",[](double x, double y, double z)
                                {
                                  if (x < 0.10  && y > 0.0) return 2;
                                  if (x < 0.005) return 2;
                                  return 1;                                
                                });
    foil2D.OutputToVtk("output2D.vtk");
    foil3DLite.OutputToVtk("output3D.vtk");
    
    delete[] newPoints;
    std::cout << "Successfully generated NACA0012 Geometry. Exiting." << std::endl;
}


