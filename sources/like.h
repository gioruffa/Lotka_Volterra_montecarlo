//-----------------------------------------------------------------------
//
//	File Name:	like.h
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

#ifndef LIKE
#define LIKE

#include <iostream>
#include "container.h"

//! how much a species is liked by another
/*!	the liked_spec_id is the species id this species like
*		and the like factor is how much he likes it.
*		there is no way to know to which species belogns this 
*		like because they were inside the speciesinfo likings so
*		it is enstablished that this like belongs to a precise
*		species. in the example rabbit
*		example: if this liking is referred to a rabbit
*		the liked_spec_id is the id of a carrot
*		and the like_factor is how much from 1 to 100 the rabbit
*		likes the carrot
*		\see SpeciesInfo
*		\see SpeciesInfo::likings
*/
struct Like : public	eco::Container
{
	//! default constructor
	/*!	\param u_spec_id	the species id
	*		\param u_like_factor the like factor
	*/
	Like(	unsigned int 	u_spec_id 		= 0	, 
				int						u_like_factor	=	0	);

	~Like();
	//!	is the like full
	/*!	controll if the liked_spec_id is > 0
	*/
	bool is_full();
	
	//!	Like were sorted by ascendent spec_id
	bool operator<(const Like&	lk);

	//!	Like were sorted by ascendent spec_id
	bool operator>(const Like&	lk);

	//!	egual operator
	bool operator==(const Like& lk);

	//!	not equal operator
	bool operator!=(const Like& lk);

	//! print liked_species_id and like_factor
	friend	std::ostream &operator<<(	std::ostream 	&os		,
																		const Like		&like	);
	

	//! the species id liked
	unsigned int 	liked_spec_id;

	//!	how much the species is liked
	/*!	positive is attraction
	*		
	*		negative is repulsion
	*
	*		0 is 0
	*/
	int						like_factor;
};

#endif
