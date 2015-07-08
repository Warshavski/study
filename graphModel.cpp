#include <iostream>
#include <cstdlib>
#include <fstream>

struct node
{
	int id;
	int contact;
	node *next;
	
	public node(int i, int c, int n)
	{
		id = i;
		contact = c;
		next = n;
	}
	
}
typedef node *link 

int main()
{
	int listSize = 0;
	
	cout << "input list size : ";
	cin >> listSize;
	
	int *chains = new int[listSize];
	
	for (int i = 0; i < listSize; ++i)
	{
		int elementID = 0;
		
		link element = new node(0,0, NULL);
		
		cout << "input element id :";
		while (cin >> elementID )
		{
			int contactID = 0;
			cout << "input contact : "
			cin >> contactID;
			
		}
	}
	
	
}