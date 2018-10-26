//-----------------------------------------------------------------------
//
//	File Name:	main.cpp
//
//-----------------------------------------------------------------------
//
//	File type:	c++ file
//
//	Author:	Giorgio Ruffa
//
//	Mail:	gioruffa@gmail.com
//
//	Version:		1.0
//
//	Date:	20101217
//
//	Status:		first realise
//
//	Copiright:	none for this moment
//
//-----------------------------------------------------------------------


#include <fstream>
#include "ecosystem.h"
#include "boost/lexical_cast.hpp"

//./fill_stream_draw.x 3 100 populations_refresh

int main(int argc , char **argv)
{
	//enter ecosystem dimension
	unsigned int dimension = 6;
	
	unsigned int species_number = 
		boost::lexical_cast<unsigned int>(argv[1]);
	
	//create the ecosystem
	EcosystemContainer eco(dimension,dimension,true,species_number);

	//initialize using file stream
	std::ifstream in("source.dat");
	eco.initialize(in);
	in.close();
	

//fill from a file
	std::ifstream ifs("source_fill.central.dat");
	eco.fill(ifs);

	int steps = boost::lexical_cast<int>(argv[2]);
	
	eco.draw_evolv_fast(steps , argv[3]);


	return 0;
}
