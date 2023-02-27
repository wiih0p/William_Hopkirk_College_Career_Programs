//Will Hopkirk
//Dr. Marz
//3/29/19
//speeding ticket machine
//cite alex baldwin & Nick

#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <vector>
#include <sstream>

using namespace std;

//global constants for fine mults and months
const double IMULT = 5.2252, HMULT = 9.4412, RMULT = 17.1525, NA = 12.152;
const string MONTH[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

//date structure with month day and year
struct Date {

	int m, d, y;
};

//ticket structure with citation number and date as well as clocked speed speed limmit and road type
struct Ticket {

	string cnum;
	Date cdate;
	int cspeed, splim;
	char rdtype;
};

//database class with ticket vector and add ticket mutator and gen report accessor
class Database {

	vector<Ticket> ticks;

public:

	void add_ticket(Ticket &ro);
	vector<Ticket> gen_report(Date startDate, Date endDate);
};

//function prototypes
bool Read(Database &db, istream &fin);
bool Write(const vector<Ticket> &ticks, ostream &fout);
int Encode(const Date &d);


int main(int argc, char *argv[]){

	//vars for dates, filestreams, etc
	Date sd, ed;
	ifstream fin;
	ofstream fout;
	istringstream sin;
	Database db;
	string inf, outf;

	//check if theres enough arguments
	if(argc != 5){
		cout << "Usage: ./lab11 <input file> <start: year-month-day> <end: year-month-day> <output or '-'>" << endl;
	return -1;
	}
	//open input file and check if it opened
	else{
		fin.open(argv[1]);
		if(!fin.is_open()){
			cout << "The file '" << argv[1] << "' cannot be read." << endl;
			return -1;
		}
		//read input file and close
		Read(db, fin);
		fin.close();
	}
	//get dates from args 3 and 4
	sin.str(argv[2]);
	sin >> sd.y >> sd.m >> sd.d;
	sin.clear();
	sin.str(argv[3]);
	sin >> ed.y >> ed.m >> ed.d;
	sin.clear();

	//get files from args 2 and 5
	inf = argv[1];
	outf = argv[4];

	//check if user put a - for output and write accordingly to either file or terminal
	if(outf == "-"){
		Write(db.gen_report(sd, ed), cout);
	}
	else{
		fout.open(outf);

		if(!fout.is_open()){
			perror(outf.c_str());
		}
		Write(db.gen_report(sd, ed), fout);
	}
}

//encodes the date to fit the format to check between
int Encode(const Date &d){

	int numdate;
	numdate = (d.y * 10000) + (d.m * 100) + d.d;
	return numdate;
}
//funct to read the input file to collect all data from it
bool Read(Database &db, istream &fin){

	istringstream sin;
	string line;
	while(getline(fin, line)){
		Ticket sptkt;
		sin.str(line);
		sin >> sptkt.cnum >> sptkt.cdate.m >> sptkt.cdate.d >> sptkt.cdate.y >> sptkt.cspeed >> sptkt.splim >> sptkt.rdtype;
		
		if(sptkt.cdate.y < 100){
			sptkt.cdate.y += 2000;
		}
		db.add_ticket(sptkt);
		sin.clear();
	}
	return true;
}
//funct to write out the report to either the file or terminal in the requested format
bool Write(const vector<Ticket> &ticks, ostream &fout){

	double fine;

	if(ticks.size() < 1){

		fout << "Empty" << endl;
	}
	else{
		
		for(unsigned int i = 0; i < ticks.size(); i++){

			fine = ticks[i].cspeed - ticks[i].splim;

			//apply speeding multipliers
			switch (ticks[i].rdtype){

				case 'i':
				case 'I':
					fine *= IMULT;
					break;
				case 'r':
				case 'R':
					fine *= RMULT;
					break;
				case 'h':
				case 'H':
					fine *= HMULT;
					break;
				default:
					fine *= NA;
					break;
			}
			if(fine < 0){
				
				fine = 0.00;
			}
			fout << setw(2) << setfill('0') << ticks[i].cdate.d << '-' << MONTH[(ticks[i].cdate.m) - 1] << '-' << ticks[i].cdate.y << ' ' << left << setw(10) << ticks[i].cnum << " $" << right << fixed << setprecision(2) << fine << endl;
		}
	}
	return true;
}

//adds ticket to ticket vector
void Database::add_ticket(Ticket &ro){

	ticks.push_back(ro);
}

//generates a report based on start and end dates
vector<Ticket> Database::gen_report(Date startDate, Date endDate) {

	vector<Ticket> report;
	int ed = Encode(endDate);
	int sd = Encode(startDate);

	for(unsigned int i = 0; i < ticks.size(); i++){

		if((Encode(ticks[i].cdate) <= ed) && (Encode(ticks[i].cdate) >= sd)){

			report.push_back(ticks[i]);
		}
	}
	return report;
}

