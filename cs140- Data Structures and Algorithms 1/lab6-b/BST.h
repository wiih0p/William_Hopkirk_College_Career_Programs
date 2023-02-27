#ifndef BST_H
#define BST_H

#include <iomanip>
#include <iostream>
#include <queue>
#include <vector>
#include <string>
using namespace std;

int y = 0;

template <class TKey>
class bst {
	struct node {
		//node(ID);
		//node();

		void print();


		TKey key;
		//ID parameter

		//parent info
		node *left;
		node *right;
		node *parent = NULL;
		int id;
	};

	public:

	//included iterator class
	class iterator {
		public:
			iterator();
			//overloaded operators (++, *, ==, !=)
			iterator &operator++();
			TKey &operator*();
			bool operator==(const iterator &) const;
			bool operator!=(const iterator &) const;
		private:
			friend class bst<TKey>;

			iterator(node *);

			node *p;
	};

	//begin and end
	iterator begin();
	iterator end();

	public:
	bst() { Troot=NULL; }
	~bst() { clear(Troot); }

	bool empty() { return Troot==NULL; }

	void insert(TKey &);

	//bounds
	iterator lower_bound(const TKey &);
	iterator upper_bound(const TKey &);

	void print_bylevel();

	private:
	void clear(node *);
	node *insert(node *, TKey &);

	//ID parameter
	node *Troot;
};

//node for taversal and another to track last time t went left
template <class TKey>
typename bst<TKey>::iterator bst<TKey>::lower_bound(const TKey &key){

	node *t;
	node *n;
	n = NULL;
	t = Troot;
	//checks vals in tree and stores in n the lower bound then returns an iterator to that
	while(t != NULL){

		if(key <= t->key){

			n = t;
			t = t->left;
		}
		else if(!(key <= t->key)){

			t = t->right;
		}
	}
	iterator i(n);
	return i;
}

//same as lower but < instead of <=
template <class TKey>
typename bst<TKey>::iterator bst<TKey>::upper_bound(const TKey &key){

	node *t;
	node *n;
	n = NULL;
	t = Troot;
	while(t != NULL){

		if(key < t->key){

			n = t;
			t = t->left;
		}
		else if(!(key < t->key)){

			t = t->right;
		}
	}
	iterator i(n);
	return i;

}

//constructors to set p to null or ptr if given
template <class TKey>
bst<TKey>::iterator::iterator(){

	p = NULL;
}

template <class TKey>
bst<TKey>::iterator::iterator(bst<TKey>::node *ptr){

	p = ptr;
}

//overload operators

//++ overload in order traversal and returns an iterator to the next node in tree
template <class TKey>
typename bst<TKey>::iterator & bst<TKey>::iterator::operator++(){

	node *n;
	if(p->right != NULL){

		p = p->right;
		while(p->left != NULL){

			p = p->left;
		}
	}else{

		n = p;
		p = p->parent;
		while(n == p->right && p->parent != NULL){

			n = p;
			p = p->parent;
		}
		if(p->parent == NULL && n == p->right){

			p = NULL;
		}
	}

	iterator i(p);
	return i;

}

//accesses node key
template <class TKey>
TKey & bst<TKey>::iterator::operator*(){

	return p->key;
}

//comparison operators
template <class TKey>
bool bst<TKey>::iterator::operator==(const bst<TKey>::iterator &rhs) const {

	if(p == rhs.p){

		return true;
	}
	return false;
}

template <class TKey>
bool bst<TKey>::iterator::operator!=(const bst<TKey>::iterator &rhs) const {

	if(p != rhs.p){

		return true;
	}
	return false;
}

//BST functions

//goes as far left of the tree as possible
template <class TKey>
typename bst<TKey>::iterator bst<TKey>::begin(){

	if(Troot == NULL){

		iterator i;
		return i;
	}else{

		node *tmp;
		tmp = Troot;
		while(tmp->left != NULL){

			tmp = tmp->left;
		}
		iterator j(tmp);
		return j;
	}

}

//returns an iterator to NULL ptr
template <class TKey>
typename bst<TKey>::iterator bst<TKey>::end(){

	iterator i;
	return i;
}

//wasnt able to iterate ids through the constructor so they are commented out
//template <>
//bst<Tkey>::node::node(){

//	id = 0;
//	parent = NULL;
//}

//template <>
//bst<Tkey>::node::node(ID){

//	id = ID;
//	parent = NULL;
//}

//added a print for the id and check for parent node
	template <class TKey>
void bst<TKey>::node::print()
{
	cout << setw(3) << id << ' ';
	cout << setw(3) << key << " : ";

	//output node and parent ID information

	if(parent == NULL){

		cout << "ROOT ";
	}else{

		cout << "P=  " << parent->id;
	}
	//change below to output subtree ID information


	if (left)  cout << " L=" << setw(3) << left->id;
	else       cout << "      ";
	if (right) cout << " R=" << setw(3) << right->id;
	else       cout << "      ";

	cout << "\n";
}

//specialized string version of the above goes here


	template <>
void bst<string>::node::print()
{
	cout << setw(3) << id << ' ';
	cout << setw(20) << key << " : ";

	//output node and parent ID information

	if(parent == NULL){

		cout << "ROOT ";
	}else{

		cout << "P=  " << parent->id;
	}
	//change below to output subtree ID information


	if (left)  cout << " L=" << setw(3) << left->id;
	else       cout << "      ";
	if (right) cout << " R=" << setw(3) << right->id;
	else       cout << "      ";

	cout << "\n";
}

//bst<TKey>::iterator functions not defined above go here

	template <class TKey>
void bst<TKey>::clear(node *T)
{
	if (T) {
		clear(T->left);
		clear(T->right);
		delete T;
		T = NULL;
	}
}

	template <class TKey>
void bst<TKey>::insert(TKey &key)
{
	Troot = insert(Troot, key);
}

//added check for id iteration as well as updating the parent nodes for each new node
	template <class TKey>
class bst<TKey>::node *bst<TKey>::insert(node *T, TKey &key)
{
	//set parent link below
	if (T == NULL) {
		//update and set node ID
		y++;
		T = new node;
		T->id = y;
		T->key = key;
	} else if (T->key == key) {
		;
	} else if (key < T->key) {
		T->left = insert(T->left, key);
		T->left->parent = T;
	} else {
		T->right = insert(T->right, key);
		T->right->parent = T;
	}

	return T;
}

//bst<TKey>::lower_bound function goes here

//bst<TKey>::upper_bound function goes here

	template <class TKey>
void bst<TKey>::print_bylevel()
{
	if (Troot == NULL)
		return;

	queue<node *> Q;
	node *T;

	Q.push(Troot);
	while (!Q.empty()) {
		T = Q.front();
		Q.pop();

		T->print();
		if (T->left)  Q.push(T->left);
		if (T->right) Q.push(T->right);
	}
}
#endif
