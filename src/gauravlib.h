#ifndef GAURAV_LIB
#define GAURAV_LIB

#include <iostream>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>
#include <sstream>
#include <filesystem>
#include <map>
#include <regex>
#include <sys/stat.h>
#include <sys/types.h>

using namespace std;
namespace fs = std::filesystem;

extern std::map <std::string, string> par;

extern const  int res;
extern const double PI;
extern const int dump;
extern const double xmax;
extern const double xmin;
extern const double weight;
extern const double finalt;
extern const double Delta;
extern const double theta;
extern const double dx;
extern const double min_h;
extern double delta;
extern const string fric_type;
extern const string Output_folder;
extern const double dratio;
extern double erosion;
extern const double Bparam;
extern const double sliding;
extern const double k;
extern const string shape;
extern double amp;
extern double sigma;
//------------------------------------------------------------------------------

//Class for storing a 2D gravity field

class Grav{
    public:
        double X1, X2;
        Grav(): X1(1), X2(0)
        {}
        
};

class AMB{
    public:
        std::vector<double> ang_mom;
        std::vector<double> inertia;
        AMB(double mom, double inr)
        {
            ang_mom=vector<double>(5,mom);
            inertia=vector<double>(5,inr);
        }
        
};

//To store conserved variables
class CV
{
    public:
        double p, q, h, u, b, x, X, psi, curve;
        Grav g ;
        CV(double h, double u, double b, double curve, Grav g, double x, double X );
        void Modify(double p, double q);
     
};

class FS
{
    public:
        double p, q;
        FS(): p(0), q(0){} 
        FS(CV w);
        FS operator+ (FS w); 
        FS operator- (FS w);  
        FS operator* (double w);   
        FS operator/ (double w);    
};




//---------------------------------------------------------------------------------

/////   I/O

//To write files 
void Write(const vector<CV> & w, string file  );
void Write ( std::map <std::string, string> par);

//To read files
void Read ( vector<double>&, string file );
bool Parameters();
void Read_grav( vector<Grav>& g, const string& file);

//To catch errors
void Error(string , string );

//to delete files
void deleteDirectoryContents(const std::string& dir_path);
std::string to_string(double value, int precision);

//----------------------------------------------------------------------------------------
///// bc.cpp
//Boundary conditions
void BC(vector<CV>& w );

//---------------------------------------------------------------------------------

//////// gavity.cpp
void Gravity(vector<CV>& w );

//--------------------------------------------------------------------------------

///////// TVD.cpp

//To be used for the limiters
double Derivative( double w1, double w2, double w3);

//Calculate minmod limiter
double Minmod(double a, double b, double c); 
FS Minmod(FS w, FS v); 
//--------------------------------------------------------------------------
//////////  IC.cpp

//To initialize the simulation
void IC (vector<CV> & w);
//To incorporate topography 
void Base(vector<CV>& w);
std::vector<double> compute_arc_lengths(const std::vector<double>& x_values);
double interpolate(double x1, double x2, double y1, double y2, double y);
void interpolate_uniform_arc_lengths(const std::vector<double>& arc_lengths, const std::vector<double>& x_values, std::vector<CV>& w );
double gaussian_derivative(double x) ;
double gaussian(double x) ;
//-------------------------------------------------------------------------

////////////// solver.cpp

//To be used in the solver terms

FS Hx( CV wl, CV wr);


//------------------------------------------------------------------------------------------

////////// Heap.cpp

//To calculate flux
FS Flux( CV w );

//To compute source terms
FS Source( CV w);
FS Eigen(CV w );
void Update_X(vector<CV>& w,double dt);

//---------------------------------------------------------------------------------

///////// characteristics.cpp

//To calculate eigen value and chareacteristics speed

double Ax(CV wl, CV wr, string s);
//values at edges
void Edge(vector<CV>& w, vector<CV>& wl, vector<CV>& wr);
void Reconstruct(CV& wl, CV w1, CV w2, CV w3, int sign );

//--------------------------------------------------------------------------------------------------------

/////// march.cpp

//To be used in the time marching
void March (vector<CV>& w);
void Predictor(vector<CV>& w,  vector<CV>& wl, vector<CV>& wr, double dt);
void Corrector(vector<CV>& w,  vector<CV>& wl, vector<CV>& wr, vector<CV>& w_init, double dt);
void Time_step(vector <CV>& wl, vector <CV>& wr, double & dt, double & t, int & timesteps);
void CFL(vector<CV>& wl,vector<CV>& wr, double & dt);

//--------------------------------------------------------------------------------------------------------------

///////// pressure_shed.cpp

//mass shedding and pressure
void Shed(vector<CV>& w);
double Psi(CV w);


////////
//double sin(double x);


#endif