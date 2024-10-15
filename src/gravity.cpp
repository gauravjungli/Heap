#include "gauravlib.h"
void Gravity(vector<CV>& w )
{   

	for (int i=1;i<res-1;i++)
	{
		w[i].g.X2 = -(w[i+1].b-w[i-1].b)/(2*dx);
		w[i].g.X1 = sqrt(1-pow(w[i].g.X2,2));
	}
	
}


