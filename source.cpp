#include <iostream>
#include <cstdlib>
using namespace std;
int main()
{
	float *ptr, *new_ptr;
	ptr = (float*) malloc(5*sizeof(float));
	if(ptr==NULL)
	{
		cout << "Memory Allocation Failed";
		exit(1);
	}

	/* Initializing memory block */
	for (int i=0; i<5; i++)
	{
		ptr[i] = i*1.5;
	}

	/* reallocating memory */
    cout<<ptr;
	new_ptr = (float*) realloc(ptr, 10*sizeof(float));
	cout<<new_ptr;
}