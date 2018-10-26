//-----------------------------------------------------------------------
//
//	File Name:	vivent.h
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
*		this file contains the interface of Vivent
*		abstract class
*/


#ifndef	VIVENT
#define VIVENT

#include "existance.h"		//ancestor class Existance
#include "miscellaneus.h"	//Gender

//!	class Vivent contain HP
/*!	the form of life seen as an object able to live
*/

class Vivent : public Existance
{
	public:
	//! default constructor
	/*!	set's dm, default is hp = 100, gender asexual;	
	*/
	Vivent( unsigned int u_hp = 100							,
					Gender u_gender = Gender("asexual")	);
	
	//! default destructor
	/*! does nothing
	*/
	~Vivent();

	//!	is_alive
	/*!	\see Existance::is_alive()
	*/
	virtual bool is_alive() = 0;

	//set-get
		//set
	//!the hp
	/*!	\see m_hp	
	*/
	unsigned int	&hp();

	//!the gender
	/*!	\see m_gender	
	*/
	Gender				&gender();
	
		//get
	//! get the hp
	unsigned int	hp()	const;
	//! get the gender
	Gender				gender()	const;

	private:
	//!	HP most important parameter
	/*! it rappresents the health of the form of life
	*		whith a value of 0 the animal is dead
	*		whith a value of 100 the animal feels very good
	*/
	unsigned int		m_hp;
	
	//!	gender of the form of life
	/*!	every form of life could be male, female, hermaphrodite (both) or asexual (nothing).
	*		it's not in our purpose to implement vegetable's gender 
	*		and hermaphrodite. but, for correctnes and thinking to future
	*		realises, gender is included in this adstract class.
	*		gender is default setted to no asexual.
	*/
	Gender m_gender;
};

#endif
