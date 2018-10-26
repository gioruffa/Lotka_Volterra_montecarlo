//-----------------------------------------------------------------------
//
//	File Name:	controller.h
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

/*!	\file controller.h
*		this file contains the container abstract class
*/

#ifndef CONTROLLER
#define CONTROLLER


//! class that generally controll

class Controller
{
	public:
	//!default constructor
	Controller();
	//!default destructor
	~Controller();

	//! check what is to controll
	/*!	abstarct function
	*/
	virtual bool check() = 0;

};



#endif
