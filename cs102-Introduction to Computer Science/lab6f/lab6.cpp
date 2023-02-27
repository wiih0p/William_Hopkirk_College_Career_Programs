//will hopkirk
//marz
//2/27/2019
//traffic ticket calculator
//cite Alex Baldwin


//include libraries needed for filestreams strings ect
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>

using namespace std;

int main(){


	// declare variables for getting dates, and constants for multipliers
	const double IMULT = 5.2252, HMULT = 9.4412, RMULT = 17.1525, NA = 12.152;
	int Sdate, Edate, Fdate;
	int smonth, sday, syear;
	int emonth, eday, eyear;
	int y, d, m;

	//declare vars for files, road factors, and array of short months
	string cnumber;
	int cspeed, slimit;
	char rdtype;
	string months[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
	double fine;
	string Tfile;
	string Rfile;


	// get and check for filename of the input file
	cout << "Enter a Ticket File: ";
	cin >> Tfile;
	ifstream fin;
	istringstream sin;
	fin.open(Tfile);
	if (!fin.is_open()){
		cout << "Unable to open " << Tfile << '.' << endl;
		return 1;
	}
	
	// get and check for the filename of the output file
	cout << "Enter a report file: ";
	cin >> Rfile;
	ofstream fout;
	fout.open(Rfile);
	if (!fin.is_open()){
		cout << "Unable to open " << Rfile << '.' << endl;
		return 2;
	}

	//get and convert the start date
	cout << "Enter report start date (mm dd yyyy): ";
	cin >> smonth >> sday >> syear;
	//checks if year has 2 digits and corrects it
	if(syear <= 99){
		syear = (syear + 2000);
	}
	Sdate = (syear * 10000 + smonth * 100 + sday);

	//get and convert the end date
	cout << "Enter report end date (mm dd yyyy): ";
	cin >> emonth >> eday >> eyear;
	if (eyear <= 99){
		eyear = (eyear + 2000);
	}
	Edate = (eyear*10000 + emonth*100 +eday);

	//declare needed variable for stringstream
	string line;

	// while loop for getting lines of data from the input file and stringing into lines 
	while(getline(fin, line)){
		stringstream sso;
		sso << line;
		sso >> cnumber >> m >> d >> y >> cspeed >> slimit >> rdtype;
		if (y <= 99){
			y = (y + 2000);
		}
		//getting overall date
		Fdate = (y * 10000 + m * 100 + d);
		cspeed = cspeed - slimit;
		m--;
		//check if date is between the start and the end
		if (Fdate < Sdate || Fdate > Edate){
			continue;
		}

		//checks for the road type and multiplies accordingly to the fine
		switch (rdtype){
		
			case 'i':
			case 'I':
				fine = cspeed * IMULT;
				break;
			case 'r':
			case 'R':
				fine = cspeed * RMULT;
				break;
			case 'h':
			case 'H':
				fine = cspeed * HMULT;
				break;
			default:
				fine = cspeed * NA;
				break;
		}

		// if the fine is less than 0, make the total fine 0
		if(fine < 0){
			fine = 0;
		}

		//formatting for output
		fout << setw(2) << setfill('0') << d << "-" << months[m] << '-' << y << ' ';
		fout << setw(10) << setfill(' ') << left << cnumber << " $";
		fout << setw(9) << setfill(' ') << right << fixed << setprecision(2) << fine << endl;
	}

	fin.close();
	fout.close();
	return 0;

}
