#include <iostream>

using namespace std;

int main()
{
	int *p[10];
	int a[6][10] = {{0,1,2,3},{4,5,6,7},{8,9,10,11}};
	for(int i = 0;i < 3; i++)
	{
		p[i] = a[i];
	}
	cout << "The *p[0] is: " << *p[0] << endl;
	cout << "The *p[0][0] is: "<< *(p[0]+1) << endl;
	return 0;
}
