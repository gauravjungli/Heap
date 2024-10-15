#include "gauravlib.h"

CV::CV(double h, double u, double b, double curve,  Grav g, double x, double X )
{	
	if (h<=min_h) 
	{  if (h<0) std::cout<<"Much smaller values encountered "<<h<<"  "<<x<< endl;
	h=min_h;
	u=0;
	}
	this->curve = curve;	this->g=g;
    this->h=h;	this->u=u;	this->b=b;
	this->x=x; 	this ->X= X;
    this->p=h; 	this->q=h*u; 
	this->psi= Psi(*this);
}

void CV::Modify(double p, double q)
{	this->h=p;
	if (h<min_h)
	{  
		if (h<0) std::cout<<"Much smaller values encountered in Modify "<<h<<"  "<<x<< endl;
		h=min_h;
		p=h;
		q=0;
	}
	this->p=p; this->q=q; 
	this->u=q/p;
	this->psi=Psi(*this);
}


FS Flux( CV w )

{
	FS f;

	 f.p = w.q;
	 f.q = (Bparam*w.u*w.u+k*w.psi*w.p/2)*w.p; 

	return f;
}


FS Source( CV w)
{
	FS source;
	double mu=tan(delta* PI / 180);
	double grav = w.p*w.g.X2-dratio*sliding*erosion*w.q/w.p;
	double fric = mu*max(w.psi,1e-8)*w.p;
	if (pow(pow(w.u,2),0.5)>1e-4) 
	{
		fric=(w.u/sqrt(pow(w.u,2)))*fric;
	}
	else 
	{
		fric=(grav>0?1:-1)*min(grav,fric);
	}
	source.p=dratio*erosion;
	source.q =grav-fric; 

	return source;
}


FS Eigen( CV w)
{
	double root,base;
	FS e;
	double param = Bparam;//*(1-2*w.curve);
	root =sqrt(w.u*w.u*(1-1/param)+w.g.X1*k*w.h/param);
	base= w.q/(w.p);
	e.p = param*(base-root);
	e.q = param*(base+root);
	return e;
}

void Update_X(vector<CV>& w,double dt)
{
	for (int i=1;i<res-1;i++)
	{
		double db_dxi=(w[i+1].b-w[i-1].b)/(2*dx);
		w[i].X = w[i].X - dt*erosion*db_dxi;
	}
	
}

