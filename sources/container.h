//-----------------------------------------------------------------------
//
//	File Name:	container.h
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

#ifndef CONTAINER
#define CONTAINER

namespace eco
{

//!Container abstract class
class Container
{
	public:
	//! default constructor
	Container();
	//! default destructor
	~Container();
	
	//! is the container full
	/*!	abstract member
	*/
	virtual bool is_full() = 0 ;	

	private:

};

}
#endif
