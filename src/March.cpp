#include "gauravlib.h"

	// predictor step for interior
void Predictor(vector<CV>& w,  vector<CV>& wl, vector<CV>& wr, double dt)
{  	
	BC(w);
	Gravity(w); 
	Edge(w,wl,wr);
	vector<CV> wtemp(w);
	
	for (int j = 2; j < res-2; j++)
	{	
		FS hl= Hx(wr[j-1],wl[j]);

		FS hr= Hx(wr[j],wl[j+1]);
		
		FS source=Source(wtemp[j]);

		double db_dxi=(wtemp[j+1].b-wtemp[j-1].b)/(2*dx);
		double c = sqrt(1-pow(db_dxi,2));
		if (wtemp[j].b-dt*c*erosion<min_h)
			source.p = source.p*(wtemp[j].b-min_h)/(c*erosion*dt);
		w[j].b=max(min_h,w[j].b-dt*c*erosion);
		w[j].curve = 1/(2*c)*(w[j+1].b+w[j-1].b-2*w[j].b)/(dx*dx);
		w[j].Modify(wtemp[j].p - ((hr.p - hl.p) / dx - source.p) * dt
		, wtemp[j].q - ((hr.q - hl.q) / dx - source.q) * dt);	
	}
}

// corrector step for interior
void Corrector(vector<CV>& w,  vector<CV>& wl, vector<CV>& wr, vector<CV>& w_init, double dt)
{	
	BC(w);
	Gravity(w); 
	Edge(w,wl,wr);
    vector<CV> wtemp(w);

	for (int j=2; j < res-2; j++)
	{
		FS hr= Hx(wr[j],wl[j+1]);
		FS hl= Hx(wr[j-1],wl[j]);
		FS source=Source( wtemp[j]);
		double db_dxi=(wtemp[j+1].b-wtemp[j-1].b)/(2*dx);
		double c = sqrt(1-pow(db_dxi,2));
		if ( wtemp[j].b-dt*c*erosion<min_h && erosion>0)
			source.p = source.p*(wtemp[j].b-min_h)/(c*erosion*dt);
		w[j].b=max(min_h,wtemp[j].b*weight+(wtemp[j].b-dt*c*erosion)*(1-weight));	
		w[j].curve = 1/(2*c)*(w[j+1].b+w[j-1].b-2*w[j].b)/(dx*dx);
		w[j].Modify ( w_init[j].p * weight + (1 - weight) * (wtemp[j].p - ((hr.p - hl.p) / dx - source.p) * dt)
		, w_init[j].q * weight + (1 - weight) * (wtemp[j].q - ((hr.q - hl.q) / dx - source.q) * dt));
	}	
	Update_X(w,dt);
}

void March (vector<CV>& w)
{	
	double dt = dx / 4;
	static int timesteps=0;
	vector<CV> wl(w),wr(w);
	double sum=0;
	string file1=Output_folder;
	if(!filesystem::exists(file1))
		filesystem::create_directory(file1);
	static double t=0;

	while(t<finalt)
	{
		if(timesteps%dump==0 )
		{	
			fs::path base_path = Output_folder;
			fs::path file_name= string("field_")+to_string(int(timesteps/dump))+string(".csv");
			fs::path full_path = base_path / file_name;
			string file2=	full_path.string();
			Write(w, file2);
		}
		
		vector<CV> w_init(w);
		if (fric_type!="constant" && fric_type!="Constant")
			delta=std::min(Delta*t,Delta);
		
		
		Shed(w);
		Predictor(w,wl,wr,dt);
		Shed(w);
		Corrector(w, wl, wr, w_init, dt);
		
		Time_step(wl,wr,dt,t,timesteps);
		sum=0;
		for (int i=2; i<res-2;i++)
			sum+=(w[i].h*dx+w[i].b*(w[i+1].X-w[i-1].X)/2);
	
		std::cout<<std::setprecision(18)<<t<<"  "<<sum<<endl;

	}
	
}


void Time_step(vector <CV>& wl, vector <CV>& wr, double & dt, double & t, int & timesteps)
{	
	CFL(wl,wr,dt);
	// time updation
	t = t + dt;
	timesteps = timesteps + 1;
}

void CFL(vector<CV>& wl,vector<CV>& wr, double & dt)
{

	double maxspeed = 0.00000001;
	// to evaluate dt from maximum speeds (CFL condition)
	for (int j = 2; j < res - 2; j++)
	{
		double eig = max(abs(Ax(wr[j-1],wl[j],"max")),abs(Ax(wr[j-1],wl[j],"min")));
		if (maxspeed < eig)
			maxspeed = eig;
	}
	//std::cout<<maxspeed<<endl;
	dt = min(dx/4,dx/4/maxspeed);
}

