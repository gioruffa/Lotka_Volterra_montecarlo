//-----------------------------------------------------------------------
//
//	File Name:	populationvariation.hpp
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

#include <utility>


//! variation of population for a species in a subsystemcontainer
/*!	this class contains the population variation for a species
*		in a determinate subsystem container
*		this class is layered inside StepLog class and parsed by
*		EcosystemContainer::step;
*		\see StepLog
*		\see EcosystemContainer::step
*		\see SubsystemContainer
*		\see subs_coord
*		\see species_id
*		\see vartiation
*/
struct PopulationVariation
{
	//!coordinates first x second y
	typedef std::pair<unsigned int , unsigned int>	coord_tp;
	
	//! default contstructor
	PopulationVariation	(	coord_tp 			u_coord_tp 		,  
												unsigned int 	u_species_id	,
												int 					u_variation		,
												long int			u_abs_time		,
												float					u_rel_time		)
		:	subs_coord(u_coord_tp		)	,
			species_id(u_species_id	)	,
			variation	(u_variation	)	,
			abs_time	(u_abs_time		)	,
			rel_time	(u_rel_time		)
	{
	}

	//!deafult bestructor
	~PopulationVariation()
	{
	}

	//!	subsystem coordinate in which occours the variation
	coord_tp	subs_coord;

	//! the species of the animal that variate
	unsigned int 	species_id;

	//!	the variation
	/*!	example: if the animal die variation is -1
	*/
	int						variation;

	//!	absolute time of the variation
	long int	abs_time;

	//!	relative time of the variation
	float			rel_time;
};
