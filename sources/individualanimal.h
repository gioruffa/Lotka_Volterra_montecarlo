//-----------------------------------------------------------------------
//
//	File Name:	individualanimal.h
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

/*!	\file individualanimal.h
*		this file contains the interface of IndividualAnimal
*/


#ifndef INDIVIDUAL_ANIMAL
#define	INDIVIDUAL_ANIMAL

#include "animal.h"

//! class	IndividualAnimal
/*!	this class rappresent the animal see as the final and 
*		real living form of life.
*		each animal differs from the other by his appetite and
*		his libido
*/
class IndividualAnimal : public Animal
{
	public:
	//! default constructor
	/*!	set appetite and libido, if none
	*		were given they were setted to 0
	*		both.	
	*/
	IndividualAnimal(	unsigned int	u_appetite	=	0	,
										unsigned int	u_libido		=	0	);
	
	//! default destructor
	/*! does nothing
	*/
	~IndividualAnimal();

	//!compute id the animal is alive
	virtual bool is_alive();

	//set-get
		//set
	//!	set appetite
	unsigned int	&appetite()	;
	//! set libido
	unsigned int	&libido()		;
		//get
	//! get appetite
	unsigned int	appetite()	const	;
	//!	get libido
	unsigned int	libido()		const	;

	//operators
	//! ostream operator
	/*!	prints al main info of the animal
	*/
	friend std::ostream	&operator<<(	std::ostream 			&os,
															const IndividualAnimal	&an);

	private:
	//!	appetite factor
	/*!	it rappresents the propensity of the animal
	*		to eat and rise when the hp of the animal were under
	*		the health status
	*
	*		\see m_health_status
	*/
	unsigned int	m_appetite;

	//!	libido factor
	/*!	pay attention: parental controll pending. btw the libido factor
	*		determines the propensity of the animal to reproduce
	*		and rises when the animal's hp where under the health status
	*		\see m_health_status
	*/
	unsigned int m_libido;
};

#endif
