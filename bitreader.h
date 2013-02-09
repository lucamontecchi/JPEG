#ifndef bitreader_h
#define bitreader_h

#include <iostream>
#include <fstream>
#include <string>
using namespace std;
typedef unsigned char byte;


class bitreader{

   byte bitreader_buf;          // un byte che serve come buffer quando non si completa la lettura di un byte
   byte bitreader_inbuf;        // numero di bit rimasti dal byte precedente
   fstream fstream_bitreader;   // fstream del bitreader
   //inserisco un parametro per controllare se l'apertura del file è andata a buon fine
   bool open;

 bool set_file(const string& read){
  fstream_bitreader.clear();
   // verifica che si riesca a aprire
   fstream_bitreader.open(read.c_str(), fstream::in | fstream::binary);
   if(!fstream_bitreader.is_open())
    return false;
   bitreader_inbuf=0;//num bit già nel buffer
   bitreader_buf=0;  //byte buffer
  return true;
 }

 bool set_file(const char* read){
  fstream_bitreader.clear();
   // verifica che si riesca a aprire
   fstream_bitreader.open(read, fstream::in | fstream::binary);
   if(!fstream_bitreader.is_open())
    return false;
   bitreader_inbuf=0;//num bit già nel buffer
   bitreader_buf=0;  //byte buffer
  return true;
 }

public:
  bitreader (const string& file): open(false){if(set_file(file)){// cout<<"apertura file riuscita"<<endl;
                                                                    open=true;}}
  bitreader (const char* file): open(false){if(set_file(file)){  //cout<<"apertura file riuscita"<<endl;
                                                                    open=true;}}
  ~bitreader(){close();}
  
  //indica se il file è stato aperto correttamente
  bool opened(){
  // cout<<"aperto: "<<(bool)open<<endl;
   return open;
  }

byte bitreader_read_byte(){
  byte b=0;

  if(!fstream_bitreader.good())
  return NULL;

  if(bitreader_inbuf!=0) //ho dei bit da recuperare nel buffer
    { 
           bitreader_buf=bitreader_buf<<(8-bitreader_inbuf);
           b=bitreader_buf;
           bitreader_buf=fstream_bitreader.get(); //leggo un byte e verifico che non sia l'EOF

          // prendo il num di bit che mi erano rimasti, il numero di bit ancora buoni nel buffer non è variato
          for(unsigned short i=7; i>=bitreader_inbuf; i-- ){ 
            if(bitreader_buf & (1<<i))
            b+=(1<<i-bitreader_inbuf); 
          }
          //cout<<"byte letto: "<<(int)b<<endl;
      return b;
    }
   else{
      b=fstream_bitreader.get();
      //cout<<"byte letto: "<<(int)b<<endl;
       if(b==EOF)  
            return EOF;
      return b;
     }
  }

byte read_bit(){//se sono arrivato alla fine del file restituisco un 2-- così so che non va bene
  if(bitreader_inbuf!=0)
   { bitreader_inbuf--;
     if(bitreader_buf & (1<<bitreader_inbuf)) {//cout<<"read 1 buff"<<endl;
            return 1; }
        else          {                        //cout<<"read 0 buff"<<endl;
            return 0; }
   }
   else 
     {   if(!fstream_bitreader.good())
            return 2;//restituisco 2 se non niente da leggere
        bitreader_inbuf=7;
        bitreader_buf=fstream_bitreader.get();
        if(bitreader_buf & (1<<7)) {           // cout<<"read 1"<<endl;
            return 1; }
        else          {                        // cout<<"read 0"<<endl;
            return 0; }
     }
}

//funzioni peek
int peek(){
   return fstream_bitreader.peek();
 }


unsigned read_nbit(unsigned short num){
unsigned u=0;
   while(num>0)
     u|= read_bit()<<--num;
   return u;
 }

bool read_nbit(unsigned& val, unsigned short num){
byte b;
val=0;
   while(num>0){
     b=read_bit();
     if(b>1)
       return false;
     val|=b <<--num;
   }
   return true;
 }


bool read_nbit(unsigned short& val, unsigned short num){
byte b;
val=0;
   while(num>0){
     b=read_bit();
     if(b>1)
       return false;
     val|=b <<--num;
   }
   return true;
 }

bool read_nbit(byte& val, unsigned short num){
byte b;
val=0;
   while(num>0){
     b=read_bit();
     if(b>1)
       return false;
     val|=b <<--num;
   }
   return true;
 }

unsigned long bitreader_read_long(){
 unsigned long l=0;
 // se non ci sono bit nel buffer leggo un long
 if(bitreader_inbuf==0){
  for(unsigned short j=0; j<sizeof(unsigned long) ;j++)
   {    if(!fstream_bitreader.good())
            return NULL;
          l=l<<8;
          l=l^(fstream_bitreader.get());
          //cout<<"l "<<l<<endl;          
   }
  }
  else{// devo leggere un num di byte pari a quelli che compongono un long
       // quindi leggo un byte alla volta con la funzione sopra definita un numero di volte pari a quello richiesto
       // il numero di bit ancora buoni nel buffer non cambia
       for(unsigned short j=0; j<sizeof(unsigned long) ;j++){ 
          if(!fstream_bitreader.good())
            return NULL;
          l=l<<8;
          l=l^(bitreader_read_byte());
       }
  }
 return l;
}

// legge 2 byte e restituisce uno short
unsigned short bitreader_read_2(){
 unsigned short l=0;
       // quindi leggo un byte alla volta con la funzione sopra definita un numero di volte pari a quello richiesto
       // il numero di bit ancora buoni nel buffer non cambia
       for(byte j=0; j<2; j++){ 
          if(!fstream_bitreader.good())
            return NULL;
          l=l<<8;
          l=l^(bitreader_read_byte());
       }
 return l;
}

// legge 4 byte e restituisce un int
unsigned int bitreader_read4(){
 unsigned int l=0;
       // quindi leggo un byte alla volta con la funzione sopra definita un numero di volte pari a quello richiesto
       // il numero di bit ancora buoni nel buffer non cambia
       for(byte j=0; j<2; j++){ 
          if(!fstream_bitreader.good())
            return NULL;
          l=l<<8;
          l=l^(bitreader_read_2());
       }
 return l;
}

// legge un byte 
bool getbyte(byte &b){

  if(!fstream_bitreader.good())
  return false;

  if(bitreader_inbuf!=0) //ho dei bit da recuperare nel buffer
    { 
           bitreader_buf=bitreader_buf<<(8-bitreader_inbuf);
           b=bitreader_buf;
           bitreader_buf=fstream_bitreader.get(); //leggo un byte e verifico che non sia l'EOF
           if(fstream_bitreader.fail() || fstream_bitreader.eof())
             return false;
          // prendo il num di bit che mi erano rimasti, il numero di bit ancora buoni nel buffer non è variato
          for(unsigned short i=7; i>=bitreader_inbuf; i-- ){ 
            if(bitreader_buf & (1<<i))
            b+=(1<<i-bitreader_inbuf); 
          }
          //cout<<"byte letto: "<<(int)b<<endl;
      return true;
    }
   else{
      b=fstream_bitreader.get();
      //cout<<"byte letto: "<<(int)b<<endl;
      if(fstream_bitreader.fail() || fstream_bitreader.eof())
            return false;
      return true;
     }
}

// legge 2 byte
bool get2(unsigned short &l){
byte b,b2;
      if(!getbyte(b))
         return false;
      
      if(!getbyte(b2))
         return false;

         l=b2;
         l=l<<8;
         l=l|b;
 return true;
}
// legge 4 byte
bool get4(int &l){
l=0;
 unsigned short b,b2;
      if(!get2(b))
         return false;
      
      if(!get2(b2))
         return false;

         l=b2;
         l=l<<16;
         l=l|b;
 return true;
}
// legge 4 byte
bool get4(unsigned long &l){
unsigned short b,b2;
l=0;
      if(!get2(b))
         return false;
      
      if(!get2(b2))
         return false;

         l=b2;
         l=l<<16;
         l=l|b;
 return true;
}

// legge 4 byte
bool get4(unsigned int &l){
unsigned short b,b2;
l=0;
      if(!get2(b))
         return false;
      
      if(!get2(b2))
         return false;

         l=b2;
         l=l<<16;
         l=l|b;
 return true;
}

// legge 2 byte big endian
bool get2be(unsigned short &l){
byte b,b2;
      if(!getbyte(b))
         return false;
      
      if(!getbyte(b2))
         return false;

         l=b;
         l=l<<8;
         l=l|b2;
 return true;
}

// legge 4 byte big endian
bool get4be(int &l){
l=0;
 unsigned short b,b2;
      if(!get2be(b))
         return false;
      
      if(!get2be(b2))
         return false;

         l=b;
         l=l<<16;
         l=l|b2;
 return true;
}

// legge 4 byte  big endian
bool get4be(unsigned int &l){
l=0;
 unsigned short b,b2;
      if(!get2be(b))
         return false;
      
      if(!get2be(b2))
         return false;

         l=b;
         l=l<<16;
         l=l|b2;
 return true;
}

// legge 4 byte  big endian
bool get4be(unsigned long &l){
l=0;
 unsigned short b,b2;
      if(!get2be(b))
         return false;
      
      if(!get2be(b2))
         return false;

         l=b;
         l=l<<16;
         l=l|b2;
 return true;
}

//leggne l byte e li inserisce in coda a c
bool read(char* c,streamsize l){
	byte b;
	//se è finito il file o c'è stato un errore setto il dato e poi restituisco file(vuol dire che non è adato bene  e il dato è da ignorare)
	for(unsigned long i=0; i<l; i++){
		if(!getbyte(b))
			return false;
		c[i]=b;
	}
	return true;
}

void close(){
    fstream_bitreader.clear();
    fstream_bitreader.close();
    bitreader_buf=NULL;
    bitreader_inbuf=NULL;
 }
};//fine classe
#endif