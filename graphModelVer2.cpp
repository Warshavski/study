#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cctype>

using namespace std;

struct node
{
	int id;
	int contactNum;
	node *next;
	
	node(int i, int cn, node *n)
	{
		id = i;
		contactNum = cn;
		next = n;
	}
	
};
typedef node *nodeLink;

struct chain
{
	int id;
	int contactCount;
	
	node *elementList;
	chain *next;
		
	chain (int i, int cc, node *el, chain *n)
	{
		id = i;
		contactCount = cc;
		elementList = el;
		next = n;
	}
};
typedef chain *chainLink;

/*
link *initializeList(const int listSize)
{
	link *list = new link[listSize];
	
	for (int i = 0; i < listSize; ++i)
		list[i] = new node(0, 0, NULL);
	return list;
}
*/
/*
*/



/* TODO rebuild for list of chains
void printList(link *list, const int listSize, char path[])
{
	const int size = 4;

	const int inRow = 20;
	ofstream outFile;
	outFile.open(path);
	
	outFile << size << '\n';
	for (int i = 0; i < listSize; ++i)
	{
		outFile << i + 1 << ";";
		
		link tempHead = list[i];
		bool isFirstElement = true;
		while(tempHead != NULL)
		{
			if (!isFirstElement)
				outFile << "|";
			outFile << tempHead->id << "," << tempHead->contact;
			tempHead = tempHead->next;
			isFirstElement = false;
		}
		outFile << "\n";
	}
	outFile.close();
}
*/


chainLink createChainList(int listSize)
{
	chainLink listHead = NULL;
	chainLink tempHead = NULL;
	
	for (int i = 0; i < listSize; ++i)
	{
		if (listHead == NULL)
		{
			listHead = new chain(i + 1, 0, NULL, NULL);
			tempHead = listHead;
		}
		else
		{
			tempHead = (tempHead->next = new chain(i + 1, 0, NULL, NULL));
		}
		
		nodeLink tempNode = NULL;
		int contactCount = 0;
		
		while(true)
		{
			int elementID = 0;
			cout << "input element : ";
			if (!(cin >> elementID))
			{
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				
				break;
			}
			else
			{
				int contactID = 0;
				cout << "  input contact id :";
				cin >> contactID;
				
				
				if (tempNode == NULL)
				{
					tempHead->elementList = new node (elementID, contactID, NULL);
					tempNode = tempHead->elementList;
				}
				else
					tempNode = (tempNode->next = new node(elementID, contactID, NULL));
				++contactCount;
				cout << endl;
			}
		}
		tempHead->contactCount = contactCount;
	}
	return listHead;
}

void printChainList(chainLink listHead)
{
	cout << "Chains list :\n" << endl;
	
	chainLink tempHead = listHead;
	while (tempHead != NULL)
	{
		cout << "chain #" << tempHead->id << " contacts : " << tempHead->contactCount << endl;
		nodeLink  tempNode = tempHead->elementList;
		while (tempNode != NULL)
		{
			cout << tempNode->id << " -> ";
			tempNode = tempNode->next;
		}
		cout << "end\n" << endl;
		tempHead = tempHead->next;
	}
}

int main()
{
	const int LIST_SIZE = 3;
	chainLink chainsList = createChainList(LIST_SIZE);
	printChainList(chainsList);
	cout << endl;
	
	
	
	
	
	
	//int listSize = 0;
	
	//cout << "input list size : ";
	//cin >> listSize;
	
	
	
	/*
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
	*/
	/*
	const char *FName="test.txt"; //Путь к файлу. Вписывайте свой.
    mystruct X;
    //memset(&ms,0,sizeof ms); //можно проинициализировать элементы структуры нулями так
    X.i = 10; //записали значение в поле i
    X.d = 2;  //записали значение в поле d
    strcpy(X.buf,"ci-plus-plus-snachala.ru"); //Записали значение в поле buf
  
   // открываем файл для записи
    ofstream f(FName,ios::binary|ios::out);
       f.write((char*)&X,sizeof X); //Записали всю структуру целиком
    f.close();
 
       strcpy(X.buf,"\0"); //я изменяю поле buf струтктуры X
       X.i=0; //я изменяю поле i структуры X
 
    //открываем файл для чтения
    ifstream in(FName,ios::binary|ios::in);
       in.read((char*)&X,sizeof X); //Читаем структуру целиком сразу
    in.close();
 
    cout<<X.buf<<"\n"; //Выводим значение из поля структуры. Оно равно ci-plus-plus-snachala.ru
    return 0;
	*/

	/*
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
	
	char path[255];
	cout << "input path : ";
	cin >> path;
	
	printList(list, listSize, path);
	*/	
}











