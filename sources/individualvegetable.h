//-----------------------------------------------------------------------
//
//	File Name:	individualvegetable.h
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

/*!	\file individualvegetable.h
*		interface of IndividualVegetable
*/

#ifndef	INDIVIDUAL_VEGETABLE
#define	INDIVIDUAL_VEGETABLE

#include "vegetable.h"

//!	class	IndividualVegetable
/*!	class rappresenting the vegetable seen as a single 
*		form of life. for this implementation no more features
*		are added in order to differentiate IndividualVegetable
*		to Specied. by the way this had been done to give the 
*		future possibiliti to implement vegetable reproduction
*		or feeding.
*/
class IndividualVegetable : public Vegetable
{
	public:
	//! default contructor
	IndividualVegetable();
	//! default destructor
	~IndividualVegetable();

	//!	prints the individual vegetable
	/*!	prints id_number , species_id and hp
	*/
	friend	std::ostream &operator<<(	std::ostream &os							,
																		const IndividualVegetable &veg);
	private:
};

#endif
