#include <iostream>
#include <iomanip>
#include <cstdio>
#include <string>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <time.h>
#include <set>
#include <stdlib.h>
#include <cstring>
#include <fstream>
#include <utility>
#include <sstream>
#include <cmath>

using namespace std;

//city class
class city{

	public:

		//checks if theres a # in the line if so ignores it and inputs the other lines
		friend istream & operator>>(istream &in, city &r) { 

			in >> r.scrap;

			if(r.scrap != '#'){
				r.zone = ((int)r.scrap - 48);
				in >> r.name;
				in >> r.type;
				in >> r.latitude;
				in >> r.longitude;
				in >> r.population;

				return in;
			}else{

				in.ignore(256, '\n');
				//cout << "skipped" << " " << r.scrap << endl;

				return in;
			}

		}
		//outputs contents of a city
		friend ostream & operator<<(ostream &out, city &r) { 

			out << setw(18) << left << r.name;
			out << setw(12) << left << r.type;
			out << setw(2) << left << r.zone;
			out << setw(10) << right << r.population;
			out << setw(8) << right << setprecision(2) << fixed << r.latitude;
			out << setw(8) << right << setprecision(2) << fixed << r.longitude;
			return out;
		}

		//get functions
		string get_name()     { return this->name;      }
		string get_type()     { return this->type;      }
		int get_zone()        { return this->zone;      }
		float get_latitude()  { return this->latitude;  }
		float get_longitude() { return this->longitude; }
		int get_population()  { return this->population;}

	private:

		//scrap is how i checked for # lines
		char scrap;
		string name;
		string type;
		int zone;
		float latitude;
		float longitude;
		int population;

};

//costtable class
class costtable{

	public:

		vector<int> mode_table;
		vector<float> distance_table;
		vector<float> time_table;
};

//reads input and fills city vector
void read_cityinfo(vector<city> &mp){

	ifstream fin;
	fin.open("city_list.txt");

	while(!fin.eof()){

		city tmp;
		fin >> tmp;
		//cout << tmp.get_zone() << " " << tmp.get_name() << " " << tmp.get_type() << " " << tmp.get_latitude() << endl;
		if(tmp.get_name() != ""){

			mp.push_back(tmp);
		}
	}
	fin.close();
}

//writes to cityinfo output folder in format
void write_cityinfo(vector<city> &mp) { 

	ofstream fout;
	fout.open("city_info.txt");

	fout << "CITY INFO (N=34):" << endl;
	fout << endl;

	for(unsigned int i = 0; i < mp.size(); i++){

		fout << setw(2) << right << i << " ";
		fout << mp[i] << endl;
	}
	//cout << "wrote" << endl;
	fout.close();
}

//smol helper to convert to radians
float toRadians(float r){

	return (r * (M_PI / 180));
}

//big function for getting all of the distances and putting them into a costtable
void haversine(vector<city> &mp, costtable &r){

	int t = 1;

	for(unsigned int i = 1; i < mp.size(); i++){

		float tmplat1;
		float tmplong1;

		tmplat1 = mp[i].get_latitude();
		tmplong1 = mp[i].get_longitude();

		for(int j = 0; j < t; j++){

			int mode = 0;
			float tmplat2;
			float tmplong2;

			tmplat2 = mp[j].get_latitude();
			tmplong2 = mp[j].get_longitude();

			float difflat = abs(tmplat1 - tmplat2);
			float difflong = abs(tmplong1 - tmplong2);

			difflat = toRadians(difflat);
			difflong = toRadians(difflong);

			//one way
			//float latt = sin(difflat * 0.5);
			//latt *= latt;
			//float longt = sin(difflong * 0.5);
			//longt *= longt;
			//float tmpd = cos(tmplat1) * cos(tmplat2);

			//float y = 2.0 * asin(sqrt(latt + tmpd*longt));

			//another way (better)(working)
			float step1 = pow(sin(difflat/2), 2) + cos(toRadians(tmplat1)) * cos(toRadians(tmplat2)) * pow(sin(difflong/2), 2);
			float step2 = 2 * asin(sqrt(step1));

			//cout << step1 << " ";
			//cout << step2 << endl;
			float distance = (3982 * step2);
			distance = 10*round(distance/10.0);

			//this section populates a mode vector for the three rules we had 0 = ground, 1 = air
			if(mp[i].get_type() == "REGIONAL" && mp[j].get_type() == "REGIONAL"){

				//cout << "REGIONAL TO REGIONAL" << " ";
				mode = 0;
			}
			else if(((mp[i].get_type() == "REGIONAL" && mp[j].get_type() == "GATEWAY") || (mp[i].get_type() == "GATEWAY" && mp[j].get_type() == "REGIONAL")) && (mp[i].get_zone() == mp[j].get_zone())){

				//cout << "GATEWAY AND REGIONAL IN SAME ZONE" << " ";
				mode = 1;
			}
			else if(mp[i].get_type() == "GATEWAY" && mp[j].get_type() == "GATEWAY"){

				//cout << "GATEWAY TO GATEWAY" << " ";
				mode = 1;
			}
			else{

				//cout << "GATEWAY AND REGIONAL IN DIFF ZONES" << " ";
			}

			r.mode_table.push_back(mode);
			//cout << mode << endl;
			r.distance_table.push_back(distance);

			//cout << i  << " " << mp[i].get_name() << " to " << mp[j].get_name() << " " << distance << "  miles" << endl;
		}
		t++;
		//cout << endl;
	}

}

//fills time vector of costtable using the mode 0 = ground 1 = air
void calc_time(costtable &r){

	for(unsigned int i = 0; i < r.distance_table.size(); i++){

		float time;
		float tmp = r.distance_table[i];

		if(r.mode_table[i] == 0){

			time = (tmp / 60);
		}
		else if(r.mode_table[i] == 1){

			time = (tmp / 570);
		}

		r.time_table.push_back(time);
	}
}

//both of the next two functions fill a table with distance or time
void write_distancetable(vector<city> &mp, costtable &r) { 


	ofstream fout;
	fout.open("city_distancetable.txt");

	int x = 0;
	int t = 1;
	fout << "DISTANCE TABLE:" << endl;
	fout << endl;
	for(unsigned int i = 1; i < mp.size(); i++){

		for(int j = 0; j < t; j++){

			string op = (mp[i].get_name() + " to " + mp[j].get_name() + " " );
			fout << " " << setfill(' ') << setw(2) << right << i << " "; 
			fout << setfill('.') << setw(38) << left << op << setprecision(1) << fixed << setfill(' ') << setw(7) << right << r.distance_table[x] << " miles" << endl;

			x++;
		}
		t++;
		fout << endl;
	}
}

void write_timetable(vector<city> &mp, costtable &r) {


	ofstream fout;
	fout.open("city_timetable.txt");

	int x = 0;
	int t = 1;
	fout << "TIME TABLE:" << endl;
	fout << endl;
	for(unsigned int i = 1; i < mp.size(); i++){

		for(int j = 0; j < t; j++){

			string op = (mp[i].get_name() + " to " + mp[j].get_name() + " " );
			fout << " " << setfill(' ') << setw(2) << right << i << " ";
			fout << setfill('.') << setw(38) << left << op << setprecision(1) << fixed << setfill(' ') << setw(5) << right << r.time_table[x] << " hours" << endl;

			x++;
		}
		t++;
		fout << endl;
	}
}

//create_citygraph() { }
//write_citygraph() { }

//dijkstra_route() { }

int main(int argc, char *argv[])
{

	//cmd line check
	if(argc != 2){

		cerr << "USAGE: ./citysim1 -graphinfo" << endl;
		return -1;
	}

	if(strcmp(argv[1], "-graphinfo") != 0) {

		cerr << "USAGE: ./citysim1 -graphinfo" << endl;
		return -1;
	}

	//  city graph declarations
	vector<city> cm;

	//read cities
	read_cityinfo(cm);

	// set up costtables
	costtable ct;

	if(strcmp(argv[1], "-graphinfo") == 0) {

		write_cityinfo(cm);
		haversine(cm, ct);
		calc_time(ct);
		write_distancetable(cm, ct);
		write_timetable(cm, ct);
	}

	//while (not done) {
	//enter> from, to cities
	//dijkstra_route(costmode,from,to) 
	//}
	return 0;
}
