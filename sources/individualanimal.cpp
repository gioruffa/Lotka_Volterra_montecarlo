//-----------------------------------------------------------------------
//
//	File Name:	individualanimal.cpp
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

/*!	\file individualanimal.cpp
*		contains the implementation of IndividualAnimal
*/

#include <iostream>	//cout (operator <<)

#include "individualanimal.h"

//////////////////
// 	INDIVIDUAL
//	ANIMAL
//////////////////

IndividualAnimal::IndividualAnimal
			(	unsigned int	u_appetite	,
				unsigned int	u_libido		)
	:	m_appetite	(u_appetite)	,
		m_libido		(u_libido)
{
}

IndividualAnimal::~IndividualAnimal()
{
}

bool IndividualAnimal::is_alive()
{
	return hp() >= 0;
}

unsigned int	&IndividualAnimal::appetite()	
{
	return m_appetite;
}

unsigned int	&IndividualAnimal::libido()		
{
	return m_libido;
}

unsigned int	IndividualAnimal::appetite()	const	
{
	return m_appetite;
}

unsigned int	IndividualAnimal::libido()		const	
{
	return m_libido;
}



//operators
std::ostream &operator<<(	std::ostream 			&os,
										const	IndividualAnimal	&an)
{
	os	<< "#id_number:	"	<<	an.id_number() 	<< std::endl
			<< "#spec_id:	"		<<	an.species_id()	<< std::endl
			<< "#Gender:	"			<<	an.gender()			<< std::endl
			<< "#hp:	"					<<	an.hp()					<< std::endl;
	return os;
}
