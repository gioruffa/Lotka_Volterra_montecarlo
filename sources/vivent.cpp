//-----------------------------------------------------------------------
//
//	File Name:	vivent.cpp
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

/*!	\file vivent.h
*		this file contains the implementation of Vivent	
*/

#include "vivent.h"

//////////////////
// 	VIVENT
//////////////////

Vivent::Vivent
			(	unsigned int u_hp	, 
				Gender u_gender		)
	:	m_hp			(u_hp)		,
		m_gender	(u_gender)
{
}

Vivent::~Vivent()
{
}

unsigned int	&Vivent::hp()
{
	return m_hp;
}

unsigned int 	Vivent::hp()	const
{
	return m_hp;
}

Gender	&Vivent::gender()
{
	return m_gender;
}

Gender	Vivent::gender()	const
{
	return m_gender;
}

