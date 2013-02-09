#include "jpeg.h"
#include "bitmap.h"
#include"bitwriter.h"
#include"ppm.h"
#include <iostream>

using namespace std;
int cb[1000][1000];
int cr[1000][1000];

int main(int argc, char *argv[]) {
 
	inizializza();
	
	image<unsigned long> img;
	read_ppm("ppm.ppm",img);
	save_ppm("copia.ppm",img);

	/*image<byte> img;

	// Leggo il file
	if (!ReadBitmap("duke.bmp",img)) {
		cout<<"Errore"<<endl;
		return -1;
	}
	
	for(int i=0; i<img.height(); ++i){
		for(int j=0; j<img.width(); ++j)
			cout<<(unsigned)img(i,j)<<" ";
		cout<<endl;
	}
	
	WriteBitmap ("copia.bmp",img);*/
	int alt = img.height();
	int larg = img.width();
	img.rgb_ycbcr(3);
	img.sottocampionare(cb,cr,2);
	
	for(int h=0; h<4; ++h){
		for(int w=0; w<4; ++w){
			cout << cb[h][w] << " ";
		}
		cout << endl;
	}

	for(int h=0; h<4; ++h){
		for(int w=0; w<4; ++w){
			cout << cr[h][w] << " ";
		}
		cout << endl;
	}
	
	int rosso[8][8];
	for(unsigned h=0; h<8; h++){
		for(unsigned w=0; w<8; w++){
		 rosso[h][w] = (int)img.get_red(h,w);
		 cout << rosso[h][w] << " ";
		}
		cout << endl;
	}

	cout << endl;
	int verde[8][8];
	for(unsigned h=0; h<8; h++){
		for(unsigned w=0; w<8; w++){
		 verde[h][w] = (int)img.get_green(h,w);
		 cout << verde[h][w] << " ";
		}
		cout << endl;
	}

	cout << endl;
	int blu[8][8];
	for(unsigned h=0; h<8; h++){
		for(unsigned w=0; w<8; w++){
		 blu[h][w] = (int)img.get_blu(h,w);
		 cout << blu[h][w] << " ";
		}
		cout << endl;
	}

	int matprova[8][8] = {{95,91,91,91,84,84,84,84},{109,99,95,91,91,84,84,84},{109,109,99,99,91,91,91,91},{120,120,109,106,99,95,91,91},{131,131,120,109,106,99,95,91},{145,139,131,120,109,106,99,95},{157,145,139,131,120,109,106,99},{169,157,145,131,131,120,109,106}};
	int tot = 0;
	int ycbcr[8][8];

	//RGBtoYCbCr(matprova,ycbcr);
	// dividere in blocchi
	// metti i 3 colori insieme nel file

	scala(matprova);
	
	float trasf[8][8];
	fdct(matprova,trasf);
	
	quantizza(1,trasf,matprova);
	
	int zig[64];
	zig_zag(matprova,zig);
	
	bitwriter bit_out("out.jpg");
	scriviHeaders(bit_out);
	scriviQT(bit_out);
	scriviHT(bit_out);
	scriviSOS(bit_out);
	huffmann(zig,bit_out);

	scriviEOI(bit_out);

	return 0;
}