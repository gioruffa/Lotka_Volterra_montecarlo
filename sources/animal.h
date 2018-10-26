//-----------------------------------------------------------------------
//
//	File Name:	animal.h
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

/*!	\file animal.h
*		interface of class Animal
*/

#ifndef ANIMAL
#define ANIMAL

#include "specied.h"	//ancestor class Specied

//!	class Animal
/*!	this class rappresents the animal as a form of life able to move
*		and eat. animals can eat other animals or vegetables. it depends
*		from the liking between the species of the two form of lifes
*/
class Animal : public Specied 
{
	public:
	//! default constructor
	/*!	does nothing
	*/
	Animal(	unsigned int	u_fight_coast = 1	);
	
	//! default destructor
	/*! does nothing
	*/
	~Animal();

	//!	implementation of Specied::live()
	/*!	\see Specied::live()
	*/
	virtual void live();

	//set-get
		//set
	//!	set fight coast
	unsigned int	&fight_coast();
		//get
	//!	get fight coast
	unsigned int fight_coast()	const;

	private:
	//!	fight coast is the coast to pay everytime a fight occours
	/*!	the pay coast is suctract from the hp both of the prey and
	*		the predator.
	*/
	unsigned int	m_fight_coast;

};

#endif
