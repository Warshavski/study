#include <iostream>
#include <cstdlib>
#include <fstream>

using namespace std;

struct node
{
	int id;
	int contact;
	node *next;
	
	node(int i, int c, node *n)
	{
		id = i;
		contact = c;
		next = n;
	}
	
};
typedef node *link;

link *initializeList(const int listSize)
{
	link *list = new link[listSize];
	
	for (int i = 0; i < listSize; ++i)
		list[i] = new node(0, 0, NULL);
	return list;
}

void printList(link *list, const int listSize)
{
	for (int i = 0; i < listSize; ++i)
	{
		link tempNode = list[i];
		while (tempNode != NULL)
		{
			cout << tempNode->id << " -> ";
			tempNode = tempNode->next;
		}
		cout << endl;
	}
	
}

int main()
{
	int listSize = 0;
	
	cout << "input list size : ";
	cin >> listSize;
	
	link *chain = initializeList(listSize);
	
	for (int i = 0; i < listSize; ++i)
	{
		link tempNode = chain[i];
		int contactID = 0;
		int elementID = 0;
		
		cout << "input element id : ";
		while (cin >> elementID)
		{
			cout << "input contact : ";
			cin >> contactID;
			if (tempNode->id == 0)
			{
				tempNode->id = contactID;
				tempNode->contact = contactID;
			}
			else
			{
				tempNode = (tempNode->next = new node(elementID, contactID, NULL));
			}
		}
	}
	
	printList(chain, listSize);
}