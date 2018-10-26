//-----------------------------------------------------------------------
//
//	File Name:	specied.cpp
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
*		implementation of Specied
*/

#include "specied.h"

//////////////////
// 	SPECIED
//////////////////

Specied::Specied	
			(	unsigned int 	u_species_id		,
				std::string 	u_species_name 	,
				unsigned int	u_life_coast		,
				unsigned int	u_health_status	,
				unsigned int	u_calories			,
				float					u_life_space		
			)
	:	m_species_id		(u_species_id)		,
		m_species_name	(u_species_name)	,
		m_life_coast		(u_life_coast)		,
		m_health_status	(u_health_status)	,
		m_calorie				(u_calories)			,
		m_life_space		(u_life_space)		
{	
}


Specied::~Specied()
{
}
/*
void Specied::live()
{
	hp() -= life_coast();
}
*/
bool Specied::is_alive()
{
	return hp()>=0;
}

unsigned int	&Specied::species_id()
{
	return m_species_id;
}

std::string		&Specied::species_name()	
{
	return m_species_name;
}

unsigned int	&Specied::life_coast()		
{
	return m_life_coast;
}

unsigned int	&Specied::health_status()
{
	return m_health_status;
}

unsigned int	&Specied::calorie()		
{
	return m_calorie;
}

float					&Specied::life_space()
{
	return m_life_space;
}


unsigned int	Specied::species_id()	const
{
	return m_species_id;
}

std::string		Specied::species_name()	const 
{
	return m_species_name;
}

unsigned int	Specied::life_coast()	const 
{
	return m_life_coast;
}

unsigned int	Specied::health_status() const 
{
	return m_health_status;
}

unsigned int	Specied::calorie()	const 
{
	return m_calorie;
}

float					Specied::life_space()	const 
{
	return m_life_space;
}


