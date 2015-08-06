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

int **createMatrix(chainLink list, int matrixSize)
{
	int **matrix = initializeMatrix(matrixSize);
	chainLink tempHead = list;
	
	while (tempHead != NULL)
	{
		nodeLink headNode = tempHead->elementList;
		nodeLink tempNode = headNode;
		
		while (tempNode != NULL)
		{
			nodeLink searchNode = headNode;
			while (searchNode != NULL)
			{
				if (searchNode->id != tempNode->id)
				{
					matrix[tempNode->id - 1][searchNode->id - 1] += 1;
				}
				searchNode = searchNode->next;
			}
			tempNode = tempNode->next;
		}
		tempHead = tempHead->next;
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

void printChainList(chainLink listHead, char path[])
{
	ofstream outfile;
	outfile.open(path);
	
	chainLink tempHead = listHead;
	while (tempHead != NULL)
	{
		outfile << tempHead->id << "," << tempHead->contactCount << "\n";
		nodeLink  tempNode = tempHead->elementList;
		while (tempNode != NULL)
		{
			outfile << tempNode->id << "," << tempNode->contactNum << "\n";
			tempNode = tempNode->next;
		}
		outfile << "-\n";
		tempHead = tempHead->next;
	}
}

int main()
{
	char path[] = "test.txt";
	ifstream outfile;
	outfile.open(path);
	
	char buff[50];
	char end[] = "-";
	
	chainLink listHead = new chain(0, 0, NULL, NULL);
	chainLink tempHead = listHead;
	
	nodeLink listNode = NULL;
	nodeLink tempNode = NULL;
	
	bool isChain = true;
	while(!outfile.eof())
	{
		outfile >> buff;
		//cout << buff << endl;
		if (strcmp(buff, end) == 0)
		{
			tempHead->elementList = listNode;
			tempHead = (tempHead->next = new chain(0, 0, NULL, NULL));
			
			listNode = NULL;
			tempNode = NULL;
			
			isChain = true;
		}
		else
		{
			char *pch = strtok (buff, ",:"); // во втором параметре указаны разделитель (пробел, запятая, точка, тире)
			
			int count = 0;
			int valBuff[2];
			while (pch != NULL)                         // пока есть лексемы
			{
				valBuff[count++] = (int)(*pch - 48);
				pch = strtok (NULL, ",:");
			}
			
			if (isChain)
			{
				//cout << valBuff[0] << "|" << valBuff[1] << endl;
				tempHead->id = valBuff[0];
				tempHead->contactCount = valBuff[1];
				isChain = false;
			}
			else
			{
				if (listNode == NULL)
				{
					listNode = new node(valBuff[0], valBuff[1], NULL);
					tempNode = listNode;
				}
				else
				{
					tempNode = (tempNode->next = new node(valBuff[0], valBuff[1], NULL));
				}
			}
		}	
	}
	outfile.close();
	
	printChainList(listHead);
	printChainList(listHead, "ntest.txt");
	
	int **matrix = createMatrix(listHead, 4);
	
	printMatrix(matrix, 4);
	
	return 0;
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
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











