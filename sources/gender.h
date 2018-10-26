//-----------------------------------------------------------------------
//
//	File Name:	gender.h
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

/*!	\file gender.h
*		\brief gender class interface
*
*
*/

#ifndef	GENDER
#define GENDER

#include <string>
#include <iostream>

//! the gender of the form of life
/*!	the possible gender of a form of life where: 
*		male , female, ermaphrodite, asexual;
*		
*		this is decided by the values of male_ and female_;
*
*		the ermaphrodite is male and female at the same time,
*		the asexual is nor male nor female.
*			
*		\see male_
*		\see female_
*
*/
class Gender
{
	public:
	//!	default constructor
	/*!	if no argument were given the form of life is considered 
	*		asexual
	*/
	Gender();

	//!	constructor using a string
	/*! \param r_gender is the string containing the gender specification
	*										given in runtime.
	*		possible values are: "male" , "female" , "ermaphrodite" , "asexual";
	*
	*		if the gender is speciefied badly the gender is set to asexual;
	*/
	Gender(std::string r_gender);
	
	//!default destructor
	~Gender();	

	//FUNCTIONS
	//! returns true if it is male
	bool 	is_male();
	//! returns true if it is female
	bool 	is_female();
	//! returns true if it is hermaphrodite
	bool	is_hermaphrodite();
	//! returns true if it is asexual
	bool	is_asexual();	

	//! get gender name
	const	std::string& gender() const;	

	//! change the sex
	/*!	need a string like the constructos
	*		\param r_gender see Gender()
	*/
	void	change_gender(std::string r_gender);

	//!	change the sex
	/*!	\param u_gender_num see numerical_gender
	*/
	void	change_gender(const unsigned int u_gender_num = 4);

	//!	numerical gender
	/*!	return a numerical id (int) which 
	*		rappresent the gender.
	*		1 is male, 2 is female, 3 is hermaphrodite 
	*		and 4 is asexual;
	*/
	unsigned int	numerical_gender() const;	
	
	//operators
	//!	operator <
	bool operator<(const Gender &gen) const
	{
	return numerical_gender()<gen.numerical_gender();
	}	

	//!	ostream operator of gender
	/*!	prints a string saying the actual gender
	*		is a wrapper of Gender::gender()
	*/
	friend std::ostream &operator<<(std::ostream &os , const Gender &gen);

	//PRIVATE MEMBERS
	private:
	//! if true is a male	
	bool	male_;
	//! if true is a female
	bool	female_;
	
	//!	name of the gender
	std::string gender_name_;
};

#endif
