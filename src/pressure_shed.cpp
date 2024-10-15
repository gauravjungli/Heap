#include "gauravlib.h"
void Shed(vector<CV>& w)
{

for ( int j = 2; j < res-2; j++)
{	
 	if (w[j].psi <= (1e-8) ) 
	{	
		cout<< "Basal friciton is turning negative at cell number "<<j<<  endl;
	} 

}

}

double Psi(CV w)
{
return	(w.g.X1+2*Bparam*w.curve*(w.u*w.u));
}