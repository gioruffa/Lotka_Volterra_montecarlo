//-----------------------------------------------------------------------
//
//	File Name:	like.cpp
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

#include "like.h"

	Like::Like(	unsigned int 	u_spec_id 		, 
							int						u_like_factor	)
	:	liked_spec_id	(u_spec_id),
		like_factor		(u_like_factor)
	{}

	Like::~Like()
	{}


	bool Like::is_full()
	{
		if (liked_spec_id > 0) return true;
		else return false;
	}
		
	bool Like::operator<(const Like&	lk)
	{	
		return liked_spec_id < lk.liked_spec_id;
	}

	bool Like::operator>(const Like&	lk)
	{	
		return liked_spec_id > lk.liked_spec_id;
	}

	bool Like::operator==(const Like& lk)
	{
		bool are_equal = true;
		if (liked_spec_id != lk.liked_spec_id) \
			are_equal = false;
		if (like_factor != lk.like_factor)\
			are_equal = false;

		std::cerr	<<	std::endl
							<<	"INSIDE =="
							<<	std::endl;	

		return are_equal;
	}
	bool Like::operator!=(const Like& lk)
	{
		bool are_not_equal = false;
		if (liked_spec_id != lk.liked_spec_id) \
			are_not_equal = true;
		if (like_factor != lk.like_factor)\
			are_not_equal = true;	

		std::cerr	<<	std::endl
							<<	"INSIDE !="
							<<	std::endl;	

		return are_not_equal;
	}
	
	bool operator==(const Like& lhs , const Like& rhs)
	{
		bool are_equal = true;
		if (lhs.liked_spec_id != rhs.liked_spec_id) \
			are_equal = false;
		if (lhs.like_factor != rhs.like_factor)\
			are_equal = false;
		
		std::cerr	<<	std::endl
							<<	"OUTSIDE =="
							<<	std::endl;
		
		return are_equal;
	}

	bool operator!=(const Like& lhs , const Like& rhs)
	{
		std::cerr	<<	std::endl
							<<	"OUTSIDE !="
							<<	std::endl;	
		return !(lhs==rhs);
	}

	//! print liked_species_id and like_factor
	std::ostream &operator<<(	std::ostream 	&os		,
														const Like		&like	)
	{
		os << "#"	<< like.liked_spec_id	
							<< " <---> "
							<< like.like_factor	;
		return os;
	}

	bool operator< (const Like& lhs , const Like& rhs)
	{
		return lhs.liked_spec_id < rhs.liked_spec_id;	
	}
	bool operator> (const Like& lhs , const Like& rhs)
	{
		return lhs.liked_spec_id > rhs.liked_spec_id;	
	}

