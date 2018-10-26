//-----------------------------------------------------------------------
//
//	File Name:	test_like_operators.cpp
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

#include <iostream>
#include <set>

#include "like.h"

#include "classcompares.hpp"

int main()
{
	Like rhs(2,0);
	Like lhs(3,0);

/*
	if (rhs != lhs)
	{
		std::cout	<<	std::endl
							<<	"NOT EQUAL"
							<<	std::endl;
	}
	else
	{
		std::cout	<<	std::endl
							<<	"EQUAL"
							<<	std::endl;
	}
*/
	std::set<Like , LikeFactorCmp>	set_try;
	set_try.insert(Like(1,0));
	set_try.insert(Like(2,0));
//	set_try.insert(Like());
//	set_try.insert(Like());


	std::cout	<<	std::endl;
	for (	std::set<Like>::const_iterator it = set_try.begin() ;
				it != set_try.end();
				it ++	)
	{
		std::cout	<<	*it
							<<	std::endl;
	}

	return 0;
}
