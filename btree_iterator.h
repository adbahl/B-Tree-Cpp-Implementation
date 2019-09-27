/**
 * Implementation of the btree iterators as (an) external class(es) in this file.
 * Created By Arvind Bahl
 **/


#ifndef BTREE_ITERATOR_H
#define BTREE_ITERATOR_H

#include <iterator>
#include <stddef.h>
#include<iostream>
#include<stack>
using namespace std;


// iterator related interface stuff here; would be nice if you called your
// iterator class btree_iterator (and possibly const_btree_iterator)

template<typename T> class btree;
template<typename T> class btree_iterator;
template<typename T> class const_btree_iterator;

template<typename T> class btree_iterator{

	friend class const_btree_iterator<T>;
public:
	typename btree<T>::Node *pNode;
	size_t pindex;
	const btree<T> *pbtree;

public:
	//constructor
	btree_iterator(typename btree<T>::Node *pNode_ = nullptr, size_t pindex_=0, const btree<T> *pbtree_ = nullptr):
		pNode(pNode_),pindex( pindex_), pbtree(pbtree_){}

	// typedefs
	typedef ptrdiff_t difference_type;
	typedef bidirectional_iterator_tag	iterator_category;
	typedef T value_type;
	typedef T& reference;
	typedef T* pointer;

	//operator overloading
	btree_iterator& operator=(const btree_iterator&);
	bool operator==(const btree_iterator&) const;
	bool operator==(const const_btree_iterator<T>& rhs) const;
	bool operator!=(const btree_iterator&) const;
	bool operator!=(const const_btree_iterator<T>& rhs) const;
	reference operator*()const;
	pointer operator->()const;
	btree_iterator& operator++();
	btree_iterator operator ++(int);
	btree_iterator& operator--();
	btree_iterator operator --(int);

};

/// const iterator class
template<typename T> class const_btree_iterator{

public:
	typename btree<T>::Node *pNode;
	size_t pindex;
	const btree<T> *pbtree;

public:
	//constructor
	const_btree_iterator(typename btree<T>::Node *pNode_ = nullptr, size_t pindex_=0, const btree<T> *pbtree_ = nullptr):
		pNode(pNode_),pindex( pindex_), pbtree(pbtree_){}

	friend class btree_iterator<T>;

	const_btree_iterator(const btree_iterator<T>& rhs):
		pNode(rhs.pNode),pindex( rhs.pindex), pbtree(rhs.pbtree){}

	//typedefs
	typedef ptrdiff_t difference_type;
	typedef bidirectional_iterator_tag	iterator_category;
	typedef T value_type;
	typedef T& reference;
	typedef T* pointer;

	//operator overloading
	const_btree_iterator& operator=(const const_btree_iterator&);
	bool operator==(const const_btree_iterator& rhs) const;
	bool operator==(const btree_iterator<T>& rhs) const;
	bool operator!=(const const_btree_iterator&) const;
	bool operator!=(const btree_iterator<T>& other) const;
	reference operator*()const;
	pointer operator->()const;
	const_btree_iterator& operator++();
	const_btree_iterator operator ++(int);
	const_btree_iterator& operator--();
	const_btree_iterator operator--(int);

};

// non constant members

// = operator overloading.
template<typename T>
btree_iterator<T>& btree_iterator<T>::operator =(const btree_iterator &rhs){

	if (this==&rhs){
		return *this;
	}
	else{

		this->pNode=rhs.pNode;
		this->pbtree= rhs.pbtree;
		this->pindex= rhs.pindex;
	}

	return *this;
}
//== operator overloading
template<typename T>
bool btree_iterator<T>::operator ==(const btree_iterator &rhs) const{

	if(pNode==rhs.pNode && pindex == rhs.pindex && pbtree == rhs.pbtree){
		return true;
	}
	return false;
}
//== operator overloading
template<typename T>
bool btree_iterator<T>::operator ==(const const_btree_iterator<T>& rhs) const {
	if(pNode==rhs.pNode && pindex == rhs.pindex && pbtree == rhs.pbtree){
		return true;
	}
	return false;
}
//* operator overloading
template<typename T>
typename btree_iterator<T>::reference btree_iterator<T>::operator*()const {
	return pNode->vNodeElement->at(pindex);
}

//-> operator overloading
template<typename T>
typename btree_iterator<T>::pointer btree_iterator<T>::operator->()const {

	return &(pNode->vNodeElement->at(pindex));
}

//!= operator overloading
template<typename T>
bool btree_iterator<T>::operator !=(const btree_iterator &rhs) const{
	return !operator==(rhs);
}

//!= operator overloading
template<typename T>
bool btree_iterator<T>::operator !=(const const_btree_iterator<T> &rhs) const{
	return !operator==(rhs);
}

//++ operator overloading
template<typename T>
btree_iterator<T>& btree_iterator<T>::operator++(){

	if(pNode == nullptr){
		this->pNode= nullptr;
		this->pbtree= pbtree;
		this->pindex= 0;
		return *this;
	}

	if(pNode->vNodeElement->size()==0){
		this->pNode= pbtree->baseNode;
		this->pbtree= pbtree;
		this->pindex= 0;
		return *this;
	}
	if(pNode->vNodeElement->size()-pindex >=2){
		this->pNode=pNode;
		this->pbtree= pbtree;
		this->pindex= pindex+1;
		return *this;
	}

	if(pNode->children->empty()){
		this->pNode=nullptr;
		this->pbtree= pbtree;
		this->pindex= 0;
		return *this;
	}

	if(pNode==pbtree->baseNode){
		for (size_t i =0; i<=pNode->vNodeElement->size(); ++i){

			if(pNode->children->at(i)->vNodeElement->size() ==0){
				if(i == pNode->vNodeElement->size()){
					this->pNode=nullptr;
					this->pbtree= pbtree;
					this->pindex= 0;
					return *this;
				}
				continue;
			}
			this->pNode=pNode->children->at(i);
			this->pbtree= pbtree;
			this->pindex= 0;
			return *this;
		}
	}

	typename btree<T>::Node * tempNode = pNode;
	typename btree<T>::Node * tParNode = tempNode->pNode_n;
	size_t pSize = tParNode->vNodeElement->size();
	size_t childNIndex = tempNode->childno;

	for(size_t i = childNIndex+1; i<= pSize; ++i ){

		if (tParNode->children->at(i)->vNodeElement->empty()){
			continue;
		}
		else{
			this->pNode=tParNode->children->at(i);
			this->pbtree= pbtree;
			this->pindex= 0;
			return *this;
		}
	}

	for(size_t i = 0; i<= pSize; ++i ){

		if (tParNode->children->at(i)->children->empty()){
			continue;
		}
		typename btree<T>::Node * chNode = tParNode->children->at(i);
			for(size_t j = 0; j<=chNode->vNodeElement->size(); ++j){
				if (chNode->children->at(j)->vNodeElement->empty()){
					continue;
				}
				else{
					this->pNode=chNode->children->at(i);
					this->pbtree= pbtree;
					this->pindex= 0;
					return *this;
				}
			}

			this->pNode=nullptr;
			this->pbtree= pbtree;
			this->pindex= 0;
			return *this;

	}
	this->pNode=nullptr;
	this->pbtree= pbtree;
	this->pindex= 0;
	return *this;
}

//++ operator overloading
template<typename T>
btree_iterator<T> btree_iterator<T>::operator ++(int){
	btree_iterator temp_return = *this;
	operator ++();
	return temp_return;
}

//-- operator overloading
template<typename T>
btree_iterator<T>& btree_iterator<T>::operator --(){


	if(pNode==nullptr){
		pNode= pbtree->lastNode;
		pindex= pbtree->lastNode->vNodeElement->size()-1;
		return *this;
	}

	if(pNode==pbtree->baseNode && pindex ==0){
		pNode= nullptr;
		pindex= 0;
		return *this;
	}

	if(pNode==pbtree->baseNode && pindex !=0){
		pindex= pindex-1;
		return *this;
	}

	typename btree<T>::Node* tempNode = pbtree->baseNode;
	std::stack<typename btree<T>::Node*> nstack;
	nstack.push(tempNode);

	while(true){

		tempNode = nstack.top();
		nstack.pop();

		if (tempNode == pNode && pindex!=0){

			pindex= pindex-1;
			return *this;
		}

		if (tempNode == pNode && pindex==0){

			pNode= nstack.top();
			pindex = pNode->vNodeElement->size()-1;
			nstack.pop();
			return *this;
		}

		for(size_t i =0; i<=tempNode->vNodeElement->size(); ++i){
			if (tempNode->children->empty()){
				continue;
			}
			nstack.push(tempNode->children->at(i));
		}
	}

	return *this;
}
//-- operator overloading
template<typename T>
btree_iterator<T> btree_iterator<T>::operator --(int){

	btree_iterator e_iter = *this;
		operator --();
		return e_iter;
}


//---constant member functions

//== operator overloading (const)
template<typename T>
const_btree_iterator<T>& const_btree_iterator<T>::operator =(const const_btree_iterator &rhs){

	if (this==&rhs){
		return *this;
	}
	else{

		this->pNode=rhs.pNode;
		this->pbtree= rhs.pbtree;
		this->pindex= rhs.pindex;
	}

	return *this;
}
//== operator overloading (const)
template<typename T>
bool const_btree_iterator<T>::operator ==(const const_btree_iterator &rhs) const{

	if(pNode==rhs.pNode && pindex == rhs.pindex && pbtree == rhs.pbtree){
		return true;
	}
	return false;
}

//== operator overloading (const)
template<typename T>
bool const_btree_iterator<T>::operator ==(const btree_iterator<T>& rhs) const {
	if(pNode==rhs.pNode && pindex == rhs.pindex && pbtree == rhs.pbtree){
		return true;
	}
	return false;
}

//== operator overloading (const)
template<typename T>
typename const_btree_iterator<T>::reference const_btree_iterator<T>::operator*()const {
	return pNode->vNodeElement->at(pindex);
}

//-> operator overloading (const)
template<typename T>
typename const_btree_iterator<T>::pointer const_btree_iterator<T>::operator->()const {

	return &(pNode->vNodeElement->at(pindex));
}

//!= operator overloading (const)
template<typename T>
bool const_btree_iterator<T>::operator !=(const const_btree_iterator &rhs) const{
	return !operator==(rhs);
}

//!= operator overloading (const)
template<typename T>
bool const_btree_iterator<T>::operator !=(const btree_iterator<T> &rhs) const{
	return !operator==(rhs);
}

//++ operator overloading (const)
template<typename T>
const_btree_iterator<T>& const_btree_iterator<T>::operator++(){

	if(pNode == nullptr){
		this->pNode= nullptr;
		this->pbtree= pbtree;
		this->pindex= 0;
		return *this;
	}

	if(pNode->vNodeElement->size()==0){
		this->pNode= pbtree->baseNode;
		this->pbtree= pbtree;
		this->pindex= 0;
		return *this;
	}
	if(pNode->vNodeElement->size()-pindex >=2){
		this->pNode=pNode;
		this->pbtree= pbtree;
		this->pindex= pindex+1;
		return *this;
	}

	if(pNode->children->empty()){
		this->pNode=nullptr;
		this->pbtree= pbtree;
		this->pindex= 0;
		return *this;
	}

	if(pNode==pbtree->baseNode){
		for (size_t i =0; i<=pNode->vNodeElement->size(); ++i){

			if(pNode->children->at(i)->vNodeElement->size() ==0){
				if(i == pNode->vNodeElement->size()){
					this->pNode=nullptr;
					this->pbtree= pbtree;
					this->pindex= 0;
					return *this;
				}
				continue;
			}
			this->pNode=pNode->children->at(i);
			this->pbtree= pbtree;
			this->pindex= 0;
			return *this;
		}
	}

	typename btree<T>::Node * tempNode = pNode;
	typename btree<T>::Node * tParNode = tempNode->pNode_n;
	size_t pSize = tParNode->vNodeElement->size();
	size_t childNIndex = tempNode->childno;

	for(size_t i = childNIndex+1; i<= pSize; ++i ){

		if (tParNode->children->at(i)->vNodeElement->empty()){
			continue;
		}
		else{
			this->pNode=tParNode->children->at(i);
			this->pbtree= pbtree;
			this->pindex= 0;
			return *this;
		}
	}

	for(size_t i = 0; i<= pSize; ++i ){

		if (tParNode->children->at(i)->children->empty()){
			continue;
		}
		typename btree<T>::Node * chNode = tParNode->children->at(i);

			for(size_t j = 0; j<=chNode->vNodeElement->size(); ++j){

				if (chNode->children->at(j)->vNodeElement->empty()){
					continue;
				}
				else{
					this->pNode=chNode->children->at(i);
					this->pbtree= pbtree;
					this->pindex= 0;
					return *this;

				}

			}

			this->pNode=nullptr;
			this->pbtree= pbtree;
			this->pindex= 0;
			return *this;

	}
	this->pNode=nullptr;
	this->pbtree= pbtree;
	this->pindex= 0;
	return *this;
}

//++ operator overloading (const)
template<typename T>
const_btree_iterator<T> const_btree_iterator<T>::operator ++(int){
	const_btree_iterator temp_return = *this;
	operator ++();
	return temp_return;
}

//-- operator overloading (const)
template<typename T>
const_btree_iterator<T>& const_btree_iterator<T>::operator --(){


	if(pNode==nullptr){
		pNode= pbtree->lastNode;
		pindex= pbtree->lastNode->vNodeElement->size()-1;
		return *this;
	}

	if(pNode==pbtree->baseNode && pindex ==0){
		pNode= nullptr;
		pindex= 0;
		return *this;
	}

	if(pNode==pbtree->baseNode && pindex !=0){
		pindex= pindex-1;
		return *this;
	}

	typename btree<T>::Node* tempNode = pbtree->baseNode;
	std::stack<typename btree<T>::Node*> nstack;
	nstack.push(tempNode);

	while(true){

		tempNode = nstack.top();
		nstack.pop();

		if (tempNode == pNode && pindex!=0){

			pindex= pindex-1;
			return *this;
		}

		if (tempNode == pNode && pindex==0){

			pNode= nstack.top();
			pindex = pNode->vNodeElement->size()-1;
			nstack.pop();
			return *this;
		}

		for(size_t i =0; i<=tempNode->vNodeElement->size(); ++i){
			if (tempNode->children->empty()){
				continue;
			}
			nstack.push(tempNode->children->at(i));
		}
	}

	return *this;
}

//-- operator overloading (const)
template<typename T>
const_btree_iterator<T> const_btree_iterator<T>::operator --(int){

	const_btree_iterator e_iter = *this;
		operator --();
		return e_iter;
}
#endif
//**********************************
