//lab10
//lab A?
//
//will Hopkirk

#include <vector>
#include <string>
#include <iostream>
#include <cstdio>
#include "bstree.hpp"
using namespace std;
using CS140::BSTree;
using CS140::BSTNode;

/* ---------------------------------------------- */
/* You'll write all of these as part of your lab. */

int BSTree::Depth(const string &key) const
{
	//get root and check if it is the end of tree
  BSTNode *n = sentinel->right;
  while(key != n->key){

	  n = (key < n->key) ? n->left : n->right;
	  if(n == sentinel){

		  return -1;
	  }
  }
  if(n->parent != sentinel){

	  return (Depth(n->parent->key) + 1);
  }else{

	  return 0;
  }
}
           
int BSTree::Height() const
{
 
	//just calling the function that gets the height
	return (recursive_find_height(sentinel->right) + 1);
}

vector <string> BSTree::Ordered_Keys() const
{
	//use make key to make a ordered vector of keys
  vector <string> ret;
  make_key_vector(sentinel->right, ret);
  return ret;
}
    
BSTree::BSTree(const BSTree &t)        
{

	//setting things up constructor
	sentinel = new BSTNode;
	sentinel->left = NULL;
	sentinel->right = sentinel;
	sentinel->parent = NULL;
	sentinel->key = "---SENTINEL---";
	sentinel->val = NULL;
	size = 0;
	*this = t;
}

BSTree& BSTree::operator= (const BSTree &t) 
{
	//copy constructor delete then rebuild
  Clear(); 
  size = t.Size();
  vector<string> keys = t.Ordered_Keys();
  vector<void *> vals = t.Ordered_Vals();
  sentinel->right = make_balanced_tree(keys, vals, 0, t.Size());
  return *this;
}

int BSTree::recursive_find_height(const BSTNode *n) const
{
  
	//error check then recursively find the heights and compare them to return the true height
    if (n == sentinel){  
        
         return -1;
    }else{  

         //find height
         int lheight = recursive_find_height(n->left);  
         int rheight = recursive_find_height(n->right);  
      
        //return larger height
         if (lheight > rheight){  
            
             return(lheight + 1);
         }else{
        
         return(rheight + 1);
         }  
     }
}

void BSTree::make_key_vector(const BSTNode *n, vector<string> &v) const
{
 
	//recursive helper for ordered keys
	if(n == sentinel){

		return;
	}
	make_key_vector(n->left, v);
	v.push_back(n->key);
	make_key_vector(n->right, v);
}

BSTNode *BSTree::make_balanced_tree(const vector<string> &sorted_keys, 
                            const vector<void *> &vals,
                            size_t first_index,
                            size_t num_indices) const
{
	//copy helper balance tree with left and right recursion like all the other helpers
  if(num_indices == 0){

	  return sentinel;
  }
  int m = (num_indices / 2) + first_index;
  BSTNode *n = new BSTNode;
  n->key = sorted_keys[m];
  n->val = vals[m];
  n->parent = sentinel;
  n->left = make_balanced_tree(sorted_keys, vals, first_index, (m - first_index));
  n->right = make_balanced_tree(sorted_keys, vals, (m + 1), (num_indices + first_index - m - 1));
  if(n->left != sentinel){

		n->left->parent = n;
  }
  if(n->right != sentinel){

		n->right->parent = n;
  }
  return n;

}
