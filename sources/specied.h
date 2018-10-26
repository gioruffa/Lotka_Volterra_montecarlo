//-----------------------------------------------------------------------
//
//	File Name:	specied.h
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

/*!	\file specied.h
*		this file contains the interface of the class Specied.
*		this class is one of the most important.
*/

#ifndef SPECIED
#define	SPECIED

#include "vivent.h"	//ancestor class Vivent
#include <string>		//string (species_name)

//!	class Specied the form of life as species belonger
/*!	probabli the most important class of this library.
*		it rappresents the form of life seen as belonging to a species
*		so it contain all the characteristcs of animal of the same species
*		like the : life coast and ...
*/

class Specied : public Vivent
{
	public:
	//! default constructor
	/*!	set private data members
	*/
	Specied	(	unsigned int 	u_species_id		=	0					,
						std::string 	u_species_name 	= "no_name"	,
						unsigned int	u_life_coast		=	1					,
						unsigned int	u_health_status	=	50				,
						unsigned int	u_calories			=	1					,
						float					u_life_space		= 10
					);
	
	//! default destructor
	/*! does nothing
	*/
	~Specied();

	//FUNCITONS
	
	//! is the Specied form of life alive?
	/*!	returns true if hp() is >= 0;
	*		if the animal is not alive it will be
	*		removed.
	*		\see EcosystemContainer::step()
	*/
	virtual bool is_alive();


	//set-get
		//set
	//! species id
	/*! \see m_species_id
	*/
	unsigned int	&species_id()		;
	
	//! species name
	/*! \see m_species_name 
	*/
	std::string		&species_name()	;

	//! life coast
	/*! \see m_life_coast 
	*/
	unsigned int	&life_coast()		;
	
	//! health statis
	/*! \see m_health_status
	*/
	unsigned int	&health_status();	

	//! calorie
	/*! \see m_calorie
	*/
	unsigned int	&calorie()			;

	//! life space
	/*! \see m_life_space
	*/
	float					&life_space()		;
	
	
		//get
	//!get the species_id
	unsigned int	species_id()		const;
	//!get the species_name
	std::string		species_name()	const;
	//!get the life_coast
	unsigned int	life_coast()		const;
	//!get the health_status
	unsigned int	health_status()	const;	
	//!get the calorie
	unsigned int	calorie()				const;
	//!get the life_space
	float					life_space()		const;


	private:

	//!	the numerical id of the species
	/*!	this is the numerical id of the species and is used to identify
	*		the species in order to compute parameters and make statistics
	*/
	unsigned int		m_species_id;

	//! the name of the species
	/*! the name of the species as it could be for a human.
	*		example: lion, bear, rabbit...
	*		this name is NOT used in any computational process,
	*		it's only for a better human understanding of the process
	*		
	*		\see m_species_id
	*/
	std::string 	m_species_name;
	
	//!	the coast of life
	/*!	every time a specied is called it had to pay a life coast.
	*		this life coast is sottraed from the m_hp when the life()
	*		member is called
	*		\see live
	*/
	unsigned int	m_life_coast;
	
	//!	health status determine when an animal feels good
	/*!	the health status has to be read as a percentage of the 
	*		total hp reachable. over this percentage the form of life 
	*		starts to feels good, so his libido rise in order to prefer the 
	*		reproduction.
	*		no plant reproduction is included in this model!
	*		but for the future realises this data member in included in
	*		Specied class
	*/
	unsigned int m_health_status;

	//!	calorie the nutritive power of the form of life
	/*!	each form of life, when eated, aliments the eater
	*		which can be only an animal (no carnivorous plants are modelled).
	*		the hp the eater receive are hp_eaten * calorie;
	*/
	unsigned int	m_calorie;

	//!	occuped space in a quadro
	/*!	the space occuped in a quadro in percentage.
	*		example: if m_life_space is 10 -> no more than
	*		ten animal of this species can be hosted in a quadro
	*/
	float	m_life_space;
};

#endif
