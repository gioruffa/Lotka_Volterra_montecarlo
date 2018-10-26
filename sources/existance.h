//-----------------------------------------------------------------------
//
//	File Name:	existance.h
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

/*!	\file existance.h
*	interface of the Abstract class Existance
*/

#ifndef EXISTANCE
#define EXISTANCE


#include "time.h"	//DateOfBirth



//!	class Existance the most abstracted object
/*!	this class is the most abstract object and
*	it's pure virtual class.
*	all the form of life in the program eredit from her
*/
class Existance
{
	public:
	//! the type of the id_number
	/*!	long unsigned int
	*/
	typedef	long unsigned int	id_type;
	
	//!default constructor
	Existance	(	DateOfBirth u_birth_date 	= DateOfBirth(0,0)	,
							id_type			u_id_number		=	0									);
	//!default destructor
	~Existance();

	//!	is the existance alive?
	/*!	returns true if the object is alive.
	*		existance cannot be whithout their
	*		specifications, so this member is 
	*		pure virtual
	*/
	virtual bool is_alive() = 0;

	//set-get
		//sets
	//!the birth date
	/*!da birth date
	*	\see DateOfBirth
	*	\see m_birth_date
	*/
	DateOfBirth	&birth_date();
	
	//!the id numer
	/*! \see m_id_number
	*/
	id_type	&id_number();

		//gets
	//! get the birth date
	const	DateOfBirth	&birth_date()	const;
	//!get the id_number
	id_type			id_number()	const;
		
	private:
	//!	when an Existace is created
	/*!	every form of life has a date of birth.
	*		see DateOfBirth for more info.
	*		\see DateOfBirth
	*/
	DateOfBirth	m_birth_date;
	
	//!unique identifier of a form of life
	/*!	this data member is different between all
	*		the form of life istanced.
	*		this property has to be granted by the gestion
	*		algorithm
	*/
	id_type	m_id_number;

};

#endif
