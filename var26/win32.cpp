#include "stdafx.h"
#include "win32.h"
#include <commdlg.h>
#include <fstream>

#define MAX_LOADSTRING 100

using namespace std;

struct SItem //��������� ������ �������� ������������� ������ �������� �����
{
 int circuit; //����� ����
 int contact; //����� ��������
 int element; //����� ��������
 SItem *next; //��������� �� ��������� ������� ������
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
TCHAR szGIFClass[]=L"��������";

int **MG=NULL; //��������� �� ������� ��������� ����������� �����
SItem *head=NULL; //��������� �� ������ ������� ������������� ������
int nelement=0; //����� ��������� � �����

//������� ����������� ������ ���� ��������� ������������� ������
void DeleteList(SItem *head)
{
 SItem *p=head; //��������� �� ������� ���� ������������� ������
 while (p!=NULL) {
  SItem *w=p; //��������� �� ��������� ����
  p=p->next; //��������� �� ��������� ����
  delete w;
 }
 head=NULL;
}

//������� ����������� ������ ������������ ������� ����������� nXn
void DeleteMatrix(int **matrix, int n)
{
 if (matrix!=NULL) { //���� ������������ ������� ���� �������� ������
  for (int i=0;i<n;i++) //���� �� ���� ������� �������
   delete[] matrix[i]; //������������ ������ ��������� ������ i �������
  delete[] matrix; //������������ ������ ���������� �� ������ �������
 }
}

//������� �������� ������������ ���������� ������� ����������� nXn, ���������� 0
int **CreateMatrix(int n)
{
 int **matrix=new int*[n]; //��������� ������ ��� ���������� �� ������ �������
 for (int i=0;i<n;i++) { //���� �� ���� ������� �������
  matrix[i]=new int[n]; //��������� ������ ��� ��������� ������ i �������
  for (int j=0;j<n;j++) matrix[i][j]=0; //���������� ������ i ������� 0
 }
 return matrix; //���������� ��������� �� �������
}

//������� �������� �� ������������� ������ � ��������� ����� �� �����
//������� ��������� ����������� ����� � �������������� �������������
//������ ���� ������ ������
void CreateMG()
{
 int **FG; //������� ������� ����� ���� �����
 SItem *p; //��������� �� ������� ������������� ������
 bool *elements; //������������ ������ ��������� ��������� ��������� � ����
 int circuit; //����� ����

 MG=CreateMatrix(nelement); //��������� ������������ ������� ��������� nelementXnelement
 FG=CreateMatrix(nelement); //��������� ������������ ������� ������� ����� ���� �����
 elements=new bool[nelement]; //��������� ������������� ������� ��������� ��������� ��������� � ����
 circuit=0; //����� �������������� ���� ����� (0-������, ��� �������������� ����)
 for (int i=0;i<nelement;i++) elements[i]=false; //����� ��������� ��������� ��������� � ����
 for (p=head;p!=NULL;p=p->next) { //����, ���� �� ����� ��������� ����� ������
  if (circuit!=p->circuit) { //���� ������� ������ ����������� ������ ���� �����
   circuit=p->circuit; //��������� ����� ��������� ���� � �������� ����� ������� ����
//������������ ������� ����� ���� �����
   for (int i=0;i<nelement;i++) //���� �� ���� ��������� �����
    if (elements[i]) //���� ������� i ������ � ����
     for (int j=0;j<nelement;j++) //���� �� ���� ��������� �����
      if ((elements[j])&&(i!=j)) //���� ������� j ������ � ���� � i �� ����� j
       FG[i][j]=FG[j][i]=1; //��������� ������� i � j � ������ ����� ����
//�������� ������ ���� ������������ ���� � ������� ��������� �����
   for (int i=0;i<nelement;i++) //���� �� ���� ������� ������� ���������
    for (int j=0;j<nelement;j++) {//���� �� ���� ��������� ������ i ������� ���������
     MG[i][j]+=FG[i][j]; //������������ �������� ij ����������� � ��������� ij ������� ������� ����� ����
     FG[i][j]=0; //��������� �������� ij ������� ������� ����� ����
     elements[i]=false; //����� �������� ��������� �������� i � ����
    }
  }
  elements[p->element-1]=true; //���������� ������� ��������� �������� � ����
 }
//������������ ������� ����� ��������� ���� �����
 for (int i=0;i<nelement;i++) //���� �� ���� ��������� �����
  if (elements[i]) //���� ������� i ������ � ����
   for (int j=0;j<nelement;j++) //���� �� ���� ��������� �����
    if ((elements[j])&&(i!=j)) //���� ������� j ������ � ���� � i �� ����� j
     FG[i][j]=FG[j][i]=1; //��������� ������� i � j � ������ ����� ����
 for (int i=0;i<nelement;i++) //���� �� ���� ������� ������� ���������
  for (int j=0;j<nelement;j++) //���� �� ���� ��������� ������ i ������� ���������
   MG[i][j]+=FG[i][j]; //������������ �������� ij ����������� � ��������� ij ������� ������� ����� ����
 DeleteMatrix(FG,nelement); //������������ ������������ ������� ������� ����� ����
 delete[] elements; //������������ ������������� ������� ��������� ��������� ��������� � ����
}

//������� ��������� ���� ������� ��� ������ ����� � ��������� �����
//� ������ ���������� ����� � ������������ ������
bool OpenFile(HWND hWnd)
{
 ifstream fi; //������� �������� �����
 char name[MAX_PATH]; //��� ����� � ��������� �����
 OPENFILENAMEA ofn; //��������� ������������ ������� �������� �����
 SItem *last=NULL; //��������� �� ��������� ������� ������������� ������
 int circuit; //����� ����
 int ncontact; //����� ��������� ����

 ZeroMemory(&ofn,sizeof(ofn)); //��������� ��������� ��� ������������ ������� �������� �����
 ofn.lStructSize=sizeof(ofn);
 ofn.hwndOwner=hWnd;
 ofn.lpstrFile=name; //���������� ��� ����� ���������� �����
 ofn.lpstrFile[0]='\0';
 ofn.nMaxFile=sizeof(name);
 ofn.lpstrFilter="Text-files\0*.TXT\0"; //���������� ������ ����� � ����������� TXT
 ofn.nFilterIndex=1;
 ofn.lpstrFileTitle=NULL;
 ofn.nMaxFileTitle=0;
 ofn.lpstrInitialDir=NULL;
 ofn.Flags=OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST; //���������� ������ ������������ �����
 if (GetOpenFileNameA(&ofn)!=TRUE) return false; //����� �� ���������, ���� ���� �� ������
 DeleteList(head); //������������ ������ ����������� ������������ ������������� ������
 DeleteMatrix(MG,nelement); //������������ ������ ������������ ������� �����������
 nelement=0;
 fi.open(name,ios_base::in); //������� �������� ����� ��� ������ ���������� �����
 while (true) { //���� ������ ����� � ��������� �����
  fi >> circuit; //������ ����� ����
  if (fi.eof()) break; //���� ��� ������ ��������� ����� �����, ������� �� �����
  fi >> ncontact; //������ ����� ��������� ����
  for (int i=0;i<ncontact;i++) {
   SItem *p=new SItem; //��������� ������ ��� ������ �������� ������
   p->next=NULL; //���������� �������� ���
   p->circuit=circuit; //���������� � ������� ������ ����� ����
   fi >> p->element; //������ ����� ��������
   fi >> p->contact; //������ ����� ��������
   if (p->element>nelement) //���� ����� �������� > �������� ����� ���������
    nelement=p->element; //���������� ����� �������� ��� ������� ����� ���������
   if (last==NULL) //���� ���������� �������� ������ ���, �� ������ ����
    head=p; //����� ������� ������ ���������� ��� ������ ���������
   else
    last->next=p; //����� ������� ������ ����������� � ������ �� ��� ��������� ���������
   last=p; //���������� ��������� ������� ������
  }
 } 
 fi.close(); //������� �������� �����
 return true;
}

//������� ������� ���������� (��������� ������������� Visual Studio)
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
//�������� ������ "�������"
 CreateWindowEx(0,L"BUTTON",L"�������",BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD,520,20,100,25,hWindow,(HMENU)IDB_OPEN,NULL,NULL); 
//�������� ������ "�����"
 CreateWindowEx(0,L"BUTTON",L"�����",BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD,520,70,100,25,hWindow,(HMENU)IDB_OUTPUT,NULL,NULL);
 ShowWindow(hWindow, nCmdShow);
 while (GetMessage(&msg, NULL, 0, 0)) {
  TranslateMessage(&msg);
  DispatchMessage(&msg);
 }
 return (int)msg.wParam;
}

//������� ����������� ������ ���������� (��������� ������������� Visual Studio)
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

//������� ������������� ���� ���������� (��������� ������������� Visual Studio)
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

//������� ��������� ���������, ����������� � ����������
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
 int wmId, wmEvent;
 PAINTSTRUCT ps;
 HDC hDC;
 char s[200]; //������� ������ ��� ������ ������ �� �����
 char val[4]; //������� ������� ��� �������������� ����� � ������
 int y; //������������ ���������� ������ ������ �� �����

 switch (message) {
case WM_COMMAND:
  wmId=LOWORD(wParam);
  wmEvent=HIWORD(wParam);
  switch (wmId)	{
 case IDB_OPEN: //������ ������ "�������"
   if (OpenFile(hWnd)) {//�������� ����� � ��������� ����� � ������ ��� � ������������ ������
    CreateMG(); //�������� ������� ��������� �����������
    MessageBox(NULL,TEXT("������� ��������� ���������"),TEXT("����������"),MB_OK);
   }
   break;
 case IDB_OUTPUT: //������ ������ "�����"
   if (MG==NULL) //���� ���� � ��������� ����� �� ��� ������, ������ ��������� �� ������
    MessageBox(NULL,TEXT("���� � ��������� ����� �� ��� ������"),TEXT("������"),MB_OK);
   else { //����� ������� ��������� �� �����
    hDC=GetDC(hWnd); //�������� Display Context ���� ����������
    y=20; //������������ ���������� ������ ������ ������ ������
    for (int i=0;i<nelement;i++) {//���� �� ������� ������� ���������
     s[0]='\0'; //����� ����������� ������� ������
     for (int j=0;j<nelement;j++) {//���� �� ��������� ������� ������� ���������
      sprintf(val,"%2i ",MG[i][j]); //����� � ������� ������� �������� �������� ij ������� ���������
      strcat(s,val); //���������� ������� ������� � ����� ������� ������
     }
     TextOutA(hDC,10,y,s,strlen(s)); //����� ������ ������� ��������� �� �����
     y+=20; //����������� ������������ ���������� ��� ������ ��������� ������ ������� ��������� �� �����
    }
    ReleaseDC(hWnd,hDC); //����������� Display Context
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
case WM_DESTROY: //������� �������� ���� ����������
  DeleteList(head); //������������ ������ ������������� ������
  DeleteMatrix(MG,nelement); //������������ ������ ������������ ������� �����������
  PostQuitMessage(0);
  break;
default:
  return DefWindowProc(hWnd, message, wParam, lParam);
 }
 return 0;
}
