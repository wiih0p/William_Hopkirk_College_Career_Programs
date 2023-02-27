/*
 *  * INSTRUCTOR-GIVEN TEMPLATE
 *   * COSC 140: Lab 8 - A Doubly-Linked List Implementation for Strings
 *    *
 *     * Description:
 *      *     Implements a doubly linked list to store std::strings. It uses a
 *       *     sentinel node and pointers to ensure bi-directional traversal. It is
 *        *     also circular, as the sentinel has access to the last and first
 *         *     elements.
 *          *
 *           * Author:
 *            *		will hopkirk
 *             */

#include <iostream>
#include "dlist.hpp"

// ----------------------------------------------------------------------------
// Constructors & Destructors                                              {{{1
// ----------------------------------------------------------------------------

/*
 * Dlist()                                                                 {{{2
 *
 * Default Constructor
 */

using namespace std;

Dlist::Dlist() {

	sentinel = new Dnode;
	sentinel->flink = sentinel;
	sentinel->blink = sentinel;
	size = 0;

}

/*
 * Dlist(const Dlist &d)                                                   {{{2
 *
 * Copy Constructor. This is a "deep copy". In other words, elements are copied
 * over from "d" by making Dnodes and coping over the elements, one-by-one.
 * They are not copied by copying pointers.
 */

Dlist::Dlist(const Dlist &d) {

	sentinel = new Dnode;
	sentinel->flink = sentinel;
	sentinel->blink = sentinel;
	size = 0;
	*this = d;
}

/*
 * operator=                                                               {{{2
 *
 * List copy operator (e.g. Assuming Dlist a, b... do "a = b"). Like the copy
 * constructor, this is also a "deep copy", not a "shallow copy".
 */

Dlist& Dlist::operator= (const Dlist &d) {
	
	this->Clear();
	this->size = 0;

	Dnode i = *d.sentinel->flink;
	for(int h = d.size; h > 0; h--){

		this->Push_Back(i.s);
		i = *i.flink;
	}
	
	return *this;
}

/*
 * ~Dlist                                                                  {{{2
 *
 * Destructor
 */

Dlist::~Dlist() {

	Clear();
	delete sentinel;
}

// ----------------------------------------------------------------------------
// Getters                                                                 {{{1
// ----------------------------------------------------------------------------


/*
 * Empty                                                                   {{{2
 *
 * Returns TRUE if the list is empty. FALSE otherwise.
 */

bool Dlist::Empty() const {
	
	return (size == 0);
}

/*
 * Size                                                                    {{{2
 *
 * Returns the number of nodes in the list (excluding the sentinel).
 */

size_t Dlist::Size() const {
	
	return size;
}

// ----------------------------------------------------------------------------
// Non-Front/Back Insertion Functions                                      {{{1
// ----------------------------------------------------------------------------

/*
 * Insert_Before                                                           {{{1
 *
 * Inserts a new node with string "s" before node "n" in the list.
 *
 * e.g. Assume list L as [S] -> [a] -> [b] -> [c].
 *      Assume n points to "b".
 *      Insert_Before("d", n)...
 *                       [S] -> [a] -> [d] -> [b] -> [c].
 */

void Dlist::Insert_Before(const std::string &s, Dnode *n) {

	Dnode *back = n->blink;
	Dnode *newn = new Dnode;
	newn->s = s;
	newn->flink = n;
	newn->blink = back;
	back->flink = newn;
	n->blink = newn;
	size++;
}

/*
 * Insert_After                                                            {{{1
 *
 * Inserts a new node with string "s" after node "n" in the list.
 *
 * e.g. Assume list L as [S] -> [a] -> [b] -> [c].
 *      Assume n points to "b".
 *      Insert_After("d", n)...
 *                       [S] -> [a] -> [b] -> [d] -> [c].
 */

void Dlist::Insert_After(const std::string &s, Dnode *n) {

	Dnode *front = n->flink;
	Dnode *newn = new Dnode;
	newn->s = s;
	newn->blink = n;
	newn->flink = front;
	front->blink = newn;
	n->flink = newn;
	size++;
}

// ----------------------------------------------------------------------------
// Deletion Handlers                                                       {{{1
// ----------------------------------------------------------------------------

/*
 * Erase                                                                   {{{2
 *
 * Deletes node "n" from the list.
 */

void Dlist::Erase(Dnode *n) {

	(void) n;

	Dnode *back = n->blink;
	Dnode *front = n->flink;
	back->flink = front;
	front->blink = back;

	delete n;
	size--;

}

/*
 * Clear                                                                   {{{2
 *
 * Clears the list of all elements except the sentinel node.
 */

void Dlist::Clear() {

	for(int i = size; i > 0; i--){

		Erase(sentinel->flink);
	}
	
}

// ----------------------------------------------------------------------------
// Push Functions                                                          {{{1
// ----------------------------------------------------------------------------

/* Put new strings on the front or back of the list */

/*
 * Push_Front                                                              {{{2
 *
 * Puts a new string on the front of the list.
 *
 * e.g. Assume list L as [S] -> [a] -> [b] -> [c].
 *      Push_Front(d)... [S] -> [d] -> [a] -> [b] -> [c].
 */

void Dlist::Push_Front(const std::string &s) {

	Insert_After(s, sentinel);
}

/*
 * Push_Back                                                               {{{2
 *
 * Puts a new string at the end of the list.
 *
 * e.g. Assume list L as [S] -> [a] -> [b] -> [c].
 *      Push_Back(d)...  [S] -> [a] -> [b] -> [c] -> [d].
 */

void Dlist::Push_Back(const std::string &s) {

	Insert_Before(s, sentinel);
}

// ----------------------------------------------------------------------------
// Pop Functions                                                           {{{1
// ----------------------------------------------------------------------------

/* Remove and return the first or last element of the list */

/*
 * Pop_Front                                                               {{{2
 *
 * Removes the first node from the list and returns its value.
 *
 * e.g. Assume list L as [S] -> [a] -> [b] -> [c].
 *      Pop_Front()...   [S] -> [b] -> [c].
 */

std::string Dlist::Pop_Front() {

	Dnode *front = sentinel->flink;
	if(sentinel->flink == sentinel){

		return "";
	}

	string ret = front->s;
	Erase(sentinel->flink);
	return ret;
}

/*
 * Pop_Back                                                                {{{2
 *
 * Removes the last node from the list and returns its value.
 *
 * e.g. Assume list L as [S] -> [a] -> [b] -> [c].
 *      Pop_Back()...    [S] -> [a] -> [b].
 */

std::string Dlist::Pop_Back() {

	Dnode *back = sentinel->blink;
	if(sentinel->blink == sentinel){

		 return "";
	 }

	string ret = back->s;
	Erase(sentinel->blink);
	return ret;

}

// ----------------------------------------------------------------------------
// Node Accessor Functions                                                 {{{1
// ----------------------------------------------------------------------------

/*
 * Begin                                                                   {{{2
 *
 * Returns a pointer to the first node on the list.
 */

Dnode *Dlist::Begin() const {
	 
	return sentinel->flink;
}

/*
 * End                                                                     {{{2
 *
 * Returns a pointer to the node past the last node on the list.
 */

Dnode *Dlist::End() const {
	
	return sentinel;
}

/*
 * Rbegin                                                                  {{{2
 *
 * Returns a pointer to the last node on the list.
 */

Dnode *Dlist::Rbegin() const {
	
	return sentinel->blink;
}

/*
 * Rend                                                                    {{{2
 *
 * Returns a pointer to the node before the first node on the list.
 */

Dnode *Dlist::Rend() const {
	
	return sentinel;
}



