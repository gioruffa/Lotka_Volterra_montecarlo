//-----------------------------------------------------------------------
//
//	File Name:	time.cpp
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

#include "time.h"


/////////////////////
//// ABSTRACT  //////
//// CLOCK     //////
/////////////////////

AbstractClock::AbstractClock()
{
}

AbstractClock::~AbstractClock()
{
}




/////////////////////
//// CLOCK     //////
/////////////////////

Clock::Clock(	long int 			u_abs 	, 
							double 				u_rel		)
	:	m_absolute_time(u_abs),
		m_relative_time(u_rel)
{
}

Clock::~Clock()
{
}

std::ostream& operator<<	(	std::ostream &os		,
														const Clock& clock	)
{
	os	<<	"absolute time: " << clock.abs() << std::endl
			<<	"relative time: " << clock.rel() << std::endl;
	return os;
}

void	Clock::calculate_relative()
{
}

void 	Clock::tick(unsigned int times)
{
	m_absolute_time += times;
	calculate_relative();
}


long int 	&Clock::abs()
{
	return m_absolute_time;
}

double		&Clock::rel()
{
	return m_relative_time;
}

const long int 	&Clock::abs()	const
{
	return m_absolute_time;
}

const double		&Clock::rel()	const
{
	return m_relative_time;
}

////////////////////////
////  DATE OF BIRTH  ///
////////////////////////

DateOfBirth::DateOfBirth(	long int 			u_abs		,
													double 				u_rel		)

	:	m_absolute(u_abs),
		m_relative(u_rel)
{
}

DateOfBirth::DateOfBirth(	const Clock& u_clock)
	:	m_absolute(u_clock.abs()),
		m_relative(u_clock.rel())
{
}



DateOfBirth::~DateOfBirth()
{
}

const long int 	&DateOfBirth::abs()	const
{
	return m_absolute;
}

const double		&DateOfBirth::rel()	const
{
	return m_relative;
}


std::ostream& operator<<(	std::ostream 				&os		,
													const DateOfBirth&	date	)
{
	os	<<	"abs: "	<<	date.abs()	<<	std::endl
			<<	"rel: "	<<	date.rel()	<<	std::endl;
	return os;
}
