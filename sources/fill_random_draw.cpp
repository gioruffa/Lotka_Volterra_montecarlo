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

//./main_random.x 3 1000 populations_refresh

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

	//fill
	eco.FillRandom();

	int steps = boost::lexical_cast<int>(argv[2]);
	
	eco.draw_evolv_fast(steps , argv[3]);


	return 0;
}
