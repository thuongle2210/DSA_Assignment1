#include <iostream>
#include <cstdlib>
#include <vector>
using namespace std;
int main()
{
	vector<int> v;
	v.push_back(1);
	v.push_back(2);
	v.push_back(3);
	vector<int>::iterator it1=v.begin();
	vector<int>::iterator it2=v.begin();
	cout<<it1==it2;

}