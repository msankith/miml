#include<iostream>
#include <stdlib.h> 
#include<array>
using namespace std;


void testing(int *a)
{
	cout<<sizeof(a)/sizeof(int)<<endl;
	//return array<int,2>;
}

void func(int *p)
{
    std::cout << "Length of array = " << (sizeof(*p)/sizeof(int)) << std::endl;
}


int main()

{
	cout<<"--"<<endl;
	int a[7];
	//std::cout << "Length of array = " << (sizeof(a)/sizeof(*a)) << std::endl;

	//int a[]={1,2,3,4,5,6};
	testing(a);
	func(a);

	//cout<<"  size "<<b.size() <<endl;
	return 0;
}