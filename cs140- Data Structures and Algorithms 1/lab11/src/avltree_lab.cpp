//lab11
//will hopkirk

#include <cmath>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include "avltree.hpp"
using namespace std;
using CS140::AVLTree;
using CS140::AVLNode;


void fix_height(AVLNode *n){

	//error check
	if(n->height == 0){

		return;
	}
	n->height = max(n->left->height, n->right->height) + 1;
}

inline void node_quickset(AVLNode *n, AVLNode *l, AVLNode *r, AVLNode *p){

	if(l) n->left = l;
	if(r) n->right = r;
	if(p) n->parent = p;
}

void rotate(AVLNode *n){

	AVLNode *p, *g, *l, *r; //parent, grandparent, left child, right child
	//direction of rotato
	//error check
	if(n->height == 0 || n->parent->height == 0){

		return;
	}

	p = n->parent;
	g = p->parent;
	l = n->left;
	r = n->right;

	if(p->right == n){

		//rotate left
		node_quickset(n, p, NULL, g);
		node_quickset(p, NULL, l, n);
		node_quickset(l, NULL, NULL, p);
		if(g->right == p){

			node_quickset(g, NULL, n, NULL);
		}else{

			node_quickset(g, n, NULL, NULL);
		}
		fix_height(n->left);
		fix_height(n->right);
		fix_height(n);
		fix_height(p);
	}
	if(p->left == n){

		//rotate right
		node_quickset(n, NULL, p, g);
		node_quickset(p, r, NULL, n);
		node_quickset(r, NULL, NULL, p);
		if(g->right == p){

			node_quickset(g, NULL, n, NULL);
		}else{

			node_quickset(g, n, NULL, NULL);
		}
		fix_height(n->left);
		fix_height(n->right);
		fix_height(n);
		fix_height(p);

	}
}
void fix_imbalance(AVLNode *n){

	AVLNode *p, *c;
	bool zigzig, zigzag;
	//check if we need zig zig or zig zag rules
	if(n->left->height > n->right->height){

		p = n->left;
		if(p->left->height > p->right->height){

			c = p->left;
			zigzig = true;
			zigzag = false;
		}else if(p->left->height < p->right->height){

			c = p->right;
			zigzig = false;
			zigzag = true;
		}else{

			c = p->left;
			zigzig = true;
			zigzag = false;
		}
	}else{

		p = n->right;
		if(p->left->height > p->right->height){

			c = p->left;
			zigzig = false;
			zigzag = true;
		}else if(p->left->height < p->right->height){

			c = p->right;
			zigzig = true;
			zigzag = false;
		}else{

			c = p->right;
			zigzig = true;
			zigzag = false;
		}
	}

	//rotate accordingly
	if(zigzig){

		rotate(p);
	}else if(zigzag){

		rotate(c);
		rotate(c);
	}

}

bool imbalance(const AVLNode *n){

	return abs((int) n->left->height - (int) n->right->height) > 1;
}

AVLTree& AVLTree::operator= (const AVLTree &t)        
{

	Clear();
	size = t.Size();
	sentinel->right = recursive_postorder_copy(t.sentinel->right);
	sentinel->right->parent = sentinel;
	return *this;
}

/* I simply took Insert and Delete from their binary search tree
   implementations.  They aren't correct for AVL trees, but they are
   good starting points.  */

bool AVLTree::Insert(const string &key, void *val)
{
	AVLNode *parent;
	AVLNode *n;

	parent = sentinel;
	n = sentinel->right;

	/* Find where the key should go.  If you find the key, return false. */

	while (n != sentinel) {
		if (n->key == key) return false;
		parent = n;
		n = (key < n->key) ? n->left : n->right;
	}

	/* At this point, parent is the node that will be the parent of the new node.
	   Create the new node, and hook it in. */

	n = new AVLNode;
	n->key = key;
	n->val = val;
	n->parent = parent;
	n->height = 1;
	n->left = sentinel;
	n->right = sentinel;

	/* Use the correct pointer in the parent to point to the new node. */

	if (parent == sentinel) {
		sentinel->right = n;
	} else if (key < parent->key) {
		parent->left = n;
	} else {
		parent->right = n;
	}

	//my add on
	size_t tmp = 0;
	n = n->parent;
	while(n != sentinel){

		tmp = n->height;
		fix_height(n);
		if(tmp != n->height){
			if(imbalance(n)){

				fix_imbalance(n);
				break;
			}
		}
		n = n->parent;
	}
	/* Increment the size */

	size++;
	return true;
}

bool AVLTree::Delete(const string &key)
{
	AVLNode *n, *parent, *mlc;
	string tmpkey;
	void *tmpval;

	/* Try to find the key -- if you can't return false. */

	n = sentinel->right;
	while (n != sentinel && key != n->key) {
		n = (key < n->key) ? n->left : n->right;
	}
	if (n == sentinel) return false;

	/* We go through the three cases for deletion, although it's a little
	   different from the canonical explanation. */

	parent = n->parent;

	/* Case 1 - I have no left child.  Replace me with my right child.
	   Note that this handles the case of having no children, too. */

	if (n->left == sentinel) {
		if (n == parent->left) {
			parent->left = n->right;
		} else {
			parent->right = n->right;
		}
		if (n->right != sentinel) n->right->parent = parent;
		delete n;
		size--;

		/* Case 2 - I have no right child.  Replace me with my left child. */

	} else if (n->right == sentinel) {
		if (n == parent->left) {
			parent->left = n->left;
		} else {
			parent->right = n->left;
		}
		n->left->parent = parent;
		delete n;
		size--;

		/* If I have two children, then find the node "before" me in the tree.
		   That node will have no right child, so I can recursively delete it.
		   When I'm done, I'll replace the key and val of n with the key and
		   val of the deleted node.  You'll note that the recursive call 
		   updates the size, so you don't have to do it here. */

	} else {
		for (mlc = n->left; mlc->right != sentinel; mlc = mlc->right) ;
		tmpkey = mlc->key;
		tmpval = mlc->val;
		Delete(tmpkey);
		n->key = tmpkey;
		n->val = tmpval;
		return true;
	}

	//my add on
	while(parent != sentinel){

		fix_height(parent);
		if(imbalance(parent)){

			fix_imbalance(parent);
		}
		parent = parent->parent;
	}
	return true;
}

/* You need to write these two.  You can lift them verbatim from your
   binary search tree lab. */

vector <string> AVLTree::Ordered_Keys() const
{
	vector <string> ret;
	make_key_vector(sentinel->right, ret);
	return ret;
}

void AVLTree::make_key_vector(const AVLNode *n, vector<string> &v) const
{
	if(n == sentinel) return;
	make_key_vector(n->left, v);
	v.push_back(n->key);
	make_key_vector(n->right, v);
}

size_t AVLTree::Height() const
{
	return sentinel->right->height;
}

/* You need to write this to help you with the assignment overload.
   It makes a copy of the subtree rooted by n.  That subtree is part
   of a different tree -- the copy will be part of the tree that
   is calling the method. */

AVLNode *AVLTree::recursive_postorder_copy(const AVLNode *n) const
{
	//same
	if(n->height == 0){

		return sentinel;
	}

	AVLNode *tmp = new AVLNode;
	tmp->height = n->height;
	tmp->key = n->key;
	tmp->val = n->val;
	tmp->left = recursive_postorder_copy(n->left);
	tmp->right = recursive_postorder_copy(n->right);

	if(tmp->left->height != 0){

		tmp->left->parent = tmp;
	}
	if(tmp->right->height != 0){

		tmp->right->parent = tmp;
	}

	return tmp;
}
