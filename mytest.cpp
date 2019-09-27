#include<iostream>

#include "btree.h"

int main(){

	std::cout<<"Hello World"<<std::endl;

	btree<int> A(3);
	auto itr1 = A.insert(50);
	auto itr2 = A.insert(40);
	auto itr3 = A.insert(30);
	auto itr4 = A.insert(20);
	auto itr5 = A.insert(10);

	A.insert(9);
	A.insert(19);
	A.insert(29);
	A.insert(39);
	A.insert(49);
	A.insert(59);

	A.insert(2);
	A.insert(12);
	A.insert(22);
	A.insert(32);
	A.insert(42);
	A.insert(52);

	A.insert(6);
	A.insert(16);
	A.insert(26);
	A.insert(36);
	A.insert(46);
	A.insert(56);
	std::cout<<"output of iterator " <<itr1.second<<std::endl;
	std::cout<<"output of iterator " <<itr2.second<<std::endl;
	std::cout<<"output of iterator " <<itr3.second<<std::endl;
	std::cout<<"output of iterator " <<itr4.second<<std::endl;
	std::cout<<"output of iterator " <<itr5.second<<std::endl;
	

	auto mnjm= A.find(59);
	auto nbm = A.find(999);

	auto  bbbb = A.find(59);

	if(mnjm ==nbm){
		std::cout<<"yes iterators are same"<<std::endl;
	}

	if(mnjm ==bbbb){
		std::cout<<"yes iterators are same"<<std::endl;
	}

	std::cout<<*mnjm<<std::endl;
	std::cout<<(mnjm!=nbm)<<std::endl;
	std::cout<<(mnjm!=bbbb)<<std::endl;

	++mnjm;
	++bbbb;
	//++mnjm;
    //++nbm;
	std::cout<<"null comp"<<(mnjm==bbbb)<<std::endl;
	//std::cout<<*mnjm<<std::endl;
	//std::cout<<*nbm<<std::endl;

	auto Rfindtest = A.insert(39);
	std::cout<<(Rfindtest.second)<<std::endl;

	auto cop= A.find(52);
	++cop;
	std::cout<<"checking ++ operator"<<*cop<<std::endl;

	std::cout<<"checking ++ operator"<<*(cop++)<<std::endl;
	std::cout<<"checking ++ operator"<<*cop<<std::endl;
	++cop;

	//std::cout<<"checking ++ operator"<<*cop<<std::endl;

	auto ccop= A.find(2536);

	if(cop==ccop){
		std::cout<<"checking ++ operator for empty"<<std::endl;
	}

	std::cout<<A<<std::endl;

	const btree<int> B = A;
	std::cout<<B<<std::endl;

	btree<int> C = A;

	C=B;

	std::cout<<C<<std::endl;

	auto mmn2 =C.end();
	--mmn2;
	std::cout<<*mmn2<<std::endl;

	auto bbcc = A.find(56);

	--bbcc;
	std::cout<<*bbcc<<std::endl;
	--bbcc;
	std::cout<<*bbcc<<std::endl;
	--bbcc;
	std::cout<<*bbcc<<std::endl;
	--bbcc;
	std::cout<<*bbcc<<std::endl;

	auto bbcc1 = A.find(50);

	//--bbcc1;
	std::cout<<*(bbcc1--)<<std::endl;
	//--bbcc1;
	std::cout<<*(bbcc1--)<<std::endl;


	btree<char> Bchar(3);
	Bchar.insert('M');
	Bchar.insert('N');
	Bchar.insert('B');
	Bchar.insert('K');
	Bchar.insert('V');
	Bchar.insert('O');

	std::cout<<Bchar<<std::endl;
}
