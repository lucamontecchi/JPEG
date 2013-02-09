#ifndef bitwriter_h  // serve per fare in modo che venga compilata una sola volta se non è già stata definita
#define bitwriter_h

#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
using namespace std;

typedef unsigned char byte;

class bitwriter{
  unsigned short bitwriter_scritti;
  byte bitwriter_buf;
  fstream fstream_bitwriter;   // fstream del bitwriter
  bool created;

//salva il nome del file salvato
void set_file(const string& file, bool esiste=false){
  fstream_bitwriter.clear();
  // se dichiaro che non esiste creo il file in modo da sovrascrivere eventualmente l'altro
  if(!esiste)
   {  fstream_bitwriter.open(file.c_str(), fstream::binary | fstream::out );
       if(fstream_bitwriter.is_open())
         created=true;
   }
  else
    { fstream_bitwriter.open(file.c_str(), fstream::binary | fstream::out | fstream::app);
       if(fstream_bitwriter.is_open())
         created=true;
    }
   }

public:

  bitwriter(const string& file,  bool esiste=false){
    created=false;
    bitwriter_scritti=0;
    bitwriter_buf=0;
    set_file(file,esiste);
  }

  bitwriter(const char* file,  bool esiste=false){
    created=false;
    bitwriter_scritti=0;
    bitwriter_buf=0;
  // se dichiaro che non esiste creo il file in modo da sovrascrivere eventualmente l'altro
  if(!esiste)
   {  fstream_bitwriter.open(file, fstream::binary | fstream::out );
       if(fstream_bitwriter.is_open())
         created=true;
   }
  else
    { fstream_bitwriter.open(file, fstream::binary | fstream::out | fstream::app);
       if(fstream_bitwriter.is_open())
         created=true;
    }
  }

  ~bitwriter(){}

//verifica se aperto in modo corretto
   bool opened(){
     return created;
   }

// calcola il numero di bit minimo necessario per un intero positivo
int bit_int(unsigned int val){
   // devo calcolare il numero di bit
  short i;

   for(i=(sizeof(unsigned int)*8)-1; i>=0 ;)
   {
    /*
     cout<<"------------------------------"<<endl;
     cout<<i<<endl;
     cout<<(val&(1<<i))<<endl;
    */
      if(val & (1<<i))
      break;
     i--;
   }
  return i+1;
}


void printBinary(const unsigned char val) {
   
   for(int i = 7; i >= 0; i--)
          if(val & (1 << i))
                    cout << "1";
                else
                    cout << "0";
   cout<<endl;
}

unsigned short byte_staff()
{ unsigned short nb;

  if(bitwriter_scritti>0){
    nb=(8-bitwriter_scritti);
    bitwriter_buf=bitwriter_buf<<nb;
    bitwriter_buf=bitwriter_buf>>nb; //questi forse vanno invertiti perchè mi mette uno zero prima
    bitwriter_scritti=0;
  }
  return nb;
}

bool write_8()
{
 // cout<<(int) bitwriter_buf<<endl;  

  if(bitwriter_scritti!=0){
  if(bitwriter_scritti<8)
  byte_staff();

  bitwriter_scritti=0;

  fstream_bitwriter.put(bitwriter_buf);
  //  cout<<(int) bitwriter_buf<<endl;  
  // printBinary(bitwriter_buf);
  }
//file.close();  // RICORDARSI DI CHIUDERE LO STREAM FILE
return 0;
}

int write_8Golomb()
{
  // cout<<bitwriter_nome_file<<endl;
  //cout<<(int) bitwriter_buf<<endl;  

  if((bitwriter_scritti!=0))
  {  
     if(bitwriter_scritti<8)
     for(byte i=8-bitwriter_scritti; i>0;i-- ){
        //cout<<"bitstaff "<<(int) bitwriter_buf<<endl;  
        bitwriter_buf=bitwriter_buf<<1;
      }
  bitwriter_scritti=0;

  fstream_bitwriter.put(bitwriter_buf);
  }
 
  // printBinary(bitwriter_buf);

return 0;
}

bool write_bit( unsigned short val)
{
   //cout<<(int) bitwriter_buf<<endl;  
  // printBinary(bitwriter_buf);
   
if(val>1)
return false;

   // ora devo inserire il bit relativo nell'ultima posizione 
   // faccio lo xor tra il buf e il char passato (l'ultima posizione è ora uno 0 facendo lo xor se il bit passato è un uno ho 1 in ultima posizione)
   bitwriter_buf= bitwriter_buf<<1; // si effettua lo shift a sinistra di uno del char
                // shifts left and adds zeros at the right end. 

   bitwriter_buf=bitwriter_buf^val;
   bitwriter_scritti++;
   //cout<<"bitwriter_scritti: "<<bitwriter_scritti<<endl;
   // cout<<(int) bitwriter_buf<<endl;  
    //printBinary(bitwriter_buf);

   if(bitwriter_scritti==8)
   {  write_8();
   }

  return true;
}

bool write_bit(byte& val)
{
  if(val>1)
   return false;

   bitwriter_buf= bitwriter_buf<<1; // si effettua lo shift a sinistra di uno del char 

   bitwriter_buf=bitwriter_buf^val;
   bitwriter_scritti++;

   if(bitwriter_scritti==8)
   {  write_8();
   }

  return true;
}

bool write_nbit(int val, unsigned short num)
{
   for(int i=num-1; i>=0; i--){
          if(val & (1 << i)){
                    if(!write_bit(1))
                        return false;}
                    else{
                      if(!write_bit(0))
                        return false;}
      }
  return true;
}

bool write_nbit(unsigned& val, unsigned short& num)
{
   for(int i=num-1; i>=0; i--){
          if(val & (1 << i)){
                    if(!write_bit(1))
                        return false;}
                    else{
                      if(!write_bit(0))
                        return false;}
      }
  return true;
}

bool write_nbit(byte& val, unsigned short num)
{
   for(int i=num-1; i >= 0; i--){
          if(val & (1<<i)){
                    if(!write_bit(1))
                        return false;}
                    else{
                      if(!write_bit(0))
                        return false;}
      }
  return true;
}

// questo scrive il numero passato utilizzando num bit
/*
int write(unsigned u, unsigned short num)
{
 		while (num>0)
			write_bit(u>>--num);
 return 0;
}
*/

int write_char(char& car){

   for(short i = 7; i >= 0; i--)
          if(car & (1 << i))
                    write_bit(1);
                else
                    write_bit(0);

return 0;
}

bool write_byte(byte& b){
   for(short i = 7; i >= 0; i--)
          if(b & (1 << i)){ // cout<<"1";
                    write_bit(1);}
                else{ // cout<<"0";
                    write_bit(0);}
//cout<<endl;
return true;
}

//scrive direttamente il byte senza curasi dei bit
bool write_byted(byte& b){
fstream_bitwriter.put(b);
return true;
}

bool write_byte(unsigned short b){

if(b>255)
return false;

   for(int i = 7; i >= 0; i--)
          if(b & (1 << i))
                    write_bit(1);
                else
                    write_bit(0);

return true;
}

// scrive 2 byte little endian
bool put2_led(unsigned short &s){
   byte b,b2;

     b=s>>8;//primo byte
     b2=s;

      if(!write_byted(b2))
         return false;
      if(!write_byted(b))
         return false;
   return true;
}

// scrive 4 byte little endian
bool put4_led(unsigned int &s){
  unsigned short b,b2;

     b=(unsigned short)s>>16;//primi 16
     b2=(unsigned short)s;
      if(!put2_led(b2))
         return false;
      if(!put2_led(b))
         return false;
   return true;
}

// scrive 4 byte little endian
bool put4_led(int &s){
  unsigned short b,b2;

     b=(unsigned short)s>>16;//primi 16
     b2=(unsigned short)s;
      if(!put2_led(b2))
         return false;
      if(!put2_led(b))
         return false;
   return true;
}

int write_int(int& b){

   for(int i = (sizeof(int)*8)-1; i >= 0; i--)
       {   if(b & (1 << i))
                    write_bit(1);
                else
                    write_bit(0);
       }
return 0;
}

int write_int(int b){

   for(int i = (sizeof(int)*8)-1; i >= 0; i--)
       {   if(b & (1 << i))
                    write_bit(1);
                else
                    write_bit(0);
       }
return 0;
}
int write_unsigned(unsigned short int& b){

  // printBinary(b);
  //cout<<(sizeof(unsigned short)*8)-1<<endl;

   for(int i = (sizeof(unsigned short)*8)-1; i >= 0; i--)
     { /// cout<<i<<endl;
       //cout<<(b & (1 << i))<<endl;
           if(b & (1 << i)){
                    write_bit(1);
                    }
                else
                    write_bit(0);
     }
return 0;
}

int write_long(unsigned long &b){
   for(int i = (sizeof(unsigned long)*8)-1; i >= 0; i--)
     { /// cout<<i<<endl;
       // cout<<(b & (1 << i))<<endl;
           if(b & (1 << i)){
                    write_bit(1);cout<<"long write 1"<<endl; 
                    }
                else
                    write_bit(0);cout<<"long write 0"<<endl; 
     }
return 0;
}

bool write4(unsigned long &b){
   for(int i = 31; i >= 0; i--)
     { /// cout<<i<<endl;
       // cout<<(b & (1 << i))<<endl;
           if(b & (1 << i)){
                    write_bit(1);
                    }
                else
                    write_bit(0); 
     }
return true;
}

// scrive i 2 byte finali così se lo short è superiore a 2 byte io cmq ne uso 2
int write_unsigned2(unsigned short int& b){
   for(int i=15; i >= 0; i--)
     { /// cout<<i<<endl;
       //cout<<(b & (1 << i))<<endl;
           if(b & (1 << i))
                    write_bit(1);
                else
                    write_bit(0);
     }
return 0;
}


int write_EOF(){
fstream_bitwriter.put(EOF);
} 

bool writed(char* c, int l){
  fstream_bitwriter.write (c,l);
if(fstream_bitwriter.fail())
  return false;//dovrei anche chiudere lo stream?
return true;
}

//scrive nb bit meno significativi di val
bool write(unsigned int val, unsigned int nb){
   for(int i=nb-1; i >= 0; i--)
     {          if(val & (1 << i))
                    write_bit(1);
                else
                    write_bit(0);
     }
   return true;
}

// scrive una stringa di uni e zero zu file
int write_bitstring(const string& code){
   for(unsigned short i=0; i<code.length(); i++)
    {
      switch(code[i]){
       case '0' : write_bit(0); //cout<<"write 0"<<endl;  //48
                break; 
       case '1' : write_bit(1); //cout<<"write 1"<<endl;  //49
                break;
       default : //cout<<"write nothing"<<endl;
                 return false;
       } 
         
    }
}

void close(){
    write_8();
    fstream_bitwriter.clear();
    fstream_bitwriter.close();
    bitwriter_buf=NULL;
    bitwriter_scritti=NULL;
 }
void closeGolomb(){
    write_8Golomb();//uso questo per avere i bit aggiunti dopo la parte che mi interessa
    fstream_bitwriter.clear();
    fstream_bitwriter.close();
    bitwriter_buf=NULL;
    bitwriter_scritti=NULL;
 }
};//fine bitwriter
#endif
