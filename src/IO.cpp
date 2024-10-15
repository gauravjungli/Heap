#include "gauravlib.h"


std::string to_string(double value, int precision) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(precision) << value;
    return oss.str();
}

void Write (const vector<CV>& w, string file)
{
ofstream myfile(file);
if (!myfile) Error("Can't open output file field",file);
for (int i=0;i<res;i++)
	myfile<<std::setprecision(18)<<w[i].X<<","<<w[i].b<<","<<w[i].h/w[i].g.X1<<","<< w[i].u<<","<<w[i].psi<<","<<w[i].curve<<"\n";
myfile.close();
}


void Read ( vector<double>& v,string file)
{
ifstream myfile(file);
if (!myfile) Error("Can't open input file",file);
double inp;
while(myfile>>inp)
		v.push_back(inp);
myfile.close();
}


void Error (string s1, string s2)
{
	std::cout<< s1<<" "<<s2<<endl;
}


void deleteDirectoryContents(const std::string& dir_path)
{
    for (const auto& entry : std::filesystem::directory_iterator(dir_path)) 
        std::filesystem::remove_all(entry.path());
}


bool Parameters()
{   
    fs::path cwd = fs::current_path();
    fs::path pwd = cwd.parent_path();
    fs::path par_path = pwd /"input" / "parameters";
	ifstream myfile(par_path);
	if (!myfile) Error("Can't open file", "parameters");
	string line;
	while (getline(myfile, line))  
    {
        if (line.find("-----")!=std::string::npos)
            continue;

        line = regex_replace(line, regex("^\\s+|\\s+$"), ""); 

        // Split the line based on multiple spaces
        istringstream iss(line);
        string key, value,arrow;

        // Get the key (potentially with spaces)
        
        getline(iss, key, '-'); 
         key = regex_replace(key, regex("^\\s+|\\s+$"), "");
        // Discard multiple spaces
        getline(iss, arrow, ' '); 
        while (iss.peek() == '\t' || iss.peek() == ' ') 
        {
            iss.get(); 
        }

        // Get the remaining part as the value
        getline(iss, value); 
        value = regex_replace(value, regex("^\\s+|\\s+$"), "");
        par[key] = value;
     
    }
    myfile.close();
    return true;
}




// Function to read a 2D array from a file
void Read_grav( vector<Grav>& g, const string& file)
{ 
    std::ifstream f(file);
    std::vector<double> row;
    if (!f) Error("Can't open file", "grav.txt");
    double num;
    int i=0;
    while (f >> num)
    {
        row.push_back(num);

        // Check if the row is complete
        if (row.size() == 2) 
        {
            g[i].X1=row[0];
            g[i].X2=row[1];
            row.clear();
            i++;
        }
    }
}