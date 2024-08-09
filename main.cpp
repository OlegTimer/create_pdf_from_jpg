/*
Add notes to photos in dir and convert to pdf. 
Cyrillic is partly supported (small to capital). Made in vector font
АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ0123456789.,+-:

*.txt files save as ANSI (1251) cyrillic (code is in UTF-8)
pdf is 600*800 pix resolution
TAB for data separation in string

place in working dir, better witout cyrillic and spaces, say: C:\1

built with embarcadero dev-c++ 6.3 with TDM-GCC 9.2.0 64 bit on Win10
project - options - parameters - linker add - gdiplus.lib (! gdiplus.h AFTER #include <windows.h>)
add  -lgdiplus   tools - compiler options - linker

code by OlegTimer. Public domain license - no warranty implied; use at your own risk.
GUI based on WinAPI by Forger:
https://github.com/yottaawesome/forger-win32-tutorial
get jpg resolution by Steve:
https://stackoverflow.com/users/2586929/stev
cpp create pdf lib by Andre Renaud:
https://github.com/AndreRenaud/PDFGen/tree/master
add pdfgen.h pdfgen.c (remake to cpp) to project dir
get all files in dir by frenchie4111:
https://gist.github.com/frenchie4111/36316489cd266adc645e
utf-8 to 1251 by Dmitry Sokolov:
https://ru.stackoverflow.com/questions/1558744/%D0%9A%D0%BE%D0%BD%D0%B2%D0%B5%D1%80%D1%82%D0%B0%D1%86%D0%B8%D1%8F-%D0%BC%D0%B5%D0%B6%D0%B4%D1%83-utf8-%D0%B8-cp1251-%D1%81-%D1%81%D0%BE%D1%85%D1%80%D0%B0%D0%BD%D0%B5%D0%BD%D0%B8%D0%B5%D0%BC-%D1%80%D0%B5%D0%B7%D1%83%D0%BB%D1%8C%D1%82%D0%B0%D1%82%D0%B0
*/

#include <windows.h>
#include <gdiplus.h>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector> 
#include <sys/stat.h>
#include "gdiplusheaders.h"

using namespace std;

const char g_szClassName[] = "myWindowClass";
HBITMAP g_bmp = NULL;
LPDISPATCH pImage;

string filename = "";
string imgname  = "";
string address = "";
string name = "";
string defect = "";
string prior = "";
string imgname0 = "";
string bonus = "";
string num = "";
string rname = "";
string datep = "";

	WNDCLASSEX wc;
	HWND hwnd;
	MSG Msg;

HWND imgnametext;
HWND imgnametext0;	
HWND adr_edit;
HWND name_edit;
HWND defect_edit;
HWND prior_edit;
HWND bonus_edit;
HWND num_edit;
HWND rname_edit;
HWND datep_edit;


bool FileExists(const char* filename) 
{
    struct stat fileInfo;
    return stat(filename, &fileInfo) == 0;
}
//stat returns 0 if the file exists and -1 if not.


string ConvertUTF8ToCP1251(const std::string& str)
{
    int len = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
    wchar_t* wstr = new wchar_t[len];
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wstr, len);
    len = WideCharToMultiByte(1251, 0, wstr, -1, NULL, 0, NULL, NULL);
    char* cp1251 = new char[len];
    WideCharToMultiByte(1251, 0, wstr, -1, cp1251, len, NULL, NULL);
    std::string result(cp1251);
    delete[] wstr;
    delete[] cp1251;
    return result;
}


void fileread(){
bool z = 	FileExists(filename.c_str());
if (!z){
	imgname  = filename.substr(0, filename.size()-4);
char ch = '\\';
size_t found = imgname.find_last_of(ch);
imgname = imgname.substr (found+1,imgname0.size()-1);
imgname+=".jpg";
 address = "";
 name = "";
 defect = "";
 prior = "";
 imgname0 = "";
 bonus = "";
 num = "";
 rname = "";
 datep = "";
std::ofstream fs(filename); 
    if(!fs)
    {
    }    
string original = "я\n"; //save as ANSI cp1251
string cp1251 = ConvertUTF8ToCP1251(original);
fs<<cp1251+imgname+"\n\n\n\n\n\n\n\n\n\n";
    fs.close();       
}

vector<string> arr;	
ifstream file(filename);

if (file.is_open()) {
	     					
    std::string line;
    while (std::getline(file, line)) {
        line.c_str();
  string delim("\n");
  size_t prev = 0;
  size_t next;
  size_t delta3 = delim.length();
  while( ( next = line.find( delim, prev ) ) != string::npos ){
    arr.push_back( line.substr( prev, next-prev ) );
    prev = next + delta3;
  }
arr.push_back( line.substr(prev ) );
    }
    file.close();
}
 imgname =arr[1];
SetWindowText(imgnametext, imgname.c_str());
 address = arr[2];
 SetWindowText(adr_edit, address.c_str());
 name = arr[3];
  SetWindowText(name_edit, name.c_str());
 defect = arr[4];
  SetWindowText(defect_edit, defect.c_str());
 prior = arr[5];
  SetWindowText(prior_edit, prior.c_str());
 imgname0 = arr[6];
 SetWindowText(imgnametext0, imgname0.c_str());
 bonus = arr[7];
   SetWindowText(bonus_edit, bonus.c_str());
 num = arr[8];
   SetWindowText(num_edit, num.c_str());
 rname =arr[9];
   SetWindowText(rname_edit, rname.c_str());
 datep = arr[10];
   SetWindowText(datep_edit, datep.c_str());
//MessageBox(0, datep.c_str(), "header", 0);
 
RedrawWindow(hwnd,NULL,NULL,RDW_INVALIDATE );
UpdateWindow(hwnd);
SetFocus(hwnd);	
	 }

void openf(){
void save (string filename);	save ( filename);
OPENFILENAME ofn;       
TCHAR szFile[260] = { 0 };      
ZeroMemory(&ofn, sizeof(ofn));
ofn.lStructSize = sizeof(ofn);
ofn.hwndOwner = hwnd;
ofn.lpstrFile = szFile;
ofn.nMaxFile = sizeof(szFile);
ofn.lpstrFilter = "JPG Files (*.jpg)\0*.jpg\0All Files (*.*)\0*.*\0";
ofn.nFilterIndex = 1;
ofn.lpstrFileTitle = NULL;
ofn.nMaxFileTitle = 0;
ofn.lpstrInitialDir = NULL;
ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

if (GetOpenFileName(&ofn) == TRUE)
{
filename = 	szFile;	
filename  = filename.substr(0, filename.size()-4);
filename+=".txt";
//MessageBox(0, filename.c_str(), "header", 0);
fileread();	  			  					  
}
	 }


void save (string filename){ //Enter or OK to save
if (!filename.empty()  ) {
std::ofstream fs(filename); 
    if(!fs)
    {
    }    
string original = "я\n"; //save as ANSI cp1251
string cp1251 = ConvertUTF8ToCP1251(original);
    fs<<cp1251;
    fs.close();    
   
std::ofstream outfile;
outfile.open(filename, std::ios_base::app); 
outfile << imgname+"\n";  
std::basic_string<TCHAR> text;
TCHAR buff[1024] = {0};
GetWindowText(adr_edit, buff, 1024); 
text = buff;  
address = text.c_str(); 
outfile << address+"\n"; 
std::basic_string<TCHAR> text2;
TCHAR buff2[1024] = {0};
GetWindowText(name_edit, buff2, 1024); 
text2 = buff2;  
name = text2.c_str();
outfile << name+"\n";
std::basic_string<TCHAR> text3;
TCHAR buff3[1024] = {0};
GetWindowText(defect_edit, buff3, 1024); 
text3 = buff3;  
defect = text3.c_str();
outfile << defect+"\n"; 
std::basic_string<TCHAR> text4;
TCHAR buff4[1024] = {0};
GetWindowText(prior_edit, buff4, 1024); 
text4 = buff4;  
prior = text4.c_str();
outfile << prior+"\n";
outfile << imgname0+"\n";    
std::basic_string<TCHAR> text5;
TCHAR buff5[1024] = {0};
GetWindowText(bonus_edit, buff5, 1024); 
text5 = buff5;  
bonus = text5.c_str();
outfile << bonus+"\n";
std::basic_string<TCHAR> text6;
TCHAR buff6[1024] = {0};
GetWindowText(num_edit, buff6, 1024); 
text6 = buff6;  
num = text6.c_str();
outfile << num+"\n";
std::basic_string<TCHAR> text7;
TCHAR buff7[1024] = {0};
GetWindowText(rname_edit, buff7, 1024); 
text7 = buff7;  
rname = text7.c_str();
outfile << rname+"\n";
std::basic_string<TCHAR> text8;
TCHAR buff8[1024] = {0};
GetWindowText(datep_edit, buff8, 1024); 
text8 = buff8;  
datep = text8.c_str();
outfile << datep+"\n";
outfile.close();

RedrawWindow(hwnd,NULL,NULL,RDW_INVALIDATE );
UpdateWindow(hwnd);
SetFocus(hwnd);	
}}


void open2(){//link to second photo
if (!filename.empty()  ) {
OPENFILENAME ofn;       
TCHAR szFile[260] = { 0 };      
ZeroMemory(&ofn, sizeof(ofn));
ofn.lStructSize = sizeof(ofn);
ofn.hwndOwner = hwnd;
ofn.lpstrFile = szFile;
ofn.nMaxFile = sizeof(szFile);
ofn.lpstrFilter = "JPG Files (*.jpg)\0*.jpg\0All Files (*.*)\0*.*\0";
ofn.nFilterIndex = 1;
ofn.lpstrFileTitle = NULL;
ofn.nMaxFileTitle = 0;
ofn.lpstrInitialDir = NULL;
ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

if (GetOpenFileName(&ofn) == TRUE)
{
imgname0 = 	szFile;	
char ch = '\\';
size_t found = imgname0.find_last_of(ch);
imgname0 = imgname0.substr (found+1,imgname0.size()-1);
save(filename);
SetWindowText(imgnametext0, imgname0.c_str());
RedrawWindow(hwnd,NULL,NULL,RDW_INVALIDATE );
UpdateWindow(hwnd);
SetFocus(hwnd);	  			  					  
}
}}


void dellink(){
if (!filename.empty()  ) {	
	imgname0 = "";
 SetWindowText(imgnametext0, imgname0.c_str());
 save(filename);
}}


void left(){
if (!filename.empty()  ) {	
save (filename)	;
string backward(string filename); filename =  backward( filename);
fileread();	
}}

void right(){
if (!filename.empty()  ) {	
save (filename)	;	
string 	filenameo = filename;
string forward(string filename); filename = forward( filename)	;
//MessageBox(0, filename.c_str(), filenameo.c_str(), 0);
if (filename!=filenameo){
fileread();	}
}}

void del(){
if (!filename.empty()  ) {
string 	filenameo = filename;
string imgnameo = imgname;
 
string forward(string filename); 
filename = forward( filename);

if (filename==filenameo){
string backward(string filename); filename =  backward( filename);
if (filename==filenameo){
//MessageBox(0, "All jpg files deleted", "Attention", 0);
 std::remove(filenameo.c_str());
imgnameo  = filenameo.substr(0, filenameo.size()-4);
imgnameo+=".jpg";
 std::remove(imgnameo.c_str());
//  system("pause");
  exit(0);
}
}

 std::remove(filenameo.c_str());
imgnameo  = filenameo.substr(0, filenameo.size()-4);
imgnameo+=".jpg";
 std::remove(imgnameo.c_str());
fileread();	
}}


void gen(){
if (!filename.empty()  ) {
save (filename)	;	
void creategentxt(); creategentxt();	
void pagegen(); pagegen();
}}


void info(){
MessageBox(0, "gen pdf", "Info", 0);	
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		case WM_CREATE:
		break;
		
		case WM_KEYDOWN:
	 switch(wParam)	
	 { 
case VK_RETURN: save(filename); break; 
case VK_LEFT: left();  break;
case VK_RIGHT: right();  break;
case VK_DELETE: del();  break;
case VK_ESCAPE:  save(filename);  exit(0);
					   }    
break;	   
		   
    case WM_COMMAND:
if (wParam == 101) { save(filename);	}
if (wParam == 102) { openf();	}
if (wParam == 103) { open2();	}
if (wParam == 104) { dellink();	}
if (wParam == 105) { left();	}
if (wParam == 106) { right();	}
if (wParam == 107) { del();	}
if (wParam == 108) { gen();	}
if (wParam == 109) { info();	}
        break;
		
		case WM_PAINT:
		{
int gdiRelativeScreenCoordCoeffget();
int gdiRelativeScreenCoordCoeff =  gdiRelativeScreenCoordCoeffget();
int maxsquarepixget();
int maxsquarepixint =  maxsquarepixget();		
			
PAINTSTRUCT ps;
HDC hdc = BeginPaint(hwnd, &ps);
Gdiplus::Graphics g(hdc); 
g.SetPageUnit(Gdiplus::UnitPixel);

Gdiplus::SolidBrush r((Gdiplus::Color(255, 255, 255)));                   
g.FillRectangle(&r, 0, 0,
 2*maxsquarepixint*gdiRelativeScreenCoordCoeff, maxsquarepixint*gdiRelativeScreenCoordCoeff);
DeleteObject(&r);

if (!imgname.empty()  ) {

wstring imgnamew;
for(int i = 0; i < imgname.length(); ++i)
imgnamew += wchar_t( imgname[i] );
const wchar_t* resultw = imgnamew.c_str();
Gdiplus::Image image(resultw);
int width, height;
FILE *image2;
int size, i = 0;
unsigned char *data;
image2 =fopen(imgname.c_str(),"rb");  
fseek(image2,  0,  SEEK_END);
size = ftell(image2);
fseek(image2,  0,  SEEK_SET);
data = (unsigned char *)malloc(size);
fread(data, 1, size, image2);
/* verify valid JPEG header */
if(data[i] == 0xFF && data[i + 1] == 0xD8 && data[i + 2] == 0xFF && data[i + 3] == 0xE0) {
     i += 4;
     /* Check for null terminated JFIF */
     if(data[i + 2] == 'J' && data[i + 3] == 'F' && data[i + 4] == 'I' && data[i + 5] == 'F' && data[i + 6] == 0x00) {
         while(i < size) {
             i++;
             if(data[i] == 0xFF){
                 if(data[i+1] == 0xC0) {
                     height = data[i + 5]*256 + data[i + 6];
                     width = data[i + 7]*256 + data[i + 8];
                     break;
                 }}}}} 
 fclose(image2);
double finwd = 1.0; double finhd = 1.0;
 if (height>maxsquarepixint || width >maxsquarepixint ){ 
double ww = (double) 1.0* width; double hh = (double) 1.0* height; 	
double wprop = ww/maxsquarepixint;
double hprop = hh/maxsquarepixint;
if (wprop>=hprop){finwd =  1/	wprop; finhd =  1/	wprop; }
else{finwd =  1/	hprop; finhd =  1/	hprop;	}
 } 
g.ScaleTransform(finwd, finhd);
g.DrawImage(&image, 0, 0);

wstring imgnamew0;
for(int i = 0; i < imgname0.length(); ++i)
imgnamew0 += wchar_t( imgname0[i] );
const wchar_t* resultw0 = imgnamew0.c_str();
Gdiplus::Image image0(resultw0);
int width0, height0;
FILE *image20;
int size0;
i = 0;
unsigned char *data0;
image20 =fopen(imgname0.c_str(),"rb");
fseek(image20,  0,  SEEK_END);
size = ftell(image20);
fseek(image20,  0,  SEEK_SET);
data0 = (unsigned char *)malloc(size0);

fread(data0, 1, size0, image20);
if(data0[i] == 0xFF && data0[i + 1] == 0xD8 && data0[i + 2] == 0xFF && data0[i + 3] == 0xE0) {
     i += 4;
     if(data0[i + 2] == 'J' && data0[i + 3] == 'F' && data0[i + 4] == 'I' && data0[i + 5] == 'F' && data0[i + 6] == 0x00) {
         while(i < size0) {
             i++;
             if(data0[i] == 0xFF){
                 if(data0[i+1] == 0xC0) {
                     height0 = data0[i + 5]*256 + data0[i + 6];
                     width0 = data0[i + 7]*256 + data0[i + 8];
                     break;
                 }}}}} 
 fclose(image20);
 finwd = 1.0;  finhd = 1.0;
 if (height0>maxsquarepixint || width0 >maxsquarepixint ){ 
double ww = (double) 1.0* width0; double hh = (double) 1.0* height0; 	
double wprop = ww/maxsquarepixint;
double hprop = hh/maxsquarepixint;
if (wprop>=hprop){finwd =  1/	wprop; finhd =  1/	wprop; }
else{finwd =  1/	hprop; finhd =  1/	hprop;	}
 } 
g.ScaleTransform(finwd, finhd);
g.DrawImage(&image0, maxsquarepixint*gdiRelativeScreenCoordCoeff, 0);
}
{
//MessageBox(0, "no files selected yet", "header", 0);	
}
  
EndPaint(hwnd, &ps);

		}
		break;
		case WM_CLOSE:
			DestroyWindow(hwnd);
		break;
		case WM_DESTROY:
			PostQuitMessage(0);
		break;
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
Gdiplus::GdiplusStartupInput gdiplusStartupInput;
ULONG_PTR gdiplusToken;
Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.style		 = 0;
	wc.lpfnWndProc	 = WndProc;
	wc.cbClsExtra	 = 0;
	wc.cbWndExtra	 = 0;
	wc.hInstance	 = hInstance;
	wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = g_szClassName;
	wc.hIconSm		 = LoadIcon(NULL, IDI_APPLICATION);

	if(!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Window Registration Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		g_szClassName,
		"Photo_to_pdf",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 240, 240,
		NULL, NULL, hInstance, NULL);
		
		SetWindowPos(hwnd, HWND_TOP, 0, 0, GetSystemMetrics(SM_CXSCREEN), //fullscreen w tab
GetSystemMetrics(SM_CYSCREEN), SWP_FRAMECHANGED); 

	if(hwnd == NULL)
	{
		MessageBox(NULL, "Window Creation Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

int gdiRelativeScreenCoordCoeffget();
int gdiRelativeScreenCoordCoeff =  gdiRelativeScreenCoordCoeffget();
int maxsquarepixget();
int maxsquarepixint =  maxsquarepixget();	
int x = 0;
int y = 0;



x=maxsquarepixint/4+5; y=maxsquarepixint+5;
HWND hwndButton = CreateWindow("BUTTON", "OK", 
WS_CHILD | WS_VISIBLE, x, y, maxsquarepixint/6, maxsquarepixint/10, hwnd, (HMENU)101, hInstance, 0);
x+=maxsquarepixint/6+10;
HWND hwndButtonl = CreateWindow("BUTTON", "<", 
WS_CHILD | WS_VISIBLE, x, y, maxsquarepixint/6, maxsquarepixint/10, hwnd, (HMENU)105, hInstance, 0);
x+=maxsquarepixint/6+10;
HWND hwndButtonr = CreateWindow("BUTTON", ">", 
WS_CHILD | WS_VISIBLE, x, y, maxsquarepixint/6, maxsquarepixint/10, hwnd, (HMENU)106, hInstance, 0);
x+=maxsquarepixint/6+10;
HWND hwndButtond = CreateWindow("BUTTON", "X", 
WS_CHILD | WS_VISIBLE, x, y, maxsquarepixint/6, maxsquarepixint/10, hwnd, (HMENU)107, hInstance, 0);
x+=maxsquarepixint/3+10;

HWND hwndButtong = CreateWindow("BUTTON", "Gen", 
WS_CHILD | WS_VISIBLE, x, y, maxsquarepixint/6, maxsquarepixint/10, hwnd, (HMENU)108, hInstance, 0);
x+=maxsquarepixint/6+10;

HWND hwndButtoni = CreateWindow("BUTTON", "?", 
WS_CHILD | WS_VISIBLE, x, y, maxsquarepixint/6, maxsquarepixint/10, hwnd, (HMENU)109, hInstance, 0);

//
x=5; y+=maxsquarepixint/10;
HWND hwndButtonop1 = CreateWindow("BUTTON", "open1", 
WS_CHILD | WS_VISIBLE, x, y, maxsquarepixint/5, maxsquarepixint/10, hwnd, (HMENU)102, hInstance, 0);
x+=maxsquarepixint/4;
imgnametext =
CreateWindow(TEXT("STATIC"),TEXT(imgname.c_str()),WS_VISIBLE | WS_CHILD, x,y,5*maxsquarepixint/2,maxsquarepixint/10,  hwnd,(HMENU)4,NULL,NULL);

x=5; y+=maxsquarepixint/10;
CreateWindow(TEXT("STATIC"),TEXT("address"),WS_VISIBLE | WS_CHILD, x,y,maxsquarepixint/5,maxsquarepixint/10,  hwnd,(HMENU)4,NULL,NULL);
x+=maxsquarepixint/4;
adr_edit=CreateWindow(TEXT("edit"),TEXT(address.c_str()), 
        WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
        x,y,5*maxsquarepixint/2,maxsquarepixint/10,hwnd,(HMENU)5,NULL,NULL );
x=5; y+=maxsquarepixint/10;
CreateWindow(TEXT("STATIC"),TEXT("name"),WS_VISIBLE | WS_CHILD, x,y,maxsquarepixint/5,maxsquarepixint/10,  hwnd,(HMENU)4,NULL,NULL);
x+=maxsquarepixint/4;
name_edit=CreateWindow(TEXT("edit"),TEXT(name.c_str()), 
        WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
        x,y,5*maxsquarepixint/2,maxsquarepixint/10,hwnd,(HMENU)5,NULL,NULL );
x=5; y+=maxsquarepixint/10;
CreateWindow(TEXT("STATIC"),TEXT("defect"),WS_VISIBLE | WS_CHILD, x,y,maxsquarepixint/5,maxsquarepixint/10,  hwnd,(HMENU)4,NULL,NULL);
x+=maxsquarepixint/4;
defect_edit=CreateWindow(TEXT("edit"),TEXT(defect.c_str()), 
        WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
        x,y,5*maxsquarepixint/2,maxsquarepixint/10,hwnd,(HMENU)5,NULL,NULL );
x=5; y+=maxsquarepixint/10;
CreateWindow(TEXT("STATIC"),TEXT("prior"),WS_VISIBLE | WS_CHILD, x,y,maxsquarepixint/5,maxsquarepixint/10,  hwnd,(HMENU)4,NULL,NULL);
x+=maxsquarepixint/4;
prior_edit=CreateWindow(TEXT("edit"),TEXT(prior.c_str()), 
        WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
        x,y,5*maxsquarepixint/2,maxsquarepixint/10,hwnd,(HMENU)5,NULL,NULL );
        
x= 5; y+=maxsquarepixint/10;
HWND hwndButtonop2 = CreateWindow("BUTTON", "link2", 
WS_CHILD | WS_VISIBLE, x, y, maxsquarepixint/5, maxsquarepixint/10, hwnd, (HMENU)103, hInstance, 0);
x+=maxsquarepixint/4;
HWND hwndButtondellink = CreateWindow("BUTTON", "x", 
WS_CHILD | WS_VISIBLE, x, y, maxsquarepixint/8, maxsquarepixint/10, hwnd, (HMENU)104, hInstance, 0);
x+=maxsquarepixint/8+5;
imgnametext0=
CreateWindow(TEXT("STATIC"),TEXT(imgname0.c_str()),WS_VISIBLE | WS_CHILD, x,y,7*maxsquarepixint/3,maxsquarepixint/10,  hwnd,(HMENU)4,NULL,NULL);

x=5; y+=maxsquarepixint/10;
CreateWindow(TEXT("STATIC"),TEXT("extra"),WS_VISIBLE | WS_CHILD, x,y,maxsquarepixint/5,maxsquarepixint/10,  hwnd,(HMENU)4,NULL,NULL);
x+=maxsquarepixint/4;
bonus_edit=CreateWindow(TEXT("edit"),TEXT(bonus.c_str()), 
        WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
        x,y,5*maxsquarepixint/2,maxsquarepixint/10,hwnd,(HMENU)5,NULL,NULL );
x=5; y+=maxsquarepixint/10;
CreateWindow(TEXT("STATIC"),TEXT("num"),WS_VISIBLE | WS_CHILD, x,y,maxsquarepixint/5,maxsquarepixint/10,  hwnd,(HMENU)4,NULL,NULL);
x+=maxsquarepixint/4;
num_edit=CreateWindow(TEXT("edit"),TEXT(num.c_str()), 
        WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
        x,y,5*maxsquarepixint/2,maxsquarepixint/10,hwnd,(HMENU)5,NULL,NULL );
x=5; y+=maxsquarepixint/10;
CreateWindow(TEXT("STATIC"),TEXT("road_n"),WS_VISIBLE | WS_CHILD, x,y,maxsquarepixint/5,maxsquarepixint/10,  hwnd,(HMENU)4,NULL,NULL);
x+=maxsquarepixint/4;
rname_edit=CreateWindow(TEXT("edit"),TEXT(rname.c_str()), 
        WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
        x,y,5*maxsquarepixint/2,maxsquarepixint/10,hwnd,(HMENU)5,NULL,NULL );
x=5; y+=maxsquarepixint/10;
CreateWindow(TEXT("STATIC"),TEXT("date"),WS_VISIBLE | WS_CHILD, x,y,maxsquarepixint/5,maxsquarepixint/10,  hwnd,(HMENU)4,NULL,NULL);
x+=maxsquarepixint/4;
datep_edit=CreateWindow(TEXT("edit"),TEXT(datep.c_str()), 
        WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
        x,y,5*maxsquarepixint/2,maxsquarepixint/10,hwnd,(HMENU)5,NULL,NULL );

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while(GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
	Gdiplus::GdiplusShutdown(gdiplusToken);
}