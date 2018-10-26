//-----------------------------------------------------------------------
//
//	File Name:	speciescontroller.cpp
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

#include <iostream>
#include <fstream>
#include <vector>			//string container in str insert
#include <algorithm>//copy , back_insert

//headers used to parse and interprete the input
#include "boost/tokenizer.hpp"		
#include "boost/lexical_cast.hpp"



#include "speciescontroller.h"



typedef SpeciesController SpecCon;

SpecCon::SpeciesController(	unsigned int u_species_number	)
	:	m_species_number(u_species_number)
{
}

SpecCon::~SpeciesController()
{
}

bool	SpecCon::check()
{
	//if the total_number of species is 0 return false
	#ifdef VERBOSE
		std::cerr	<<	std::endl
							<<	"CHECKING SPECIESCONTROLLER"
							<<	std::endl;
	#endif

	bool all_good = true;
	bool right_number = true;
	bool right_size = true;
	bool all_full		= true;
	
	//no species is not valid
	if (m_species_number <= 0)
	{
		right_number = false;
		all_good = false;
	}
	//if the size of the info containers
	//match whith the number of species
	//all fails were checked by other methods
	//like insert and string_parser
	if (m_infos.size() != m_species_number)
	{
		right_size = false;
		all_good = false;
	}
	//controll if all the SpeciesInfo were full
	for (	SpecCon::infos_it it = m_infos.begin();
				it != m_infos.end();
				it++)
	{
		if(it->second.is_full()==false)
		{
			all_full = false;
			all_good = false;
		}
	}

	//say something
	if (!right_number)
	{
		std::cerr	<<	std::endl
							<<	"ERROR: the m_species_number you entered in "
							<<	"construciton of the speciescontroller is <= 0"
							<<	std::endl;

	}
	if (!right_size)
	{
		std::cerr	<<	std::endl
							<<	"ERROR: the number of species info you entered"
							<<	"is not equal to the total species number."
							<<	std::endl;
	}
	if(!all_full)
	{
		std::cerr	<<	std::endl
							<<	"ERROR: one or more species info is uncorrect or"
							<<	"uncomplete. see SpeciesInfo::is_full()"
							<<	std::endl;
	}


	#ifdef VERBOSE
		std::cerr	<<	std::endl
							<<	"SPECIESCONTROLLER CHECK DONE"
							<<	std::endl;
	#endif

	if (all_good)	return true;
	else return false;	
}

std::pair<SpecCon::infos_it , bool> 
	SpecCon::insert(
		const SpeciesInfo &spi
	)
	{
		typedef std::pair <SpecCon::infos_it , bool>
			returned_tp;
		
		//controll if the species id is valid
		if (spi.species_id == 0)
		{	
			std::cerr	<<	std::endl
								<<	"ERROR: SpeciesInfo inserted not valid. "
								<<	"the spec id is 0."
								<<	"no species whith id 0 were valid"	<<	std::endl;
			return returned_tp (m_infos.end() , false);
		}
		else
		{	
			//controll if insertion take place
			//map prevent us to insert two elements
			//whith the same key
			returned_tp inserted = 
				m_infos.insert
				(
					std::pair<unsigned int, SpeciesInfo>
						(	spi.species_id	,
							spi						)
				);

			if(inserted.second)
			{
				std::cerr	<<	std::endl
									<<	"Insertion of SpeciesInfo os Species number "
									<<	spi.species_id	
									<< " ("	<< spi.species_name	<< ")"
									<< " done." << std::endl;
				return returned_tp(inserted.first,true);
			}
			
			else	
			{
				std::cerr	<< std::endl
									<< 	"ERROR: insertion of Species Info " << std::endl
									<< spi	<< std::endl
									<<	" failed. maibe a Species Info whith this spec id "
									<<	"already exists"	
									<<	std::endl;
				return returned_tp(m_infos.end(),false);
			}	
		}
	}

SpecCon::infos_const_it SpecCon::get_info(const unsigned int u_spec_id)
{
	return m_infos.find(u_spec_id);
}

const SpeciesInfo& SpecCon::get_info_ref(const unsigned int u_spec_id) const
{
	SpecCon::infos_const_it finded_info_it = m_infos.find(u_spec_id);

	if (finded_info_it == m_infos.end())
	{
		std::cerr	<<	std::endl
							<<	"ERROR: the species id you were lookinf for"
							<<	" does not exists"
							<<	std::endl;
	}
	
	return finded_info_it->second;

}


	
	
int	SpecCon::get_like_factor(	const unsigned int predator_spec_id	,	
													const unsigned int prey_spec_id			)
{
	//in this way i don't have to do the search 2 times
	//can't use const because get hungry calling get_like_factor()
	SpecCon::infos_it finded_predator = 
		m_infos.find(predator_spec_id);

	if ( finded_predator  == m_infos.end())
	{
		std::cerr	<< std::endl 
							<< "WARNING: SpeciesAnalizer::like_factor() :"
							<< "the predatpr searched does_not exists: "
							<< predator_spec_id
							<< ". the function will return 0"
							<< std::endl << std::endl;
	return 0;
	}

	return finded_predator->second.get_like_factor(prey_spec_id);
}


const std::map <unsigned int, SpeciesInfo> &SpecCon::get_infos() const
{
	return m_infos;
}

std::map <unsigned int, SpeciesInfo> &SpecCon::get_infos()
{
	return m_infos;
}

bool SpecCon::string_parser(	const std::string &str	,
														SpeciesInfo				&spi	)
{
	typedef boost::tokenizer<boost::char_separator<char> > 
    tokenizer_tp;
	boost::char_separator<char> sep("|");

	tokenizer_tp tokenizer(str, sep);
	
	//this is a line whith no information
	if(tokenizer.begin() == tokenizer.end())	return false;
	
	
	typedef std::vector<std::string> tokens_container;
	tokens_container tk_cont;	//contain all tokens separed by "|"

	//we copy the content of the string iside the
	//tk container to better handle it
	std::copy(tokenizer.begin() 					, 
						tokenizer.end() 						, 
						std::back_inserter(tk_cont)	);

	#ifdef DEBUG
		for (	std::vector<std::string>::iterator it = tk_cont.begin();
					it	!= tk_cont.end();
					it ++	)
		{
			std::cerr	<< std::endl	
								<< *it;
		}
	#endif

	//controll if the size of the container
	//so the number of tokens passed is 7 + the total
	//species number
	const unsigned int right_info_num = 7;

	//note that -1 requires becuase the species doesn't like itselfe
	if(tk_cont.size() != m_species_number + right_info_num - 1 )
	{
		std::cerr	<< std::endl
							<< "WARNING: "
							<< "SpeciesController: "
							<< "line skipped during the inizialization process "
							<< "because the number of tokens is not egual to "
							<< "the total species number plus "
							<< right_info_num
							<< " , which is the number of generic features for a "
							<< "species."
							<< std::endl;
		return false;
	}

	//temporary variables to save the data
	//we don't really need them but
	//for me is clearer
	unsigned int 	spec_id,	life_coast,	\
								health_status, calorie;
	
	float life_space;

	//now lexical casts begins
	using boost::lexical_cast;
  using boost::bad_lexical_cast;

	//casting spec_id
	try
	{
		spec_id = lexical_cast<unsigned int>(tk_cont[0]);
  }
  catch(bad_lexical_cast &)
  {
		std::cerr	<< std::endl 
							<< "WARNING: "
							<< "SpeciesController: "
							<< "the spec_id field in the tokens is not a "
							<< "number. line skipped"
							<< std::endl;
		return false;
  }
	
	//casting life_coast
	try
	{
		life_coast = lexical_cast<unsigned int>(tk_cont[3]);
  }
  catch(bad_lexical_cast &)
  {
		std::cerr	<< std::endl 
							<< "WARNING: "
							<< "SpeciesController: "
							<< "the life_coast field in the tokens is not a "
							<< "number. line skipped"
							<< std::endl;
		return false;
  }
	if	(life_coast > 100 )
	{
		std::cerr	<< std::endl 
							<< "WARNING: "
							<< "SpeciesController: "
							<< "the life_coast field in the tokens is more than 100. "
							<< "line skipped"
							<< std::endl;
		return false;
	}
	
	//casting health_status
	try
	{
		health_status = lexical_cast<unsigned int>(tk_cont[4]);
  }
  catch(bad_lexical_cast &)
  {
		std::cerr	<< std::endl 
							<< "WARNING: "
							<< "SpeciesController: "
							<< "the health_status field in the tokens is not a "
							<< "number. line skipped"
							<< std::endl;
		return false;
  }
	if	(health_status> 100 )
	{
		std::cerr	<< std::endl 
							<< "WARNING: "
							<< "SpeciesController: "
							<< "the health_status field in the tokens is more than 100. "
							<< "line skipped"
							<< std::endl;
		return false;
	}

	//casting calorie
	try
	{
		#ifdef DEBUG
			std::cerr	<< std::endl 	<< "calorie tok: "
								<< tk_cont[6]	<< std::endl;
		#endif
		calorie = lexical_cast<unsigned int>(tk_cont[5]);
  }
  catch(bad_lexical_cast &)
  {
		std::cerr	<< std::endl 
							<< "WARNING: "
							<< "SpeciesController: "
							<< "the calorie field in the tokens is not a "
							<< "number. line skipped"
							<< std::endl;
		return false;
  }
	if	(calorie< 0 )
	{
		std::cerr	<< std::endl 
							<< "WARNING: "
							<< "SpeciesController: "
							<< "the calorie field in the tokens is less than 0. "
							<< "line skipped"
							<< std::endl;
		return false;
	}

	//casting life_space
	try
	{
		life_space = lexical_cast<float>(tk_cont[6]);
  }
  catch(bad_lexical_cast &)
  {
		std::cerr	<< std::endl 
							<< "WARNING: "
							<< "SpeciesController: "
							<< "the life_space field in the tokens is not a "
							<< "number. line skipped"
							<< std::endl;
		return false;
  }
	if	(life_space> 100 )
	{
		std::cerr	<< std::endl 
							<< "WARNING: "
							<< "SpeciesController: "
							<< "the life_space field in the tokens is more than 100. "
							<< "line skipped"
							<< std::endl;
		return false;
	}

	//vegetable or animal?
	bool is_an;
	if 			(tk_cont[2] == "vegetable") is_an = false;
	else if	(tk_cont[2] == "animal")		is_an = true;
	else 
	{
		std::cerr	<< std::endl 
							<< "WARNING: "
							<< "SpeciesController: "
							<< "no animal or vegetable field found valid. "
							<< "skipping line."
							<< std::endl;
		return false;
	}

	//at this point we should have all the infos
	//remains only like starting from tk_cont[7]

	//species_name -> not so important if fails

	//write down what we have
	std::string species_name(tk_cont[1]);
	//writing data features inserted
	spi.species_id 		= spec_id;
	spi.species_name	= species_name;
	spi.life_coast		= life_coast;
	spi.health_status	= health_status;
	spi.calorie				=	calorie;
	spi.life_space		=	life_space;
	spi.is_animal			=	is_an;

	//go for like

	//-1 because a species didn't eat itself
	for (	unsigned int i = right_info_num; 
				i < right_info_num + m_species_number - 1 ; 
				i++)
	{
		boost::char_separator<char> like_sep("-");
		tokenizer_tp like_tok(tk_cont[i] , like_sep);			
		
		std::vector<std::string> like_cont;
		
		std::copy(	like_tok.begin()							,
								like_tok.end()								,
								std::back_inserter(like_cont)	);
	
		//tokens should be 2, the liked_spec_id and the like_factor
		if (like_cont.size() != 2)
		{
			std::cerr	<< std::endl
								<< "WARNING: "
								<< "SpeciesController: "
								<< "line skipped during the inizialization process "
								<< "because a like has not the form: "
								<< "liked_spec_id-like_factor"
								<< std::endl;

			return false;
		}
	
		
		//building a cash like to insert
		Like cash_like;

		try
		{
			cash_like.liked_spec_id	= 
				lexical_cast<unsigned int>(like_cont[0]);
			cash_like.like_factor 	= 
				lexical_cast<unsigned int>(like_cont[1]);
 		}
	 	catch(bad_lexical_cast &)
 	 	{
			std::cerr	<< std::endl 
								<< "WARNING: "
								<< "SpeciesController: "
								<< "a Like field is not a"
								<< "number. line skipped"
								<< std::endl;
			return false;
  	}
		if	(cash_like.like_factor > 100 )
		{
			std::cerr	<< std::endl 
								<< "WARNING: "
								<< "SpeciesController: "
								<< "the like_factor in the tokens is more than 100. "
								<< "line skipped"
								<< std::endl;
			return false;
		}
		//controll if a like has the same species id
		//of the species itself
		if(cash_like.liked_spec_id == spec_id)
		{
			std::cerr	<< std::endl 
								<< "WARNING: "
								<< "SpeciesController: "
								<< "error occurred in parsing this line: "
								<< "a liked_specied_id of a Like has the same "
								<< "value of the species itself: "
								<< cash_like.liked_spec_id
								<< ". "
								<< "Cannibalsim is not implemented in this realise"
								<< std::endl;	
			return false;
		}

		//if it is vegetable no positive like factors
		//were valid -> no Carnivorous plant
		if (!is_an	and	cash_like.like_factor > 0)
		{
			std::cerr	<< std::endl 
								<< "WARNING: "
								<< "SpeciesController: "
								<< "a like factor is postive for a vegetable species"
								<< "this is not valid, no carnivourous plants are "
								<< "permitted."
								<< "like factor setted to 0"
								<< std::endl;
			cash_like.like_factor = 0;
		}
	
		
		if(spi.insert_like(cash_like) == false)
		{
			std::cerr	<< std::endl 
								<< "WARNING: "
								<< "SpeciesController: "
								<< "error occurred in parsing this line:"
								<< "have you entered 2 like whith the same spec_id: "
								<< cash_like.liked_spec_id
								<< std::endl;	
			return false;
		}

	}	//ends for
	
	//writing all down

	//at this point all the data's should be right
	return true;
}



std::ifstream &operator>>(	std::ifstream 	&is		,
													SpeciesController	&sa	)
{
	  bool input_done = false;
		
		//if (sa.m_species_number <= 0);

		
    while(!input_done)
    {
     std::stringbuf rsbuf;
     std::string    line;

     is.get(rsbuf);
     line=rsbuf.str();

		 //controll for comments
		 if (*line.begin() == '#')\
		 	continue;


     if (!line.empty())
     {
      is.clear(); // We may be at EOF after reading a valid record.

			SpeciesInfo cash_info;
			
			cash_info.total_species_number() = sa.m_species_number;

			if(sa.string_parser(line , cash_info) == false ) 
	      continue; // Skip malformed lines 

			std::pair<SpecCon::infos_it , bool> returned_spi_insertion	=
				sa.insert(cash_info);

			if(returned_spi_insertion.second == false)
			{
				std::cerr	<<	std::endl
									<<	"ERROR: insertion of the following species info"
									<<	"is failed. check data field and correct"
									<<	std::endl
									<<	cash_info
									<<	std::endl;
			}
			else
			{
				#ifdef	RUN_DBG
					//get info ref
						//i hate maps
					SpecCon::infos_const_it	it_to_print = 
						returned_spi_insertion.first;

					const SpeciesInfo	&info_to_print	=	
						it_to_print->second;

					std::cerr	<<	std::endl
										<<	"INSERTED INFO :"
										<<	std::endl
										<<	info_to_print
										<<	std::endl;
				#endif
			}

      input_done = true;
			//looppa giustamente
    }
       
    // Reading an empty string will set std::ios::failbit, which we
    // don't want to consider an error.
    if (is.rdstate() == std::ios::failbit) is.clear();
    if (! is.good()) input_done = true; 
    else
    {
  	 	is.get();   // Skip past delimiter (newline). 
	   	is.clear(); // Be happy if it doesn't succeed.
  	}
  }

  return is;

}


std::istream &operator>>(	std::istream 	&is		,
													SpeciesController	&sa	)
{
		using boost::lexical_cast;
  	using boost::bad_lexical_cast;
	  bool input_done = false;

    while(!input_done)
    {
			//line to build to pass to the line_parser
			std::string line;
			unsigned int cash_int = 0;
			std::string cash_string;
			
			std::cout << std::endl 
								<< "Insert species_id: ";
			std::cin	>> cash_int;
			if(cash_int <= 0)
			{
				std::cerr	<< std::endl	
									<< "ERROR: species_id could not be = or < to 0. "
									<< "use only positive values. "
									<< std::endl;
				continue;
			}
			
			try
			{
				line += lexical_cast<std::string>	(cash_int);
				line += "|";
			}
			catch(bad_lexical_cast &)
			{
							std::cerr	<< std::endl 
											<< "ERROR: "
											<< "field is not a"
											<< "number. line skipped"
											<< std::endl;
							continue;
			}
			
			//inserting species_name
			std::cout << std::endl 
								<< "Insert species_name: ";
			std::cin	>> cash_string;
			line = line + cash_string + "|";
			cash_string.clear();

			//inserting vegetable or animal
			std::cout << std::endl 
								<< "if is animal print \"animal\" "
								<< "if is vegetable print \"vegetable\" :";
			std::cin	>> cash_string;
			line = line + cash_string + "|";
			cash_string.clear();

			//insert life_coast
			std::cout << std::endl 
								<< "Insert life_coast: ";
			std::cin	>> cash_int;
			try
			{
				line += lexical_cast<std::string>	(cash_int);
				line += "|";
			}
			catch(bad_lexical_cast &)
			{
							std::cerr	<< std::endl 
											<< "ERROR: "
											<< "field is not a"
											<< "number. line skipped"
											<< std::endl;
							continue;
			}

			//insert health_status
			std::cout << std::endl 
								<< "Insert health_status: ";
			std::cin	>> cash_int;
			try
			{
				line += lexical_cast<std::string>	(cash_int);
				line += "|";
			}
			catch(bad_lexical_cast &)
			{
							std::cerr	<< std::endl 
											<< "ERROR: "
											<< "field is not a"
											<< "number. line skipped"
											<< std::endl;
							continue;
			}

			//insert calories
			std::cout << std::endl 
								<< "Insert calories: ";
			std::cin	>> cash_int;
			try
			{
				line += lexical_cast<std::string>	(cash_int);
				line += "|";
			}
			catch(bad_lexical_cast &)
			{
							std::cerr	<< std::endl 
											<< "ERROR: "
											<< "field is not a"
											<< "number. line skipped"
											<< std::endl;
							continue;
			}

			//insert life_space
			std::cout << std::endl 
								<< "Insert life_space: ";
			std::cin	>> cash_int;
			try
			{
				line += lexical_cast<std::string>	(cash_int);
				line += "|";
			}
			catch(bad_lexical_cast &)
			{
							std::cerr	<< std::endl 
											<< "ERROR: "
											<< "field is not a"
											<< "number. line skipped"
											<< std::endl;
							continue;
			}

			//
			// inserting likes
			//
			std::cout	<< std::endl
								<< "Inserting Likes for #"
								<< sa.m_species_number
								<< " species."
								<< std::endl;

			for(unsigned int i=0; i < sa.m_species_number-1; i++)
			{
				//insert liked_specied_id
				std::cout << std::endl 
									<< "Insert liked_species_id: ";
				std::cin	>> cash_int;
				try
				{
								line += lexical_cast<std::string>	(cash_int);
								line += "-";
				}
				catch(bad_lexical_cast &)
				{
								std::cerr	<< std::endl 
												<< "ERROR: "
												<< "field is not a"
												<< "number. line skipped"
												<< std::endl;
								continue;
				}

				//insert like_factor
				std::cout << std::endl 
									<< "Insert like_factor: ";
				std::cin	>> cash_int;
				try
				{
								line += lexical_cast<std::string>	(cash_int);
								line += "|";
				}
				catch(bad_lexical_cast &)
				{
								std::cerr	<< std::endl 
												<< "ERROR: "
												<< "field is not a"
												<< "number. line skipped"
												<< std::endl;
								continue;
				}
	
			}
			//there should be a "|" at the end
			line.erase(--line.end());

			//line formed, passing to the parser
			std::cerr	<<	std::endl	
								<<	"insertion ended. "
								<<	"your line should be: "
								<<	std::endl
								<<	line
								<<	std::endl;

     	if (!line.empty())
     	{
      is.clear(); // We may be at EOF after reading a valid record.

			SpeciesInfo cash_info;

			cash_info.total_species_number() = sa.m_species_number;

			if(sa.string_parser(line , cash_info) == false ) 
	      continue; // Skip malformed lines 

			std::pair<SpecCon::infos_it , bool> returned_spi_insertion	=
				sa.insert(cash_info);
			if(returned_spi_insertion.second == false)
				continue;
			else
			{
				#ifdef	RUN_DBG
					//get info ref
						//i hate maps
					SpecCon::infos_const_it	it_to_print = 
						returned_spi_insertion.first;

					const SpeciesInfo	&info_to_print	=	
						it_to_print->second;
							

					std::cerr	<<	std::endl
										<<	"INSERTED INFO IN SPECIESC"
										<<	std::endl
										<<	info_to_print
										<<	std::endl;
				#endif
			}

      input_done = true;
			//looppa giustamente
    	}
       
    // Reading an empty string will set std::ios::failbit, which we
    // don't want to consider an error.
    if (is.rdstate() == std::ios::failbit) is.clear();
    if (! is.good()) input_done = true; 
    else
    {
  	 	is.get();   // Skip past delimiter (newline). 
	   	is.clear(); // Be happy if it doesn't succeed.
  	}
  }

  return is;

}




std::ostream &operator<<(	std::ostream 					&os	,
													const SpeciesController	&sa	)
{
	
	os	<< "#SpeciesController"			<< std::endl
			<< "#Species number: "		<< sa.m_species_number
			<< std::endl;

	for (	SpecCon::infos_const_it it = sa.m_infos.begin();
				it != sa.m_infos.end();
				it ++	)
	{
		os	<< "#Species information: "	<< std::endl
				<< it->second	<< std::endl;
	}

	return os;
}

unsigned int SpecCon::species_number()
{
	return m_species_number;
}
