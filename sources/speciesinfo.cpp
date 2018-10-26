//-----------------------------------------------------------------------
//
//	File Name:	speciesinfo.cpp
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

	#include "speciesinfo.h"

	//!default constructor
	SpeciesInfo::SpeciesInfo	
				(	unsigned int 	u_species_id		,
					std::string 	u_species_name 	,
					unsigned int	u_life_coast		,
					unsigned int	u_health_status	,
					unsigned int	u_calories			,
					float					u_life_space		,
					bool					u_is_animal			,
					std::map<unsigned int, Like>	u_likings,
					unsigned int	u_tot_spec_num	

				)
		:	species_id		(u_species_id)		,
			species_name	(u_species_name)	,
			life_coast		(u_life_coast)		,
			health_status	(u_health_status)	,
			calorie				(u_calories)			,
			life_space		(u_life_space)		,
			is_animal			(u_is_animal)			,
			likings				(u_likings)				,
			m_total_species_number(u_tot_spec_num)
	{	
	}

	SpeciesInfo::~SpeciesInfo()
	{
	}

	bool SpeciesInfo::is_full()
	{
		bool all_good = true;
		if (species_id <= 0 )
		{
			all_good = false;
		}
		if (!check_likings())
		{
			all_good = false;
		}
		
		if (!all_good) return false;
		else return true;

	}

	bool SpeciesInfo::insert_like(const Like lk)
	{
		//controll if the species id is valid
		if (lk.liked_spec_id == 0)
		{	
			std::cerr	<<	std::endl
								<<	"ERROR: like inserted not valid. the spec id is 0."
								<<	"no species whith id 0 were valid"	<<	std::endl;
			return false;
		}
		else if (lk.liked_spec_id == species_id)
		{	
			std::cerr	<<	std::endl
								<<	"ERROR: like inserted not valid. the liked_spec_id " 
								<<	"is equal to the id of this species"	<<	std::endl;
			return false;
		}

		else
		{	
			//controll if insertion take place
			//map prevent us to insert two elements
			//whith the same key

		std::pair<SpeciesInfo::likings_it  , bool> 
			returned = likings.insert
					(
						std::pair<unsigned int, Like>(	lk.liked_spec_id	,
																						lk							)
					);
		if	(returned.second == true)
			{
				//iterator to last insert
			
				//use this whith set
				//likings_by_lk_factor_iterator last_lk_factor =
				//	(likings_by_lk_factor.insert( lk )).first;
			
				//multiset
				likings_by_lk_factor_iterator last_lk_factor =
					likings_by_lk_factor.insert( lk );

				#ifdef VERBOSE
				
				const Like&	like_to_print	=	
					*last_lk_factor;

				

					std::cerr	<<	std::endl
										<<	"Like Insertion Completed of like"
										<<	std::endl
										<<	like_to_print
										<<	std::endl;
										
				#endif 

				return true;
			}

			else	
			{
				std::cerr	<< std::endl
									<< 	"ERROR: insertion of like " << lk
									<<	" failed. maibe a like whith this spec id "
									<<	"already exists"	
									<<	std::endl;
				return false;
			}	
		}
	}


	bool	SpeciesInfo::insert_like(	const int u_spec_id			,
																	const int u_like_factor	)
	{
		//controll if the species id is valid
		if (u_spec_id	== 0)
		{	
			std::cerr	<<	std::endl
								<<	"ERROR: like inserted not valid. the spec id is 0."
								<<	"no species whith id 0 were valid"	<<	std::endl;
			return false;
		}
		else
		{	
			//controll if insertion take place
			//map prevent us to insert two elements
			//whith the same key
			if(
					likings.insert
					(
						std::pair<unsigned int, Like>	
							(	
								u_spec_id,
								Like(u_spec_id , u_like_factor)
							)
					).second == true
				)
			{
				return true;
			}
			
			else	
			{
				std::cerr	<< std::endl
									<< 	"ERROR: insertion of like " 
									<< 	Like(u_spec_id , u_like_factor)
									<<	" failed. maibe a like whith this spec id "
									<<	"already exists"	
									<<	std::endl;
				return false;
			}	
		}
	}

int SpeciesInfo::get_like_factor(	const unsigned int u_spec_id)
{
	std::map<unsigned int, Like>::const_iterator finded =
		likings.find(u_spec_id);
	if(finded == likings.end())
	{
		std::cerr	<< std::endl 
							<< "WARNING: Like::get_like_factor() :"
							<< "the prey searched does_not exists: "
							<< u_spec_id
							<< ". the function will return 0"
							<< std::endl << std::endl;
	return 0;
	}
	return finded->second.like_factor;
}



//!operator <
bool SpeciesInfo::operator< (const SpeciesInfo &info)
{
	return species_id < info.species_id;
}

//!operator >
bool SpeciesInfo::operator> (const SpeciesInfo &info)
{
	return species_id > info.species_id;
}

	
	//!operator >>
std::istream &operator>>	(	std::istream	&is		,
														SpeciesInfo		&info	)
{
	is	>> info.species_id 
			>> info.species_name
			>> info.life_coast
			>> info.health_status
			>> info.calorie
			>> info.life_space;

	return is;

}

//!operator <<
/*!	prints al the information stored and the likes
*/
std::ostream &operator<<	(	std::ostream			&os	,
														const SpeciesInfo	&info)
{
	os	<< "#SPECIES_INFO: "	<< std::endl;
	os	<< "#species_id:	"		<< info.species_id
			<<	std::endl
			<< "#species_name: "	<< info.species_name
			<<	std::endl
			<< "#life_coast: "		<< info.life_coast
			<<	std::endl
			<< "#health_status: "	<< info.health_status
			<<	std::endl
			<< "#calorie: "				<< info.calorie
			<<	std::endl
			<< "#life_space: "		<< info.life_space
			<<	std::endl
			<< "#is_animal: "			<< info.is_animal
			<<	std::endl;

	os	<< "	#Likings: "	<< std::endl;
	//printing likings

	for	(	SpeciesInfo::likings_const_it it = info.likings.begin();
				it	!=	info.likings.end();
				it++	)
	{
		os << it->second	<<	std::endl;
	}
	
	os << "	#Likings by like factor: "	<<	std::endl;

	for(	SpeciesInfo::likings_by_lk_factor_const_iterator f_it =
					info.likings_by_lk_factor.begin();
				f_it != info.likings_by_lk_factor.end();
				f_it ++)
	{
			
		const Like&	like_to_print	=	
			*f_it;

		os <<	like_to_print	<<	std::endl;

	}

	return os;

}

unsigned int &SpeciesInfo::total_species_number()
{
	return m_total_species_number;
}

bool SpeciesInfo::check_likings()
{	
	if (m_total_species_number - 1 == likings.size())	\
		return true;
	else	\
		return false;
}

std::string SpeciesInfo::get_info_string()
{
	std::string to_return;

	to_return += boost::lexical_cast<std::string>(species_id);
	to_return += "|";

	to_return += species_name;
	to_return += "|";

	if (is_animal)	to_return += "animal|"		;
	else						to_return += "vegetable|"	;

	to_return += boost::lexical_cast<std::string>(life_coast);
	to_return += "|";

	to_return += boost::lexical_cast<std::string>(health_status);
	to_return += "|";

	to_return += boost::lexical_cast<std::string>(calorie);
	to_return	+= "|"; 

	to_return += boost::lexical_cast<std::string>(life_space);
	to_return	+= "|"; 

	//likings remanining
	for (	likings_const_it it = likings.begin()	;
				it != likings.end();
				it ++	)
	{	
		//get the like
		const Like &like = it->second;
		std::string like_string;
		
		like_string += boost::lexical_cast<std::string>(like.liked_spec_id);
		like_string += "-";
		like_string += boost::lexical_cast<std::string>(like.like_factor);
		like_string += "|";

		to_return += like_string;
	}


	return to_return;
}



