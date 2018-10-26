//-----------------------------------------------------------------------
//
//	File Name:	time.h
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

/*!	\file	time.h
*		\brief	classes to manipulate and determine the time of the system
*
*		is really difficult to determine time in this context.
*		
*		we have developed two types of time: relative and absolute
*
*		relative time:
*		the easyest way is to think to what is a ecosistem cicle.
*		An ecosistem cicle is concluded when all the animals in the ecosistem where called.
*		for "call" we intend every time a form of life interact with another form of life,
*		so if an animal fight whith another this constitutes o total of 2 calls)
*		as you can imagine it could be really difficult to controll that all the animals were called
*		so if we give to te ecosistem cicle a value of 1, each call to an animal has the time value
*		interval of 1/total_number_of_forms_of_life present in the ecosistem.
*		so the running relative time is the number of cicles passed and the quantiti of the cicle running
*
*		absolute time:
*		nothing different from the number of calls occured from the creation of the first form of life
*
*		
*/

#ifndef TIME
#define TIME

#include <iostream>


//! abstract class for the clock
class AbstractClock
{
	public:
	AbstractClock();
	~AbstractClock();

	//! make the clock tick
	/*!	abstract function
	*/
	virtual void tick(unsigned int times = 1) = 0 ;
};

//!	real clock able to give the time of the sistem
/*!	give the absolute and relative time of the sistem
*/
class Clock : public AbstractClock
{
	public:
	//! prints the time
	friend std::ostream&	operator<<(	std::ostream &os					,
															const Clock& clock	);
	
	//!constructor giving initial conditions
	/*!	construcor giving the initial absolute
	*		time and relative time.
	*		default is 0,0
	*		
	*		\param u_abs starting absolute time
	*		\param u_rel starting relative time
	*/
	Clock(	long int 	u_abs = 0,
					double		u_rel = 0);
	~Clock();
	
	//! make the clock tick
	/*!	increase the m_absolute time and
	*		calculate the relative time
	*/
	virtual	void	tick(unsigned int times = 1);
	
	//! calculate the relative time
	void calculate_relative();

	//!set abs
	long int 	&abs();
	//!set rel
	double		&rel();

	//!get abs
	const long int 	&abs() const;
	//!get rel 
	const double		&rel() const;

	private:
	//!absolute time
	long int 	m_absolute_time;
	//!relative time
	double		m_relative_time;
	
};



//! simple class for the date of birh
/*!	this class is a simple wrapper that contain the information relatives to the 
*		date of life of the form of life. it contains the relative and the absolute 
*		date of birth
*/

class DateOfBirth
{
	public:
	//! stream to print the DateOfBirth
	friend std::ostream& operator<<(std::ostream 			&os		,
																	const DateOfBirth	&date	);

	//!default creator
	/*!	gives all 0 value, not assign
	*		clock_ref
	*/
	DateOfBirth(	long int			u_abs = 0	,
								double				u_rel	= 0	);
	
	//!creator whith a clock
	/*!	set the birth date using the clock
	*		you pass it
	*/
	DateOfBirth(const Clock& u_clock);
	
	
	//!default destructor
	~DateOfBirth();


	//set-get
	//!get abs
	const long int 	&abs()	const;	
	//!set abs
	long int				&abs();
	//!get rel
	const double		&rel() const;
	//!set rel
	double					&rel();

	private:
	//! absolute date of birth
	/*! date of birth expressed as the number of calls occurred before the creation
	*		of the form of life
	*/
	long int 	m_absolute;
	//!	relative date of birth
	/*! date of birth expressed as number of cicle occurred before the creation of
	*		the form of life
	*/
	double		m_relative;
};

#endif
