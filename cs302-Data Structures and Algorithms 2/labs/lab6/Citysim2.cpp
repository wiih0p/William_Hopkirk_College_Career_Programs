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
#include <map>

using namespace std;

//city class
class city{

	public:

		//checks if theres a # in the line if so ignores it and inputs the other lines
		friend istream & operator>>(istream &in, city &r) { 

			r.time = -1;
			r.visited = false;
			r.backedge = NULL;
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

		vector<city> adj;
		int pos;
		float time;
		city *backedge;
		bool visited;

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

//helper class
struct edge{

	edge(float d, int yeet, int yote){

		distance = d;
		i = yeet;
		j = yote;
	}

	float distance;
	float time;
	int i;
	int j;
};
//costtable class
class costtable{

	public:

		costtable(vector<city> &mp){

			mappo = mp;
		}

		float lookup(int mode, int yeet, int yote){

			if(yeet < yote){

				int tmp;
				tmp = yeet;
				yeet = yote;
				yote = tmp;
			}

			float ret;
			if(mode == 0){

				for(unsigned int h = 0; h < this->edge_table.size(); h++){

					if(yeet == this->edge_table[h].i && yote == this->edge_table[h].j){

						ret = this->edge_table[h].distance;
					}
				}

			}
			else if(mode == 1){

				for(unsigned int h = 0; h < this->edge_table.size(); h++){

					if(yeet == this->edge_table[h].i && yote == this->edge_table[h].j){

						ret = this->edge_table[h].time;
					}
				}

			}
			return ret;
		}

		vector<city> mappo;
		vector<int> mode_table;
		vector<float> distance_table;
		vector<float> time_table;
		vector<edge> edge_table;
};

//reads input and fills city vector
void read_cityinfo(vector<city> &mp){

	ifstream fin;
	fin.open("city_list.txt");

	int i = 0;
	while(!fin.eof()){

		city tmp;
		fin >> tmp;
		//cout << tmp.get_zone() << " " << tmp.get_name() << " " << tmp.get_type() << " " << tmp.get_latitude() << endl;
		if(tmp.get_name() != ""){

			tmp.pos = i;
			mp.push_back(tmp);
			i++;
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
			edge e(distance, mp[i].pos, mp[j].pos);
			r.edge_table.push_back(e);

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
		r.edge_table[i].time = time;
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

//find closest gateway to a given city
city FCG(costtable &r, vector<city> &mp, int mode, city &y){

	city ret;

	if(mode == 0) {

		float tmp = 20000.0;
		//find closest gateway for regional in the same zone
		int zone = y.get_zone();

		for(unsigned int i = 0; i < mp.size(); i++){

			if(mp[i].get_name() != y.get_name()){

				if(mp[i].get_type() == "GATEWAY"){

					if(mp[i].get_zone() == zone){

						if(r.lookup(0, mp[i].pos, y.pos) < tmp){

							ret = mp[i];
							tmp = r.lookup(0, mp[i].pos, y.pos);
						}
					}
				}
			}
		}
	}

	//find closest gateway to gatway in another zone
	else if(mode == 1){

		int zone = y.get_zone();

		for(unsigned int i = 0; i < mp.size(); i++){

			float tmp = 5200.0;
			if(mp[i].get_name() != y.get_name()){

				if(mp[i].get_type() == "GATEWAY"){

					if(mp[i].get_zone() != zone){

						if(r.lookup(0, mp[i].pos, y.pos) < tmp){

							ret = mp[i];
							tmp = r.lookup(0, mp[i].pos, y.pos);
						}
					}
				}
			}
		}

	}

	return  ret;
}

//fills all cities adjacency lists with ~provided~ rules
void create_citygraph(costtable &r, vector<city> &mp) { 

	for(unsigned int i = 0; i < mp.size(); i++){

		bool yeet = false;
		city curCity = mp[i];
		for(unsigned int j = 0; j < mp.size(); j++){

			city curCity1 = mp[j];

			if(curCity.get_name() != curCity1.get_name()){

				if(curCity.get_type() == "REGIONAL"){

					if(curCity1.get_type() == "REGIONAL" && curCity.get_zone() == curCity1.get_zone()){

						mp[i].adj.push_back(curCity1);
					}
					else if(curCity1.get_type() == "GATEWAY" && curCity.get_zone() == curCity1.get_zone()){

						if(yeet == false){

							mp[i].adj.push_back(FCG(r, mp, 0, curCity));
							yeet = true;
						}

					}
				}
				else{
					if(curCity1.get_type() == "REGIONAL" && curCity.get_zone() == curCity1.get_zone()){

						if(FCG(r, mp, 0, curCity1).pos == curCity.pos){

							mp[i].adj.push_back(curCity1);
						}
					}
					else if(curCity1.get_type() == "GATEWAY" && curCity.get_zone() == curCity1.get_zone()){

						mp[i].adj.push_back(curCity1);
					}
					else if(curCity1.get_type() == "GATEWAY" && curCity.get_zone() != curCity1.get_zone()){

						if(FCG(r, mp, 1, curCity).pos == curCity1.pos){

							mp[i].adj.push_back(curCity1);
						}
						else if(FCG(r, mp, 1, curCity1).pos == curCity.pos){

							mp[i].adj.push_back(curCity1);
						}		
					}
				}
			}
		}
	}
}

	/*
	 this was dumb
	//add additional adjacencies
	
	vector<city> addit;
	addit = mp;

	for(unsigned int i = 0; i < mp.size(); i++){

		for(unsigned int j = 0; j < mp.size(); j++){

			for(unsigned int k = 0; k < mp[j].adj.size(); k++){

				if(mp[j].adj[k].pos == mp[i].pos){
					
					addit[i].adj.push_back(mp[j]);
				}
			}
		}

		for(unsigned int j = 0; j < addit.size(); j++){

			mp[i].adj.push_back(addit[j]);
		}
	}

	*/



//writes the adjacencies of each city
void write_citygraph(costtable &r, vector<city> &mp) { 

	ofstream fout;
	fout.open("city_graph.txt");

	fout << "CITY GRAPH:" << endl << endl;

	for(unsigned int i = 0; i < mp.size(); i++){

		fout << " " << setfill(' ') << setw(2) << right << mp[i].pos << " " << mp[i].get_name() << endl;

		for(unsigned int j = 0; j < mp[i].adj.size(); j++){

			fout << " " << setfill(' ') << setw(5) << right << mp[i].adj[j].pos << " " << setw(19) << left << mp[i].adj[j].get_name() << " ";
			fout << setprecision(1) << fixed << setw(6) << right << r.lookup(0, mp[i].pos, mp[i].adj[j].pos) << " miles" << " ";
			fout << setprecision(1) << fixed << setw(5) << right << r.lookup(1, mp[i].pos, mp[i].adj[j].pos) << " hours" << endl;
		}
		fout << endl;
	}
}

//meant to find shortest route
void dijkstra_route(costtable &r, vector<city> &mp, int i, int j) { 


	vector<city> path;
	float totmiles;
    float tottime;
	city from = mp[i];
	city to = mp[j];
	city temp;
	float tmptime;
	from.time = 0;

	multimap<float, city> yeet;

	multimap<float, city>::iterator it;
	yeet.insert(make_pair<float, city>(0, from));

	while(!yeet.empty()){

		it = yeet.begin();
		temp = it->second;
		tmptime = it->first;
		yeet.erase(it);

		for(unsigned int i = 0; i < temp.adj.size(); i++){

			tottime = temp.adj[i].time + tmptime;

			tottime += r.lookup(1, temp.pos, temp.adj[i].pos);

			if((temp.adj[i].time == -1 || temp.adj[i].time > tottime) && temp.pos != to.pos){

				temp.adj[i].time = tottime;
				temp.adj[i].backedge = &temp;
				cout << "found totally accurate path to " << temp.adj[i].backedge->get_name() << endl;
				yeet.insert(make_pair<float, city>(tottime, temp.adj[i]));
			}
		}
	}

	temp = to;
	while(temp.backedge != NULL){

		path.push_back(*temp.backedge);
		temp = *temp.backedge;
	}

	/*
	for(unsigned int i = 0; i < path.size(); i++){

		cout << path[i].get_name() << endl;
	}
	*/

}

int main(int argc, char *argv[])
{

	//cmd line check
	if(argc != 2){

		cerr << "USAGE: ./citysim1 -graphinfo" << endl;
		return -1;
	}

	//  city graph declarations
	vector<city> cm;

	//read cities
	read_cityinfo(cm);

	// set up costtables
	costtable ct(cm);

	if(strcmp(argv[1], "-graphinfo") == 0) {

		write_cityinfo(cm);
		haversine(cm, ct);
		calc_time(ct);
		write_distancetable(cm, ct);
		write_timetable(cm, ct);

		create_citygraph(ct, cm);

		write_citygraph(ct, cm);

		//cout << cm[0].adj[2] << endl;
		//dijkstra_route(ct, cm, 23, 8);
	}
	 if(strcmp(argv[1], "-distance") == 0 || strcmp(argv[1], "-time") == 0) {

		write_cityinfo(cm);
		haversine(cm, ct);
		calc_time(ct);
		write_distancetable(cm, ct);
		write_timetable(cm, ct);

		create_citygraph(ct, cm);

		write_citygraph(ct, cm);

		string c1, c2;
		int p1, p2;
		cout << "Enter> ";
		cin >> c1 >> c2;

		for(unsigned int i = 0; i < cm.size(); i++){

			if(c1 == cm[i].get_name()){

				p1 = cm[i].pos;
			}
			if(c2 == cm[i].get_name()){

				p2 = cm[i].pos;
			}
		}

		dijkstra_route(ct, cm, p1, p2);
		cout << "Definitely made it to: " << c2 << endl;
									
	 }
																		 

	//while (not done) {
	//enter> from, to cities
	//dijkstra_route(costmode,from,to) 
	//}
	return 0;
}
