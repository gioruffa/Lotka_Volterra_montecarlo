//---------------------------------------------------------------------------------------------------
//
//	File name: 	grafico.hpp
//
//	File type:	c++ file
//
//	Authors:		Alberto Dragoni, Paolo Comensoli
//
//	Description:	function of class Grafico
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
#include"grafico.hpp"
#include"time.h"
#include"stdlib.h"
using namespace std;

Grafico::Grafico(string immagine):_N_grafici(0),_immagine(immagine){

//	system("sleep 2s");
	_file=new char[100];
	time_t seconds;
	seconds= time(NULL);
	
	
	sprintf(_file,"comandi_gnuplot_%d.plt",(int)seconds);

	_comandi.open(_file,ios::out);
	_comandi<<"#!/usr/bin/gnuplot \n";
	_comandi<<"set terminal png\n";
	_comandi<<"set output \""<<_immagine<<"\"\n\n";
}

Grafico::~Grafico(){

	_comandi<<"\nset output \""<<_immagine<<"\"\n";
	_comandi<<"replot\n";
	_comandi.close();
	_comandi.clear();
	
	char comando[200];

	sprintf(comando,"chmod a+x %s; ./%s",_file,_file);
	system(comando);
delete _file;
}

void Grafico::Set_title(string title){
	_comandi<<"set title \""<<title<<"\"\n";
return;
}

void Grafico::Set_xlabel(string label){
	_comandi<<"set xlabel \""<<label<<"\"\n";
return;
}

void Grafico::Set_ylabel(string label){
	_comandi<<"set ylabel \""<<label<<"\"\n";
return;
}

void Grafico::Set_labels(string X,string Y){
	Set_xlabel(X);
	Set_ylabel(Y);
return;
}

void Grafico::Set_xrange(double min,double max){
	_comandi<<"set xrange["<<min<<":"<<max<<"]\n";
return;
}
void Grafico::Set_yrange(double min,double max){
	_comandi<<"set yrange["<<min<<":"<<max<<"]\n";
return;
}


void Grafico::Add_grafico(double* X,double* Y,int N,string titl){
	_N_grafici++;
	fstream dati;
	char file[100];
	
	//Creo il file contenete i dati
	sprintf(file,"%s_%d.dat",_file,_N_grafici);
	dati.open(file,ios::out);
	for(int i=0;i<N;i++)
		dati<<X[i]<<" "<<Y[i]<<endl;
	
	if(_N_grafici>1)_comandi<<"re";
	_comandi<<"plot\""<<file<<"\" title \""<<titl<<"\"\n";
	
return;
}

void Grafico::Set_data_style(string style){
	_comandi<<"set data style "<<style<<endl;
return;
}

void Grafico::Legend_position(int n){ //0-4
	if(n<0 || n>4) return;
	if(n==0) _comandi<<"unset key\n";
	if(n==1) _comandi<<"set key top right\n";
	if(n==2) _comandi<<"set key bottom right\n";
	if(n==3) _comandi<<"set key bottom left\n";
	if(n==4) _comandi<<"set key top left\n";	

return;
}












