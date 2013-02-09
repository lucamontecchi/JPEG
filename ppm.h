#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "image.h"

using namespace std;
typedef unsigned char byte;

// converte un intero in stringa
string intToString(int num){
	stringstream out;
	out << num;
	return out.str();
}

//converte una stringa in un intero
int stringToInt(string num){
	int intero;
	stringstream out(num);
	out >> intero;
	return intero;
}

// scrive in little endian su un file gli ultimi n byte di un unsigned long
bool write(fstream& f, unsigned long val, byte num){
	byte b;
	byte max;

	for(byte i=1; i<=num; ++i){
		b=0; 
		max= (i-1)*8;

		for(short j=8*i-1; j>=max; --j){
			if(val &(1<<j))
				b=(b<<1)+1;
			else
				b=(b<<1);
		}
		f.put(b);
	}
	return true;
}

// scrive solo la parte blu, num indica se uno o due byte a colore
bool writeblu(fstream& f, unsigned long val, byte num){
	byte b;
	byte max;

	for(byte i=1; i<=num; ++i){
		f.put(0);
		f.put(0);
	}

	for(byte i=1; i<=num; ++i){
		b=0; 
		max= (i-1)*8;

		for(short j=8*i-1; j>=max; --j){
			if(val &(1<<j))
				b=(b<<1)+1;
			else
				b=(b<<1);
		}
		f.put(b);
	}
	return true;
}

// scrive solo la parte verde, num indica se uno o due byte a colore
bool writegreen(fstream& f, unsigned long val, byte num){
	byte b;
	byte max;

	for(byte i=1; i<=num; ++i){
		f.put(0);
	}

	for(byte i=1; i<=num; ++i){
		b=0; 
		max= (i-1+num)*8;

		for(short j=8*(i+num)-1; j>=max; --j)	{
			if(val &(1<<j))
				b=(b<<1)+1;
			else
				b=(b<<1);
		}
		f.put(b);
	}

	for(byte i=1; i<=num; ++i){
		f.put(0);
	}
	return true;
}

// scrive solo la parte rossa, num indica se uno o due byte a colore
bool writered(fstream& f, unsigned long val, byte num){
	byte b;
	byte max;

	for(byte i=1; i<=num; ++i){
		b=0; 
		max= (i-1+2*num)*8;

		for(short j=8*(i+2*num)-1; j>=max; --j){
			if(val &(1<<j))
				b=(b<<1)+1;
			else
				b=(b<<1);
		}
		f.put(b);
	}

	for(byte i=1; i<=num; ++i){
		f.put(0);
		f.put(0);
	}
	return true;
}

// legge una linea dell'immagine (fino al \n)
string read_all_line(fstream& imm){
	string linea;
	char buf;

	do{
		imm.get(buf);
 
		if(imm.fail() || buf=='\n'){
			break;
		}
		linea+=buf;
	}while(true);

	return linea; 
}

// legge fino a un delimitatore (delimitatori diversi: anche \t o ' ')
string read_line(fstream& imm){
	string linea;
	char buf;

	do{
		imm.get(buf);
 
		if(imm.fail() || buf=='\n' || buf==' ' || buf=='\t'){
			break;
		}
		linea+=buf;
	}while(true);

	return linea; 
}

// legge una linea dell'immagine, se è un commento legge un'altra linea
// continua finchè non trova una linea che non è un commento
string read_line_no_comment(fstream& imm){
	string linea;

	do{
		linea=read_all_line(imm);
   
		if(linea[0]!='#')
			break;
	}
	while(true);

	return linea; 
}

// legge un file ppm produce un oggetto image
bool read_ppm(const string& nome,  image<unsigned long>& ima){
	fstream file(nome.c_str(), fstream::binary | fstream::in);
	if(!file)
		return false;

	if(read_line(file)!="P6"){
		cout<<"il file non è una immagine PPM o è sbagliata la codifica del magic number"<<endl;
		return false;
	}

	string riga;
	string w, h;
	byte bytepixel=3; //numero di byte per pixel
	unsigned int colors;
	int larg, alt;
	char buf;

	riga=read_line_no_comment(file);  

	w= riga.substr(0, riga.find(' '));
	h= riga.substr(riga.find(' ')+1, riga.size());
	larg=stringToInt(w);   
	alt=stringToInt(h);
	colors=stringToInt(read_line(file));
	if(colors>255)
		bytepixel=6;

	ima.resize(larg,alt);
	ima.set_colors(colors);

	//segue tutto l'elenco dei pixel
	for(unsigned j=0; j<ima.height(); ++j)
		for(unsigned i=0; i<ima.width(); ++i)
			if(!file.read((char *) &ima(i,j),bytepixel))  
				return false;

	return true;
}

// salva un oggetto image in un file ppm
bool save_ppm(const string& nome, image<unsigned long>& imm){
	fstream file(nome.c_str(), fstream::binary | fstream::out); 
	if(!file)
		return false;

	byte num_bytes=3;
	if(imm.colors()>255)
		num_bytes=6;

	file <<"P6"<<endl;
	file<<"# immagine ppm"<<endl;
	file<<imm.width() <<" "<<imm.height() <<endl;
	file<<imm.colors()<<endl;

	for(int j=0; j<imm.height(); ++j)
		for(int i=0; i<imm.width() ; ++i)
			write(file, imm(i,j), num_bytes);   

	file.clear();
	file.close();
	return true;
}