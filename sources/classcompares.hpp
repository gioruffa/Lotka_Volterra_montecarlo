//-----------------------------------------------------------------------
//
//	File Name:	classcompares.hpp
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

/*!	\file classcompares.hpp
*		file containing the functors used to compare classes
*/

#ifndef CLASSCOMPARES
#define CLASSCOMPARES
//!	used in SpeciesInfo.h
/*!	compares the like_factors of 2 like giving rhe 
*		biggerone. this is done to have the highest 
*		Like first.
*		\see SpeciesInfo::m_likings_by_lk_factor
*/

struct LikeRefCmp
	{
		//! returns lhs>rhs
		bool operator()(const unsigned int &lhs ,
										const unsigned int &rhs	)
		{return lhs>rhs;}
	};


struct LikeFactorCmp
{
	bool operator () (const Like &lhs ,
										const Like &rhs	)
	{return lhs.like_factor > rhs.like_factor;}
};
#endif
