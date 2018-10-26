//-----------------------------------------------------------------------
//
//	File Name:	disegna.cpp
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

#include "grafico.hpp"
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <sstream>

#include "boost/shared_array.hpp"
#include "boost/lexical_cast.hpp"
#include "boost/tuple/tuple.hpp"
#include "boost/tokenizer.hpp"


int main (int argc , char **argv)
{
	//=============================//
	// open the file 
	//=============================//
	std::string namefile = 
		boost::lexical_cast<std::string>(argv[1]);
	
	std::ifstream ifs(namefile.c_str());


	//=============================//
	//	create the containers of informations
	//=============================//
	typedef boost::tuple <	unsigned int 							,	//0 species
													std::vector<unsigned int>	,	//1 time
													std::vector<unsigned int>	>	//2 populationsa
		data_tp;

	typedef	std::map <unsigned int, data_tp>	
		datas_tp;

	datas_tp datas;


	while (ifs.good())
	{
		std::stringbuf rsbuf;
		std::string			line;

		ifs.get(rsbuf);
		line = rsbuf.str();

		if (*line.begin() == '#') continue;
			
		if (!line.empty())
		{	
			std::string spec_begin("SPECIES_ID");
			size_t found;
			found = line.find(spec_begin);
			if (found!=string::npos)
			{
				size_t dot = line.find(":");
				if (dot!=string::npos)
				{	
					//take substr
					std::string spec_str = 
						line.substr(dot+2);

					unsigned int species_id =
					
					boost::lexical_cast<unsigned int>(spec_str);

					data_tp cash_data;
					cash_data.get<0>() = species_id;
					datas.insert(std::pair <unsigned int , data_tp>
						(species_id , cash_data)
					);

				}
			}
			else continue;

		}
		if (ifs.rdstate() == std::ios::failbit) ifs.clear();
    if (! ifs.good()) break; 
    else
    {
  	 	ifs.get();   // Skip past delimiter (newline). 
	   	ifs.clear(); // Be happy if it doesn't succeed.
  	}
	}
	
	//=============================//
	//	cont formed -> fill
	//=============================//

ifs.close();
ifs.open(namefile.c_str());

	while (ifs.good())
	{
		std::stringbuf rsbuf;
		std::string			line;

		ifs.get(rsbuf);
		line = rsbuf.str();

		if (*line.begin() == '#') continue;
			
		if (!line.empty())
		{
			std::string spec_begin("SPECIES_ID");
			size_t found;
			found = line.find(spec_begin);
			if (found!=string::npos) continue;
			typedef boost::tokenizer<boost::char_separator<char> > 
    		tokenizer_tp;
			boost::char_separator<char> sep("|");

			tokenizer_tp tokenizer(line, sep);
	
			//this is a line whith no information
			if(tokenizer.begin() == tokenizer.end())	return false;
	
	
			typedef std::vector<std::string> tokens_container;
			tokens_container tk_cont;	//contain all tokens separed by "|"

			//we copy the content of the string iside the
			//tk container to better handle it
			std::copy(tokenizer.begin() 					, 
								tokenizer.end() 						, 
								std::back_inserter(tk_cont)	);
			//get species_id
			unsigned int spec_id =
				boost::lexical_cast<unsigned int>(tk_cont[0]);
			
			//get the data
			data_tp &spec_data = datas.find(spec_id)->second;
			
			//get vectors ref
			std::vector<unsigned int>	&pops = spec_data.get<2>();
			std::vector<unsigned int>	&times = spec_data.get<1>();
			
			//get time and pop
			unsigned int time = 
				boost::lexical_cast<unsigned int>(tk_cont[3]);
			unsigned int pop 	= 
				boost::lexical_cast<unsigned int>(tk_cont[2]);

			pops.push_back(pop);
			times.push_back(time);

		}
		if (ifs.rdstate() == std::ios::failbit) ifs.clear();
    if (! ifs.good()) break; 
    else
    {
  	 	ifs.get();   // Skip past delimiter (newline). 
	   	ifs.clear(); // Be happy if it doesn't succeed.
  	}
	}

	//=============================//
	// 	containers filled
	//	create vectors
	//=============================//
	Grafico gra("immagine_" + namefile);
	gra.Set_title(namefile);

	for ( datas_tp::iterator it = datas.begin() ;
				it != datas.end();
				it ++)
	{
		data_tp &data = it->second;
		std::vector<unsigned int>	&pops = data.get<2>();
		std::vector<unsigned int>	&times = data.get<1>();
		boost::shared_array<double> 
			times_arr (new double  [times.size()]);
		boost::shared_array<double> 
			pops_arr (new double [pops.size()]);
		
		//scopiazz
		unsigned int t_pos = 0;
		for (	std::vector<unsigned int>::iterator t_it =
						times.begin();
					t_it != times.end();
					t_it ++)
		{
			times_arr[t_pos] = *t_it;
			t_pos ++;
		}

		unsigned int p_pos = 0;
		for (	std::vector<unsigned int>::iterator p_it =
						pops.begin();
					p_it != pops.end();
					p_it ++)
		{
			pops_arr[p_pos] = *p_it;
			p_pos ++;
		}

		gra.Add_grafico(	times_arr.get()	,
											pops_arr.get()	,
											times.size()		,
											boost::lexical_cast<std::string>(data.get<0>())
											);
	}


	
	return 0;
}
