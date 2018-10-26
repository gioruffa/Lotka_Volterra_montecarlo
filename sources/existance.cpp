//-----------------------------------------------------------------------
//
//	File Name:	existance.cpp
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

/*!	\file existance.cpp
*	implementation of the Abstract class Existance
*/


#include "existance.h"

//////////////////
// 	EXISTANCE
//////////////////


Existance::Existance	
			(	DateOfBirth 				u_birth_date	,	
				Existance::id_type	u_id_number		
			)
	:	m_birth_date	(u_birth_date)	,
		m_id_number		(u_id_number)
{
}

Existance::~Existance ()
{
}

DateOfBirth	&Existance::birth_date()
{
	return m_birth_date;
}

Existance::id_type	&Existance::id_number()
{
	return	m_id_number;
}


const DateOfBirth	&Existance::birth_date()	const
{
	return m_birth_date;
}


Existance::id_type	Existance::id_number() const
{
	return	m_id_number;
}


