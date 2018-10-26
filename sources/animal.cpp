//-----------------------------------------------------------------------
//
//	File Name:	animal.cpp
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

/*!	\file animal.cpp
*		implementation of class Animal
*/


#include "animal.h"

//////////////////
// 	ANIMAL
//////////////////

Animal::Animal
			(	unsigned int u_fight_coast )
	:	m_fight_coast	(u_fight_coast)
{
}

Animal::~Animal()
{
}


void Animal::live()
{
	hp() = hp() - life_coast();
}

unsigned int	&Animal::fight_coast()
{
	return	m_fight_coast;
}

unsigned int Animal::fight_coast()	const
{
	return m_fight_coast;
}

