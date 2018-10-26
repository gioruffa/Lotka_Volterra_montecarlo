//-----------------------------------------------------------------------
//
//	File Name:	steplog.hpp
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

#include "populationvariation.hpp"
#include <vector>
#include <iostream>

//!	log of the function step
/*!	this class is a wrapper containg
*		population variations.
*		this class is a parameter of 
*		Ecosystem::step. each variation is parsed by step function
*		and modify the rappresentation of the ecosystem.
*		
*		in future realises this class could be used to produce efficiently
*		stats of population trend.
*	
*		\see PopulationVariation
*		\see EcosystemContainer::step
*/

struct StepLog
{
	//!default contructor
	StepLog()
	{
	}
	//!default destructor
	~StepLog()
	{
	}
	//data members
	//!population variation
	/*!
	*		\see PopulationVatiation
	*/
	std::vector<PopulationVariation>	variations;
};

