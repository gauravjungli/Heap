
#include "gauravlib.h"
#include <chrono>

std::map <std::string, string> par;

bool set_parameter=Parameters();
const int res= (int) round(stod(par["Resolution"]));
const double PI= M_PI;
const int dump= int(stod(par["dump"]));
const double xmax=   stod(par["Length of simulation"]);
const double xmin=   -xmax;
const double weight= stod(par["weight"]); 
const double finalt= stod(par["Landslide simulation period"]);
const double Delta= stod(par["Friction angle"]); 
const double theta= stod(par["theta"]); 
const double dx= (xmax-xmin)/(res);
const double min_h=pow(dx,3);
double delta=Delta;
const string fric_type=par["Friction type"];
const double dratio =  stod(par["Density ratio"]);
double erosion = stod(par["Erosion rate"]);
const double Bparam = stod(par["Boussinesq parameter"]);
const double sliding = stod(par["Sliding velocity"]);
const double k = stod(par["Earth pressure coefficient"]);
double amp = stod(par["Amplitude of gaussian"]);
double sigma = stod(par["Standard deviation"]);
const string Output_folder=par["Data folder"];

int main()
{

	chrono::steady_clock sc;
	auto start = sc.now();
	std::string file=Output_folder;
	
	//std::ofstream outfile("c++_output.txt",std::ofstream::app);  // Create or open output file

	//std::streambuf *coutbuf = std::cout.rdbuf(); 
   // std::streambuf *cerrbuf = std::cerr.rdbuf(); 
 //   if (outfile.is_open()) {
 //       std::cout.rdbuf(outfile.rdbuf()); // Redirect cout
  //      std::cerr.rdbuf(outfile.rdbuf()); // Redirect cerr
	//}

	fs::path base_path = file;
	
	vector<CV> w;

	//uncomment only for the solo run

	if(filesystem::exists(file))
		deleteDirectoryContents(file);
	filesystem::create_directory(file); 
	

	

	fs::path file_name= string("field")+string(".csv");
	fs::path full_path = base_path / file_name;
	string file1=	full_path.string();


	IC(w);

	March(w);
	
	auto end = sc.now();
	auto time_span = static_cast<chrono::duration<double>>(end - start);   // measure time span between start & end
   	cout<<"Operation took: "<<time_span.count()<<" seconds !!! "<<endl;
	cout<<"----------------------------------------------------------------------------"<<endl;
	cout<<"----------------------------------------------------------------------------"<<endl;

	//std::cout.rdbuf(coutbuf); 
   // std::cerr.rdbuf(cerrbuf);
	//outfile.close();
	return 0;
}
