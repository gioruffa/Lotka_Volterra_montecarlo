//---------------------------------------------------------------------------------------------------
//
//	File name: 	grafico.hpp
//
//	File type:	c++ file
//
//	Authors:		Alberto Dragoni, Paolo Comensoli
//
//	Description:	declaration of class Grafico
//	
//	Version:		0.2
//
//	First version:	12.04.2010
//
//	Last update:	13.04.2010
//	
//	Status:		development
//
//	Copiright:	none
//
//	Detailed 
//	Description:	Tramite il programma GnuPlot crea un grafico e lo salva
//			
//---------------------------------------------------------------------------------------------------

#ifndef GRAFICO__HPP
#define GRAFICO__HPP

#include"fstream"
#include"string"
using std::fstream;
using std::string;

class Grafico{

 private:
	fstream _comandi;
	char *_file;
	int _N_grafici;
	string _immagine;

 public:
	Grafico(string);
	~Grafico();
	
	void Set_title(string );

	void Set_xlabel(string);
	void Set_ylabel(string);
	void Set_labels(string,string);

	void Set_xrange(double,double);
	void Set_yrange(double,double);

	void Add_grafico(double*,double*,int,string);
	
	void Set_data_style(string);
	void Legend_position(int); //0-4

};
#endif
