#define PI 3.14159265359
#include <string>
#include <iostream>
#include <vector>
#include <cmath>
#include "geolytical.h"
#include "naca.h"
#include "NamedArgs.h"

extern "C"
{
    void Initialize(int argc, char** argv);
}

void Initialize(int argc, char** argv)
{
  NamedArgs args(argc, argv);
  
  //Logical to switch between 2D & 3D cases
  bool is3D = args.Bool("is3D");

  //Chord length (also acts as scaling factor if chord length is not unity)
  double chordLength = args.Double("c");

  //Description for a symmetric, 4-digit NACA airfoil
  //NACA-00xx
  int xx  = args.Int("Camber");

  //Name of output filename
  std::string out_filename = args.String("OutputFile");

  //Information for extrusion of 2D VTK to 3D
  int nlevels_z = args.Int("Nz");
  double zstart = args.Double("zs");
  double zend   = args.Double("ze");
 
  //Number of regions with grid spacings other than the default grid spacing  
  //nRefinementRegions = 0 gives a uniformly distributed VTK across the chord length
  //Read in the number of points in the different regions of the airfoil
  std::vector<int> nPtRegions = args.IntVec("nRefinement");
  int nRegions = nPtRegions.size();
  int nRefinementRegions = nRegions-1;

  std::vector<double>  xRefinement = args.DoubleVec("xRefinement");

  if (nRefinementRegions != xRefinement.size())
    {
      std::cout << "[E] Size of input arguments \"nRefinement\" and \"xRefinement\" is inconsistent." << std::endl;
      std::cout << "[E] Size of \"nRefinement\"         : " << nRegions <<std::endl;
      std::cout << "[E] Expected size of \"xRefinement\": " << nRegions-1 <<std::endl;
      std::cout << "[E] Actual size of \"xRefinement\"  : " << xRefinement.size() <<std::endl;
      abort();
    }  
  
  // Default grid-spacing for the airfoil assuming no refinement regions exist, i.e., uniform grid-spacing from x = 0 to 1
  int nxloc = nPtRegions[0];
  double defaultDx = 1.0/nxloc;

  //nxloc = Total number of points along the chord (includes all points across all regions, 0 & 1 included)
  std::vector<double> xloc(2*nxloc,0.0), yloc(2*nxloc,0.0);
  
  if (nRefinementRegions == 0)
    {
      std::vector<double> xy_data;
      xy_data = uniformlyDistributedNACA00xx(xx, nxloc, defaultDx);
      geolytical::ExtrudedCurve2D foil3D(10, 0.0, 0.2, xy_data.data(), xy_data.size()/2);
      foil3D.OutputToVtk("output3D.vtk");
    }
  else
    {      
      double xstart = 0.0;
      double xend   = 0.0;
      int npts      = 0;
      std::vector<double> xy_0, xy_n, xy_i, xy_data, xy_data_final;
      for (int i = 0; i < nRegions; i++)
        {
          if (i == 0)
            {
              xstart   = 0.0;
              xend     = xRefinement[0];
              npts     = nPtRegions[0];
              xy_0     = NACA00xxPointDistribution(xx, npts, xstart, xend);
            }
          else if (i == (nRegions - 1))
            {
              xstart  = xRefinement.back();
              xend    = 1.0;
              npts    = nPtRegions.back();
              xy_n    = NACA00xxPointDistribution(xx, npts, xstart, xend);
              if (nRegions == 2)
                {
                  xy_data = concatenatePointDistributions(xy_0,xy_n);
                }
              else
                {
                  xy_data = concatenatePointDistributions(xy_data,xy_n);
                }
            }
          else
            {
              xstart   = xRefinement[i-1];
              xend     = xRefinement[i  ];
              npts     = nPtRegions[i];
              xy_i     = NACA00xxPointDistribution(xx, npts, xstart, xend);
              if (i == 1)
                {
                  xy_data  = concatenatePointDistributions(xy_0,xy_i);
                }
              else
                {
                  xy_data  = concatenatePointDistributions(xy_data,xy_i);
                }
            }
        } 
      xy_data_final = mirrorAndRearrangeNACA00xxData(xy_data);
      if (!is3D)
        {
          geolytical::Curve2D foil2D(&xy_data_final[0], xy_data_final.size()/2);
          foil2D.AddIntegerScalar("Components",[](double x, double y, double z)
                                  {
                                    if (x < 0.075) return 2;
                                    if (x > 0.075 && y > 0.0) return 3;
                                    return 1;                                
                                  });          
          foil2D.OutputToVtk(out_filename);
        }      
      else
        {
          geolytical::ExtrudedCurve2D foil3D(nlevels_z,zstart,zend,&xy_data_final[0], xy_data_final.size()/2);
          foil3D.AddIntegerScalar("Components",[](double x, double y, double z)
                                  {
                                    if (x < 0.075) return 2;
                                    if (x > 0.075 && y > 0.0) return 3;
                                    return 1;                                
                                  });
          foil3D.OutputToVtk(out_filename);
        }
      
    }
}
