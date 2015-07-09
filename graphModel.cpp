#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cctype>

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

/*
*/
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
		cout << "end" << endl;
	}
	
}

void printList(link *list, const int listSize, char path[])
{
	/*
	char buff[1024];
	ifstream myfile(path);
	
	int i = 0;
	if (myfile.is_open())
	{
		while (!myfile.eof())
		{
			myfile.getline(buff,1024); 
			ParseString(buff);
			arr[i].no = atoi(valuesArr[0]);
			strcpy(arr[i].fio, valuesArr[1]);
			strcpy(arr[i].facult, valuesArr[2]);
			strcpy(arr[i].shifr, valuesArr[3]);
			++i;
		}
		myfile.close();
	}
	else
		cout << "Can't open file";
	*/
}

void insertElement(link ch)
{
	int elementID = 0;
	int contactID = 0;
	
	bool uspex = true;
	while(uspex)
	{
		cout << "enter id : ";
		if (!(cin >> elementID))
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			//cout << "Invalid input.  Try again: ";
			uspex = false;
		}
		else
		{
			
			cout << "\tinput contact id :";
			cin >> contactID;
			if (ch->id != 0)
			{
				ch = (ch->next = new node(elementID, contactID, NULL));
			}
			else
			{
				ch->id = elementID;
				ch->contact = contactID;
			}
			cout << endl;
		}
	}
}

int **initializeMatrix(const int matrixSize)
{
	int **matrix = new int*[matrixSize];
	for (int i = 0; i < matrixSize; ++i)
	{
		matrix[i] = new int[matrixSize];
		for (int j = 0; j < matrixSize; ++j)
			matrix[i][j] = 0;
	}
	return matrix;
}

void printMatrix(int **matrix, int matrixSize)
{
	for (int i = 0; i < matrixSize; ++i)
	{
		for (int j = 0; j < matrixSize; ++j)
			cout << matrix[i][j] << " ";
		cout << endl;
	}
}

int **createMatrix(link *list, const int listSize, int matrixSize)
{
	int **matrix = initializeMatrix(matrixSize);
	for (int i = 0; i < listSize; ++i)
	{
		link tempHead = list[i];
		
		while (tempHead->next != NULL)
		{
			link tempNode = list[i];
			while (tempNode->next != NULL)
			{
				if (tempHead->id != tempNode->id)
					++matrix[tempHead->id][tempNode->id];
				tempNode = tempNode->next;
				
			}
			tempHead = tempHead->next;
		}
	}
	return matrix;
}

int main()
{
	// кол-во элементов, пока х3 как подсчитать. Вводи вручную
	const int matrixSize = 5;
	int listSize = 0;
	
	cout << "input list size : ";
	cin >> listSize;
	
	link *chain = initializeList(listSize);
	
	for (int i = 0; i < listSize; ++i)
	{
		link tempNode = chain[i];
		insertElement(tempNode);	
	}
	
	printList(chain, listSize);
	
	int **matrix = createMatrix(chain, listSize, matrixSize);
	
	printMatrix(matrix, matrixSize);
	
}










