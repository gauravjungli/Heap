#include "gauravlib.h"

void IC (vector<CV> & w)
{   
    vector<double>  b(res,min_h),curve(res,0), X(res,0), x(res,0);
    vector <double> h(res,min_h), u(res,0);
    vector<Grav> g (res);
        for (int i=0;i<res;i++)
        {   
            x[i]=  dx * (i+0.5);
        }
    
    if (w.empty())
    {
	    for (int j = 0; j < res; j++)
        {
            CV temp(h[j],u[j],b[j],curve[j],g[j],x[j], X[j]);
            w.push_back(temp);
        } 
    }
	Base(w);   
}

void Base(vector<CV>& w)
{   
     // Define grid points in x direction
    int N = 10*res+1; // Number of grid points

    std::vector<double> x_values(N);
    
    for (int i=0;i<N;i++)
        {   
            x_values[i]= (xmax)/(N-1) * i;
        }
    
    // Step 1: Compute arc lengths for each grid point
    std::vector<double> arc_lengths = compute_arc_lengths(x_values);
    
    // Step 2: Interpolate to get x at uniformly distributed arc lengths
    interpolate_uniform_arc_lengths(arc_lengths, x_values, w);
    

    for (int j=0;j<res;j++)
            w[j].b=max(min_h,gaussian(w[j].X));
     Gravity(w);  
    for (int j=1;j<res-1;j++)
    {
        double c = sqrt(1-pow((w[j+1].b-w[j-1].b)/(2*dx),2));
        w[j].curve = 1/(2*c)*(w[j+1].b+w[j-1].b-2*w[j].b)/(dx*dx);
        w[j].Modify(w[j].p,w[j].q);
    }   
}


// Gaussian function
double gaussian(double x) 
{
    return amp * exp(-(x ) * (x) / (2.0 * sigma * sigma));
}

// Derivative of Gaussian
double gaussian_derivative(double x) 
{
    return -x / (sigma * sigma) * gaussian(x);
}

// Function to compute arc length at each grid point
std::vector<double> compute_arc_lengths(const std::vector<double>& x_values) 
{
    std::vector<double> arc_lengths(x_values.size(), 0.0);

    double arc_length = 0.0;

    for (size_t i = 1; i < x_values.size(); ++i) 
    {
        double dx = x_values[i] - x_values[i - 1];
        double dy_dx1 = gaussian_derivative(x_values[i - 1]);
        double dy_dx2 = gaussian_derivative(x_values[i]);
        double integrand1 = sqrt(1 + dy_dx1 * dy_dx1);
        double integrand2 = sqrt(1 + dy_dx2 * dy_dx2);
        arc_length += 0.5 * (integrand1 + integrand2) * dx; // Trapezoidal rule
        arc_lengths[i] = arc_length;
    }

    return arc_lengths;
}

// Linear interpolation function
double interpolate(double x1, double x2, double y1, double y2, double y) 
{
    return x1 + (y - y1) * (x2 - x1) / (y2 - y1);
}

// Function to get uniform arc length parametrization
void interpolate_uniform_arc_lengths(const std::vector<double>& arc_lengths, const std::vector<double>& x_values, std::vector<CV>& w ) 
{

    size_t j = 0;
    for (int i = res/2; i < res; ++i)
     {
        // Find the interval where the target arc length lies
        while (j < arc_lengths.size() - 1 && arc_lengths[j + 1] < (w[i].x-xmax)) {
            ++j;
        }
        
        // Linearly interpolate the x value corresponding to the uniform arc length
        w[i].X = interpolate(x_values[j], x_values[j + 1], arc_lengths[j], arc_lengths[j + 1], (w[i].x-xmax));
    }

    for (int i = res/2-1; i >= 0; --i)
    {
        w[i].X=-w[res-1-i].X;
    }
}
