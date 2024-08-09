#include "pdfgen.h"
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <winsock.h>
#include <windows.h>
#include <string>
#include <locale>
#include <streambuf>
#include <sstream>
#include <vector>
using namespace std;

int pagew=600;
int pageh=800;
int s=7;
int letdiv = s*6/5;
int fat =s/10;
int leftmargin=s;
int h=pageh/2-2*s;
int w=leftmargin;
int maxsquarepix = 250;
int gdiRelativeScreenCoordCoeff = 4; //how gdi pixels are 4-6 less, than actual pixels on monitor

int getpagew(){return pagew;}
int getpageh(){return pageh;}
int maxsquarepixget(){return maxsquarepix;}
int gdiRelativeScreenCoordCoeffget(){return gdiRelativeScreenCoordCoeff;}


std::string cuttab(std::string const& s)
{
    std::string::size_type pos = s.find('\t');
    if (pos != std::string::npos)
    {
        return s.substr(0, pos);
    }
    else
    {
        return s;
    }
}


void pagegen() {

  struct   pdf_info info = {
         ""
    };

    struct pdf_doc *pdf = pdf_create(PDF_A4_WIDTH, PDF_A4_HEIGHT, &info);
    
	
char filename[] = "__finlist.txt"; 
    string myString;
    ifstream myfile;
    myfile.open(filename);

   if(!myfile.is_open()) {
      perror("Error open");
      exit(EXIT_FAILURE);
   }
    while(getline(myfile, myString)) { //read _fin file line by line start
    
if (myString.size()<2) {continue;}   
pdf_append_page(pdf);

std::string s0 = cuttab(myString);
std::string s1 = myString.substr(s0.length()+1, myString.length());
std::string s2 = cuttab(s1);
    	 
int width, height;
FILE *image;
int size, i = 0;
unsigned char *data;
image =fopen(s0.c_str(),"rb");  
fseek(image,  0,  SEEK_END);
size = ftell(image);
fseek(image,  0,  SEEK_SET);
data = (unsigned char *)malloc(size);
fread(data, 1, size, image);
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
 fclose(image);
int finw = width; int finh= height;
 if (height>pageh/2 || width >pagew ){
double finwd = 0.0; double finhd = 0.0; 
double ww = (double) 1.0* finw; double hh = (double) 1.0* finh; 	
double wprop = ww/pagew;
double hprop = hh/(pageh/2);
if (wprop>=hprop){finwd =  ww/	wprop; finhd =  hh/	wprop; }
else{finwd =  ww/	hprop; finhd =  hh/	hprop;	}
finw = (int) finwd; finh = (int)finhd; 
 } 
pdf_add_image_file(pdf, NULL, 0, pageh/2 , finw, finh, s0.c_str());

if (!s2.empty()  ) {  
size=0; i = 0;
image =fopen(s2.c_str(),"rb");  
fseek(image,  0,  SEEK_END);
size = ftell(image);
fseek(image,  0,  SEEK_SET);
data = (unsigned char *)malloc(size);
fread(data, 1, size, image);
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
 fclose(image);
 finw = width;  finh= height;
 if (height>pageh/4 || width >pagew ){
double finwd = 0.0; double finhd = 0.0; 
double ww = (double) 1.0* finw; double hh = (double) 1.0* finh; 	
double wprop = ww/pagew;
double hprop = hh/(pageh/4);
if (wprop>=hprop){finwd =  ww/	wprop; finhd =  hh/	wprop; }
else{finwd =  ww/	hprop; finhd =  hh/	hprop;	}
finw = (int) finwd; finh = (int)finhd; 
 } 
pdf_add_image_file(pdf, NULL, 0, 0 , finw, finh, s2.c_str());
}
    
//add text to page start 
h=pageh/2-2*s;
w=leftmargin;
for (int j=s0.length()+s2.length()+1; j<myString.length(); j++){
int z = myString[j];
if (z<0){z+=256;}
if (z==184){z=168;}
if (z>=224 && z<=255){z-=32;}

switch (z){		
case 9:
w=leftmargin-letdiv;
h-=2*s;
 break;

case 192:
    pdf_add_line(pdf, NULL, w, h, w+s/2, h+s, fat, PDF_BLACK);
    pdf_add_line(pdf, NULL, w+s/2, h+s, w+s, h, fat, PDF_BLACK);
    pdf_add_line(pdf, NULL, w+s/4, h+s/2, w+3*s/4, h+s/2, fat, PDF_BLACK);
 break;

 case 193: 
    pdf_add_line(pdf, NULL, w, h, w, h+s, fat, PDF_BLACK);
    pdf_add_line(pdf, NULL, w, h+s, w+s, h+s, fat, PDF_BLACK);
    pdf_add_ellipse(pdf, NULL, w+s/2, h+s/3, s/2, s/3, 1, PDF_RGB(0, 0, 0), PDF_TRANSPARENT);
 break;

 case 194:     
    pdf_add_line(pdf, NULL, w, h, w, h+s, fat, PDF_BLACK);
	pdf_add_ellipse(pdf, NULL, w+s/2, h+s/3, s/2, s/4, 1, PDF_RGB(0, 0, 0), PDF_TRANSPARENT);
	pdf_add_ellipse(pdf, NULL, w+s/3, h+s-s/4, s/3, s/4, 1, PDF_RGB(0, 0, 0), PDF_TRANSPARENT);  
 break;

 case 195:   	
    pdf_add_line(pdf, NULL, w, h, w, h+s, fat, PDF_BLACK);
	pdf_add_line(pdf, NULL, w, h+s, w+s, h+s, fat, PDF_BLACK);
 break;

 case 196:   	
    pdf_add_line(pdf, NULL, w+s/5, h+s/5, w+s/2, h+s, fat, PDF_BLACK);
    pdf_add_line(pdf, NULL, w+s/2, h+s, w+s-s/5, h+s/5, fat, PDF_BLACK);
    pdf_add_line(pdf, NULL, w, h+s/5, w+s, h+s/5, fat, PDF_BLACK);
    pdf_add_line(pdf, NULL, w, h, w, h+s/5, fat, PDF_BLACK);
    pdf_add_line(pdf, NULL, w+s, h, w+s, h+s/5, fat, PDF_BLACK);
  break;

 case 197:    
    pdf_add_line(pdf, NULL, w, h, w, h+s, fat, PDF_BLACK);
	pdf_add_line(pdf, NULL, w, h+s, w+s, h+s, fat, PDF_BLACK);
	pdf_add_line(pdf, NULL, w, h+s/2, w+s, h+s/2, fat, PDF_BLACK);
	pdf_add_line(pdf, NULL, w, h, w+s, h, fat, PDF_BLACK);
 break;

 case 168: 	
    pdf_add_line(pdf, NULL, w, h, w, h+s, fat, PDF_BLACK);
	pdf_add_line(pdf, NULL, w, h+s, w+s, h+s, fat, PDF_BLACK);
	pdf_add_line(pdf, NULL, w, h+s/2, w+s, h+s/2, fat, PDF_BLACK);
	pdf_add_line(pdf, NULL, w, h, w+s, h, fat, PDF_BLACK);
	pdf_add_ellipse(pdf, NULL, w+s/3, h+s+s/5, s/5, s/5, 1, PDF_RGB(0, 0, 0), PDF_RGB(0, 0, 0));  	
	pdf_add_ellipse(pdf, NULL, w+s-s/5, h+s+s/5, s/5, s/5, 1, PDF_RGB(0, 0, 0), PDF_RGB(0, 0, 0));
 break;

 case 198: 	
    pdf_add_line(pdf, NULL, w+s/2, h, w+s/2, h+s, fat, PDF_BLACK);
	pdf_add_line(pdf, NULL, w, h, w+s, h+s, fat, PDF_BLACK);
	pdf_add_line(pdf, NULL, w, h+s, w+s, h, fat, PDF_BLACK);
 break;

 case 199: 	
    pdf_add_line(pdf, NULL, w+s, h, w+s, h+s, fat, PDF_BLACK);
	pdf_add_line(pdf, NULL, w, h+s, w+s, h+s, fat, PDF_BLACK);
	pdf_add_line(pdf, NULL, w, h+2*s/3, w+s, h+2*s/3, fat, PDF_BLACK);
	pdf_add_line(pdf, NULL, w, h, w+s, h, fat, PDF_BLACK);
 break;

 case 200: 	
    pdf_add_line(pdf, NULL, w+s, h, w+s, h+s, fat, PDF_BLACK);
	pdf_add_line(pdf, NULL, w, h, w, h+s, fat, PDF_BLACK);
	pdf_add_line(pdf, NULL, w, h, w+s, h+s, fat, PDF_BLACK);
 break;

 case 201: 	
    pdf_add_line(pdf, NULL, w+s, h, w+s, h+s, fat, PDF_BLACK);
	pdf_add_line(pdf, NULL, w, h, w, h+s, fat, PDF_BLACK);
	pdf_add_line(pdf, NULL, w, h, w+s, h+s, fat, PDF_BLACK);
	pdf_add_ellipse(pdf, NULL, w+s/2, h+s+s/5, s/5, s/5, 1, PDF_RGB(0, 0, 0), PDF_RGB(0, 0, 0));
 break;

 case 202: 		
	pdf_add_line(pdf, NULL, w, h, w, h+s, fat, PDF_BLACK);
	pdf_add_line(pdf, NULL, w, h+s/2, w+s, h+s, fat, PDF_BLACK);
	pdf_add_line(pdf, NULL, w, h+s/2, w+s, h, fat, PDF_BLACK);	
 break;

 case 203: 	
	pdf_add_line(pdf, NULL, w, h, w+s/2, h+s, fat, PDF_BLACK);
    pdf_add_line(pdf, NULL, w+s/2, h+s, w+s, h, fat, PDF_BLACK);
 break;

 case 204:     
    pdf_add_line(pdf, NULL, w+s, h, w+s, h+s, fat, PDF_BLACK);
	pdf_add_line(pdf, NULL, w, h, w, h+s, fat, PDF_BLACK);
	pdf_add_line(pdf, NULL, w, h+s, w+s/2, h, fat, PDF_BLACK);
	pdf_add_line(pdf, NULL, w+s/2, h, w+s, h+s, fat, PDF_BLACK);
 break;

 case 205: 	
    pdf_add_line(pdf, NULL, w+s, h, w+s, h+s, fat, PDF_BLACK);
	pdf_add_line(pdf, NULL, w, h, w, h+s, fat, PDF_BLACK);
	pdf_add_line(pdf, NULL, w, h+s/2, w+s, h+s/2, fat, PDF_BLACK);
 break;

 case 206: 		
	pdf_add_ellipse(pdf, NULL, w+s/2, h+s/2, s/2, s/2, 1, PDF_RGB(0, 0, 0), PDF_TRANSPARENT); 
 break;

 case 207: 		
    pdf_add_line(pdf, NULL, w+s, h, w+s, h+s, fat, PDF_BLACK);
	pdf_add_line(pdf, NULL, w, h, w, h+s, fat, PDF_BLACK);
	pdf_add_line(pdf, NULL, w, h+s, w+s, h+s, fat, PDF_BLACK);
 break;

 case 208: 		
    pdf_add_line(pdf, NULL, w, h, w, h+s, fat, PDF_BLACK);
	pdf_add_ellipse(pdf, NULL, w+s/3, h+s-s/4, s/3, s/3, 1, PDF_RGB(0, 0, 0), PDF_TRANSPARENT);
 break;

 case 209: 		
    pdf_add_line(pdf, NULL, w, h, w, h+s, fat, PDF_BLACK);
	pdf_add_line(pdf, NULL, w, h+s, w+s, h+s, fat, PDF_BLACK);
	pdf_add_line(pdf, NULL, w, h, w+s, h, fat, PDF_BLACK);
 break;

 case 210: 		
    pdf_add_line(pdf, NULL, w+s/2, h, w+s/2, h+s, fat, PDF_BLACK);
	pdf_add_line(pdf, NULL, w, h+s, w+s, h+s, fat, PDF_BLACK);
 break;

 case 211: 		
	pdf_add_line(pdf, NULL, w, h, w+s, h+s, fat, PDF_BLACK);
	pdf_add_line(pdf, NULL, w, h+s, w+s/2, h+s/2, fat, PDF_BLACK);
 break;

 case 212: 	
	pdf_add_line(pdf, NULL, w+s/2, h, w+s/2, h+s, fat, PDF_BLACK);
	pdf_add_ellipse(pdf, NULL, w+s/2, h+s-s/3, s/3, s/3, 1, PDF_RGB(0, 0, 0), PDF_TRANSPARENT); 
 break;

 case 213: 	
	pdf_add_line(pdf, NULL, w, h, w+s, h+s, fat, PDF_BLACK);
	pdf_add_line(pdf, NULL, w, h+s, w+s, h, fat, PDF_BLACK);
 break;

 case 214: 	
    pdf_add_line(pdf, NULL, w, h+s, w, h+s/5, fat, PDF_BLACK);
	pdf_add_line(pdf, NULL, w, h+s/5, w+s, h+s/5, fat, PDF_BLACK);
	pdf_add_line(pdf, NULL, w+s-s/5, h+s/5, w+s-s/5, h+s, fat, PDF_BLACK);
	pdf_add_line(pdf, NULL, w+s, h+s/5, w+s, h, fat, PDF_BLACK);
 break;

 case 215: 	
    pdf_add_line(pdf, NULL, w+s, h, w+s, h+s, fat, PDF_BLACK);
	pdf_add_line(pdf, NULL, w, h+s/2, w, h+s, fat, PDF_BLACK);
	pdf_add_line(pdf, NULL, w, h+s/2, w+s, h+s/2, fat, PDF_BLACK);	
 break;

 case 216: 	
    pdf_add_line(pdf, NULL, w, h+s, w, h+s/5, fat, PDF_BLACK);
    pdf_add_line(pdf, NULL, w+s/2, h+s, w+s/2, h+s/5, fat, PDF_BLACK);
	pdf_add_line(pdf, NULL, w, h+s/5, w+s, h+s/5, fat, PDF_BLACK);
	pdf_add_line(pdf, NULL, w+s, h+s/5, w+s, h+s, fat, PDF_BLACK);
 break;

 case 217: 	
    pdf_add_line(pdf, NULL, w, h+s, w, h+s/5, fat, PDF_BLACK);
    pdf_add_line(pdf, NULL, w+s/2, h+s, w+s/2, h+s/5, fat, PDF_BLACK);
	pdf_add_line(pdf, NULL, w, h+s/5, w+s, h+s/5, fat, PDF_BLACK);
	pdf_add_line(pdf, NULL, w+s-s/5, h+s/5, w+s-s/5, h+s, fat, PDF_BLACK);
	pdf_add_line(pdf, NULL, w+s, h+s/5, w+s, h, fat, PDF_BLACK);
 break;

 case 218: 	
	pdf_add_line(pdf, NULL, w, h+s, w+s/2, h+s, fat, PDF_BLACK);
	pdf_add_line(pdf, NULL, w+s/2, h, w+s/2, h+s, fat, PDF_BLACK);
	pdf_add_ellipse(pdf, NULL, w+2*s/3, h+s/3, s/4, s/4, 1, PDF_RGB(0, 0, 0), PDF_TRANSPARENT); 
 break;

 case 219: 	
	pdf_add_line(pdf, NULL, w, h, w, h+s, fat, PDF_BLACK);
	pdf_add_ellipse(pdf, NULL, w+s/3, h+s/3, s/4, s/4, 1, PDF_RGB(0, 0, 0), PDF_TRANSPARENT);
	pdf_add_line(pdf, NULL, w+s, h, w+s, h+s, fat, PDF_BLACK);
 break;

 case 220: 		
	pdf_add_line(pdf, NULL, w+s/2, h, w+s/2, h+s, fat, PDF_BLACK);
	pdf_add_ellipse(pdf, NULL, w+2*s/3, h+s/3, s/4, s/4, 1, PDF_RGB(0, 0, 0), PDF_TRANSPARENT); 
 break;

 case 221: 	
    pdf_add_line(pdf, NULL, w+s, h, w+s, h+s, fat, PDF_BLACK);
	pdf_add_line(pdf, NULL, w, h+s, w+s, h+s, fat, PDF_BLACK);
	pdf_add_line(pdf, NULL, w+s/2, h+s/2, w+s, h+s/2, fat, PDF_BLACK);
	pdf_add_line(pdf, NULL, w, h, w+s, h, fat, PDF_BLACK);
 break;

 case 222: 	
    pdf_add_line(pdf, NULL, w, h, w, h+s, fat, PDF_BLACK);
    pdf_add_line(pdf, NULL, w, h+s/2, w+s/2, h+s/2, fat, PDF_BLACK);
    pdf_add_ellipse(pdf, NULL, w+2*s/3, h+s/2, s/4, s/2, 1, PDF_RGB(0, 0, 0), PDF_TRANSPARENT); 
  break;

 case 223:    
	pdf_add_line(pdf, NULL, w+s, h, w+s, h+s, fat, PDF_BLACK);
	pdf_add_line(pdf, NULL, w, h, w+s, h+s/2, fat, PDF_BLACK);
    pdf_add_ellipse(pdf, NULL, w+s-s/4, h+s-s/4, s/4, s/4, 1, PDF_RGB(0, 0, 0), PDF_TRANSPARENT); 
 break;

 case 48:     
    // 0
	pdf_add_ellipse(pdf, NULL, w+s/2, h+s/2, s/4, s/2, 1, PDF_RGB(0, 0, 0), PDF_TRANSPARENT); 
//	pdf_add_line(pdf, NULL, w, h, w+s, h+s, fat, PDF_BLACK);
 break;

 case 49: 	
	pdf_add_line(pdf, NULL, w+s, h, w+s, h+s, fat, PDF_BLACK);
	pdf_add_line(pdf, NULL, w+s, h+s, w+s/2, h+s/2, fat, PDF_BLACK);
 break;

 case 50: 	
    pdf_add_line(pdf, NULL, w+s, h+s, w+s, h+s/2, fat, PDF_BLACK);
	pdf_add_line(pdf, NULL, w, h+s, w+s, h+s, fat, PDF_BLACK);
	pdf_add_line(pdf, NULL, w, h+s/2, w+s, h+s/2, fat, PDF_BLACK);
	pdf_add_line(pdf, NULL, w, h, w+s, h, fat, PDF_BLACK);
	pdf_add_line(pdf, NULL, w, h, w, h+s/2, fat, PDF_BLACK);
 break;

 case 51: 	
    pdf_add_line(pdf, NULL, w+s, h, w+s, h+s, fat, PDF_BLACK);
	pdf_add_line(pdf, NULL, w, h+s, w+s, h+s, fat, PDF_BLACK);
	pdf_add_line(pdf, NULL, w, h+s/2, w+s, h+s/2, fat, PDF_BLACK);
	pdf_add_line(pdf, NULL, w, h, w+s, h, fat, PDF_BLACK);
 break;

 case 52: 	
    pdf_add_line(pdf, NULL, w+s, h, w+s, h+s, fat, PDF_BLACK);
	pdf_add_line(pdf, NULL, w, h+s/2, w, h+s, fat, PDF_BLACK);
	pdf_add_line(pdf, NULL, w, h+s/2, w+s, h+s/2, fat, PDF_BLACK);	
 break;

 case 53: 	
    pdf_add_line(pdf, NULL, w, h+s, w, h+s/2, fat, PDF_BLACK);
	pdf_add_line(pdf, NULL, w, h+s, w+s, h+s, fat, PDF_BLACK);
	pdf_add_line(pdf, NULL, w, h+s/2, w+s, h+s/2, fat, PDF_BLACK);
	pdf_add_line(pdf, NULL, w, h, w+s, h, fat, PDF_BLACK);
	pdf_add_line(pdf, NULL, w+s, h, w+s, h+s/2, fat, PDF_BLACK);
 break;

 case 54: 	
    pdf_add_line(pdf, NULL, w, h+s, w, h, fat, PDF_BLACK);
	pdf_add_line(pdf, NULL, w, h+s, w+s, h+s, fat, PDF_BLACK);
	pdf_add_line(pdf, NULL, w, h+s/2, w+s, h+s/2, fat, PDF_BLACK);
	pdf_add_line(pdf, NULL, w, h, w+s, h, fat, PDF_BLACK);
	pdf_add_line(pdf, NULL, w+s, h, w+s, h+s/2, fat, PDF_BLACK);
 break;

 case 55: 		
    pdf_add_line(pdf, NULL, w+s/2, h, w+s/2, h+s, fat, PDF_BLACK);
    pdf_add_line(pdf, NULL, w, h+s, w+s/2, h+s, fat, PDF_BLACK);
	pdf_add_line(pdf, NULL, w, h+s/2, w+s, h+s/2, fat, PDF_BLACK);
 break;

 case 56: 		
    pdf_add_line(pdf, NULL, w, h+s, w, h, fat, PDF_BLACK);
	pdf_add_line(pdf, NULL, w, h+s, w+s, h+s, fat, PDF_BLACK);
	pdf_add_line(pdf, NULL, w, h+s/2, w+s, h+s/2, fat, PDF_BLACK);
	pdf_add_line(pdf, NULL, w, h, w+s, h, fat, PDF_BLACK);
	pdf_add_line(pdf, NULL, w+s, h, w+s, h+s, fat, PDF_BLACK);
 break;

 case 57: 	
    pdf_add_line(pdf, NULL, w, h+s, w, h+s/2, fat, PDF_BLACK);
	pdf_add_line(pdf, NULL, w, h+s, w+s, h+s, fat, PDF_BLACK);
	pdf_add_line(pdf, NULL, w, h+s/2, w+s, h+s/2, fat, PDF_BLACK);
	pdf_add_line(pdf, NULL, w, h, w+s, h, fat, PDF_BLACK);
	pdf_add_line(pdf, NULL, w+s, h, w+s, h+s, fat, PDF_BLACK);
 break;

 case 46: 	
	pdf_add_ellipse(pdf, NULL, w+s/2, h+s/4, s/5, s/5, 1, PDF_RGB(0, 0, 0), PDF_RGB(0, 0, 0));  
 break;

 case 44: 	
	pdf_add_ellipse(pdf, NULL, w+s/2, h+s/4, s/5, s/5, 1, PDF_RGB(0, 0, 0), PDF_RGB(0, 0, 0)); 
	pdf_add_line(pdf, NULL, w+s-s/3, h-s/3, w+s-s/3, h+s/3, fat*2, PDF_BLACK); 
 break;

 case 43: 	
    pdf_add_line(pdf, NULL, w+s/2, h, w+s/2, h+s, fat*2, PDF_BLACK);
	pdf_add_line(pdf, NULL, w, h+s/2, w+s, h+s/2, fat*2, PDF_BLACK);
 break;

 case 45: 	
	pdf_add_line(pdf, NULL, w, h+s/2, w+s, h+s/2, fat*2, PDF_BLACK);
 break;

 case 58: 		
	pdf_add_ellipse(pdf, NULL, w+s/2, h+s/4, s/5, s/5, 1, PDF_RGB(0, 0, 0), PDF_RGB(0, 0, 0));
	pdf_add_ellipse(pdf, NULL, w+s/2, h+s, s/5, s/5, 1, PDF_RGB(0, 0, 0), PDF_RGB(0, 0, 0));
	 break;
	  
 case 32: 		
	 break;


       default: 
            break;
    }
    w+=letdiv;

}
//add text to page end

}//read _fin file line by line end
    
    pdf_save(pdf, "__output.pdf");
    pdf_destroy(pdf);
}