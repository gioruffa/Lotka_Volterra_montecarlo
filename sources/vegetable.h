//-----------------------------------------------------------------------
//
//	File Name:	vegetable.h
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

/*!	\file vegetable.h
*		interface of class Vegetable
*/

#ifndef VEGETABLE
#define VEGETABLE

#include "specied.h"

//! class Vegetable
/*!	this class is present only for a modeling purpose. 
*		his implementation is given to future generations.
*/
class Vegetable : public Specied
{
	public:
	//! deafult constructor
	Vegetable();
	//! default destructor
	~Vegetable();

	private:
};

#endif
