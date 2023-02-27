//will hopkirk
//2/12/19
//marz
//check if a date is between the current date

#include <iostream>
#include <limits>

using namespace std;

int main(){

	int startYear;
	int startDay;
	int startMonth;
	int startTime;
	int endYear;
	int endDay;
	int endMonth;
	int endTime;
	int year;
	int day;
	int month;
	int time;

	
		cout << "Enter start date: ";
		cin >> startYear >> startMonth >> startDay;

		startTime = ((startYear * 365) + (startMonth * 30) + startDay);

		cout << "Enter end date  : ";
		cin >> endYear >> endMonth >> endDay;

		endTime = ((endYear * 365) + (endMonth *30) +endDay);

		cout << "Enter date      : ";
		cin >> year >> month >> day;

		time = ((year * 365) + (month * 30) + day);

		if(time < endTime && time > startTime){

			cout << "IN RANGE" << endl;
		}
		else{
			cout << "OUT OF RANGE" << endl;
		}

			






}
