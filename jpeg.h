#include "bitwriter.h";
#include <math.h>
#include <iostream>

using namespace std;

typedef unsigned char byte;

struct coefficienti { // entry delle tabelle di Huffmann AC e DC
	byte length;
	unsigned code;
};

static int SOI = 65496;
static int APP0 = 65504;
static int APP0_length = 16;
static int JF = 19014;
static int IF0 = 4802048;
static int version = 257;
static int units = 0;
static int XDensity = 1;
static int YDensity = 1;
static int XThumbnail = 0;
static int YThumbnail = 0;
static int DQT = 65499;
static int SOF0 = 65472;
static int DHT = 65476;
static int SOS = 65498;
static int EOI = 65497;

static int matquant[8][8] = {{16,11,10,16,24,40,51,61},{12,12,14,19,26,58,60,55},{14,13,16,24,40,57,69,56},{14,17,22,29,51,87,80,62},{18,22,37,56,68,109,103,77},{24,35,55,64,81,104,113,92},{49,64,78,87,103,121,120,101},{72,92,95,98,112,100,103,99}};
static float pi = 3.1415926535;
coefficienti dc_table[12];
coefficienti ac_table[16][11];

void inizializza(){
	ac_table[0][0].length = 4;		ac_table[0][0].code = 9;
	ac_table[0][1].length = 2;		ac_table[0][1].code = 0;
	ac_table[0][2].length = 2;		ac_table[0][2].code = 1;
	ac_table[0][3].length = 3;		ac_table[0][3].code = 4;
	ac_table[0][4].length = 4;		ac_table[0][4].code = 11;
	ac_table[0][5].length = 5;		ac_table[0][5].code = 26;
	ac_table[0][6].length = 7;		ac_table[0][6].code = 120;
	ac_table[0][7].length = 8;		ac_table[0][7].code = 240;
	ac_table[0][8].length = 10;		ac_table[0][8].code = 1014;
	ac_table[0][9].length = 16;		ac_table[0][9].code = 65410;
	ac_table[0][10].length = 16;	ac_table[0][10].code = 65411;
	ac_table[1][0].length = -1;		ac_table[1][0].code = -1;
	ac_table[1][1].length = 4;		ac_table[1][1].code = 12;
	ac_table[1][2].length = 5;		ac_table[1][2].code = 27;
	ac_table[1][3].length = 7;		ac_table[1][3].code = 121;
	ac_table[1][4].length = 9;		ac_table[1][4].code = 502;
	ac_table[1][5].length = 11;		ac_table[1][5].code = 2038;
	ac_table[1][6].length = 16;		ac_table[1][6].code = 65412;
	ac_table[1][7].length = 16;		ac_table[1][7].code = 65413;
	ac_table[1][8].length = 16;		ac_table[1][8].code = 65414;
	ac_table[1][9].length = 16;		ac_table[1][9].code = 65415;
	ac_table[1][10].length = 16;	ac_table[1][10].code = 65416;
	ac_table[2][0].length = -1;		ac_table[2][0].code = -1;
	ac_table[2][1].length = 5;		ac_table[2][1].code = 28;
	ac_table[2][2].length = 8;		ac_table[2][2].code = 249;
	ac_table[2][3].length = 10;		ac_table[2][3].code = 1015;
	ac_table[2][4].length = 12;		ac_table[2][4].code = 4084;
	ac_table[2][5].length = 16;		ac_table[2][5].code = 65417;
	ac_table[2][6].length = 16;		ac_table[2][6].code = 65418;
	ac_table[2][7].length = 16;		ac_table[2][7].code = 65419;
	ac_table[2][8].length = 16;		ac_table[2][8].code = 65420;
	ac_table[2][9].length = 16;		ac_table[2][9].code = 65421;
	ac_table[2][10].length = 16;	ac_table[2][10].code = 65422;
	ac_table[3][0].length = -1;		ac_table[3][0].code = -1;
	ac_table[3][1].length = 6;		ac_table[3][1].code = 58;
	ac_table[3][2].length = 9;		ac_table[3][2].code = 503;
	ac_table[3][3].length = 12;		ac_table[3][3].code = 4085;
	ac_table[3][4].length = 16;		ac_table[3][4].code = 65423;
	ac_table[3][5].length = 16;		ac_table[3][5].code = 65424;
	ac_table[3][6].length = 16;		ac_table[3][6].code = 65425;
	ac_table[3][7].length = 16;		ac_table[3][7].code = 65426;
	ac_table[3][8].length = 16;		ac_table[3][8].code = 65427;
	ac_table[3][9].length = 16;		ac_table[3][9].code = 65428;
	ac_table[3][10].length = 16;	ac_table[3][10].code = 65429;
	ac_table[4][0].length = -1;		ac_table[4][0].code = -1;
	ac_table[4][1].length = 6;		ac_table[4][1].code = 59;
	ac_table[4][2].length = 10;		ac_table[4][2].code = 1016;
	ac_table[4][3].length = 16;		ac_table[4][3].code = 65430;
	ac_table[4][4].length = 16;		ac_table[4][4].code = 65431;
	ac_table[4][5].length = 16;		ac_table[4][5].code = 65432;
	ac_table[4][6].length = 16;		ac_table[4][6].code = 65433;
	ac_table[4][7].length = 16;		ac_table[4][7].code = 65434;
	ac_table[4][8].length = 16;		ac_table[4][8].code = 65435;
	ac_table[4][9].length = 16;		ac_table[4][9].code = 65436;
	ac_table[4][10].length = 16;	ac_table[4][10].code = 65437;
	ac_table[5][0].length = -1;		ac_table[5][0].code = -1;
	ac_table[5][1].length = 7;		ac_table[5][1].code = 122;
	ac_table[5][2].length = 11;		ac_table[5][2].code = 2039;
	ac_table[5][3].length = 16;		ac_table[5][3].code = 65438;
	ac_table[5][4].length = 16;		ac_table[5][4].code = 65439;
	ac_table[5][5].length = 16;		ac_table[5][5].code = 65440;
	ac_table[5][6].length = 16;		ac_table[5][6].code = 65441;
	ac_table[5][7].length = 16;		ac_table[5][7].code = 65442;
	ac_table[5][8].length = 16;		ac_table[5][8].code = 65443;
	ac_table[5][9].length = 16;		ac_table[5][9].code = 65444;
	ac_table[5][10].length = 16;	ac_table[5][10].code = 65445;
	ac_table[6][0].length = -1;		ac_table[6][0].code = -1;
	ac_table[6][1].length = 7;		ac_table[6][1].code = 123;
	ac_table[6][2].length = 12;		ac_table[6][2].code = 4086;
	ac_table[6][3].length = 16;		ac_table[6][3].code = 65446;
	ac_table[6][4].length = 16;		ac_table[6][4].code = 65447;
	ac_table[6][5].length = 16;		ac_table[6][5].code = 65448;
	ac_table[6][6].length = 16;		ac_table[6][6].code = 65449;
	ac_table[6][7].length = 16;		ac_table[6][7].code = 65450;
	ac_table[6][8].length = 16;		ac_table[6][8].code = 65451;
	ac_table[6][9].length = 16;		ac_table[6][9].code = 65452;
	ac_table[6][10].length = 16;	ac_table[6][10].code = 65453;
	ac_table[7][0].length = -1;		ac_table[7][0].code = -1;
	ac_table[7][1].length = 8;		ac_table[7][1].code = 250;
	ac_table[7][2].length = 12;		ac_table[7][2].code = 4087;
	ac_table[7][3].length = 16;		ac_table[7][3].code = 65454;
	ac_table[7][4].length = 16;		ac_table[7][4].code = 65455;
	ac_table[7][5].length = 16;		ac_table[7][5].code = 65456;
	ac_table[7][6].length = 16;		ac_table[7][6].code = 65457;
	ac_table[7][7].length = 16;		ac_table[7][7].code = 65458;
	ac_table[7][8].length = 16;		ac_table[7][8].code = 65459;
	ac_table[7][9].length = 16;		ac_table[7][9].code = 65460;
	ac_table[7][10].length = 16;	ac_table[7][10].code = 65461;
	ac_table[8][0].length = -1;		ac_table[8][0].code = -1;
	ac_table[8][1].length=9;		ac_table[8][1].code=504;
	ac_table[8][2].length=15;		ac_table[8][2].code=32704;
	ac_table[8][3].length=16;		ac_table[8][3].code=65462;
	ac_table[8][4].length=16;		ac_table[8][4].code=65463;
	ac_table[8][5].length=16;		ac_table[8][5].code=65464;
	ac_table[8][6].length=16;		ac_table[8][6].code=65465;
	ac_table[8][7].length=16;		ac_table[8][7].code=65466;
	ac_table[8][8].length=16;		ac_table[8][8].code=65467;
	ac_table[8][9].length=16;		ac_table[8][9].code=65468;
	ac_table[8][10].length=16;		ac_table[8][10].code=65469;
	ac_table[9][0].length = -1;		ac_table[9][0].code = -1;
	ac_table[9][1].length=9;		ac_table[9][1].code=505;
	ac_table[9][2].length=16;		ac_table[9][2].code=65470;
	ac_table[9][3].length=16;		ac_table[9][3].code=65471;
	ac_table[9][4].length=16;		ac_table[9][4].code=65472;
	ac_table[9][5].length=16;		ac_table[9][5].code=65473;
	ac_table[9][6].length=16;		ac_table[9][6].code=65474;
	ac_table[9][7].length=16;		ac_table[9][7].code=65475;
	ac_table[9][8].length=16;		ac_table[9][8].code=65476;
	ac_table[9][9].length=16;		ac_table[9][9].code=65477;
	ac_table[9][10].length=16;		ac_table[10][10].code=65478;
	ac_table[10][0].length = -1;	ac_table[10][0].code = -1;
	ac_table[10][1].length=9;		ac_table[10][1].code=506;
	ac_table[10][2].length=16;		ac_table[10][2].code=65479;
	ac_table[10][3].length=16;		ac_table[10][3].code=65480;
	ac_table[10][4].length=16;		ac_table[10][4].code=65481;
	ac_table[10][5].length=16;		ac_table[10][5].code=65482;
	ac_table[10][6].length=16;		ac_table[10][6].code=65483;
	ac_table[10][7].length=16;		ac_table[10][7].code=65484;
	ac_table[10][8].length=16;		ac_table[10][8].code=65485;
	ac_table[10][9].length=16;		ac_table[10][9].code=65486;
	ac_table[10][10].length=16;		ac_table[10][10].code=65487;
	ac_table[11][0].length = -1;	ac_table[11][0].code = -1;
	ac_table[11][1].length=10;		ac_table[11][1].code=1017;
	ac_table[11][2].length=16;		ac_table[11][2].code=65488;
	ac_table[11][3].length=16;		ac_table[11][3].code=65489;
	ac_table[11][4].length=16;		ac_table[11][4].code=65490;
	ac_table[11][5].length=16;		ac_table[11][5].code=65491;
	ac_table[11][6].length=16;		ac_table[11][6].code=65492;
	ac_table[11][7].length=16;		ac_table[11][7].code=65493;
	ac_table[11][8].length=16;		ac_table[11][8].code=65494;
	ac_table[11][9].length=16;		ac_table[11][9].code=65495;
	ac_table[11][10].length=16;		ac_table[11][10].code=65496;
	ac_table[12][0].length = -1;	ac_table[12][0].code = -1;
	ac_table[12][1].length=10;		ac_table[12][1].code=1018;
	ac_table[12][2].length=16;		ac_table[12][2].code=65497;
	ac_table[12][3].length=16;		ac_table[12][3].code=65498;
	ac_table[12][4].length=16;		ac_table[12][4].code=65499;
	ac_table[12][5].length=16;		ac_table[12][5].code=65500;
	ac_table[12][6].length=16;		ac_table[12][6].code=65501;
	ac_table[12][7].length=16;		ac_table[12][7].code=65502;
	ac_table[12][8].length=16;		ac_table[12][8].code=65503;
	ac_table[12][9].length=16;		ac_table[12][9].code=65504;
	ac_table[12][10].length=16;		ac_table[12][10].code=65505;
	ac_table[13][0].length = -1;	ac_table[13][0].code = -1;
	ac_table[13][1].length=11;		ac_table[13][1].code=2040;
	ac_table[13][2].length=16;		ac_table[13][2].code=65506;
	ac_table[13][3].length=16;		ac_table[13][3].code=65507;
	ac_table[13][4].length=16;		ac_table[13][4].code=65508;
	ac_table[13][5].length=16;		ac_table[13][5].code=65509;
	ac_table[13][6].length=16;		ac_table[13][6].code=65510;
	ac_table[13][7].length=16;		ac_table[13][7].code=65511;
	ac_table[13][8].length=16;		ac_table[13][8].code=65512;
	ac_table[13][9].length=16;		ac_table[13][9].code=65513;
	ac_table[13][10].length=16;		ac_table[13][10].code=65514;
	ac_table[14][0].length = -1;	ac_table[14][0].code = -1;
	ac_table[14][1].length=16;		ac_table[14][1].code=65515;
	ac_table[14][2].length=16;		ac_table[14][2].code=65516;
	ac_table[14][3].length=16;		ac_table[14][3].code=65517;
	ac_table[14][4].length=16;		ac_table[14][4].code=65518;
	ac_table[14][5].length=16;		ac_table[14][5].code=65519;
	ac_table[14][6].length=16;		ac_table[14][6].code=65520;
	ac_table[14][7].length=16;		ac_table[14][7].code=65521;
	ac_table[14][8].length=16;		ac_table[14][8].code=65522;
	ac_table[14][9].length=16;		ac_table[14][9].code=65523;
	ac_table[14][10].length=16;		ac_table[14][10].code=65524;
	ac_table[15][0].length=11;		ac_table[15][0].code=2041;
	ac_table[15][1].length=16;		ac_table[15][1].code=65525;
	ac_table[15][2].length=16;		ac_table[15][2].code=65526;
	ac_table[15][3].length=16;		ac_table[15][3].code=65527;
	ac_table[15][4].length=16;		ac_table[15][4].code=65528;
	ac_table[15][5].length=16;		ac_table[15][5].code=65529;
	ac_table[15][6].length=16;		ac_table[15][6].code=65530;
	ac_table[15][7].length=16;		ac_table[15][7].code=65531;
	ac_table[15][8].length=16;		ac_table[15][8].code=65532;
	ac_table[15][9].length=16;		ac_table[15][9].code=65533;
	ac_table[15][10].length=16;		ac_table[15][10].code=65534;	
	dc_table[0].length=2;			dc_table[0].code=0;
	dc_table[1].length=3;			dc_table[1].code=2;
	dc_table[2].length=3;			dc_table[2].code=3;
	dc_table[3].length=3;			dc_table[3].code=4;
	dc_table[4].length=3;			dc_table[4].code=5;
	dc_table[5].length=3;			dc_table[5].code=6;
	dc_table[6].length=4;			dc_table[6].code=14;
	dc_table[7].length=5;			dc_table[7].code=30;
	dc_table[8].length=6;			dc_table[8].code=62;
	dc_table[9].length=7;			dc_table[9].code=126;
	dc_table[10].length=8;			dc_table[10].code=254;
	dc_table[11].length=9;			dc_table[11].code=510;
}
//--------------------------------------------------------------------------
void RGBtoYCbCr(int block[8][8],int ycbcr[8][8]){
	for(int i=0; i<8; ++i){
		for(int j=0; j<8; ++j){

		}
	}
}
//--------------------------------------------------------------------------
void scriviHeaders(bitwriter& bit_out){
	bit_out.write(SOI,16);
	bit_out.write(APP0,16);
	bit_out.write(APP0_length,16);
	bit_out.write(JF,16);
	bit_out.write(IF0,24);
	bit_out.write(version,16);
	bit_out.write(units,8);
	bit_out.write(XDensity,16);
	bit_out.write(YDensity,16);
	bit_out.write(XThumbnail,8);
	bit_out.write(YThumbnail,8);
}

void scriviEOI(bitwriter& bit_out){
	bit_out.write_8Golomb();
	bit_out.write(EOI,16);
}


//--------------------------------------------------------------------------
void quantizza(float valquant,float block[8][8],int Quantizzata[8][8]){
 unsigned int Quantizzazione[8][8];
	for(byte i=0;i<8;i++)
	 for(byte j=0;j<8;j++){
		 Quantizzazione[i][j]=matquant[i][j]*valquant;
		 if(block[i][j]<0){
			 Quantizzata[i][j]=(int)((block[i][j]/Quantizzazione[i][j])-0.5);
		 }
		 else{
			 Quantizzata[i][j]=(int)((block[i][j]/Quantizzazione[i][j])+0.5);
		 }
	 }
}
//----------------------------------------------------------------------------
void fdct(int block[8][8],float trasf[8][8]){
 for(int u=0;u<8;u++)
	 for(int v=0;v<8;v++){
		trasf[u][v]=0;
		for (int x=0; x<8; x++) 
			for (int y=0; y<8; y++) 
				trasf[u][v]+=block[x][y]*cos(((2*x+1)*u*pi)/16.0)*cos(((2*y+1)*v*pi)/16.0);				
		trasf[u][v]/=4;
		if(u==0)
			trasf[u][v]/=sqrt((float)2);
		if(v==0)
			trasf[u][v]/=sqrt((float)2);
	 }
}
//------------------------------------------------------------------
void zig_zag(int Quantizzata[8][8],int zig_zag[64]){
	zig_zag[0] = Quantizzata[0][0];	 zig_zag[1] = Quantizzata[0][1];  zig_zag[2] = Quantizzata[1][0];  zig_zag[3] = Quantizzata[2][0];
	zig_zag[4] = Quantizzata[1][1];	 zig_zag[5] = Quantizzata[0][2];  zig_zag[6] = Quantizzata[0][3];  zig_zag[7] = Quantizzata[1][2];
	zig_zag[8] = Quantizzata[2][1];	 zig_zag[9] = Quantizzata[3][0];  zig_zag[10] = Quantizzata[4][0]; zig_zag[11] = Quantizzata[3][1];
	zig_zag[12] = Quantizzata[2][2]; zig_zag[13] = Quantizzata[1][3]; zig_zag[14] = Quantizzata[0][4]; zig_zag[15] = Quantizzata[0][5];
	zig_zag[16] = Quantizzata[1][4]; zig_zag[17] = Quantizzata[2][3]; zig_zag[18] = Quantizzata[3][2]; zig_zag[19] = Quantizzata[4][1];
	zig_zag[20] = Quantizzata[5][0]; zig_zag[21] = Quantizzata[6][0]; zig_zag[22] = Quantizzata[5][1]; zig_zag[23] = Quantizzata[4][2];
	zig_zag[24] = Quantizzata[3][3]; zig_zag[25] = Quantizzata[2][4]; zig_zag[26] = Quantizzata[1][5]; zig_zag[27] = Quantizzata[0][6];
	zig_zag[28] = Quantizzata[0][7]; zig_zag[29] = Quantizzata[1][6]; zig_zag[30] = Quantizzata[2][5]; zig_zag[31] = Quantizzata[3][4];
	zig_zag[32] = Quantizzata[4][3]; zig_zag[33] = Quantizzata[5][2]; zig_zag[34] = Quantizzata[6][1]; zig_zag[35] = Quantizzata[7][0];
	zig_zag[36] = Quantizzata[7][1]; zig_zag[37] = Quantizzata[6][2]; zig_zag[38] = Quantizzata[5][3]; zig_zag[39] = Quantizzata[4][4];
	zig_zag[40] = Quantizzata[3][5]; zig_zag[41] = Quantizzata[2][6]; zig_zag[42] = Quantizzata[1][7]; zig_zag[43] = Quantizzata[2][7];
	zig_zag[44] = Quantizzata[3][6]; zig_zag[45] = Quantizzata[4][5]; zig_zag[46] = Quantizzata[5][4]; zig_zag[47] = Quantizzata[6][3];
	zig_zag[48] = Quantizzata[7][2]; zig_zag[49] = Quantizzata[7][3]; zig_zag[50] = Quantizzata[6][4]; zig_zag[51] = Quantizzata[5][5];
	zig_zag[52] = Quantizzata[4][6]; zig_zag[53] = Quantizzata[3][7]; zig_zag[54] = Quantizzata[4][7]; zig_zag[55] = Quantizzata[5][6];
	zig_zag[56] = Quantizzata[6][5]; zig_zag[57] = Quantizzata[7][4]; zig_zag[58] = Quantizzata[7][5]; zig_zag[59] = Quantizzata[6][6];
	zig_zag[60] = Quantizzata[5][7]; zig_zag[61] = Quantizzata[6][7]; zig_zag[62] = Quantizzata[7][6]; zig_zag[63] = Quantizzata[7][7];

	for(byte i=0;i<64;i++)
		cout << zig_zag[i] << " ";
	cout << endl;
}
//-------------------------------------------------
void scriviQT(bitwriter& bit_out){
	bit_out.write(DQT,16);
	bit_out.write(67,16); // DQT_length
	bit_out.write(0,8); // DQT precision and ID
	int QT_zigzag[64];
	zig_zag(matquant,QT_zigzag);
	for(unsigned i=0; i<64; ++i){
		bit_out.write(QT_zigzag[i],8);
	}
}
//-------------------------------------------------
void scriviHT(bitwriter& bit_out){
	bit_out.write(SOF0,16);
	bit_out.write(17,16); // SOF0_length
	bit_out.write(8,8); // Precision

	// scrivi qui image length e width
	bit_out.write(8,16);
	bit_out.write(8,16);
	
	bit_out.write(3,8); //NOC: 3 = a colori
	bit_out.write(74240,24); // info su Y
	bit_out.write(135425,24); // info su Cb
	bit_out.write(200961,24); // info su Cr

	// Tabella di Huffmann DC
	bit_out.write(DHT,16);
	bit_out.write(31,16); // lunghezza
	bit_out.write(0,8); // class=0 (DC), id=0
	bit_out.write(66817,32); // 00 01 05 01 (0 codici lunghi 1, 1 codice lungo 2, 5 codici lunghi 3, 1 codice lungo 4)
	bit_out.write(16843009,32); // 01 01 01 01
	bit_out.write(16777216,32); // 01 00 00 00
	bit_out.write(0,32); // 00 00 00 00
	for(int i=0; i<12; ++i){ // scrivo la DC Huffmann Table
		bit_out.write(dc_table[i].code,8);
	}

	// Tabella di Huffmann AC
	bit_out.write(DHT,16);
	bit_out.write(181,16); // lunghezza
	bit_out.write(16,8); // class=1 (AC), id=0
	bit_out.write(131331,32); // 00 02 01 03
	bit_out.write(50463747,32); // 03 02 04 03
	bit_out.write(84214788,32); // 05 05 04 04
	bit_out.write(381,32); // 00 00 01 7D
	for(int l=1; l<=16; ++l){
		for(int i=0; i<16; ++i){
			for(int j=0; j<11; ++j){
				if(ac_table[i][j].length == l){
					bit_out.write(ac_table[i][j].code,8);
				}
			}
		}
	}
}
//-------------------------------------------------
void scriviSOS(bitwriter& bit_out){
	bit_out.write(SOS,16);

	bit_out.write(12,16); // lunghezza

	bit_out.write(3,8); // Number Of Components
	bit_out.write(1,8);
	bit_out.write(0,8); // id tabelle DC e AC
	bit_out.write(34669329,32);
	bit_out.write(0,8);
}
//-------------------------------------------------
void huffmann(int zig_zag[64], bitwriter& bit_out){
	int DC = zig_zag[0];
	int SSSS = log((float)abs(DC))/log(2.0f) + 1; //log in base 2
	bit_out.write(dc_table[SSSS].code,dc_table[SSSS].length);
	if(DC>0){
		bit_out.write(DC,SSSS);
	}
	else if(DC<0){
		bit_out.write(DC-1,SSSS);
	}

	int NNNN = 0;
	int sedicizeri = 0;
	for(int i=1; i<64; ++i){
		if(zig_zag[i] == 0){
			++NNNN;
			if(NNNN == 16){
				++sedicizeri;
			}
		}
		else{
			for(int j=0; j<sedicizeri; ++j){
				bit_out.write(ac_table[15][0].code,ac_table[15][0].length);
			}
			int AC = zig_zag[i];
			int SSSS = log((float)abs(AC))/log(2.0f) + 1; //log in base 2
			bit_out.write(ac_table[NNNN][SSSS].code,ac_table[NNNN][SSSS].length);
			if(AC>0){
				bit_out.write(AC,SSSS);
			}
			else if(AC<0){
				bit_out.write(AC-1,SSSS);
			}
			NNNN = 0;
		}
	}
	bit_out.write(10,4); //EOB
}
//-------------------------------------------------
void scala(int block[8][8]){
	for(int i=0; i<8; i++)
		for(int j=0; j<8; j++)
			block[i][j] -= 128;
}
//-----------------------------------------
void stampa_blocco(byte block[8][8]){
	for(byte i=0; i<8; i++){
		for(byte j=0; j<8; j++){
			cout << block[i][j] << "\t";
		}
		cout << endl;
	}
	cout<<endl;
}
//-----------------------------------------
void stampa_blocco(int block[8][8]){
	for(byte i=0; i<8; i++){
		for(byte j=0; j<8; j++){
			cout << block[i][j] << "\t";
		}
		cout << endl;
	}
	cout<<endl;
}
//-----------------------------------------
void stampa_blocco(float block[8][8]){
	for(byte i=0; i<8; i++){
		for(byte j=0; j<8; j++){
			if(block[i][j]<0){
				cout << (int)(block[i][j]-0.5) << " ";
			}
			else{
				cout << (int)(block[i][j]+0.5) << " ";
			}
		}
		cout << endl;
	}
	cout<<endl;
}