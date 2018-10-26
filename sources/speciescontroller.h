//-----------------------------------------------------------------------
//
//	File Name:	speciescontroller.h
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

/*!	\file speciescontroller.h
*		SpeciesController interface
*/

#ifndef SPEC_ANALIZER
#define SPEC_ANALIZER

#include <map>
#include <iostream>		//operator << and >>
#include <fstream>		//operator >>
#include <string>			//insert whith strings

#include "controller.h"
#include "speciesinfo.h"



//!	contain info about the species in the ecosystem
/*!	this class contains all the information about
*		the species present in the ecosystem.
*		this class is the UNIQUE reference for the 
*		features of a species.
*		this is class inside the ecosystem to provide 
*		the values relative the species.
*		in this way al the form of life in the ecosystem
*		of the same species have the same value for dm 
*		relative to the species.
*		
*		this class is also used to determine how much species
*		like each others. for like we intend "to prefer for dinner".
*
*		particular attention was given to the affidability and to the
*		rightness of the data field inserted. controll statement preservers
*		from multiple insertions of lines and wrong likes.
*		the computational coast is hight but this element has to be build rightly
*		and only one time for ecosystem so is a good fee to pay.
*
*/
class SpeciesController: public Controller
{

	public:

	//!	the type of the infos map
	/*!	typedef std::map<unsigned int, SpeciesInfo>
	*/
	typedef std::map<unsigned int, SpeciesInfo> infos_tp;

	//! info iterator
	/*!	\see m_infos	
	*/
	typedef	std::map<unsigned int, SpeciesInfo>::iterator
		infos_it;

	//! info const_iterator
	/*!	\see m_infos	
	*/
	typedef	std::map<unsigned int, SpeciesInfo>::const_iterator
		infos_const_it;

	//! default contructor
	/*!	if no u_species_number is passed will set to 0 and check() will fail
	*/
	SpeciesController(	unsigned int u_species_number = 0);
	//! default destrictor
	~SpeciesController();


	//!	check if the controll is completed	
	/*!	\returns true only if those condition are simultaneus:
	*		-	total species number is not 0
	*		- the number of SpeciesInfos coincide with the number of species in ecosystem
	*		-	every SpeciesInfo returns true if is_full() is called
	*		\see SpeciesInfo::is_full()
	*/
	virtual bool check();	

	//!insert a species info
	std::pair<infos_it , bool> insert(const SpeciesInfo &spi);

	//!get infos about a species
	/*!	\return a const iterator to a pair which the second
	*		member is the SpeciesInfo. if the species does not
	*		exists returns an iterator to m_infos.end();
	*/
	infos_const_it	get_info(const unsigned int u_spec_id);

	//! get the reference to the species info whith u_spec_id as species_id
	const SpeciesInfo&	get_info_ref(const unsigned int u_spec_id) const;
	
	//!get like_factor
	/*!	\param predator_spec_id the species id of the Species
	*			which wants to eat.
	*		\param prey_spec_id	the species id that will be eaten
	*		\return the like factor. how much the predator like the prey
	*
	*		\see Like::like_factor
	*/
	int	get_like_factor(	const unsigned int predator_spec_id	,
												const unsigned int prey_spec_id			);

	
	//! get the species infos
	/*!	\returns a const reference to species infos container,
	*			which is a map;
	*/
	const std::map<unsigned int, SpeciesInfo> &get_infos () const;

	//! get the infos
	std::map<unsigned int, SpeciesInfo> &get_infos ();

	//! get the total species number
	unsigned int species_number ();

	//!	operator >>
	/*!	load all SpeciesInfo for all species.
	*		take the first line of the stream and passes it a string_parser.
	*		to be used whith filestream
	*		\see string_parser for the data format;
	*/
	friend	std::ifstream &operator>>(	std::ifstream 	&is	,
																		SpeciesController	&sa	);
	//!	operator >>	
	/*!	load all SpeciesInfo for all species.
	*		to be used whith std::cin,
	*		ask for the fields an compose a line to pass to line_parser
	*		then if the line is well composed insert the SpeciesInfo
	*/
	friend	std::istream &operator>>(	std::istream 	&is	,
																		SpeciesController	&sa	);

	//!operator	<<
	friend	std::ostream &operator<<(	std::ostream 					&is	,
																		const SpeciesController	&sa	);


	private:


	//! string parser
	/*!	parse an input line and put's the input in
	*		the spi field
	*
	*		Input Format:
	*		the line passed contains numerous info divided by a "|"
	*		here's the order of the fields:
	*		(note that we are in C++ so the first element has position 0)
	*
	*		-	species_id 		(0 is not valid)
	*		-	species_name	
	*		-	"vegetable" or "animal"
	*		-	life_coast
	*		-	health_status
	*		-	calories
	*		-	life_space
	*		
	*		after 6 tokens have to insert the Like of the species in the format: 
	*		liked_species-like_factor. pay attention to the "-".
	*		as usual every Like has to be separated by "|".
	*		return m_infos;	
	*		if the numbers of Likes is not = to the total species number 
	*		in the ecosystem - 1 . the function will return false;
	*
	*		example:	if we have 3 species in the ecosystem (1 is lion, 
	*		2	is gazzella , 3 is grass):
	*
	*		"1|lion|animal|10|50|1|45|2-50|3-0"
	*
	*
	*		\param str string containing the data
	*		\param spi SpeciesInfo in which put data
	*		\return true if the string were well done,
	*			false if the string wasn't formatted correctly
	*		\see operator>>
	*		
	*/
	bool string_parser	(	const std::string	&str ,
												SpeciesInfo				&spi );



	//! total number of species in the ecosystem
	unsigned int m_species_number;
	

	
	//!	container of species info
	/*!	species infos were sorted by spec_id
	*		\see SpeciesInfo
	*		\see SpeciesInfo::operator<
	*/
	std::map<unsigned int, SpeciesInfo>	m_infos;
};

#endif
