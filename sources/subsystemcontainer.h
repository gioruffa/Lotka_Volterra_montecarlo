//-----------------------------------------------------------------------
//
//	File Name:	subsystemcontainer.h
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

/*!	\file subsystemcontainer.h
*		file of the subsystem container,
*		it contains the specification of the container.
*		
*/


#ifndef SUB_SYS_CONTAINER
#define SUB_SYS_CONTAINER
//	std inclusion
#include <utility>	//pair

// boost multi_index container includes
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>

//to use members's return values as key
#include <boost/multi_index/mem_fun.hpp>

//to use composite key
#include <boost/multi_index/composite_key.hpp>

//ancestor class
#include "container.h"
#include "beeings.h"

//species controller need by count_vivents
#include "speciescontroller.h"

//sepcies info need by count_vivents
#include "speciesinfo.h"



	using namespace ::boost;

	using namespace ::boost::multi_index;

//!	sub ecosystem container
/*!	this class contain the sub ecosistem composed
*		by all individual animals and vegetables.
*/


class SubsystemContainer : public eco::Container
{
	public:

//////////////////////////
//	typedef for					//
//	animal multi_index	//
//	container						//
//////////////////////////
	
	//multi_index tags
		//!boost multyindex::ordered_index tag
		struct	id{};
		//!boost multyindex::ordered_index tag
		struct	eat{};
		//!boost multyindex::ordered_index tag
		struct	reproduction{};
		//!boost multyindex::ordered_index tag
		struct	spec_id{};

	//multi_index tags	
	
//! animals container typedef
/*!	following the boost::multi_index tradition
*		the typedef of this container is particularli
*		cumbersome, but this is a small rate to pay
*		in front of the extreme power of these containers
*
*		boost multi_index containers gave us the possibility
*		to sort and index the elements of a single container
*		in different ways.
*
*		in addiction we uses the feature of composite_key
*		indexing. this give us the possibility (for example)
*		to have contiguity for all the animals belonging to
*		the same species and then have it sorted by ascendent hp.
*		
*		in this implementation we have numerous indexes:
*
*	
*		-	0 composite key index rappresenting the attitude
*				to be eaten. IndividualAnimal were sorted first by 
*				species_id , then by hp.
*
*
*		- 1 composite key index rappresenting the "sexual charming"
*				IndividualAnimal were sorted first by species_id , 
*				then by Gender , and finally by hp;
*
*
*		- 2 order unique by the IndividualAnimal id_number. this
*				means that no animals whith same id were admitted
*
*
*		-	3	order non unique by species id,
*				this order is used to fast compute the number of 
*				animals of the same species and similar purposes
*/
typedef	multi_index_container<
	IndividualAnimal,
	indexed_by<

//	//-0- index ordered by eating attitude
		ordered_non_unique<	tag<eat>,
			composite_key<
				IndividualAnimal,
				const_mem_fun<
					Specied,	
					unsigned int,		
					&IndividualAnimal::species_id
				>,//const_mem_fun
				const_mem_fun<
					Vivent, 	
					unsigned int,		
					&IndividualAnimal::hp
				>//const_mem_fun
			>//composite_key
		>,//oreder_non_unique

//	//-1- index ordered by reproductive attiude
		ordered_non_unique<	tag<reproduction>,
			composite_key<
				IndividualAnimal,
				const_mem_fun<
					Specied,
					unsigned int,		
					&IndividualAnimal::species_id
				>,//const_mem_fun
				const_mem_fun<
					Vivent,
					Gender,
					&IndividualAnimal::gender	
				>,//const_mem_fun
				const_mem_fun<
					Vivent, 	
					unsigned int,
					&IndividualAnimal::hp
				>//cont_mem_fun
			>//comp_key
		>,//order_non_unique

//	//-2- index ordered by the Existance::id_number() member
		ordered_unique<	tag<id>,
			const_mem_fun<	
				Existance, 
				Existance::id_type ,
				&IndividualAnimal::id_number
			>
		>,//order_unique
		
//	//-3- index ordered by the Specied::species_id() member
		ordered_non_unique<	tag<spec_id>,
			const_mem_fun<
				Specied, 
				unsigned	int,
				&IndividualAnimal::species_id
			>
		>//order_unique

	>//indexed by
>	animal_set;		//multi index_container


//	index typedef
//!	typedef for animal index 0 eat
typedef	animal_set::index<eat>::type					index_an_by_eat;
//!	typedef for animal index 1 reproduce
typedef	animal_set::index<reproduction>::type	index_an_by_reproduce;
//!	typedef for animal index 2 id_number
typedef	animal_set::index<id>::type						index_an_by_id;
//!	typedef for animal index 3 species_id
typedef	animal_set::index<spec_id>::type			index_an_by_spec_id;


//	index_iterators
//!	typedef for eat animal index iterator
typedef	index_an_by_eat::iterator	an_eat_it;
//!	typedef for eat animal index const iterator 
typedef	index_an_by_eat::const_iterator	an_eat_const_it;

//!	typedef for reproduce animal index iterator
typedef	index_an_by_reproduce::iterator	an_reproduce_it;
//!	typedef for reproduce animal index const iterator 
typedef	index_an_by_reproduce::const_iterator	an_reproduce_const_it;

//!	typedef for id animal index iterator
typedef	index_an_by_id::iterator	an_id_it;
//!	typedef for id animal index const iterator 
typedef	index_an_by_id::const_iterator	an_id_const_it;

//!	typedef for speces id animal index iterator
typedef	index_an_by_spec_id::iterator	an_spec_id_it;
//!	typedef for speces id animal index const iterator 
typedef	index_an_by_spec_id::const_iterator	an_spec_id_const_it;


////////////////////////////
//	typedef for						//
//	vegetable multi_index	//
//	container							//
////////////////////////////

	
//! vegetables container typedef
/*!	this container is similar but less complicated
*		than the former. this is due to the fact that
*		this version of the project does not implement
*		vegetable reproduction.
*
*
*		Vegetable can only be eaten so the indexes are:
*
*
*		-	0 composite key index rappresenting the attitude
*				to be eaten. IndividualAnimal were sorted first by 
*				species_id , then by hp.
*
*
*		- 1 order unique by the IndividualAnimal id_number. this
*				means that no animals whith same id were admitted
*
*
*		-	2	order non unique by species id,
*				this order is used to fast compute the number of 
*				animals of the same species and similar purposes
*/
typedef	multi_index_container<
	IndividualVegetable,
	indexed_by<

//	//-0- index ordered by eating attitude
		ordered_non_unique<	tag<eat>,
			composite_key<
				IndividualVegetable,
				const_mem_fun<
					Specied,	
					unsigned int,		
					&IndividualAnimal::species_id
				>,//const_mem_fun
				const_mem_fun<
					Vivent, 	
					unsigned int,		
					&IndividualAnimal::hp
				>//const_mem_fun
			>//composite_key
		>,//oreder_non_unique

//	//-1- index ordered by the Existance::id_number() member
		ordered_unique<	tag<id>,
			const_mem_fun<	
				Existance, 
				Existance::id_type ,
				&IndividualAnimal::id_number
			>
		>,//order_unique
		
//	//-2- index ordered by the Specied::species_id() member
		ordered_non_unique<	tag<spec_id>,
			const_mem_fun<
				Specied, 
				unsigned	int,
				&IndividualAnimal::species_id
			>
		>//order_unique

	>//indexed by
>	vegetable_set;		//multi index_container


//	index typedef
//!	typedef for vegetable index 0 eat
typedef	vegetable_set::index<eat>::type			index_veg_by_eat;
//!	typedef for vegetable index 1 id_number
typedef	vegetable_set::index<id>::type			index_veg_by_id;
//!	typedef for vegetable index 2 species_id
typedef	vegetable_set::index<spec_id>::type	index_veg_by_spec_id;


//	index_iterators
//!	typedef for eat vegetal index iterator
typedef	 index_veg_by_eat::iterator	veg_eat_it;
//!	typedef for eat vegetal index const iterator 
typedef	 index_veg_by_eat::const_iterator	veg_eat_const_it;

//!	typedef for id vegetal index iterator
typedef	 index_veg_by_id::iterator	veg_id_it;
//!	typedef for id vegetal index const iterator 
typedef	 index_veg_by_id::const_iterator	veg_id_const_it;

//!	typedef for speces id vegetal index iterator
typedef	 index_veg_by_spec_id::iterator	veg_spec_id_it;
//!	typedef for speces id vegetal index const iterator 
typedef	 index_veg_by_spec_id::const_iterator	veg_spec_id_const_it;




//////////////////
//	subsystem		//
//	typedefs		//
//////////////////

//! the type of the subsystem
/*!	first member is an animal_set second is a vegetable_set
*/
typedef	std::pair	<animal_set , vegetable_set> subsystem_tp;

////////////////////
//	constructors	//
//	destructors		//
////////////////////

	//!default constructor
	/*!set's the position of the subsystem
	*		\param u_x x position
	*		\param u_y y position
	*/
	SubsystemContainer(	unsigned int u_x	=	0	,
											unsigned int u_y	=	0	);
	
	//!default destructor
	~SubsystemContainer();	

//////////////
//	members	//
//////////////

	//!	is the container full
	/*!	potrebbe restare per implementazioni barbare
	*		del tipo: c'è ancora spazio per qualcosa?
	*/
	virtual bool is_full();

	//!	is full for this species
	/*!	\param sample sample specied animal to control
	*		free space
	*/
	virtual	bool is_full(Specied &sample);

	//!	is full for this species
	/*!	\param u_spec_id is the id of the species to controll
	*			
	*/
	virtual	bool is_full(const unsigned int	u_spec_id);

	//!	insert a vivent
	/*!	return true if the vivent had been inserted
	*		correctly
	*/
	bool	insert(IndividualAnimal			&an	);
	//! insert a vivent
	bool	insert(IndividualVegetable	&veg);

	//!	remove an animal
	/*!	remove the animal whith specified id
	*		return true if the operation succeed
	*		\param u_id	id of the animal to be removed
	*/
	bool	remove(const long unsigned int u_id);


	//!	find animal
	/*!	returns an iterator to the searched
	*		vivent
	*		\param u_id id of the animal to search to
	*		\return	id iterator of the id index pointing to the searched
	*		animal. if not found returns end()
	*/
	an_id_it	find_animal	(const long unsigned int u_id);

	//!	find vegetable
	/*!	returns an iterator to the searched
	*		vivent
	*		\param u_id id of the vegetable to search to
	*		\return	id iterator of the id index pointing to the searched
	*		vegetable. if not found returns end()
	*/
	veg_id_it	find_vegetable	(const long unsigned int u_id);
	

	//!	count the number of vivent in this subsystem
	/*!	returns the number of vivent of a determinate species id
	*
	*		\param u_spec_id	the species id of the species
	*		\param u_spec_con	the species controller. necessary to determinate
	*			if vegetable or animal.
	*		\return a pair whit first member the number of vivent counted,
	*			at second member true if the spec id is correct, false if spec id
	*			is not correct or SpeciesController had not been passed
	*/
	std::pair< unsigned int , bool> count_vivents	
															(	const unsigned int 			u_spec_id		= 0	,
																SpeciesController	u_spec_con	=	
																		SpeciesController(0)								);

	//!	count the number of vivent in this subsystem
	/*!	returns the number of vivent of a determinate species id
	*		if no species id is indicated returns the total ammount
	*		of vivent
	*		\param u_spec_info species info of the species
	*		\return a pair whit first member the number of vivent counted,
	*			at second member true if the info is correct, false if species
	*			info had noot been passed;
	*/
	std::pair< unsigned int , bool> count_vivents	
															(const SpeciesInfo u_spec_info);

////////////////
//	operators	//
////////////////

	//!	ostream operator of SubsystemContainer
	/*!	modify the stream printing:
	*		
	*		-	the susbsytem coordinates
	*		-	animal_set size
	*		-	vegetable_set size
	*		-	all the animals and the vegetables
	*
	*/
	friend std::ostream	&operator<<	(	std::ostream &os 						, 
																const SubsystemContainer	&subc);




//////////////
//	set-get	//
//////////////

		//set
	//!set the sub ecosystem
	subsystem_tp	&sub_ecosystem();
	//!set animal_set
	animal_set		&animal_sub_ecosystem();
	//!set vegetable_set
	vegetable_set	&vegetable_sub_ecosystem();

	//!set x_position
	unsigned int	&x_position();
	//!set y_position
	unsigned int	&y_position();
		//get
	//!get the sub ecosystem
	const	subsystem_tp	&sub_ecosystem()					const;
	//!get animal_set
	const	animal_set		&animal_sub_ecosystem()		const;
	//!get vegetable_set
	const	vegetable_set	&vegetable_sub_ecosystem()const;

	//!get m_x_position
	unsigned int	x_position()	const;
	//!get m_y_position
	unsigned int	y_position()	const;


	private:
	//!	the sub ecosistem
	/*!	contains animals and vegetables
	*/
	subsystem_tp	m_sub_ecosystem;
	
	//! x position of the subsystem in the ecosystem
	unsigned int	m_x_position;
	//! y position of the subsystem in the ecosystem
	unsigned int	m_y_position;


			
};

#endif
