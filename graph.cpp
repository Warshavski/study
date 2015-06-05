#include "stdafx.h"
#include <vector>
#include <bitset>
#include <cstring>
#include <cassert>
#include <list>
#include <string>

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

static struct nullptr_t 
{
	template<typename T> operator T* () const 
	{
		return 0;
	}
} nullptr;
/*
struct node 
{
	int entrySum = 0;
	node *next;
	
	node (int es, node *n)
	{
		entrySum = 0;
		next = n;
	}
};
*/

/* Структура для смежных вершин вершины графа 
	- id - номер вершины
	- *next - указатель на следующий узел в списке
*/
struct point
{
	int id;
	//int entrySum;
	point *next;
	//point *prev;
	
	point (int i, point *n ) // int es, point *p
	{
		id = i;
		//entrySum = es;
		next = n;
		//prev = p;
	}
};
typedef point *vertex;

/* Структура для вершин графа 
	- id - номер вершины
	- entrySum - степень захода вершины
	- *next - указатель на следующий узел в списке
	- *entry - указатель на первую вершину из списка смежных вершин
*/
struct node
{
	int id;
	int entrySum;
	node *next;
	point *entry;
	
	node(int i, int es, node *n, point *e)
	{
		id = i;
		entrySum = es;
		next = n;
		entry = e;
	}
};
typedef node *link;

/* Заполнение матрицы из консоли 
	- size - размер матрицы
*/
int **FillMatrix(int size)
{
	// Выделение памяти для массива указателей на каждую строку матрицы
	int **matrix = new int*[size];
	for (int i = 0; i < size; ++i)
	{
		// Выделение памяти для массива значений (т.е. для каждой строки матрицы)
		matrix[i] = new int[size];
		cout << "Line <" << i+1 << ">" << endl;
		
		// Заполнение строки матрицы
		for (int j = 0; j < size; ++j)
		{
			cout << "\tInput value <" << j + 1 << "> : ";
			cin >> matrix[i][j];
		}
	}
	
	// возвращение заполненой матрицы
	return matrix;
}

/* Заполнение матрицы из файла (перегрузка функции)
	- *path - указатель на массив символов(строку в стиле C),
		путь к файлу
	- &size - ссылка на переменную, хранящую размер массива
*/
int **FillMatrix(char *path, int &size)
{
	FILE *file = NULL;
	
	// Если файл не открыт
    if ((file = fopen(path, "r")) == NULL) 
	{
        fprintf(stderr, "can't open file\n");
        return NULL;//EXIT_FAILURE;
    }
	
	// Чтение из файла первого значения в переменную size
	// (в нашем случае размер матрицы)
	fscanf(file, "%1d", &size);
	
	// Выделение памяти под массив указателей на каждую строку матрицы
	int **matrix = new int*[size];
	for (int j = 0; j < size; ++j)
	{
		// Выделение памяти для массива значений (т.е. для каждой строки матрицы)
		matrix[j] = new int[size];
		// последовательное чтение значений из файла в матрицу
		for (int i = 0; i < size && fscanf(file, "%1d", &matrix[j][i]) == 1; ++i);
	}
	
	// закрытие файла
    fclose(file);
	
    //return EXIT_SUCCESS;
	// возвращение заполненой матрицы
	return matrix;
}

/*
	FILE *file = NULL;
    //int i, data[256] = { 0 };
 
    if ((file = fopen("matrix.txt", "r")) == NULL) {
        fprintf(stderr, "can't open file\n");
        return EXIT_FAILURE;
    }
	
	
	//++i;
	
	int size = 0;
	fscanf(file, "%1d", &size);
	
	int **matrix = new int*[size];
	for (int j = 0; j < size; ++j)
	{
		matrix[j] = new int[size];
		for (int i = 0; i < size && fscanf(file, "%1d", &matrix[j][i]) == 1; ++i);
	}
	
    
    for (int i = 0; i < size; ++i)
	{
		for (int j = 0; j < size; ++j)
		{
			cout << matrix[i][j] << " ";
		}
		cout << endl;
	}
	
    //while (i--) 
       //printf("%d, ", data[i - 1]);
 
    fclose(file);
    //return EXIT_SUCCESS;

*/

/* Вывод одномерного массива
	- *arr - указатель на одномерный массив
	- size - размер массива
*/
void PrintArray(int *arr, int size)
{
	for (int i = 0; i < size; ++i)
		cout << arr[i] << " ";
	cout << endl;
}

/* Вывод двумерного массива (перегрузка функции)
	- **matrix - указатель на матрицу
	- rows - кол-во строк матрицы
	- cols - кол-во столбцов матрицы
*/
void PrintArray(int **matrix, int rows, int cols)
{
	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j < cols; ++j)
			cout << matrix[i][j] << " ";
		cout << endl;
	}
}

/* Создание списка вершин
	- **matrix - указатель на матрицу, которой задается графа
	- size - размер матрицы (в конкретном случае матрица кваратная)
	- &min - ссылка на переменную, хранящую минимальную степень захода
	- &minCounter - ссылка на переменную, хранящую кол-во вершин с 
		минимальной степенью захода
*/
link CreateList(int **matrix, int size, int &min, int &minCounter)
{
	// указатель на начало списка
	link head = nullptr;
	// временный указатель, который будет перемещаться для заполнения списка
	link temp = nullptr;
	
	for (int i = 0; i < size; ++i)
	{
		// если элементов в списке еще нет
		if (head == NULL)
		{
			/* Создание первого узла списка с использованием конструктора структуры
				 node(int i, int es, node *n, point *e)
			*/
			head = new node(i, 0, NULL, NULL);
			
			// передвинаем временный указатель на вершину
			temp = head;
		}
		/* если вершина уже задана, 
			создается новый узел в списке и
			временный указатель передвигается на него
		*/
		else
			temp = (temp->next = new node(i, 0, NULL, NULL));
		
		// счетчик вхождений
		int counter = 0;
		// заполнение списка смежных узлов с вершиной графа
		for (int j = 0; j < size; ++j)
		{
			// если есть связь с вершиной
			if (matrix[i][j] == 1)
			{
				// создание новой вершины в списке смежных вершин
				temp->entry = new point(j, temp->entry);
				//ver[i] = new point(j, ver[i]);
				++counter;
			}
		}
		// заполнение степени вхождения
		temp->entrySum = counter;
		
		// проверка на минимальную степень захода
		if (min == -1 || min > counter)
		{
			min = counter;
			minCounter = 0;
		}
		// увеличение счетчика вершин с мин. степенью захода 
		if (counter == min)
			++minCounter;
	}
	temp = nullptrж	
	// возвращение указателя на начало списка
	return head;
}

/* Вывод списка вершин
	- head - указатель на начало списка
*/
void PrintList(link head)
{
	// временный указатель на начало списка вершин, который будем передвигать
	link tempH = head;
	// временный указатель на начало списка смежных вершин, который будем передвигать
	vertex tempE = nullptr;
	
	// перемещение указателя до того момента пока он не будет указываль на NULL
	while (tempH != NULL)
	{
		// вывод информации о узле из списка вершин графа
		cout << "Point #" << tempH->id << " | entry sum : " << tempH->entrySum << endl;
		cout << "Entries : ";
		
		// перемещение указателя на вершину из списка смежных вершин
		tempE = tempH->entry;
		// перемещение указателя до того момента пока он не будет указываль на NULL
		while (tempE != NULL)
		{
			// вывод информации о смежной вершине
			cout << tempE->id << " ";
			// перемещение указателя на следующий узел
			tempE = tempE->next;
		}
		
		cout << "\n" << endl;
		//перемежение указателя на следующий узел
		tempH = tempH->next;
	}
	
	tempE = nullptr;
	tempH = nullptr;
}

/* Бинарный поиск (рекурсивный вызов)
	- *arr - массив в котором проводится поиск
	- value - искомое значение
	- low - нижняя граница поиска
	- hight - верхняя граница поиска
*/
bool BinarySearch(int* arr, int value, int low, int hight)
{
	if (low > hight)
	{
		//cout << "Error";
		return false;
	}
	int mid = (low + hight) / 2;
	
	if (arr[mid] == value)
		return true;
	if (arr[mid] < value)
		return BinarySearch(arr, value, mid + 1, hight);
	if (arr[mid] > value)
		return BinarySearch(arr, value, low, mid - 1);
	return false;
	
	/* ?? Разница ??
	int mid = (low + hight) / 2;
	if (mid == 1)
	{
		if (arr[0] == value)
			return true;
		else 
			return false;
	}
	
	if (value < arr[mid])
		return BinarySearch(arr, value, low, mid - 1);
	else if (value > arr[mid])
		return BinarySearch(arr, value, mid + 1, hight);
	else
		return true;
	*/
}

///* Итеративная версия сортировки слиянием («Восходящая сортировка слиянием»).

//Start MergeSort
/* Проверка массива на пустоту (не верное)
	- *arr - указатель на массив
	- size - размер массива
*/
bool IsEmpty(int *arr, int size)
{
	for (int i = 0; i < size; ++i)
		if (arr[i] != 0)
			return false;
	return true;
}

/* Слияние двух массивов
	- *A - указатель на первый массив
	- alen - длина первого массива
	- *B - указатель на второй массив
	- blen - длина второго массива
*/
int *Merge (int *A, int alen, int *B, int blen)
{
	/*
	if (IsEmpty(A, alen) == true)
		return B;
	if (IsEmpty(B, blen))
		return A;
	*/
	int* result = new int[alen + blen];
	
	int index = 0;
	while (alen && blen)
	{
		if (*A < *B)
		{
			result[index] = *A;
			++A;
			--alen;
		}
		else
		{
			result[index] = *B;
			++B;
			--blen;
		}
		++index;
	}
	
	if (alen == 0)
		for (; blen != 0; --blen, ++index, ++B)
			result[index] = *B;	
	else if (blen == 0)
		for (; alen != 0; --alen, ++index, ++A)
			result[index] = *A;
	return result;
}

/* Сортировка слиянием
	- *mas - указатель на сортируемый массив
	- len - длина массива
*/
void MergeSort(int* mas, int len)
{
	int n = 1, l, ost;
	int* mas1;
	while (n < len)
	{
		l = 0;
		while (l < len)
		{
			if (l + n >= len) 
				break;
				
			ost = (l + n * 2 > len) ? (len - (l + n)) : n;
			mas1 = Merge(mas + l, n, mas + l  + n, ost);
			
			for (int i = 0; i < n + ost; i++) 
				mas[l+i] = mas1[i];
			delete [] mas1;
			l += n * 2;
		}
		n *= 2;
	}
}
// end MergeSort

/* Поиск вершин для удаления
	- head - указатель на начало списка
	- entrySum - степень захода
	- headCount(переименовать) - кол-во вершин с указанной степенью захода
*/
int *FindEntries(link head, int entrySum, int headCount)
{
	/* вычисление размера массива
		учитывается кол-во смежных вершин в каждом узле,
		где степень вхождения минимальна
		(лучший вариант, когда вершина всего одна, 
		тогда в массиве не будет лишних элементов,
		также можно сжать массив удалив из него повторяющиеся элементы
	*/
	int arrSize = headCount + headCount * entrySum;
	int *arr = new int[arrSize];
	int i = 0;
	
	link tempH = head;
	vertex tempE;
	while (tempH != NULL)
	{
		if (tempH->entrySum == entrySum)
		{
			arr[i] = tempH->id;
			++i;
			
			tempE = tempH->entry;
			while (tempE != NULL)
			{
				arr[i] = tempE->id;
				tempE = tempE->next;
				++i;
			}
			cout << endl;	
		}
		tempH = tempH->next;
	}
	tempH = nullptr;
	tempE = nullptr;
	/* сортировка массива
		т.к. придется часто обращаться к массиву и искать в нем значения,
		то лучшим вариантом будет использовать бинарный поиск(временная сложность O(log(n))), 
		он в свою очередь работает на отсортированном массиве,
		сортировка слияние используется потому, что временная сложность в худшем случае O(n log(n))
	*/
	MergeSort(arr, arrSize);
	return arr;
}

/* Удаление вершин из списка
	- *arr - массив
	- size - размер массива
	- head - указатель на начало списка
	
	Нужен рефакторинг :
  - выделить отдельную функцию под удаление узлов списка смежных вершин
  - выделить отдельную функцию под удаление узлов списка вершин
  - уменьшить кол-во указателей
  - ?? заменить односвязный список двусвязным ??
*/
void Delete(int *arr, int size, link head)
{
	link tempH = head;
	vertex tempE;
	
	link prevH = tempH;
	while (tempH != NULL)
	{
		tempE = tempH->entry;
		vertex prevE = tempE;
		
		//Place for Tag #1
		
		//tempH = tempH->next;
		//cout << endl;
		
		if (BinarySearch(arr, tempH->id, 0, size))
		{
			/* удаление смежных вершин
				[1]->[2]->[3]->NULL - исходный список
				[1]-> ->  [3]->NULL
					 [2]-> - удаление 
				[1]->[3];
			*/
			while (tempH->entry != NULL)
			{
				vertex temp = tempH->entry;
				tempH->entry = temp->next;
				delete temp;
			}
			//cout << "delete head : " << tempH->id << endl;
			link temp = tempH;
			tempH = tempH->next;
			prevH->next = tempH;
			if (temp == head)
				head = tempH;	
			delete temp;
		}
		else
		{
			//start Tag #1
			while (tempE != NULL)
			{		
				//prevE = tempE;
				//bool BinarySearch(int* arr, int value, int low, int hight)
				if (BinarySearch(arr, tempE->id, 0, size))
				{
					vertex temp = tempE;
					tempE = tempE->next;
					prevE->next = tempE;
					if (temp == tempH->entry)
						tempH->entry = tempE;	
					delete temp;
					
					--tempH->entrySum;
				}
				else
				{
					prevE = tempE;
					tempE = tempE->next;
				}
			}	
			//end Tag #1
			
			prevH = tempH;
			tempH = tempH->next;
		}
		prevE = nullptr;
	}
	
	tempH = nullptr;
	prevH = nullptr;
	tempE = nullptr;
}


// новая версия удаления узлов
void DeleteNodes(link head, int *arr, int size)
{
	//bool BinarySearch(int* arr, int value, int low, int hight)
	link tempH = head;
	link prevH = tempH;
	while (tempH != NULL)
	{
		//cout << tempH->id << endl;
		if (BinarySearch(arr, tempH->id, 0, size))
		{
			vertex prevE = tempH->entry;
			while (tempH->entry != NULL)
			{
				vertex temp = tempH->entry;
				tempH->entry = temp->next;
				delete temp;
			}
		}
		tempH = tempH->next;
	}
}

int main(int argc, char *argv[])
{
	/* Examples 
		{0, 1, 1, 0},
		{1, 0, 1, 0},
		{1, 1, 0, 1},
		{0, 0, 1, 0}	
		-------------
		{0, 1, 1, 0},
		{0, 0, 0, 0},
		{0, 1, 0, 1},
		{0, 0, 1, 0}		
	*/
	
	int size = 4;
	/* Ввод матрицы из консоли 
	cout << "Input matrix size : ";
	cin >> size;
	int **matrix = FillMatrix(size);//new int*[size];
	//PrintMatrix(matrix, size);
	*/
	
	/* Заполнение матрицы из файла 
		---------------------------
		Cтруктура файла для матрицы:
			Первая строка содержит размер матрицы,
			остальные строки содержат значения матрицы
			строки отделяются символом '\n'
			значения в строках отделяются пробелом.
		--Пример матрицы размером 4x4:
			4
			0 0 1 1
			1 0 1 0
			1 1 0 1
			0 0 1 0
		---------------------------
	*/
	char path[255] = "matrix.txt";
	int **matrix = FillMatrix(path ,size);
	
	
	int minSum = -1;  // минимальная степень захода
	int minVerCo = 0; // кол-во вершин сс минимальной степенью захода
	
	// создание списка вершин
	link head = CreateList(matrix, size, minSum, minVerCo);
	
	// печать списка вершин
	PrintList(head);
	
	// печать минимальной степени захода и кол-ва вершин с этой степенью
	cout << "Min entry sum : " << minSum << " | count : " << minVerCo << endl;

	// размер массива вершин для удаления
	int arrSize = minVerCo + minVerCo * minSum;
	
	//поиск вершин для удаления
	int *delID = FindEntries(head, minSum, minVerCo);
	
	//PrintArray(delID, arrSize);
	//DeleteNodes(head, delID, arrSize);
	// удаление вершин
	Delete(delID, arrSize, head);
	
	cout << "\nAfter delete\n" << endl;
	
	// печать списка
	PrintList(head);
	
	/* Array of list
	//vertex *ver = new vertex[size];
	//int k, f;	
	//for (int i = 0; i < size; ++i)
		//ver[i] = NULL;
	*/
	/*
	for (int i = 0; i < size; ++i)
	{
		cout << "Point #" << i << " : ";
		while (ver[i] != NULL)
		{
			cout << ver[i]->id << " ";
			ver[i] = ver[i]->next;
		}
		cout << endl;
	}
	cout << "min : " << min << endl;
	*/
	/*
	while (cin >> k >> f)
	{
		
		//ver[f] = new point(k, ver[f]);
		//ver[k] = new point(f, ver[k]);
	}
	*/
	/*
	for (int i = 0; i < size; ++i)
	{
		for (int j = 0; j < size; ++j)
		{
			if (matrix[i][j] == 1)
			{
				ver[j] = new point(i + 1, ver[j]);
				ver[i] = new point(j + 1, ver[i]); 
			}
		}
	}
	*/
		
	return 0;
}