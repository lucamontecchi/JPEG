#ifndef image_h  
#define image_h
#include <vector>
#include <string>
#include <fstream>
using namespace std;
typedef unsigned char byte;

template<typename T>
class image {
	unsigned _width,_height,_linewidth, _gray, _colors; //colors per immagini a colori
	T *_ptr;

//legge uno specifico numero di byte nel pixel a partire da una specifica posizione num=numero byte da prendere, pos= posizione da cui si inizia a prenderli (la base) voglio gli ultimi due-> passo 0
unsigned get_pixelbytes(T val, byte num, byte pos){
 if((num+pos)> sizeof(T))
    return 0;

   unsigned b=0;
   short max=pos*8; 
  // cout<<(int)max<<" max"<<endl;
  // cout<<"da "<<(int)(8*(num+pos)-1)<<endl;

   for(short j=8*(num+pos)-1; j>=max; j--)
       {  if(val &(1<<j))
            b=(b<<1)+1;
           else
            b=(b<<1);
       }
return b;
}


//-----------scrive gli ultimi n byte di un unsigned long
bool write(fstream& f, T val, byte num){
byte b;
byte max;

  for(byte i=1; i<=num; i++)
  {  b=0; 
     max= (i-1)*8;

     for(short j=8*i-1; j>=max; j--)
       {  if(val &(1<<j))
            b=(b<<1)+1;
           else
            b=(b<<1);
       }
   f.put(b);
  }

return true;
}
//-----------------------------------------

//-----------------------------------------
public:
	// Costruttore vuoto
	image() : _width(0),_height(0),_linewidth(0),_ptr(0),_gray(255),_colors(0){}
	// Costruttore con dimensioni
	image (unsigned w, unsigned h) : _ptr(0),_gray(255),_colors(0){
		resize (w,h);
	}
	// Costruttore di copia
	image (const image &img) : _ptr(0) {
		resize (img._width, img._height);
		for (unsigned i=0; i<_linewidth*_height; i++)
			_ptr[i] = img._ptr[i];
                _gray=img.gray();
                _colors=img.colors();
	}
	// Operatore di assegnamento
	image &operator= (const image &img) {
		if (this==&img)
			return *this;
		resize (img._width,img._height);
		for (unsigned i=0; i<_linewidth*_height; i++)
			_ptr[i] = img._ptr[i];
                _gray=img.gray();
                _colors=img.colors();
		return *this; 
	}

	~image() { 
		delete _ptr;
	}

	void resize (unsigned w, unsigned h) {
		delete _ptr;
		_width = w;
		_height = h;
		_linewidth = ((w+3)>>2)<<2;   //setta / verifica che la larghezza sia multiplo di 8 
		_ptr = new T[_linewidth*_height];
	}

	T& operator() (unsigned x, unsigned y) {
		return _ptr[y*_linewidth+x];
	}

	unsigned width() const { return _width; }
	unsigned height() const { return _height; }
	unsigned linewidth() const { return _linewidth; }
        unsigned gray() const {return _gray;}
        void set_gray(unsigned val){_gray=val;}
        unsigned colors() const {return _colors;}
        void set_colors(unsigned val){_colors=val;}

	T *ptr() { return _ptr; }
	T *lineptr (unsigned y) { return _ptr+_linewidth*y; }

//---------------------------------------istogramma---------------------------------------------------
//-------------------------------------------------------------------------
// crea file istogramma
bool ist(string fileNome="ist.txt"){
 unsigned el;
  if(_colors>0)
     el=_colors;
  else
     el=_gray;

  vector <unsigned> ist(el);

        fill(ist.begin(),ist.end(),0);

	for (unsigned y=0; y<_height; ++y) {
		for (unsigned x=0; x<_width; ++x) {
			ist[_ptr[y*_linewidth+x]]++;}
         }

	fstream fileIst(fileNome.c_str(), fstream::out);
        if(!fileIst)
           return false;
	for (unsigned i=0;i<ist.size();++i)
		fileIst << i << "\t" << ist[i] << endl;
        fileIst.clear();
        fileIst.close();
return true;
}
//-------------------------------------------------------------------------

//---------------------------------------occorrenze di un valore specifico----------------------------
unsigned occ(const int& val){
  unsigned occo=0;
 	for (unsigned y=0; y<_height; ++y) {
		for (unsigned x=0; x<_width; ++x) {
			if(_ptr[y*_linewidth+x]==val)
                          occo++;}
         }
  return occo;
}

//specchia l'immagine rispetto la larghezza
image<T> specchia_w(){
  int base=_width-1;
  image<T> tmp;
  tmp.resize(_width,_height);
  tmp.set_colors(_colors);

     	for (unsigned y=0; y<_height; ++y) {
		for (unsigned x=0; x<_width; ++x) {
                    tmp(base-x,y)=_ptr[y*_linewidth+x];        
          }}
  return tmp;
} 

//specchia l'immagine rispetto l'altezza
image<T> specchia_h(){
  int base=_height-1;
  image<T> tmp;
  tmp.resize(_width,_height);
  tmp.set_colors(_colors);

     	for (unsigned y=0; y<_height; ++y) {
		for (unsigned x=0; x<_width; ++x) {
                    tmp(x,base-y)=_ptr[y*_linewidth+x];        
          }}
  return tmp;
}

//specchia l'immagine rispetto l'altezza e larghezza
image<T> specchia(){
  int base_h=_height-1;
  int base_w=_width-1;
  image<T> tmp;
  tmp.resize(_width,_height);
  tmp.set_colors(_colors);

     	for (unsigned y=0; y<_height; ++y) {
		for (unsigned x=0; x<_width; ++x) {
                    tmp(base_w-x,base_h-y)=_ptr[y*_linewidth+x];        
          }}
  return tmp;
}

//salva l'immagine (i suoi pixel) in un formato "grezzo"
// nome del file su cui salvare, numero di byte per pixel
bool save_raw(const string& nome, unsigned short& num_byte){
fstream file(nome.c_str(), fstream::binary | fstream::out); 
   if(!file)
     return false;
  //per non sbagliare scrivo larghezza, altezza e colori
  file<<_width<<" "<<_height <<endl;
  file<<_colors<<endl;

   for(unsigned int h=0; h<_height; h++){
   for(unsigned int w=0; w<_width; w++)
   {  write(file, _ptr[h*_linewidth+w], num_byte);   
   }
     ///file<<endl;
   }
   file.clear();
   file.close();
return true;
}

//converte da RGB a YCbCr
// prende ogni pixel di una immagine (3 o 6 byte) in rgb e lo passa in YCbCr
bool rgb_ycbcr(unsigned short num_byte){ //numero di byte per pixel
   short r, g, b;
   short y, cb, cr;
   byte per_color=num_byte/3;    
   short max=255;
     if(num_byte>3) 
        max=65535;

   for(unsigned int h=0; h<_height; h++)
   for(unsigned int w=0; w<_width; w++)
   { 
     b=get_pixelbytes( _ptr[h*_linewidth+w], per_color, 0);    
     g=get_pixelbytes( _ptr[h*_linewidth+w], per_color, per_color);
     r=get_pixelbytes( _ptr[h*_linewidth+w], per_color, per_color*2);
  
     y = 0.299*r + 0.587*g + 0.114*b ;
     cb= -0.1687*r -0.3313*g + 0.5*b +128;
     cr= 0.5*r - 0.4187*g -0.0813*b +128;

     if(y>max) y=max; else if(y<0) y=0; if(cb>max) cb=max; else if(cb<0) cb=0; if(cr>max) cr=max; else if(cr<0) cr=0;
     //cout<<y<<" "<<cb<<" "<<cr<<" "<<endl;
      _ptr[h*_linewidth+w]=y; 
      _ptr[h*_linewidth+w]=(( _ptr[h*_linewidth+w]<<(8*per_color))|cb);
      _ptr[h*_linewidth+w]=(( _ptr[h*_linewidth+w]<<(8*per_color))|cr);
 
   }
   
   //save_raw("ikji", num_byte);
   return true;
}

//converte da YCbCr a RGB 
bool ycbcr_rgb(unsigned short& num_byte){ //numero di byte per pixel
    short r, g, b;
    short y, cb, cr;
   byte per_color=num_byte/3;    
    short max=255;
     if(num_byte>3) 
        max=65535;

   for(unsigned int h=0; h<_height; h++)
   for(unsigned int w=0; w<_width; w++)
   { cr=get_pixelbytes( _ptr[h*_linewidth+w], per_color, 0);    
     cb=get_pixelbytes( _ptr[h*_linewidth+w], per_color, per_color);
     y=get_pixelbytes( _ptr[h*_linewidth+w], per_color, per_color*2);
     
     r= y +1.402*(cr-128);
     g= y -0.34414 *(cb-128)-0.71414*(cr-128);
     b= y+ 1.772*(cb-128);

     if(r>max) r=max; else if(r<0) r=0; if(g>max) g=max; else if(g<0) g=0; if(b>max) b=max; else if(b<0) b=0;

      _ptr[h*_linewidth+w]=r;   _ptr[h*_linewidth+w]=(( _ptr[h*_linewidth+w]<<(8*per_color))|g);  _ptr[h*_linewidth+w]=(( _ptr[h*_linewidth+w]<<(8*per_color))|b);
   }
   return true;
}


//num= valore di sottocampionamento  pixDim dimensione del pixel in byte
bool sottocampionare(const string& nome_y,const string& nome_cb,const string& nome_cr, int num, unsigned short pixDim){
   if(pixDim>sizeof(T))
   return false;

fstream filey(nome_y.c_str(), fstream::binary | fstream::out); 
   if(!filey)
     return false;
fstream filecb(nome_cb.c_str(), fstream::binary | fstream::out); 
   if(!filecb)
     return false;
fstream filecr(nome_cr.c_str(), fstream::binary | fstream::out); 
   if(!filecr)
     return false;
   
   unsigned short dim_cam=pixDim/3; //la dimensione di ogni campo è un terzo di quella del pixel  
   unsigned alt=_height;
   if((_height%num) !=0)//se l'altezza non è multipla di num
          alt=_height-(num-1);

   unsigned maxl, maxa;
   unsigned cr=0, cb=0, y;
   unsigned num2=num*num;

  //scrivo file y
  for(unsigned i=0; i<_height; i++){
    for(unsigned j=0; j<_width; j++){
             y=get_pixelbytes(_ptr[i*_linewidth+j], dim_cam, 2*dim_cam);//recupero il valore di y
             write(filey, y, dim_cam);
         }
       //filey<<endl;
    }

  //sottocampiono e scrivo il resto
  for(unsigned i=0; i<alt; i=i+num){
          maxa=i+num;
    for(unsigned j=0; j<_width; j=j+num){
          maxl=j+num;  
          for(unsigned e=i; e<maxa; e++)
          for(unsigned d=j; d<maxl; d++){       
             //cr e cb devo sommare i valori dei vari pixel  
              if(d<_width){
               cr+=get_pixelbytes(_ptr[e*_linewidth+d], dim_cam, dim_cam);
               cb+=get_pixelbytes(_ptr[e*_linewidth+d], dim_cam, 0);
              }
          }

          if(j+num>_width){
           cr/=(num*(_width-j)); cb/=(num*(_width-j)); }
          else
          {cr/=num2; cb/=num2;} //divido per il numero di pixel del quadrato
                            write(filecr, cr, dim_cam);  
                            write(filecb, cb, dim_cam);  
          cr=0; cb=0;
    }
     //filecr<<endl; filecb<<endl;
   }


  //se altezza <_height devo fare l'ultima riga devo aggiungere i valori alla fine
  if(alt<_height) 
   {  maxa=alt+num;

       for(unsigned j=0; j<_width; j+num){
          maxl=j+num; 
         for(unsigned e=alt; e<maxa; e++)
          for(unsigned d=j; d<maxl; d++){      
             //cr e cb devo sommare i valori dei vari pixel  
              if(d<_width){
               cr+=get_pixelbytes(_ptr[e*_linewidth+d], dim_cam, dim_cam);
               cb+=get_pixelbytes(_ptr[e*_linewidth+d], dim_cam, 0);
              }
          }
          if(j+num>_width){
           cr/=((_height-alt)*(_width-j)); cb/=((_height-alt)*(_width-j)); }
          else
          {cr/=(_height-alt)*num; cb/=(_height-alt)*num;} //divido per il numero di pixel del quadrato
                            write(filecr, cr, dim_cam);  
                            write(filecb, cb, dim_cam);  
          cr=0; cb=0;
    }
    // filecr<<endl; filecb<<endl;
   }    
   return true;
}

bool sottocampionare(int cb_arr[1000][1000], int cr_arr[1000][1000], int num){

   unsigned short dim_cam=1; //la dimensione di ogni campo è un terzo di quella del pixel  
   unsigned alt=_height;
   if((_height%num) !=0)//se l'altezza non è multipla di num
          alt=_height-(num-1);

   unsigned maxl, maxa;
   unsigned cr=0, cb=0, y;
   unsigned num2=num*num;

  //sottocampiono e scrivo il resto
  for(unsigned i=0; i<alt; i=i+num){
          maxa=i+num;
    for(unsigned j=0; j<_width; j=j+num){
          maxl=j+num;  
          for(unsigned e=i; e<maxa; e++)
          for(unsigned d=j; d<maxl; d++){       
             //cr e cb devo sommare i valori dei vari pixel  
              if(d<_width){
               cr+=get_pixelbytes(_ptr[e*_linewidth+d], dim_cam, dim_cam);
               cb+=get_pixelbytes(_ptr[e*_linewidth+d], dim_cam, 0);
              }
          }

          if(j+num>_width){
           cr/=(num*(_width-j)); cb/=(num*(_width-j)); }
          else
          {cr/=num2; cb/=num2;} //divido per il numero di pixel del quadrato
		  cr_arr[i/2][j/2] = cr;
		  cb_arr[i/2][j/2] = cb;
          cr=0; cb=0;
    }
   }


  //se altezza <_height devo fare l'ultima riga devo aggiungere i valori alla fine
  if(alt<_height) 
   {  maxa=alt+num;

       for(unsigned j=0; j<_width; j+num){
          maxl=j+num; 
         for(unsigned e=alt; e<maxa; e++)
          for(unsigned d=j; d<maxl; d++){      
             //cr e cb devo sommare i valori dei vari pixel  
              if(d<_width){
               cr+=get_pixelbytes(_ptr[e*_linewidth+d], dim_cam, dim_cam);
               cb+=get_pixelbytes(_ptr[e*_linewidth+d], dim_cam, 0);
              }
          }
          if(j+num>_width){
           cr/=((_height-alt)*(_width-j)); cb/=((_height-alt)*(_width-j)); }
          else
          {cr/=(_height-alt)*num; cb/=(_height-alt)*num;} //divido per il numero di pixel del quadrato
		  cr_arr[alt/2][j/2] = cr;
		  cb_arr[alt/2][j/2] = cb;
          cr=0; cb=0;
    }
    // filecr<<endl; filecb<<endl;
   }    
   return true;
}

//divide le componenti rgb dell'immagine
bool divide_rgb(const string& nome_r,const string& nome_g,const string& nome_b, unsigned short pixDim){
   if(pixDim>sizeof(T))
   return false;

fstream filer(nome_r.c_str(), fstream::binary | fstream::out); 
   if(!filer)
     return false;
fstream fileg(nome_g.c_str(), fstream::binary | fstream::out); 
   if(!fileg)
     return false;
fstream fileb(nome_b.c_str(), fstream::binary | fstream::out); 
   if(!fileb)
     return false;
  
    unsigned short dim_cam=pixDim/3; //la dimensione di ogni campo è un terzo di quella del pixel  

    for(unsigned h=0; h<_height; h++){
      for(unsigned w=0; w<_width; w++){
                  write(filer, get_pixelbytes(_ptr[h*_linewidth+w], dim_cam, 2*dim_cam), dim_cam);
                  write(fileg, get_pixelbytes(_ptr[h*_linewidth+w], dim_cam, dim_cam) , dim_cam);  
                  write(fileb, get_pixelbytes(_ptr[h*_linewidth+w], dim_cam, 0) , dim_cam);           
      }}

 return true;
}

unsigned get_red(unsigned h, unsigned w){
	return get_pixelbytes(_ptr[h*_linewidth+w], 1, 2);
}

unsigned get_green(unsigned h, unsigned w){
	return get_pixelbytes(_ptr[h*_linewidth+w], 1, 1);
}

unsigned get_blu(unsigned h, unsigned w){
	return get_pixelbytes(_ptr[h*_linewidth+w], 1, 0);
}

//ricostruisce l'immagine a partire da file che contengono le varie parti  num= valore del sottocampionamento, pixDim=numbyte per pixel
bool reconstr_ycbcr(const string& nome_y, const string& nome_cb,const string& nome_cr, int num, unsigned short pixDim){
  if(pixDim>sizeof(T))
   return false;

fstream filey(nome_y.c_str(), fstream::binary | fstream::in); 
   if(!filey)
     return false;
fstream filecb(nome_cb.c_str(), fstream::binary | fstream::in); 
   if(!filecb)
     return false;
fstream filecr(nome_cr.c_str(), fstream::binary | fstream::in); 
   if(!filecr)
     return false;

   int cb=0, cr=0, y=0;
   unsigned short dim_cam=pixDim/3; //la dimensione di ogni campo è un quarto di quella del pixel 
   unsigned alt=_height;
   if((_height%num) !=0)//se l'altezza non è multipla di num
          alt=_height-(num-1);

   //ricopio y così com'è
   for(unsigned h=0; h<_height; h++){
      for(unsigned w=0; w<_width; w++){
         filey.read( (char*) &_ptr[h*_linewidth+w], dim_cam);
      }
   }

unsigned j=0, i=0;
unsigned maxl, maxa;

   //devo leggere i valori di cr e cb
   while((filecb.good()) && (filecr.good())){
        filecb.read( (char*) &cb, dim_cam);
        filecr.read( (char*) &cr, dim_cam);

        maxa=i+num;  maxl=j+num;
        if(maxa>_height)    maxa=_height;
        if(maxl>_width) maxl=_width;

        //quindi num*num volte       
        for(unsigned h=i; h<maxa; h++)  
         for(unsigned w=j; w<maxl; w++){  
                    _ptr[h*_linewidth+w]=((_ptr[h*_linewidth+w]<<(dim_cam*8)))| cr;
                    _ptr[h*_linewidth+w]=((_ptr[h*_linewidth+w]<<(dim_cam*8)))| cb;           
         }
         j+=num;
         if(j>=_width){
           i+=num;
           j=0;}
   }
return true;
}

//ricostruisce l'immagine a partire da file che contengono le varie parti pixDim=numbyte per pixel
bool reconstr_rgb(const string& nome_r, const string& nome_g,const string& nome_b, unsigned short pixDim){
  if(pixDim>sizeof(T))
   return false;

fstream filer(nome_r.c_str(), fstream::binary | fstream::in); 
   if(!filer)
     return false;
fstream fileg(nome_g.c_str(), fstream::binary | fstream::in); 
   if(!fileg)
     return false;
fstream fileb(nome_b.c_str(), fstream::binary | fstream::in); 
   if(!fileb)
     return false;

   int b=0, g=0, r=0;
   unsigned short dim_cam=pixDim/3; //la dimensione di ogni campo è un quarto di quella del pixel 

   //ricopio y così com'è
   for(unsigned h=0; h<_height; h++){
      for(unsigned w=0; w<_width; w++){
         filer.read( (char*) &_ptr[h*_linewidth+w], dim_cam);
         fileb.read( (char*) &b, dim_cam);
         fileg.read( (char*) &g, dim_cam);
                      _ptr[h*_linewidth+w]=((_ptr[h*_linewidth+w]<<(dim_cam*8)))| g;
                      _ptr[h*_linewidth+w]=((_ptr[h*_linewidth+w]<<(dim_cam*8)))| b; 
      }
   }

return true;
}
};//fine classe image
#endif
