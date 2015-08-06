#include "stdafx.h"
#include "win32.h"
#include <commdlg.h>
#include <fstream>

#define MAX_LOADSTRING 100

using namespace std;

struct SItem //структура одного элемента динамического списка описания схемы
{
 int circuit; //номер цепи
 int contact; //номер контакта
 int element; //номер элемента
 SItem *next; //указатель на следующий элемент списка
};
 
// Forward declarations of functions included in this code module:
ATOM			MyRegisterClass(HINSTANCE hInstance);
BOOL			InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

// Global Variables:
HWND hWindow;
HINSTANCE hInst;				// current instance
TCHAR szTitle[MAX_LOADSTRING];			// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];		// the main window class name
TCHAR szGIFClass[]=L"Практика";

int **MG=NULL; //указатель на матрицу связности мультиграфа схемы
SItem *head=NULL; //указатель на первый элемент динамического списка
int nelement=0; //число элементов в схеме

//функция освобождает память всех элементов динамического списка
void DeleteList(SItem *head)
{
 SItem *p=head; //указатель на текущий узел динамического списка
 while (p!=NULL) {
  SItem *w=p; //указатель на удаляемый узел
  p=p->next; //указатель на следующий узел
  delete w;
 }
 head=NULL;
}

//функция освобождает память динамической матрицы размерности nXn
void DeleteMatrix(int **matrix, int n)
{
 if (matrix!=NULL) { //если динамической матрице была выделена памяти
  for (int i=0;i<n;i++) //цикл по всем строкам матрицы
   delete[] matrix[i]; //освобождение памяти элементов строки i матрицы
  delete[] matrix; //освобождение памяти указателей на строки матрицы
 }
}

//функция создания динамической квадратной матрицы размерности nXn, заполенной 0
int **CreateMatrix(int n)
{
 int **matrix=new int*[n]; //выделение памяти для указателей на строки матрицы
 for (int i=0;i<n;i++) { //цикл по всем строкам матрицы
  matrix[i]=new int[n]; //выделение памяти для элементов строки i матрицы
  for (int j=0;j<n;j++) matrix[i][j]=0; //заполнение строки i матрицы 0
 }
 return matrix; //возвращаем указатель на матрицу
}

//функция создания по динамическому списку с описанием схемы по цепям
//матрицы связности мультиграфа схемы с использованием представления
//каждой цепи полным графом
void CreateMG()
{
 int **FG; //матрица полного графа цепи схемы
 SItem *p; //указатель на элемент динамического списка
 bool *elements; //динамический массив признаков вхождения элементов в цепь
 int circuit; //номер цепи

 MG=CreateMatrix(nelement); //выделение динамической матрицы связности nelementXnelement
 FG=CreateMatrix(nelement); //выделение динамической матрицы полного графа цепи схемы
 elements=new bool[nelement]; //выделение динамического массива признаков вхождения элементов в цепь
 circuit=0; //номер обрабатываемой цепи схемы (0-значит, нет обрабатываемой цепи)
 for (int i=0;i<nelement;i++) elements[i]=false; //сброс признаков вхождения элементов в цепь
 for (p=head;p!=NULL;p=p->next) { //цикл, пока не будет достигнут конец списка
  if (circuit!=p->circuit) { //если элемент списка принадлежит другой цепи схемы
   circuit=p->circuit; //запомнить номер следующей цепи в качестве номер текущей цепи
//формирование полного графа цепи схемы
   for (int i=0;i<nelement;i++) //цикл по всем элементам схемы
    if (elements[i]) //если элемент i входит в цепь
     for (int j=0;j<nelement;j++) //цикл по всем элементам схемы
      if ((elements[j])&&(i!=j)) //если элемент j входит в цепь и i не равно j
       FG[i][j]=FG[j][i]=1; //соединяем вершины i и j в полном графе цепи
//добавить полный граф обработанной цепи к матрице связности схемы
   for (int i=0;i<nelement;i++) //цикл по всем строкам матрицы связности
    for (int j=0;j<nelement;j++) {//цикл по всем элементам строки i матрицы связности
     MG[i][j]+=FG[i][j]; //суммирование элемента ij мультиграфа с элементом ij матрицы полного графа цепи
     FG[i][j]=0; //обнуление элемента ij матрицы полного графа цепи
     elements[i]=false; //сброс признака вхождения элемента i в цепь
    }
  }
  elements[p->element-1]=true; //установить признак вхождения элемента в цепь
 }
//формирование полного графа последней цепи схемы
 for (int i=0;i<nelement;i++) //цикл по всем элементам схемы
  if (elements[i]) //если элемент i входит в цепь
   for (int j=0;j<nelement;j++) //цикл по всем элементам схемы
    if ((elements[j])&&(i!=j)) //если элемент j входит в цепь и i не равно j
     FG[i][j]=FG[j][i]=1; //соединяем вершины i и j в полном графе цепи
 for (int i=0;i<nelement;i++) //цикл по всем строкам матрицы связности
  for (int j=0;j<nelement;j++) //цикл по всем элементам строки i матрицы связности
   MG[i][j]+=FG[i][j]; //суммирование элемента ij мультиграфа с элементом ij матрицы полного графа цепи
 DeleteMatrix(FG,nelement); //освобождение динамической матрицы полного графа цепи
 delete[] elements; //освобождение динамического массива признаков вхождения элементов в цепь
}

//функция открывает окне диалога для выбора файла с описанием схемы
//и читает содержимое файла в динамический список
bool OpenFile(HWND hWnd)
{
 ifstream fi; //входной файловый поток
 char name[MAX_PATH]; //имя файла с описанием схемы
 OPENFILENAMEA ofn; //структура стандартного диалога открытия файла
 SItem *last=NULL; //указатель на последний элемент динамического списка
 int circuit; //номер цепи
 int ncontact; //число контактов цепи

 ZeroMemory(&ofn,sizeof(ofn)); //обнуление структуры для стандартного диалога открытия файла
 ofn.lStructSize=sizeof(ofn);
 ofn.hwndOwner=hWnd;
 ofn.lpstrFile=name; //переменная для имени выбранного файла
 ofn.lpstrFile[0]='\0';
 ofn.nMaxFile=sizeof(name);
 ofn.lpstrFilter="Text-files\0*.TXT\0"; //показывать только файлы с расширением TXT
 ofn.nFilterIndex=1;
 ofn.lpstrFileTitle=NULL;
 ofn.nMaxFileTitle=0;
 ofn.lpstrInitialDir=NULL;
 ofn.Flags=OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST; //показывать только существующие файлы
 if (GetOpenFileNameA(&ofn)!=TRUE) return false; //выйти из процедуры, если файл не выбран
 DeleteList(head); //освобождение памяти предыдущего загруженного динамического списка
 DeleteMatrix(MG,nelement); //освобождение памяти динамической матрицы мультиграфа
 nelement=0;
 fi.open(name,ios_base::in); //открыть файловый поток для чтения выбранного файла
 while (true) { //цикл чтения файла с описанием схемы
  fi >> circuit; //читаем номер цепи
  if (fi.eof()) break; //если при чтении достигнут конец файла, выходим из цикла
  fi >> ncontact; //читаем число контактов цепи
  for (int i=0;i<ncontact;i++) {
   SItem *p=new SItem; //выделение памяти для нового элемента списка
   p->next=NULL; //следующего элемента нет
   p->circuit=circuit; //записываем в элемент списка номер цепи
   fi >> p->element; //читаем номер элемента
   fi >> p->contact; //читаем номер контакта
   if (p->element>nelement) //если номер элемента > текущего числа элементов
    nelement=p->element; //запоминаем номер элемента как текущее число элементов
   if (last==NULL) //если последнего элемента списка нет, то список пуст
    head=p; //новый элемент списка становится его первым элементом
   else
    last->next=p; //новый элемент списка добавляется в список за его последним элементом
   last=p; //запоминаем последний элемент списка
  }
 } 
 fi.close(); //закрыть файловый поток
 return true;
}

//главная функция приложения (создается автоматически Visual Studio)
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;

	hInst=hInstance;
	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_WIN32, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);
	if (!InitInstance (hInstance, nCmdShow)) return FALSE;
//создание кнопки "Открыть"
 CreateWindowEx(0,L"BUTTON",L"Открыть",BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD,520,20,100,25,hWindow,(HMENU)IDB_OPEN,NULL,NULL); 
//создание кнопки "Вывод"
 CreateWindowEx(0,L"BUTTON",L"Вывод",BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD,520,70,100,25,hWindow,(HMENU)IDB_OUTPUT,NULL,NULL);
 ShowWindow(hWindow, nCmdShow);
 while (GetMessage(&msg, NULL, 0, 0)) {
  TranslateMessage(&msg);
  DispatchMessage(&msg);
 }
 return (int)msg.wParam;
}

//функция регистрации класса приложения (создается автоматически Visual Studio)
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra	= 0;
	wcex.cbWndExtra	= 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_BTNFACE+1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_WIN32);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	return RegisterClassEx(&wcex);
}

//функция инициализации окна приложения (создается автоматически Visual Studio)
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable
   hWindow = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      50,100,650,450, NULL, NULL, hInstance, NULL);
   if (!hWindow) return FALSE;
   ShowWindow(hWindow, nCmdShow);
   UpdateWindow(hWindow);
   return TRUE;
}

//функция обработки сообщений, поступающих в приложение
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
 int wmId, wmEvent;
 PAINTSTRUCT ps;
 HDC hDC;
 char s[200]; //рабочая строка для вывода текста на экран
 char val[4]; //рабочая область для преобразования числа в строку
 int y; //вертикальная координата вывода текста на экран

 switch (message) {
case WM_COMMAND:
  wmId=LOWORD(wParam);
  wmEvent=HIWORD(wParam);
  switch (wmId)	{
 case IDB_OPEN: //нажата кнопка "Открыть"
   if (OpenFile(hWnd)) {//открытие файла с описанием схемы и чтение его в динамический список
    CreateMG(); //создание матрицы связности мультиграфа
    MessageBox(NULL,TEXT("Матрица связности построена"),TEXT("Информация"),MB_OK);
   }
   break;
 case IDB_OUTPUT: //нажата кнопка "Вывод"
   if (MG==NULL) //если файл с описанием схемы не был открыт, выдать сообщение об ошибке
    MessageBox(NULL,TEXT("Файл с описанием схемы не был открыт"),TEXT("Ошибка"),MB_OK);
   else { //вывод матрицы связности на экран
    hDC=GetDC(hWnd); //получить Display Context окна приложения
    y=20; //вертикальная координата вывода первой строки текста
    for (int i=0;i<nelement;i++) {//цикл по строкам матрицы связности
     s[0]='\0'; //сброс содержимого рабочей строки
     for (int j=0;j<nelement;j++) {//цикл по элементам строкам матрицы связности
      sprintf(val,"%2i ",MG[i][j]); //вывод в рабочую область значения элемента ij матрицы связности
      strcat(s,val); //добавление рабочей области в конец рабочей строки
     }
     TextOutA(hDC,10,y,s,strlen(s)); //вывод строки матрицы свяхности на экран
     y+=20; //увеличиваем вертикальную координату для вывода следующей строки матрицы связности на экран
    }
    ReleaseDC(hWnd,hDC); //освобождаем Display Context
   }
   break;
default:
   return DefWindowProc(hWnd, message, wParam, lParam);
  }
  break;
case WM_PAINT:
  hDC=BeginPaint(hWnd, &ps);
  EndPaint(hWnd, &ps);
  break;
case WM_DESTROY: //команда удаления окна приложения
  DeleteList(head); //освобождение памяти динамического списка
  DeleteMatrix(MG,nelement); //освобождение памяти динамической матрицы мультиграфа
  PostQuitMessage(0);
  break;
default:
  return DefWindowProc(hWnd, message, wParam, lParam);
 }
 return 0;
}
