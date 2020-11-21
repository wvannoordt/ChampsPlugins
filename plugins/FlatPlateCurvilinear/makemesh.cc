#include <iostream>
#include <string>
#include <fstream>
#include <sys/stat.h>
#include <unistd.h>
#include <cmath>
#include <stdio.h>
#include "NamedArgs.h"
#define DEBUGOUTPUT 1
#define d_abs(myarg) (((myarg)<0) ? -(myarg) : (myarg))
bool keepOld = false;
inline bool FileExists (const std::string& name)
{
    std::ifstream f(name.c_str());
    return f.good();
}
void OutputVtk3D(double* x, double* y, double* z, int nxNodes, int nyNodes, int nzNodes, std::string filename)
{
    std::ofstream myfile(filename);
    myfile << "# vtk DataFile Version 3.0" << std::endl;
    myfile << "vtk mesh output" << std::endl;
    myfile << "ASCII" << std::endl;
    myfile << "DATASET RECTILINEAR_GRID" << std::endl;
    myfile << "DIMENSIONS " << nxNodes << " " << nyNodes << " " << nzNodes << std::endl;
    myfile << "X_COORDINATES " << nxNodes << " double" << std::endl;
    myfile << x[0];
    for (int i = 1; i < nxNodes; i++) myfile << " " << x[i];
    myfile << std::endl;
    myfile << "Y_COORDINATES " << nyNodes << " double" << std::endl;
    myfile << y[0];
    for (int i = 1; i < nyNodes; i++) myfile << " " << y[i];
    myfile << std::endl;
    myfile << "Z_COORDINATES " << nzNodes << " double" << std::endl;
    myfile << z[0];
    for (int i = 1; i < nzNodes; i++) myfile << " " << z[i];
    myfile << std::endl;
    myfile.close();
}
void OutputVtk2D(double* x, double* y, double* z, int nxNodes, int nyNodes, int nzNodes, std::string filename)
{
    std::ofstream myfile(filename);
    myfile << "# vtk DataFile Version 3.0" << std::endl;
    myfile << "vtk mesh output" << std::endl;
    myfile << "ASCII" << std::endl;
    myfile << "DATASET RECTILINEAR_GRID" << std::endl;
    myfile << "DIMENSIONS " << nxNodes << " " << nyNodes << " " << 1 << std::endl;
    myfile << "X_COORDINATES " << nxNodes << " double" << std::endl;
    myfile << x[0];
    for (int i = 1; i < nxNodes; i++) myfile << " " << x[i];
    myfile << std::endl;
    myfile << "Y_COORDINATES " << nyNodes << " double" << std::endl;
    myfile << y[0];
    for (int i = 1; i < nyNodes; i++) myfile << " " << y[i];
    myfile << std::endl;
    myfile << "Z_COORDINATES " << 1 << " double" << std::endl;
    myfile << 0.0;
    myfile << std::endl;
    myfile.close();
}
void OutputCsv(double* x, double* y, double* z, int nxNodes, int nyNodes, int nzNodes)
{
    std::ofstream myfilex("x.csv");
    for (int i = 0; i < nxNodes; i++) myfilex << x[i] << std::endl;    
    myfilex.close();
    
    std::ofstream myfiley("y.csv");
    for (int i = 0; i < nyNodes; i++) myfiley << y[i] << std::endl;
    myfiley.close();
    
    std::ofstream myfilez("z.csv");
    for (int i = 0; i < nzNodes; i++) myfilez << z[i] << std::endl;
    myfilez.close();
}

double FindGrowthRate(double x0, double xn, double dx, double Ngrowth, double Ntotal)
{
    double alpha = (xn-x0) / dx;
    double value = 1.4;
    double error = 10000;
    int maxIts = 1000;
    double errorTol = 1e-7;
    int Ng = Ngrowth;
    int N = Ntotal;
    int numIts = 0;
    while (d_abs(error) > errorTol)
    {
        double r = value;
        //double f = (N-Ng)*pow(r, Ng)*(1-r) - pow(r, Ng) + 1 - alpha*(1-r);
        //double fprime = (N-Ng)*pow(r, Ng-1)*(1-r) - (N-Ng)*pow(r, Ng) - pow(r, Ng) + 1 + alpha;
        double f = -alpha + (N-Ng)*pow(r, Ng) + (1-pow(r, Ng))/(1-r);
        double fprime = ((r-1)*(Ng*pow(r, Ng-1)) + 1 - pow(r, Ng))/((1-r)*(1-r)) + Ng*(N-Ng)*pow(r, Ng-1);
        error = f/fprime;
        value = value - 0.1*error;
        if (numIts++ > maxIts)
        {
            std::cout << "FindGrowthRate diverged!" << std::endl;
            abort();
        }
    }
    std::cout << "Solved.\nError: " << error << "\nValue: " << value << std::endl;
    return value;
}

int GetNum(double x0, double xn, double dx, double r, int nb, int gb)
{
    //solve for N
    double alpha = (xn-x0) / dx;
    int Ng = nb*gb;
    int N = Ng + (pow(r, -Ng)*(alpha*(1-r) + pow(r, Ng) - 1))/(1-r);
    return nb*(N/nb)+1;
}

void ComputeYDirectionStuff(int* nyCells, int* numBlocksY, double** yNodes, int blockSize, double* growthRateEstimate, double ymin, double ymax, int growthBlocks, double dy)
{
    
    int NytotalNodes = GetNum(ymin, ymax, dy, *growthRateEstimate, blockSize, growthBlocks);
    double growthRate = FindGrowthRate(ymin, ymax, dy, growthBlocks*blockSize, NytotalNodes-1);
    *growthRateEstimate = growthRate;
    *nyCells = NytotalNodes-1;
    *numBlocksY = NytotalNodes/blockSize;
    *yNodes = (double*)malloc(NytotalNodes*sizeof(double));
    double* y = *yNodes;
    int Ng = growthBlocks*blockSize;
    double r = growthRate;
    for (int i = 0; i <= Ng; i++)
    {
        y[i] = ymin + dy*(1-pow(r, i))/(1-r);
    }
    for (int i = Ng+1; i < NytotalNodes; i++)
    {
        y[i] = y[Ng] + (i-Ng)*pow(r, Ng)*dy;
    }
    std::cout << "Nodes (y):    " << NytotalNodes << std::endl;
    std::cout << "Target ymax:  " << ymax << std::endl;
    std::cout << "Compute ymax: " << y[NytotalNodes-1] << std::endl;
    
}

static inline std::string NiceCommaString(size_t num)
{
    std::string numstr = std::to_string(num);
    std::string output = "";
    for (int i = numstr.length()-1; i >= 0; i--)
    {
        output = numstr[i] + output;
        if ((i!=0)&&(i!=numstr.length()-1)&&((numstr.length()-i)%3)==0) output = "," + output;
    }
    return output;
}

void OutputInfoFile(int xNodes, int yNodes, int zNodes, int blockSize, std::string filename)
{
    std::ofstream myfile(filename);
    myfile << "blocks (X):" << std::endl;
    myfile << ((xNodes-1) / blockSize) << std::endl;
    myfile << "blocks (Y):" << std::endl;
    myfile << ((yNodes-1) / blockSize) << std::endl;
    myfile << "blocks (Z):" << std::endl;
    myfile << ((zNodes-1) / blockSize) << std::endl;
    myfile.close();
}

void OutputMesh(double* x, double* y, double* z, int nx, int ny, int nz)
{
    int numZones = 1;
    FILE* fileWriter;
    if (!(FileExists("mesh_2D.p3d")&&keepOld))
    {
        fileWriter = fopen("mesh_2D.p3d", "w+b");
        fwrite(&numZones, 1, sizeof(int), fileWriter);
        fwrite(&nx, 1, sizeof(int), fileWriter);
        fwrite(&ny, 1, sizeof(int), fileWriter);
        for (int j = 0; j < ny; j++){for (int i = 0; i < nx; i++){fwrite((x+i), 1, sizeof(double), fileWriter);}}
        for (int j = 0; j < ny; j++){for (int i = 0; i < nx; i++){fwrite((y+j), 1, sizeof(double), fileWriter);}}
        fclose(fileWriter);
    }
    if (!(FileExists("mesh_3D.p3d")&&keepOld))
    {
        fileWriter = fopen("mesh_3D.p3d", "w+b");
        fwrite(&numZones, 1, sizeof(int), fileWriter);
        fwrite(&nx, 1, sizeof(int), fileWriter);
        fwrite(&ny, 1, sizeof(int), fileWriter);
        fwrite(&nz, 1, sizeof(int), fileWriter);
        for (int k = 0; k < nz; k++){for (int j = 0; j < ny; j++){for (int i = 0; i < nx; i++){fwrite((x+i), 1, sizeof(double), fileWriter);}}}
        for (int k = 0; k < nz; k++){for (int j = 0; j < ny; j++){for (int i = 0; i < nx; i++){fwrite((y+j), 1, sizeof(double), fileWriter);}}}
        for (int k = 0; k < nz; k++){for (int j = 0; j < ny; j++){for (int i = 0; i < nx; i++){fwrite((z+k), 1, sizeof(double), fileWriter);}}}
        fclose(fileWriter);
    }
}

extern "C"
{
    void Initialize(int argc, char** argv);
}

void Initialize(int argc, char** argv)
{
    NamedArgs args(argc, argv);
    double xmin = args.Double("xmin");
    double xmax = args.Double("xmax");
    double ymin = args.Double("ymin");
    double ymax = args.Double("ymax");
    double zmin = args.Double("zmin");
    double zmax = args.Double("zmax");
    double growthRate = args.Double("growthRate");
    int growthBlocks = args.Int("growthBlocks");
    double Re = args.Double("Re");
    double Cf = args.Double("Cf");
    double dxPlus = args.Double("dxPlus");
    double dyPlus = args.Double("dyPlus");
    double dzPlus = args.Double("dzPlus");
    int blockSize = args.Int("blockSize");
    
    
    keepOld = false;
    double dy = dyPlus / (Re * sqrt(0.5*Cf));
    double dx = dxPlus * (dy/dyPlus);
    double dz = dzPlus * (dy/dyPlus);
    std::cout << "-----  INPUT  -----" << std::endl;
    std::cout << "Growth: " << growthRate << std::endl;
    std::cout << "(dx, dy, dz)^+ = (" << dxPlus << ", " << dyPlus << ", " << dzPlus << ")" << std::endl;
    std::cout << "(dx, dy, dz)   = (" << dx << ", " << dy << ", " << dz << ")" << std::endl;
    
    int nxCells = blockSize*((int)((xmax-xmin)/dx)/blockSize);
    int numBlocksX = nxCells/blockSize;
    int nzCells = blockSize*((int)((zmax-zmin)/dz)/blockSize);
    int numBlocksZ = nzCells/blockSize;
    
    std::cout << "----- COMPUTE -----" << std::endl;
    int nyCells, numBlocksY;
    double* yNodes;
    ComputeYDirectionStuff(&nyCells, &numBlocksY, &yNodes, blockSize, &growthRate, ymin, ymax, growthBlocks, dy);
    double* xNodes = (double*)malloc((nxCells+1)*sizeof(double));
    double* zNodes = (double*)malloc((nzCells+1)*sizeof(double));
    int nxNodes = nxCells + 1;
    int nyNodes = nyCells + 1;
    int nzNodes = nzCells + 1;
    for (int i = 0; i < nxNodes; i++) xNodes[i] = xmin + i*(xmax-xmin)/nxCells;
    for (int i = 0; i < nzNodes; i++) zNodes[i] = zmin + i*(zmax-zmin)/nzCells;
    
    std::cout << "----- OUTPUT  -----" << std::endl;
    double actualDx = (xmax-xmin)/nxCells;
    double actualDz = (zmax-zmin)/nzCells;
    std::cout << "Growth: " << growthRate << std::endl;
    std::cout << "(dx, dy, dz)^+ = (" << dxPlus << ", " << dyPlus << ", " << dzPlus << ")" << std::endl;
    std::cout << "(dx, dy, dz)   = (" << actualDx << ", " << dy << ", " << actualDz << ")" << std::endl;
    size_t totalMeshSize = nxCells*nyCells*nzCells;
    std::cout << "Mesh size: " << nxCells << " x " << nyCells << " x " << nzCells << std::endl;
    std::cout << "Total points: " << NiceCommaString(totalMeshSize) << std::endl;
    std::cout << "Estimated Proc Count:     " << (int)((double)totalMeshSize/150000) << std::endl;
    double timePerStep = 3.0;
    std::cout << "Estimated Timestep (s):   " << timePerStep << std::endl;
    
#if(DEBUGOUTPUT)
    //OutputCsv(xNodes, yNodes, zNodes, nxNodes, nyNodes, nzNodes);
    OutputVtk3D(xNodes, yNodes, zNodes, nxNodes, nyNodes, nzNodes, "mesh3D.vtk");
    OutputVtk2D(xNodes, yNodes, zNodes, nxNodes, nyNodes, nzNodes, "mesh2D.vtk");
#endif
    OutputInfoFile(nxNodes, nyNodes, nzNodes, blockSize, ".mesh.info");
    OutputMesh(xNodes, yNodes, zNodes, nxNodes, nyNodes, nzNodes);
    
    free(xNodes);
    free(yNodes);
    free(zNodes);
}
