#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

//class def with subclass node 
class list{

	private:

		//node class has a constructor node pointer and element int for card face
		class node{
			public:
				node();
				node *next;
				int element;
		};
	public:

		//list constructor and destructor, insert function, and friended overload for << as well as node pointer for the sentinel (head of list)
		list();
		~list();
		bool insert(int inst);
		friend ostream &operator<<(ostream &out, const list &card);
	private:
		node *head;
};

const string face[] = { "Ace", "2", "3", "4", "5", "6", "7",
	"8", "9", "10", "Jack", "Queen", "King" }; 
const string suit[] = { "Clubs", "Diamonds", "Hearts", "Spades" };

string random_card(bool verbose=false) {
	string card;

	card = face[ rand()%13 ];
	card += " of ";
	card += suit[ rand()%4 ];

	if (verbose)
		cout << card << "\n";

	return card;
}

int main(int argc, char *argv[])
{
	bool verbose = false;
	int seedvalue = 0;

	for (int i=1; i<argc; i++) {
		string option = argv[i];
		if (option.compare(0,6,"-seed=") == 0) {
			seedvalue = atoi(&argv[i][6]);
		} else if (option.compare("-verbose") == 0) {
			verbose = true;
		} else 
			cout << "option " << argv[i] << " ignored\n";
	}

	srand(seedvalue);

	int table[4][13] = {};
	list hand[4] = {};
	int tmp;
	bool over = false;
	while (!over) {

		string card = random_card(verbose);

		//getting string data with istringstream
		string f;
		string junk;
		string s;
		istringstream c;
		c.str(card);
		c >> f;
		c >> junk;
		c >> s;

		//go through and fint the spot in the table that corresponds to the card values then add 1 card to that slot
		for(unsigned int h = 0; h < 4; h++){

			if(suit[h] == s){
				for(unsigned int i = 0; i < 13; i++){

					if(face[i] == f){

						//insert and check if inserted or deleted and adjust table properly
						if(hand[h].insert(i)){

							if(table[h][i] == 0){
								table[h][i]++;
							}
						}else{
							table[h][i]--;
						}
						
					}
				} 
			}
		}


		//check for break conditions
		for(int h = 0; h < 4; h++){

			if((table[h][10] > 0) && (table[h][11] > 0) && (table[h][12] > 0)){

				tmp = h;
				over = true;
			}
		}
	}

	//print in format
	cout << "   Clubs: " << hand[0];
	if(tmp == 0){

		cout << " **";
	}
	cout << endl;
	cout << "Diamonds: " << hand[1];
	if(tmp == 1){

		cout << " **";
	}
	cout << endl;
	cout << "  Hearts: " << hand[2];
	if(tmp == 2){

		cout << " **";
	}
	cout << endl;
	cout << "  Spades: " << hand[3];
	if(tmp == 3){

		cout << " **";
	}
	cout << endl;

}

//constructor to set next to null originally
list::node::node(){

	next = NULL;

}

//list constructor sets head to a new node
list::list(){

	head = new node;
}

//list destructor destroys head to prevent mem leaks
list::~list(){

	delete head;
}

//insert sets prev to the head and p to the next then inserts while p is null the first time to get something in the list. then while p != null (aka not at end of list) if p's element is greater than what we are inserting insert between prev and p and return true (1) if p -> element is equal then delet it from the list and return false (0) after checks increment through the list and repeat till end of list
bool list::insert(int inst){

	node *prev = head;
	node *p = prev -> next;
	node *insert = new node;

	if(p == NULL){

		prev -> next = insert;
		insert -> next = p;
		insert -> element = inst;
		return true;
	}else{
		while(p != NULL){

			if(p -> element > inst){

				prev -> next = insert;
				insert -> next = p;
				insert -> element = inst;
				return true;
			}

			if(p -> element == inst){

				prev -> next = p -> next;
				delete p;
				return false;
			}

			prev++;
			p++;
			
		}
	}
}
//overload the << operator to be able to print out the list. makes a node pointer and loops through till end of list outting the element as an index for global face to give faces as strings
ostream &operator<<(ostream &out, const list &card){

	list::node *p = card.head -> next;
	while(p != NULL){
		out << face[p -> element] << " ";

		p = p->next;
	}
	return out;

}
