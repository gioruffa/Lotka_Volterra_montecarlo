//-----------------------------------------------------------------------
//
//	File Name:	gender.cpp
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

/*!	\file gender.cpp
*		class Gender implementation
*/


#include <iostream>

#include "gender.h"

Gender::Gender()
{
	male_ 	= false;
	female_	=	false;
}

Gender::~Gender()
{
}

Gender::Gender(std::string r_gender)
{
	if 			(r_gender == "male")
	{
		male_ 	= true;
		female_ = false;
		gender_name_ = "male";
	}
	else if	(r_gender == "female")
	{
		male_ 	=	false;
		female_	=	true;
		gender_name_ = "female";
	}

	// really easy to do it wrong so i put some casistic
	else if	(			r_gender == "hermaphrodite" 
						or 	r_gender == "ermaphrodite"
						or	r_gender == "ermaprodite"
						or	r_gender == "hermafrodite"
						or	r_gender == "ermafrodite"
					)
	{
		male_		= true;
		female_	=	false;
		gender_name_ = "hermaphrodite";
	}
	else if (r_gender == "asexual")
	{
		male_		= false;
		female_	= false;
		gender_name_ = "asexual";
	}
	else
	{
		male_		= false;
		female_	= false;
		gender_name_ = "asexual";
	}

}

const std::string& Gender::gender()	const	{return gender_name_;}

bool Gender::is_male()	
{
	if (gender_name_ == "male") return true;
	else return false;
}

bool Gender::is_female()	
{
	if (gender_name_ == "female") return true;
	else return false;
}

bool Gender::is_hermaphrodite()	
{
	if (gender_name_ == "hermaphrodite") return true;
	else return false;
}

bool Gender::is_asexual()	
{
	if (gender_name_ == "asexual") return true;
	else return false;
}

void	Gender::change_gender(std::string r_gender)
{
	if 			(r_gender == "male")
	{
		male_ 	= true;
		female_ = false;
		gender_name_ = "male";
	}
	else if	(r_gender == "female")
	{
		male_ 	=	false;
		female_	=	true;
		gender_name_ = "female";
	}

	// really easy to do it wrong so i put some casistic
	else if	(			r_gender == "hermaphrodite" 
						or 	r_gender == "ermaphrodite"
						or	r_gender == "ermaprodite"
						or	r_gender == "hermafrodite"
						or	r_gender == "ermafrodite"
					)
	{
		male_		= true;
		female_	=	false;
		gender_name_ = "hermaphrodite";
	}
	else if (r_gender == "asexual")
	{
		male_		= false;
		female_	= false;
		gender_name_ = "asexual";
	}
	else
	{
		male_		= false;
		female_	= false;
		gender_name_ = "asexual";
	}

}

void	Gender::change_gender(const unsigned int u_gender_num)
{
	switch(u_gender_num)
	{
		case 1:	//male
			male_ 	= true;
			female_ = false;
			gender_name_ = "male";
			break;

		case 2:	//female
			male_ 	=	false;
			female_	=	true;
			gender_name_ = "female";
			break;

		case 3:	//erma
			male_		= true;
			female_	=	false;
			gender_name_ = "hermaphrodite";
			break;
		
		case 4:	//asex
			male_		= false;
			female_	= false;
			gender_name_ = "asexual";
			break;

		default:
			std::cerr	<<	std::endl	<<	"WARNING: "	<<	std::endl
								<<	"numerical gender entry not valid."
								<<	" Only integers from 1 to 4 (included) are ammitted"
								<<	std::endl	<<	"You courrently passed the value: "	
								<<	u_gender_num <<	std::endl 
								<<	"	ATTENTION: gender will be set to value 4 as \"asexual \" "
								<<	std::endl	<<	std::endl;
			male_		= false;
			female_	= false;
			gender_name_ = "asexual";
	}
}


unsigned int Gender::numerical_gender() const
{
	if (gender_name_ == "male") 					return 1;

	if (gender_name_ == "female") 				return 2;

	if (gender_name_ == "hermaphrodite") 	return 3;

	if (gender_name_ == "asexual") 				return 4;

	//if none it's asexual
	return 4;	
}

//!	ostream operator of gender
/*!	prints a string saying the actual gender
*		is a wrapper of Gender::gender()
*/
std::ostream &operator<<(	std::ostream &os 	, 
													const Gender &gen	)
{
	os	<< gen.gender();
	return os;
}
