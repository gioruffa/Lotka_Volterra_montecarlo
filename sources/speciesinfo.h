//-----------------------------------------------------------------------
//
//	File Name:	speciesinfo.h
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

//! \file speciesinfo.h
/*!	this file only contains a struct layered inside 
*		the SpeciesAnalizer class.
*/

#ifndef SPECIES_INFO
#define SPECIES_INFO

#include <string>
#include <map>			//map (likings)
#include <iostream>	//operator >> , << , cerr
#include <set>

#include "boost/lexical_cast.hpp"	//get_string_info()

#include "specied.h"
#include "like.h"
#include "container.h"
#include "classcompares.hpp"



//! species info containers
/*!	this struct contains data
*		relatives to the characteristcs of a species. 
*		his centrall role is to give a unique reference for
*		the features of a species. so that different animals of
*		the same species must have the same id
*		\see SpeciesAnalizer
*/
struct	SpeciesInfo : public eco::Container
{
	//!likings type
	typedef	std::map<unsigned int, Like>	likings_map_tp;

	//! iteratore per membro di likings
	typedef likings_map_tp::iterator	
		likings_it;

	//! iteratore constante per membro di likings
	typedef likings_map_tp::const_iterator	
		likings_const_it;

	//!	multimap sorted by like_factor
	/*!	key is the like_factor and value is an iterator
	*		to the corresponding element in likings_map
	*/
	typedef std::multiset< Like , LikeFactorCmp >
		likings_by_lk_factor_tp;
	
	//!	iterator to access like sorted by lk factor
	typedef likings_by_lk_factor_tp::iterator	
		likings_by_lk_factor_iterator;
	
	//!	const iterator to access like sorted by lk factor
	typedef likings_by_lk_factor_tp::const_iterator	
		likings_by_lk_factor_const_iterator;

	//!default constructor
	SpeciesInfo	(	unsigned int 	u_species_id	=	0						,
								std::string 	u_species_name 	= "no_name"	,
								unsigned int	u_life_coast		=	1					,
								unsigned int	u_health_status	=	50				,
								unsigned int	u_calories			=	1					,
								float					u_life_space		= 10				,
								bool					u_is_animal			=	0					,
								std::map<unsigned int, Like>	u_likings =
									std::map<unsigned int, Like>()					,
								unsigned int	u_tot_spec_num	=	0					
							);

	//!default destructor
	~SpeciesInfo();


	//!controll if there is a species id instantiated
	bool is_full();

	//!insert a Like in likings map
	bool 	insert_like(	const Like lk	);
	//! create and insert a like whith passed parameter
	/*!	\see Like
	*/
	bool	insert_like(	const int u_spec_id			,
											const int u_like_factor	);

	//!get the like factor of a liked specied
	/*!	\param u_spec_id the species id of the liked species
	*/
	int	get_like_factor(	const unsigned int u_spec_id	);

	
	//!operator <
	bool operator< (const SpeciesInfo &info);

	//!operator >
	bool operator> (const SpeciesInfo &info);

	//!operator >>
	/*!	
	*		DO NOT USE ME!
	*/
	friend std::istream  &operator>>(	std::istream 	&is		,	
																		SpeciesInfo		&info	);
	
	//!operator <<
	/*!	prints al the information stored and the likes
	*/
	friend std::ostream	&operator<<(	std::ostream 				&os		,
																		const SpeciesInfo		&info	);

	//!	the numerical id of the species
	/*!	this is the numerical id of the species and is used to identify
	*		the species in order to compute parameters and make statistics
	*/
	unsigned int		species_id;

	//! the name of the species
	/*! the name of the species as it could be for a human.
	*		example: lion, bear, rabbit...
	*		this name is NOT used in any computational process,
	*		it's only for a better human understanding of the process
	*		
	*		\see species_id
	*/
	std::string 	species_name;
	
	//!	the coast of life
	/*!	every time a specied is called it had to pay a life coast.
	*		this life coast is sottraed from the m_hp when the life()
	*		member is called
	*		\see live
	*/
	unsigned int	life_coast;
	
	//!	health status determine when an animal feels good
	/*!	the health status has to be read as a percentage of the 
	*		total hp reachable. over this percentage the form of life 
	*		starts to feels good, so his libido rise in order to prefer the 
	*		reproduction.
	*		no plant reproduction is included in this model!
	*		but for the future realises this data member in included in
	*		Specied class
	*/
	unsigned int health_status;

	//!	calorie the nutritive power of the form of life
	/*!	\see Species::m_calorie
	*/
	unsigned int	calorie;

	//!	occuped space in a quadro
	/*!	the space occuped in a quadro in percentage.
	*/
	float	life_space;
		
	//!	true if is animal , false is vegetable
	/*!	variable used only for a better understanding of the
	*		reader.
	*/
	bool is_animal;

	//!	the likings
	/*!	how much a species like others.
	*		
	*		the key is the spec_id searched.
	*		and the sort is provided by Like::operator<
	*		using the default set constructor
	*	
	*		the nature of the container ensures the inexistance of
	*		two egual species	

	*		\todo scrivi delle considerazioni finali sul fatto
	*		che i multi_index sono più comodi in questi casi
	*		anche per emulare una map isi isi
	*/
	likings_map_tp	likings;


	//!likings sorted by lk_factor
	/*!	why i didn't use a smart_ptr map? 
	*		because i would have had to
	*		make the likings_map_tp made of boost smart pointers and then
	*		create this multimap. it was too late so i decided to make it
	*		composed of iterators instead of pointers, references or copies.
	*
	*		so why you did not use a boost:multiindex container?
	*		bacause this project has a didactic scope so i want to get both
	*		the experiences in order to have an idea of good and evil of 
	*		stl vs multiindex.
	*
	*		if your last question is: "is it better to use multiindex in this 
	*		situation? isn't it?"
	*
	*		the answer is one and only : "YES!"
	*		
	*/
	likings_by_lk_factor_tp	likings_by_lk_factor;

	
	//! set total_species_number
	unsigned int &total_species_number();

	//! get a string of all the data formatted
	/*!	the string is composed as is parsed by 
	*		SpeciesController::string_parser
	*		\see SpeciesController
	*/
	std::string get_info_string();
	
	private:

	//!check if the size of likings is = to m_total_species_number
	bool check_likings();
	
	
	
	//!total number of species present int the ecosystem
	/*!	this member is used to controll if the insertion has been
	*		completed
	*/
	unsigned int m_total_species_number;

};

#endif
