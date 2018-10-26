//-----------------------------------------------------------------------
//
//	File Name:	individualvegetable.cpp
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

/*!	\file individualvegetable.h
*		implementation of IndividualVegetable
*/

#include <iostream>

#include "individualvegetable.h"
//////////////////
// 	INDIVIDUAL
//	VEGETABLE
//////////////////

IndividualVegetable::IndividualVegetable()
{
}

IndividualVegetable::~IndividualVegetable()
{
}

//!	prints the individual vegetable
/*!	prints id_number , species_id and hp
*/
std::ostream &operator<<(	std::ostream &os								,
													const IndividualVegetable &veg	)
{
	os	<< "#id_number:	"	<<	veg.id_number() 	<< std::endl
			<< "#spec_id:	"		<<	veg.species_id()	<< std::endl
			<< "#hp:	"					<<	veg.hp()					<< std::endl;	
	
	return os;	
}
