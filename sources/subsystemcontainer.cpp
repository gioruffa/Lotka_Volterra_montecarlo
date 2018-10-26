//-----------------------------------------------------------------------
//
//	File Name:	subsystemcontainer.cpp
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

//!	\file	subsystemcontainer.cpp
/*!	implementaions of subsystemcontainer.h
*/

#include <iostream>

#include "subsystemcontainer.h"

/////////////////////
//	SUBSYSTEM
//	CONTAINER
/////////////////////
typedef SubsystemContainer	SubC;


SubsystemContainer::SubsystemContainer
			(	unsigned int u_x,
				unsigned int u_y	)
	:	m_x_position(u_x)	,
		m_y_position(u_y)
{
}

SubsystemContainer::~SubsystemContainer()
{
}


/**************
* 	set-get		*
**************/

	/*** set ***/

SubC::subsystem_tp	&SubC::sub_ecosystem()
{
	return 	m_sub_ecosystem;
}

SubC::animal_set	&SubC::animal_sub_ecosystem()
{
	return	m_sub_ecosystem.first;
}

SubC::vegetable_set	&SubC::vegetable_sub_ecosystem()
{
	return	m_sub_ecosystem.second;
}

unsigned int	&SubC::x_position()	
{
	return	m_x_position;
}

unsigned int	&SubC::y_position()	
{
	return	m_y_position;
}

	/*** get ***/

const	SubC::subsystem_tp	&SubC::sub_ecosystem() const
{
	return m_sub_ecosystem;
}

const	SubC::animal_set	&SubC::animal_sub_ecosystem()	const
{
	return	m_sub_ecosystem.first;
}

const	SubC::vegetable_set	&SubC::vegetable_sub_ecosystem()	const
{
	return	m_sub_ecosystem.second;
}

unsigned int	SubC::x_position()	const
{
	return	m_x_position;
}

unsigned int	SubC::y_position()	const
{
	return	m_y_position;
}


/**************
*		members		*
**************/





bool	SubC::is_full(Specied &sample)
{
	return is_full(sample.species_id());
}



bool	SubC::is_full(const unsigned int u_spec_id )
{
	//appo references
	SubC::index_an_by_spec_id&		an_index	=
		animal_sub_ecosystem().get<SubC::spec_id>()	;
	
	SubC::index_veg_by_spec_id&	veg_index	=
		vegetable_sub_ecosystem().get<SubC::spec_id>();

	//bool controll variables
		//is there any animal or vegetable?
	bool	is_ani = false , is_veg = false;

	//is there any animal whith this id?
	an_spec_id_const_it an_search_it = 
		an_index.find(u_spec_id);

	if(an_search_it != an_index.end())
	{
		is_ani = true;

	#ifdef DEBUG
			std::cerr	<<	std::endl
								<<	"ANIMAL FINDED"	<< std::endl
								<<	*an_search_it
								<<	std::endl;
		
	#endif

	}

	if (an_search_it ==	an_index.end())
	{	
	#ifdef DEBUG
		std::cerr	<< std::endl
							<<	"NO ANIMAL FOUND WHITH SPEC_ID "
							<<	u_spec_id	<< std::endl;
	#endif
		is_ani =	false;
	}


	//is there any vegetable whith this id?
	veg_spec_id_const_it veg_search_it = 
		veg_index.find(u_spec_id);
	
	if (veg_search_it	!=	veg_index.end())
	{
		is_veg = true;	

	#ifdef DEBUG
			std::cerr	<<	std::endl
								<<	"VEGETABLE FINDED"	<< std::endl
								<<	*veg_search_it
								<<	std::endl;
		
	#endif

	}

	if (veg_search_it ==	veg_index.end())	
	{
	#ifdef DEBUG
		std::cerr	<< std::endl
							<<	"NO VEG FOUND WHITH SPEC_ID "
							<<	u_spec_id	<< std::endl;
	#endif
		is_veg =	false;
	}
	
	
	#ifdef DEBUG
		std::cerr	<<	std::endl	<< "controll structures:"
							<<	std::endl
							<< 	"is ani: "	<< is_ani	<< std::endl
							<<	"is veg: "	<< is_veg << std::endl;
	#endif

	//if there is no animal or vegetable the container is empty
	if (is_ani == false \
				and \
			is_veg == false	)
	{	
		#ifdef DEBUG
			std::cerr	<<	std::endl	<< "Subsys container empty"
								<<	std::endl;
		#endif
		return false;
	}

	//	there could be an error. we could have both an animal and a 
	//	vegetable with the same id. this is not allowed in this model	
	if (is_ani ==	true \
				and \
			is_veg == true )
	{
		std::cerr	<<	std::endl	<< "ERROR: "
							<<	"the Subsystem container ( "
							<<	x_position()	<<	", "
							<<	y_position()	<<	") "
							<<	"contains an animal and a vegetable "
							<<	"whith the same species_id = "
							<<	u_spec_id	<<	". "	
							<<	"Species could be only vegetable or "
							<<	"animal. (no spongebob please) "
							<<	std::endl;

		return true;	//this prevent form insertion of this animal
	}

	//if the container contains at least one animal it controlls
	//the amount of free space for this animal using the variable
	//Specied::m_life_space
	
		//first get the m_life_space variable 
		//from the finded animal;
	float 				vivent_ls;			//<-	life_space
	unsigned int 	vivent_number;	//<-	num of vivent with this spec_id
	float					available_space;//<-	available space for this species
	
	//in the case we are looking for an animal
	if (is_ani)	
	{
		vivent_ls 			= an_search_it->life_space();
		vivent_number 	= an_index.count(u_spec_id);
	}

	//in the case we are looking for a vegetable
	if (is_veg)	
	{
		vivent_ls = veg_search_it->life_space();
		vivent_number = veg_index.count(u_spec_id);
	}
	//compute the remaining space


	available_space	= 100. - (vivent_ls * vivent_number);

	#ifdef DEBUG
		std::cerr	<<	std::endl	
							<<	"AVAILABLE SPACE for spec_id : "
							<<	u_spec_id	<<	std::endl
							<<	available_space	<<	std::endl
							<<	"vivent numbers "	<<	vivent_number	<<	std::endl
							<<	"vivent life_space "	<<	vivent_ls << std::endl
							<< 	std::endl;
	#endif
	
	if (available_space >= vivent_ls)	return false;
	else	return true;
	
}

bool	SubC::is_full()
{
	return false;
}


bool	SubC::insert(IndividualAnimal	&an)
{
	//controll if there is space enought
	if(is_full(an))
	{
		std::cerr	<< 	std::endl
							<< 	"WARNING: "
							<<	"Failed insertion of animal: " << std::endl
							<<	an	<< std::endl
							<<	"ERROR: the container is full for this species"
							<<	std::endl;
		return false;
	}
	
	//insert and controll if the insertion succeed
	//see multi_index::insert() for details
	if	(animal_sub_ecosystem().insert(an).second == true)
	{
		return true;
	}
	else 
	{
		std::cerr	<<	std::endl	<<	"ERROR: "	
							<<	"Failed insertion of animal: " << std::endl
							<<	an	<< std::endl;
		return false;
	}

}


bool	SubC::insert(IndividualVegetable &veg)
{
	//controll if there is space enought
	if(is_full(veg))
	{
		std::cerr	<< 	std::endl
							<< 	"WARNING: "
							<<	"Failed insertion of animal: " << std::endl
							<<	veg	<< std::endl
							<<	"ERROR: the container is full for this species"
							<<	std::endl;
		return false;
	}

	//insert and controll if the insertion succeed
	//see multi_index::insert() for details
	if	(vegetable_sub_ecosystem().insert(veg).second == true)
	{
		return true;
	}
	else 
	{
		std::cerr	<<	std::endl	<<	"ERROR: "	
							<<	"Failed insertion of vegetable: " << std::endl
							<<	veg	<< std::endl;
		return false;
	}

}


bool	SubC::remove(const long unsigned int u_id)
{	
		//appo reference to the index
	SubC::index_an_by_id&		an_index	=
		animal_sub_ecosystem().get<SubC::id>()	;
	
		//erase returns the number of erased elements
	if(	an_index.erase(u_id) > 0)	return true;
	else	return false;
}


SubC::an_id_it	SubC::find_animal(const long unsigned int u_id)
{
		//appo reference to the index
	SubC::index_an_by_id&		an_index	=
		animal_sub_ecosystem().get<SubC::id>()	;
		
		//iterator to found element
	SubC::an_id_it	found_it;
	
	found_it	=	an_index.find(u_id);

	return found_it;
}

SubC::veg_id_it	SubC::find_vegetable(const long unsigned int u_id)
{
		//appo reference to the index
	SubC::index_veg_by_id&	veg_index	=
		vegetable_sub_ecosystem().get<SubC::id>();


		//iterator to found element
	SubC::veg_id_it	found_it;
	
	found_it	=	veg_index.find(u_id);

	return found_it;
}


std::pair< unsigned int , bool> SubC::count_vivents
														(	const unsigned int 			u_spec_id		,
															SpeciesController	u_spec_con	)
{
	if (u_spec_id == 0 or u_spec_con.check() == false)
	{
		std::cerr	<<	std::endl
							<<	"WARNING: "
							<<	"in SubsystemContainer::count_vivents: "
							<<	"the species id or the subsystem container you entered "
							<<	"where not valid. "
							<<	std::endl;
		return std::pair<unsigned int , bool> (0,false);
	}

	//get the species info
	SpeciesController::infos_const_it spec_inf_it = 
		u_spec_con.get_info(u_spec_id);

	//controoll if the info exists
	if (spec_inf_it == u_spec_con.get_infos().end())
	{
		std::cerr	<<	std::endl
							<<	"WARNING: "
							<<	"in SubsystemContainer::count_vivents: "
							<<	"the species id you entered "
							<<	"does not exists. "
							<<	std::endl;
		return std::pair<unsigned int , bool> (0,false);
	}

	return (count_vivents (spec_inf_it->second));


}


std::pair< unsigned int , bool> SubC::count_vivents	
															(const SpeciesInfo u_spec_info)
{
	if (u_spec_info.species_id == 0)
	{
		std::cerr	<<	std::endl
							<<	"WARNING: "
							<<	"in SubsystemContainer::count_vivents: "
							<<	"the species info you entered"
							<<	"is not valid. forgot to pass it??? "
							<<	std::endl;
		return std::pair<unsigned int , bool> (0,false);
	}
	
	//is animal or vegetable
		//need to know where to search

	std::pair<unsigned int , bool> to_return (0,false);

	if (u_spec_info.is_animal)
	{
		//get the container
		const animal_set&	set = animal_sub_ecosystem();
	
		//get the spec_id_indices
		const index_an_by_spec_id&	index	=	set.get<spec_id>();

		//count
		unsigned int number = index.count(u_spec_info.species_id);
		to_return.first 	= number;
		to_return.second 	= true;
	}
	else	//is vegetable
	{
		//get the container
		const vegetable_set& set = vegetable_sub_ecosystem();

		//get the spec_id_indices
		const index_veg_by_spec_id&	index = set.get<spec_id>();

		//count
		unsigned int number	=	index.count(u_spec_info.species_id);
		to_return.first		=	number;
		to_return.second	=	true;
	}

	//return
	return to_return;

}

/**************
*		operators	*
**************/
	//!	ostream operator of SubsystemContainer
	/*!	modify the stream printing:
	*		
	*		-	the susbsytem coordinates
	*		-	animal_set size
	*		-	vegetable_set size
	*		-	all the animals and the vegetables
	*
	*/
std::ostream &operator<<(	std::ostream &os	,	
													const SubC &subc	)
{

//appo references
	const SubC::index_an_by_id&		an_index	=
		subc.animal_sub_ecosystem().get<SubC::id>()	;

	const	SubC::index_veg_by_id&	veg_index	=
		subc.vegetable_sub_ecosystem().get<SubC::id>();

	
	os	<<	"----------------------"	<<	std::endl;
	os	<<	"#CONTAINER "	<<	subc.x_position()
			<<	" , "	<<	subc.y_position()	<<	std::endl;
	os	<<	"----------------------"	<<	std::endl;
	os	<<	std::endl
			<<	"ANIMAL SET SIZE: "	
			<<	subc.animal_sub_ecosystem().size()
			<<	std::endl
			<<	"VEGETABLE SET SIZE: "	
			<<	subc.vegetable_sub_ecosystem().size()
			<<	std::endl;
	//prints animals
	os	<<	std::endl	<< "#ANIMALS: "	<<	std::endl;
	for(SubC::an_id_const_it it = an_index.begin();\
			it != an_index.end();
			it ++)
	{
		std::cout	<<	std::endl	
							<<	"------------------"	<<	std::endl
							<<	*it	
							<<	"------------------"		
							<<	std::endl;

	}
	os	<<	std::endl	;
	os	<<	"----------------------"	<<	std::endl;

	//prints vegetables
	os	<<	std::endl <<  "#VEGETABLES: "	<<	std::endl;
	for(SubC::veg_id_const_it it = veg_index.begin();\
			it != veg_index.end();
			it ++)
	{
		std::cout	<<	std::endl	
							<<	"------------------"	<<	std::endl
							<<	*it	
							<<	"------------------"		
							<<	std::endl;

	}


	return os;
}



