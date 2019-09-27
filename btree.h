/**
 * The btree is a linked structure which operates much like
 * a binary search tree, save the fact that multiple client
 * elements are stored in a single node.  Whereas a single element
 * would partition the tree into two ordered subtrees, a node 
 * that stores m client elements partition the tree 
 * into m + 1 sorted subtrees.
 * Created by Arvind Bahl.
 */

#ifndef BTREE_H
#define BTREE_H

#include <iostream>
#include <cstddef>
#include <utility>
#include <vector>
#include <algorithm>
#include<queue>
using namespace std;

//include the iterator
#include "btree_iterator.h"

// we do this to avoid compiler errors about non-template friends

template<typename T> class btree;
template<typename T> std::ostream &operator<<(std::ostream&, const btree<T>&);

template <typename T> 
class btree {
 public:
  /** Iterator typedefs here **/

	friend class btree_iterator<T>;
	friend class const_btree_iterator<T>;
	typedef btree_iterator<T> iterator;
	typedef const_btree_iterator<T> const_iterator;
	typedef std::reverse_iterator<iterator> reverse_iterator;
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
 
  /**
   * btree constructor: @param maxNodeElems the maximum number of elements
   *        that can be stored in each B-Tree node
   */
  btree(size_t maxNodeElems = 40);
  
  /** 
   * Copy constructor
   * Creates a new B-Tree as a copy of original.
   *
   * @param original a const lvalue reference to a B-Tree object
   */
  btree(const btree<T>& original);

  /** 
   * Move constructor
   * Creates a new B-Tree by "stealing" from original.
   * @param original an rvalue reference to a B-Tree object
   */
  btree(btree<T>&& original);
    
  /** 
   * Copy assignment
   * Replaces the contents of this object with a copy of rhs.
   * @param rhs a const lvalue reference to a B-Tree object
   */
  btree<T>& operator=(const btree<T>& rhs);

  /** 
   * Move assignment
   * Replaces the contents of this object with the "stolen"
   * contents of original.
   *
   * @param rhs a const reference to a B-Tree object
   */
  btree<T>& operator=(btree<T>&& rhs);

  /**
   * Puts a breadth-first traversal of the B-Tree onto the output
   * stream os. Elements supports the output operator.
   * Elements are separated by space.
   * @param os a reference to a C++ output stream
   * @param tree a const reference to a B-Tree object
   * @return a reference to os
   */
  friend std::ostream& operator<< <T> (std::ostream& os, const btree<T>& tree);

  /**
   * The following are iterator functions
   * -- begin() 
   * -- end() 
   * -- rbegin() 
   * -- rend() 
   * -- cbegin() 
   * -- cend() 
   * -- crbegin() 
   * -- crend() 
   */ 
	iterator begin() const;
	iterator end() 	const;
	const_iterator cbegin() const;
	const_iterator cend() const;
	reverse_iterator rbegin() const;
	reverse_iterator rend() const;
	const_reverse_iterator crbegin() const;
	const_reverse_iterator crend() const;
  /**
    * Returns an iterator to the matching element, or whatever 
    * the non-const end() returns if the element could 
    * not be found.  
    */
  iterator find(const T& elem);
    
  /**
    * @param elem the client element we are trying to match.
    * @return an iterator to the matching element, or whatever the
    *         const end() returns if no such match was ever found.
    */
  const_iterator find(const T& elem) const;
      
  /**
    * @param elem the element to be inserted.
    * @return a pair whose first field is an iterator positioned at
    *         the matching element in the btree, and whose second field 
    *         stores true if and only if the element needed to be added 
    *         because no matching element was there prior to the insert call.
    */
  std::pair<iterator, bool> insert(const T& elem);

  /**
    * Destructor implentation to check that implementation does not leak memory!
    */
  ~btree();
  
public:
  // The Node class, it's constructors.
  class Node{

	  public:
	  	  std::vector<T> *vNodeElement;
	  	  Node *pNode_n;
	  	  std::vector<Node*> *children;
	  	  size_t maxNElems_b;
	  	  size_t num_element =0;
	  	  size_t childno;

	  	  Node(const T& e_Value, size_t maxNElems_b_ =40, Node *pNode_ = nullptr){

	  		vNodeElement = new std::vector<T>;
	  		vNodeElement->push_back(e_Value);
	  		++num_element;
	  		childno =0;
	  		pNode_n = pNode_;
	  		children = new std::vector<Node*>;
	  		maxNElems_b = maxNElems_b_;
	  		std::sort(vNodeElement->begin(), vNodeElement->end());
	  	  }

	  	  Node( size_t maxNElems_b_ =40, Node *pNode_ = nullptr){

	  		vNodeElement = new std::vector<T>;
	  		num_element =0;
	  		childno =0;
	  		pNode_n = pNode_;
	  		children = new std::vector<Node*>;
	  		maxNElems_b = maxNElems_b_;
	  	  }

	  	  ~Node(){

	  		  delete vNodeElement;
	  		  delete children;
	  	  }

	  	  // = operator overloading for Node.
	  	  Node& operator=(const Node &r_value_){

	  		if (this != & r_value_){
	  		vNodeElement = r_value_.vNodeElement;
	  		num_element = r_value_.num_element;
	  		pNode_n = r_value_.pNode_n;
	  		children = r_value_.children;
	  		maxNElems_b = r_value_.maxNElems_b;}
	  		return *this;
	  	  }
  };

public:
  Node *baseNode;
  Node *firstNode;
  Node *lastNode;
  size_t maxNodeElems_t;
  size_t btree_size;
};

//btree constructor.
template<typename T>
btree<T>::btree(size_t maxNodeElems_){

	baseNode = nullptr;
	firstNode = nullptr;
	lastNode=nullptr;
	maxNodeElems_t = maxNodeElems_;
	btree_size =0;
}

//btree destructor
template<typename T>
btree<T>::~btree(){

	delete baseNode;
}

//btree iterator
template<typename T>
std::pair<typename btree<T>::iterator, bool> btree<T>::insert(const T &elem){

	if (baseNode == nullptr){
		baseNode = new Node(elem);
		firstNode = baseNode;
		lastNode = baseNode;
		baseNode->maxNElems_b = maxNodeElems_t;
		btree_size=1;
		return std::make_pair(iterator(baseNode, 0, this), true);
	}

	auto findResult = find(elem);

	if(&(*findResult.pNode)==nullptr){
		// do nothing
}
	else{
		return std::make_pair(findResult, false);
	}

	if ((baseNode->num_element) < maxNodeElems_t){

		baseNode->vNodeElement->push_back(elem);
		++baseNode->num_element;
		std::sort(baseNode->vNodeElement->begin(), baseNode->vNodeElement->end());

		return std::make_pair(iterator(baseNode, 0, this), true); //***** delete it


	}

	Node *tempNode= baseNode;
	while(true){

		if (tempNode->children->empty()){

			for (size_t i =0; i<=maxNodeElems_t; ++i){

				Node *NewNode = new Node(maxNodeElems_t,tempNode);
				NewNode->childno =i;
				lastNode = NewNode;
				tempNode->children->push_back(NewNode);

			}

			for(size_t i=0; i<=maxNodeElems_t; ++i){

				if(i == maxNodeElems_t){
					tempNode->children->at(i)->vNodeElement->push_back(elem);
					++tempNode->children->at(i)->num_element;
					std::sort(tempNode->children->at(i)->vNodeElement->begin(), tempNode->children->at(i)->vNodeElement->end());
					auto ind = std::find(tempNode->children->at(i)->vNodeElement->begin(), tempNode->children->at(i)->vNodeElement->end(), elem );
					auto pos = std::distance(tempNode->children->at(i)->vNodeElement->begin(),ind);
					return std::make_pair(iterator(tempNode->children->at(i), size_t(pos), this), true);

				}

				if(elem < tempNode->vNodeElement->at(i)){
					tempNode->children->at(i)->vNodeElement->push_back(elem);
					++tempNode->children->at(i)->num_element;
					std::sort(tempNode->children->at(i)->vNodeElement->begin(), tempNode->children->at(i)->vNodeElement->end());
					auto ind = std::find(tempNode->children->at(i)->vNodeElement->begin(), tempNode->children->at(i)->vNodeElement->end(), elem );
					auto pos = std::distance(tempNode->children->at(i)->vNodeElement->begin(),ind);
					return std::make_pair(iterator(tempNode->children->at(i), size_t(pos), this), true);//******implement find as sorted i is changed.
				}

			}

		}

		else{

			for(size_t i=0; i<=maxNodeElems_t; ++i){

				if(i == maxNodeElems_t){

					if (tempNode->children->at(i)->vNodeElement->size()<maxNodeElems_t){
						tempNode->children->at(i)->vNodeElement->push_back(elem);
						++tempNode->children->at(i)->num_element;
						std::sort(tempNode->children->at(i)->vNodeElement->begin(), tempNode->children->at(i)->vNodeElement->end());
						auto ind = std::find(tempNode->children->at(i)->vNodeElement->begin(), tempNode->children->at(i)->vNodeElement->end(), elem );
						auto pos = std::distance(tempNode->children->at(i)->vNodeElement->begin(),ind);
						return std::make_pair(iterator(tempNode->children->at(i), size_t(pos), this), true);//******implement find as sorted i is changed.
					}

					else{
						tempNode = tempNode->children->at(i);
						break;
					}
				}

				else{
					if(elem < tempNode->vNodeElement->at(i)){
						if (tempNode->children->at(i)->vNodeElement->size()<maxNodeElems_t){
							tempNode->children->at(i)->vNodeElement->push_back(elem);
							++tempNode->children->at(i)->num_element;
							std::sort(tempNode->children->at(i)->vNodeElement->begin(), tempNode->children->at(i)->vNodeElement->end());
							auto ind = std::find(tempNode->children->at(i)->vNodeElement->begin(), tempNode->children->at(i)->vNodeElement->end(), elem );
							auto pos = std::distance(tempNode->children->at(i)->vNodeElement->begin(),ind);
							return std::make_pair(iterator(tempNode->children->at(i),size_t(pos), this), true);
						}
						else{
							tempNode = tempNode->children->at(i);
							break;
						}
					}
					else{
						continue;
					}
				}
			}
		}
	}
}

// btree iterators begin
template<typename T> typename btree<T>::iterator
btree<T>::begin() const {
	return iterator(baseNode, 0, this);
}

// btree iterators end
template<typename T> typename btree<T>::iterator
btree<T>::end() const {
	return iterator(nullptr, 0, this);
}
// btree iterators cbegin
template<typename T>
typename btree<T>::const_iterator btree<T>::cbegin() const {
	return const_iterator(begin());
}

// btree iterators cend
template<typename T>
typename btree<T>::const_iterator btree<T>::cend() const {
	return const_iterator(end());
}
// btree iterators rbegin
template<typename T>
typename btree<T>::reverse_iterator btree<T>::rbegin() const {
	return reverse_iterator(end());
}
// btree iterators rend
template<typename T>
typename btree<T>::reverse_iterator btree<T>::rend() const {
	return reverse_iterator(begin());
}
// btree iterators crbegin
template<typename T>
typename btree<T>::const_reverse_iterator btree<T>::crbegin() const {
	return const_reverse_iterator(end());
}
// btree iterators crend
template<typename T>
typename btree<T>::const_reverse_iterator btree<T>::crend() const {
	return const_reverse_iterator(begin());
}

//************end other above type cend and all

//find the element in tree and return iterator
template<typename T>
typename btree<T>::iterator btree<T>::find(const T& elem){

	if (baseNode == nullptr){
		return iterator(NULL, 0, this);
	}

	Node *tempNode = baseNode;
	while(true){

		size_t nodesize = tempNode->vNodeElement->size();
		if (nodesize==0){
			return iterator(NULL, 0, this);
		}
		for(size_t i =0; i<nodesize; i++ ){
			if ( tempNode->vNodeElement->at(i) == elem){
				return iterator(tempNode, i, this);
			}

			if (i == nodesize-1 && elem > tempNode->vNodeElement->at(i)){

				if (tempNode->children->empty() || tempNode->children->at(i+1)->vNodeElement->empty()){
					return iterator(NULL, 0, this);
				}
				else{
					tempNode = tempNode->children->at(i+1);
					break;
				}
			}

			if (elem > tempNode->vNodeElement->at(i)){

				continue;
			}

			else if (elem < tempNode->vNodeElement->at(i)){

				if (tempNode->children->empty() || tempNode->children->at(i)->vNodeElement->empty()){
					return iterator(NULL, 0, this);
				}
				else{
					tempNode = tempNode->children->at(i);
					break;
				}
			}
		}

	}
}

//find the element in tree and return const iterator
template<typename T>
typename btree<T>::const_iterator btree<T>::find(const T& elem) const{

	if (baseNode == nullptr){
		return iterator(NULL, 0, this);
	}
	Node *tempNode = baseNode;
	while(true){

		size_t nodesize = tempNode->vNodeElement->size();
		if (nodesize==0){
			return iterator(NULL, 0, this);
		}
		for(size_t i =0; i<nodesize; i++ ){
			if ( tempNode->vNodeElement->at(i) == elem){
				return const_iterator(tempNode, i, this);
			}

			if (i == nodesize-1 && elem > tempNode->vNodeElement->at(i)){
				if (tempNode->children->empty() || tempNode->children->at(i+1)->vNodeElement->empty()){
					return const_iterator(NULL, 0, this);
				}
				else{
					tempNode = tempNode->children->at(i+1);
					break;
				}
			}
			if (elem > tempNode->vNodeElement->at(i)){
				continue;
			}

			else if (elem < tempNode->vNodeElement->at(i)){
				if (tempNode->children->empty() || tempNode->children->at(i)->vNodeElement->empty()){
					return const_iterator(NULL, 0, this);
				}
				else{
					tempNode = tempNode->children->at(i);
					break;
				}
			}
		}

	}
}

//<<operator overloading
template<typename T>
std::ostream& operator<<(std::ostream& output, const btree<T>& inputtree) {

	if (inputtree.baseNode == nullptr){
		output<<"";
		return output;
	}

	typename btree<T>::Node* tempNode = inputtree.baseNode;
	std::queue<typename btree<T>::Node*> nQueue;
	nQueue.push(tempNode);

	while(!nQueue.empty()){

		tempNode = nQueue.front();
		nQueue.pop();
		for(size_t i =0; i<tempNode->vNodeElement->size(); ++i){

			output << tempNode->vNodeElement->at(i) << " ";
		}

		for(size_t i =0; i<=tempNode->vNodeElement->size(); ++i){
			if (tempNode->children->empty()){
				continue;
			}
			nQueue.push(tempNode->children->at(i));
		}

	}

	return output;
}
//copy constructor
template<typename T>
btree<T>::btree(const btree<T>& inputtree) :baseNode(nullptr), firstNode(nullptr), lastNode(nullptr), maxNodeElems_t(
		inputtree.maxNodeElems_t), btree_size(0){

	typename btree<T>::Node* tempNode = inputtree.baseNode;
	std::queue<typename btree<T>::Node*> nQueue;
	nQueue.push(tempNode);

	while(!nQueue.empty()){

		tempNode = nQueue.front();
		nQueue.pop();
		for(size_t i =0; i<tempNode->vNodeElement->size(); ++i){

			insert(tempNode->vNodeElement->at(i));
		}

		for(size_t i =0; i<=tempNode->vNodeElement->size(); ++i){
			if (tempNode->children->empty()){
				continue;
			}
			nQueue.push(tempNode->children->at(i));
		}

	}
}
//move constructor
template<typename T>
btree<T>::btree(btree<T> && rhs) :baseNode(rhs.baseNode), firstNode(rhs.firstNode), lastNode(rhs.lastNode),
	maxNodeElems_t(rhs.maxNodeElems_t), btree_size(rhs.btree_size) {
}

//operator = overloading
template<typename T>
btree<T>& btree<T>::operator=(const btree<T>& inputtree) {
	if(this != &inputtree ){
		baseNode = nullptr;
		firstNode = nullptr;
		lastNode=nullptr;
		maxNodeElems_t = inputtree.maxNodeElems_t;
		btree_size =0;
		typename btree<T>::Node* tempNode = inputtree.baseNode;
		std::queue<typename btree<T>::Node*> nQueue;
		nQueue.push(tempNode);

		while(!nQueue.empty()){
			tempNode = nQueue.front();
			nQueue.pop();
			for(size_t i =0; i<tempNode->vNodeElement->size(); ++i){
				insert(tempNode->vNodeElement->at(i));
			}
			for(size_t i =0; i<=tempNode->vNodeElement->size(); ++i){
				if (tempNode->children->empty()){
					continue;
				}
				nQueue.push(tempNode->children->at(i));
			}
		}
	}

	return *this;
}
//move operator = overloading
template<typename T> btree<T>&
btree<T>::operator=(btree<T> && original) {
	if (this != &original) {
		delete baseNode;

		baseNode = nullptr;
		firstNode = nullptr;
		lastNode = nullptr;
		baseNode = original.baseNode;
		firstNode = original.firstNode;
		lastNode = original.lastNode;
		maxNodeElems_t = original.maxNodeElems_t;
		btree_size = original.btree_size;


		original.baseNode = new Node(maxNodeElems_t);
		original.firstNode = baseNode;
		original.lastNode = baseNode;
	}
	return *this;
}
#endif
//**********************************
