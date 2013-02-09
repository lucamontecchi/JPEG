#ifndef bitmap_h  
#define bitmap_h

#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include "image.h"
#include "bitreader.h"
#include "bitwriter.h"
using namespace std;

typedef unsigned char	byte;

struct BITMAPFILEHEADER {
	unsigned short bfType;
	unsigned int bfSize;
	unsigned int bfReserved;
	unsigned int bfOffBits;

	// Recupera l'intero BitmapFileHeader
	bool get(bitreader &bitread) {
		if (!bitread.get2(bfType)) return false;     //deve leggere 2 byte
		if (!bitread.get4(bfSize)) return false;     //deve leggere 4 byte
		if (!bitread.get4(bfReserved)) return false; //deve leggere 4 byte 
		if (!bitread.get4(bfOffBits)) return false;  //deve leggere 4 byte
		return true;
	}
  
    void print(){ // Stampa il BitmapFileHeader
		cout<<" BITMAPFILEHEADER---------------"<<endl;
        cout<<"bfType "<<bfType<<endl;
        cout<<"bfSize "<<bfSize<<endl;
        cout<<"bfReserved "<<bfReserved<<endl;
        cout<<"bfOffBits "<<bfOffBits<<endl;
        cout<<" ---------------BITMAPFILEHEADER"<<endl; 
    }

	bool put(bitwriter &bitwrite) { // Scrive su file il BitmapFileHeader
		if (!bitwrite.put2_led(bfType)) return false;
		if (!bitwrite.put4_led(bfSize)) return false;
		if (!bitwrite.put4_led(bfReserved)) return false;
		if (!bitwrite.put4_led(bfOffBits)) return false;
		return true;
	}
};

struct BITMAPINFOHEADER {
	unsigned int biSize;    //4
	int biWidth;    //4
	int biHeight;   //4
	unsigned short biPlanes;   //2
	unsigned short biBitCount; //2
	unsigned int biCompression; //4
	unsigned int biSizeImage;   //4
	int biXPelsPerMeter;//4
	int biYPelsPerMeter;//4
	unsigned int biClrUsed;     //4
	unsigned int biClrImportant;//4

	bool get(bitreader &bitread) { // legge il BitmapInfoHeader
		if (!bitread.get4(biSize)) return false;
		if (!bitread.get4(biWidth)) return false;
		if (!bitread.get4(biHeight)) return false;
		if (!bitread.get2(biPlanes)) return false;
		if (!bitread.get2(biBitCount)) return false;
		if (!bitread.get4(biCompression)) return false;
		if (!bitread.get4(biSizeImage)) return false;
		if (!bitread.get4(biXPelsPerMeter)) return false;
		if (!bitread.get4(biYPelsPerMeter)) return false;
		if (!bitread.get4(biClrUsed)) return false;
		if (!bitread.get4(biClrImportant)) return false;
		return true;
	}

    void print(){ // Stampa a video il BitmapInfoHeader
        cout<<" BITMAPINFOHEADER---------------"<<endl;
        cout<<"biSize "<<biSize<<endl;
        cout<<"biWidth "<<biWidth<<endl;
        cout<<"biHeight "<<biHeight<<endl;
        cout<<"biPlanes "<<biPlanes<<endl;
        cout<<"biBitCount "<<biBitCount<<endl;
        cout<<"biCompression "<<biCompression<<endl;
        cout<<"biSizeImage "<<biSizeImage<<endl;
        cout<<"biXPelsPerMeter "<<biXPelsPerMeter<<endl;
        cout<<"biYPelsPerMeter "<<biYPelsPerMeter<<endl;
        cout<<"biClrUsed "<<biClrUsed<<endl;
        cout<<"biClrImportant "<<biClrImportant<<endl;
        cout<<" ---------------BITMAPINFOHEADER"<<endl; 
    }

	bool put(bitwriter &bitwrite) { // Scrive su file il BitmapInfoHeader
		if (!bitwrite.put4_led(biSize)) return false;
		if (!bitwrite.put4_led(biWidth)) return false;
		if (!bitwrite.put4_led(biHeight)) return false;
		if (!bitwrite.put2_led(biPlanes)) return false;
		if (!bitwrite.put2_led(biBitCount)) return false;
		if (!bitwrite.put4_led(biCompression)) return false;
		if (!bitwrite.put4_led(biSizeImage)) return false;
		if (!bitwrite.put4_led(biXPelsPerMeter)) return false;
		if (!bitwrite.put4_led(biYPelsPerMeter)) return false;
		if (!bitwrite.put4_led(biClrUsed)) return false;
		if (!bitwrite.put4_led(biClrImportant)) return false;
		return true;
	}
};

// elemento della tavolozza
struct RGBQUAD {
	byte rgbBlue;
	byte rgbGreen;
	byte rgbRed;
	byte rgbReserved;

	bool get(bitreader &bitread) { // legge un colore della tavolozza da file
		if (!bitread.getbyte(rgbBlue)) return false;
		if (!bitread.getbyte(rgbGreen)) return false;
		if (!bitread.getbyte(rgbRed)) return false;
		if (!bitread.getbyte(rgbReserved)) return false;
		return true;
	}

	bool put(bitwriter &bitwrite) { // scrrive un colore della tavolozza su file
		if (!bitwrite.write_byted(rgbBlue)) return false;
		if (!bitwrite.write_byted(rgbGreen)) return false;
		if (!bitwrite.write_byted(rgbRed)) return false;
		if (!bitwrite.write_byted(rgbReserved)) return false;
		return true;
	}
};

bool ReadBitmap(const char *szFileName, image<byte> &img) {
	//leggo tramite bitreader
    bitreader input(szFileName);
    if(!input.opened()){
        cout<<"apertura file fallita"<<endl;
		return false;
	}

	// headers
	BITMAPFILEHEADER bfh;
	if (!bfh.get(input)){
        cout<<"errore lettura bitmap file header"<<endl;
		return false;
	}
	BITMAPINFOHEADER bih;
	if (!bih.get(input)){
        cout<<"errore lettura bitmap file info header"<<endl;
		return false;
	}

    //salvo il valore di grigi-- se maggiore di 255 salvo 255
    if(bih.biClrUsed>255)
		img.set_gray(255);
    else
        img.set_gray(bih.biClrUsed);
       
    //salvo il valore dei colori
    img.set_colors(bih.biClrUsed);

	// Calcolo l'altezza
	unsigned h;
	if(bih.biHeight>0)
		h=bih.biHeight;
	else
		h=-bih.biHeight;

	// Ridimensiono l'immagine
	img.resize(bih.biWidth,h);

	vector<RGBQUAD> palette(bih.biClrUsed); // recupero la tavolozza
	for (unsigned i=0; i<bih.biClrUsed; i++){
		if (!palette[i].get(input)){
			return false;
		}
    }

	// Leggo i pixel 
	if (bih.biHeight>0) {
		for (int y=img.height()-1; y>=0; y--){
			if(!input.read ((char *)img.lineptr(y), img.linewidth())){ // leggo un'intera linea per volta
				cout<<"errore nella lettura della linea: "<<y<<" con biHeight>0"<<endl;
                return false;
			}
		}
	}
	else {
        int max=(img.height()-1);
		for (int y=0; y< max; y++)
			if(!input.read ((char *)img.lineptr(y), img.linewidth())){
	            cout<<"errore nella lettura della linea: "<<y<<" con biHeight<=0"<<endl;
                return false;
			}
	}

	return true;
}

bool WriteBitmap(const char *szFileName, image<byte> &img) {

    bitwriter out(szFileName);
    if(!out.opened())
		return false;

    unsigned colori;
    if(img.colors()>0)
        colori=img.colors();
    else
        colori=img.gray();

	// compongo il BitmapFileHeader
	BITMAPFILEHEADER bfh;
	bfh.bfType = (unsigned short)('B') | (unsigned short)(('M')<<8); // "BM" in un unsigned short little endian
	bfh.bfSize = 14+40+colori*4+img.linewidth()*img.height();//colori*4 palette
	bfh.bfReserved = 0;
	bfh.bfOffBits = 14+40+colori*4;
	if (!bfh.put(out))
		return false;

	// compongo il BitmapInfoHeader
	BITMAPINFOHEADER bih;
	bih.biSize = 40;
	bih.biWidth = img.width();
	bih.biHeight = img.height();
	bih.biPlanes = 1;
	bih.biBitCount = 8;
	bih.biCompression = 0;
	bih.biSizeImage = 0;
	bih.biXPelsPerMeter = 0;
	bih.biYPelsPerMeter = 0;
	bih.biClrUsed = colori;           
	bih.biClrImportant = colori;  //tutti i colori per ora
	if (!bih.put(out))
		return false;

	// creo la tavolozza
	for (unsigned i=0; i<bih.biClrUsed; i++) {
		RGBQUAD rgbq;

		rgbq.rgbBlue = i;
		rgbq.rgbGreen = i;
		rgbq.rgbRed = i;
		rgbq.rgbReserved = 0;

		if (!rgbq.put(out))
			return false;
	}

	// Scrivo i pixel
	for (int y=img.height()-1; y>=0; y--)
		if(!out.writed ((char *)img.lineptr(y),img.linewidth())) // scrivo un'intera linea alla volta
		    return false;

	return true;
}

/* //main di prova
int main(int argc, char *argv[]) {
	// Verifico che ci sia un parametro
	if (argc!=2) {
		cout << "Sintassi: nome_file" << endl;
		return -1;
	}

	// Dichiaro l'immagine
	image<byte> img;

	// Leggo il file
	if (!ReadBitmap(argv[1],img)) {
		// Controllo se ci sono errori
		cout << "Impossibile leggere il file " << argv[1] << endl;
		return -1;
	}

	// Scrivo il bitmap
	WriteBitmap ("copia",img);

	return 0;
}
*/

#endif