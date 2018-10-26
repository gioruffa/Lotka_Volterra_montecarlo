//-----------------------------------------------------------------------
//
//	File Name:	fieldchangers.hpp
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

/*!	\file fieldchangers.hpp
*		this file include numberous functors
*		(or unary function) used to change fields
*		of individual animals or individual vegetables 
*		(passed to the index::modify() member).
*		
*/

#ifndef FIELDCHANGERS
#define	FIELDCHANGERS

#include "beeings.h"

#ifdef SAFE
//!functor to change IndividualAnimal hp
/*!	this version does not controll if hp's where
*		more than 100 or less than 0
*/
struct change_animal_hp
{
	public:
	//! constructor setting new hp value
	change_animal_hp(unsigned int &nw_hp)
		: m_nw_hp(nw_hp)	{}

	//!change the value
	void operator()	(IndividualAnimal &an)
	{
		an.hp() = m_nw_hp;
	}

	private:
	//!new hp value
	unsigned int m_nw_hp;

};
#endif
#ifndef SAFE

//!functor to change IndividualAnimal hp
/*!	this version controll if the hp passed
*		are negative. if so set it to 0
*		if they'r more than 100 set it to 0 
*		because of uncorrect usage of unsigned int
*/
struct change_animal_hp
{
	public:
	//! constructor setting new hp value
	change_animal_hp(int  &nw_hp)
		: m_nw_hp(nw_hp)	{}

	//!change the value
	void operator()	(IndividualAnimal &an)
	{
		if (m_nw_hp < 0)
		{
			#ifdef VERBOSE
				std::cerr	<<	std::endl
									<<	"WARNING: hp field negative "
									<<	m_nw_hp
									<<	" ,will be setted to 0"
									<<	std::endl;
			#endif
			m_nw_hp = 0;
		}
		if (m_nw_hp > 100)
		{
			#ifdef VERBOSE
				std::cerr	<<	std::endl
									<<	"WARNING: hp field is > than 100 "
									<<	m_nw_hp
									<<	" ,will be setted to 0"
									<<	std::endl;
			#endif
			m_nw_hp = 0;
		}

		an.hp() = m_nw_hp;
	}

	private:
	//!new hp value
	int m_nw_hp;

};

#endif


//!functor to cahnge IndividualAnimal libido
struct change_animal_libido
{
	public:
	//! constructor setting new lidibo value
	change_animal_libido(unsigned int nw_libido)
		: m_nw_libido(nw_libido)	{}

	//!change the value
	void operator()	(IndividualAnimal &an)
	{
		an.libido() = m_nw_libido;
	}

	private:
	//! new libido value
	unsigned int m_nw_libido;

};

//!	functor to change IndividualAnimal appetite
struct change_animal_appetite
{
	public:
	
	//! constructor setting new appetite value
	change_animal_appetite(unsigned int nw_appetite)
		: m_nw_appetite(nw_appetite)	{}

	//!change the value
	void operator()	(IndividualAnimal &an)
	{
		an.appetite() = m_nw_appetite;
	}

	private:
	//! new appetite value
	unsigned int m_nw_appetite;

};


#endif

