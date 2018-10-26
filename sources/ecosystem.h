//-----------------------------------------------------------------------
//
//	File Name:	ecosystem.h
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

/*!	\file ecosystem.h
*
*		the ecosystem contains all the form of life.
*		is divided in subecosystems.
*		\see subecosystem
*/

#ifndef ECOSYSTEM
#define ECOSYSTEM

#include <string>
#include <iostream>

#include "boost/multi_array.hpp"

//boost random generator
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/variate_generator.hpp>

#include "container.h"
#include "subsystemcontainer.h"
#include "speciescontroller.h"
#include "steplog.hpp"

//!contains all the form of life
class EcosystemContainer	: public eco::Container
{
	public:
	//!the type of the ecosystem is a boost multi_array container
	/*!	roughtly is a matrix
	*/
	typedef boost::multi_array<SubsystemContainer, 2> ecosys_type;



	//!default constructor
	EcosystemContainer( unsigned int 	u_x_size					=	0			,	
											unsigned int 	u_y_size					=	0			,
											bool					u_bound 					= true	,
											unsigned int	u_species_number 	= 1			,
											bool					u_random_seed			= false	);
	//!default destructor
	~EcosystemContainer();

	//! is the system full?
	/*!	\todo implement
	*/
	virtual bool is_full();

	//!	initialize the sistem for the step()
	/*!	
	*		\param is	generical input stream
	*		\return true if initialization suceed, false if fails
	*/	
	void initialize(std::istream 	&is);

	//!	initialize the sistem for step()
	/*!	\param infs	input file stream
	*		\return true if initialization suceed, false if fails
	*/
	void initialize(std::ifstream &infs);

	//!insert animal in subecosystem container
	/*!	
	*		\param u_an the animal to inser
	*		\param u_x	the x coord of the subsystem you want to
	*			insert in
	*		\param u_y	the y coord of the subsystem you want to
	*			insert in
	*		\returns true if insertion succed, false if fails
	*/	bool insert(	IndividualAnimal&		u_an	,
								const int 					u_x		,
								const int						u_y		);

	//!insert vegetable in the ecosystem container
	/*!	\param u_veg the vegetable to insert
	*		\param u_x	the x coord of the subsystem you want to
	*			insert in
	*		\param u_y	the y coord of the subsystem you want to
	*			insert in
	*		\returns true if insertion succed, false if fails
	*/
	bool insert(	IndividualVegetable&	u_veg	,
								const int 						u_x		,
								const int							u_y		);
	
	//!	step evolution
	/*!	make a step in the evolution of the system
	*/
	bool 	step();
	//!	step evoultion producing steplog
	/*!	make a step in the evolution of the system and
	*		produce log.
	*		
	*		this function in used in draw_evolv_fast
	*		\see StepLog
	*		\see EcosystemContainer::draw_evolv_fast
	*/
	bool  step(	StepLog &log  );

	//!	fill random ecosystem
	/*!	each subsystem will be filled by a random number
	*		of vivent for each species whith random hp and
	*		random sex. the distribution is uniform and linear.
	*		this filling method respect the 
	*		SubsystemContainer::is_full.
	*
	*		this method is written ad FillRandom and not fillrandom
	*		just to do a tribute to root-cern lib.
	*/
	void FillRandom();
	
	//! fill the ecosystem from a file
	bool fill(std::ifstream &ifs);


	//! total number of specied of this species
	/*!	\returns a pair whith at first member the number of
	*			animals present in the ecosystem and at second member
	*			true if the species exists and false if not
	*/
	std::pair< unsigned int , bool > specied_population
		(	const unsigned int u_spec_id = 0	);

	//! draw and evolv
	/*!	evolv the sistem for a number of steps
	*		and draw his evolution
	*		this method is slower than draw_evolv_fast because for every step
	*		it reset the TH2F and compute the total population for every 
	*		species in every single subsystem. although the rappresentation
	*		is always right.
	*/
	void draw_evolv(const unsigned int steps);

	//!	draw the evolution of the system in a fast way
	/*!	the system will evolv and for each step will be draw something
	*		according to options parameter.
	*		this method is faster than draw_evolv because it parses the 
	*		log produced by step( StepLog) and modify the rappresentation
	*		whithout performing a query for every subsystem and for every 
	*		species. it doesn't reset the TH2F but only modify the wheight
	*		of the interested bins
	*/
	void draw_evolv_fast(	const unsigned int steps 										,	 
												std::string options = 
													std::string("refresh_populations")				);

	//!make the system evolv
	/*!	\param steps number times step() will be runned
	*		\return false if step fails.
	*/
	bool 	evolv(unsigned int steps );

	//!make the system evolv and create a log
	/*!	\param steps number times step() will be runned
	*		\param Logs	a vector of StepLog
	*		\return false if step fails.
	*		\see StepLog
	*/
	bool	evolv(	unsigned int steps  				, 
								std::vector<StepLog> &logs  );

	//!evolv the system and write logs
	/*!	evolv the system and once finisched or once step() returns false;
	*		write using the ofs to a file the results.
	*		results are elaborations of StepLog
	*		\param steps number times step() will be runned
	*		\param ofs output file stream in which buffer
	*		\return false if step fails.
	*/
	bool	evolv(	unsigned int steps					,
								std::ofstream &ofs					);
	//set-get
		//set
	//!set the ecosystem
	ecosys_type					&ecosystem();
	//!set the SubsystemContainer
	/*!	\param u_x 	x coordinate of the subsystem
	*		\param u_y	y coordinate of the subsystem
	*/
	SubsystemContainer	&subsystem(	const int u_x	, 
																	const int u_y	);

		//get
	//!get the ecosystem
	const ecosys_type					&ecosystem()									const;	

	//!get the SubsystemContainer
	/*!	\param u_x 	x coordinate of the subsystem
	*		\param u_y	y coordinate of the subsystem
	*/
	const SubsystemContainer	&subsystem(int u_x , int u_y)	const;

	

	friend std::ifstream& operator>>(	std::ifstream 			&is	,
																		EcosystemContainer 	&eco);

	private:

	//!	create a newborn for the species indicated
	/*!	this function is used inside step()
	*		\param species_id the species id of the newborn
	*		\param subs_x	the x coord of the subsystem in which the newborn
	*			should be
	*		\param subs_y	the y coord of the subsystem in which the newborn
	*			should be
	*		\param log the StepLog
	*		\returns first false if insertion fail, second false if there is
	*			no space for the newborn
	*
	*
	*/
	std::pair<bool , bool> m_newborn	( const	unsigned int 	species_id	,
																			const unsigned int 	subs_x			,
																			const unsigned int 	subs_y			,
																			unsigned int 				u_hp				);

	//!	create a newborn for the species indicated
	/*!	this function is used inside step()
	*		\param species_id the species id of the newborn
	*		\param subs_x	the x coord of the subsystem in which the newborn
	*			should be
	*		\param subs_y	the y coord of the subsystem in which the newborn
	*			should be
	*		\param log the StepLog
	*		\returns first false if insertion fail, second false if there is
	*			no space for the newborn
	*
	*		\see StepLog
	*
	*/
	std::pair<bool , bool> m_newborn	( const	unsigned int 	species_id	,
																			const unsigned int 	subs_x			,
																			const unsigned int 	subs_y			,
																			unsigned int				u_hp				,
																			StepLog &log										);

	//!	get a random int
	/*!	create a random integet from a to b included.
	*		the sistribution is uniform.
	*		boost random numbers generator where used
	*/
	int m_rand_int (int a=0 , int b=0);

	//!	remove animal from the index provided
	/*!	\param dead_an the animal
	*		\param eat_index the eat index of the 
	*			SubsystemContainer::vegetable_set
	*/
	void m_dead	(	SubsystemContainer::an_eat_it dead_an 							,
								SubsystemContainer::index_an_by_eat	&eat_index			);

	//!	remove animal from the index provided and create a log
	/*!	\param dead_an the animal
	*		\param eat_index the eat index of the 
	*			SubsystemContainer::vegetable_set
	*		\param log insert a PopulationVariation inside the StepLog
	*		\param x	the x coord of the subsystem in which the anima is
	*		\param y	the y coord of the subsystem in which the anima is
	*/
	void m_dead	(	SubsystemContainer::an_eat_it 			dead_an 				,
								SubsystemContainer::index_an_by_eat	&eat_index			,
								StepLog 														&log						,
								int x ,	int y																				);

	//!	remove animal from the index provided
	/*!	\param dead_an the animal
	*		\param id_index the id index of the 
	*			SubsystemContainer::vegetable_set
	*/
	void m_dead	(	SubsystemContainer::an_id_it 				dead_an 			,
								SubsystemContainer::index_an_by_id	&id_index			);

	//!	remove animal from the index provided and create a log
	/*!	\param dead_an the animal
	*		\param id_index the id index of the 
	*			SubsystemContainer::vegetable_set
	*		\param log insert a PopulationVariation inside the StepLog
	*		\param x	the x coord of the subsystem in which the anima is
	*		\param y	the y coord of the subsystem in which the anima is
	*/
	void m_dead	(	SubsystemContainer::an_id_it 				dead_an 			,
								SubsystemContainer::index_an_by_id	&id_index			,
								StepLog															&log					,
								int x ,	int y																			);

	//!	remove animal from the index provided
	/*!	\param dead_an the animal
	*		\param id_index the id index of the 
	*			SubsystemContainer::vegetable_set
	*/
	void m_dead (	SubsystemContainer::an_reproduce_it 			dead_an			,
								SubsystemContainer::index_an_by_reproduce &repr_index	);

	//!	remove animal from the index provided and create a log
	/*!	\param dead_an the animal
	*		\param id_index the id index of the 
	*			SubsystemContainer::vegetable_set
	*		\param log insert a PopulationVariation inside the StepLog
	*		\param x	the x coord of the subsystem in which the anima is
	*		\param y	the y coord of the subsystem in which the anima is
	*/
	void m_dead (	SubsystemContainer::an_reproduce_it 			dead_an			,
								SubsystemContainer::index_an_by_reproduce &repr_index	,
								StepLog																		&log				,
								int x , int y																					);

	//! translate the coordinate if there were no boundaries
	/*!	if there were no boundaries the x and y coord could be
	*		more than m_x_size or m_y_size or less than 0.
	*		this function provide a translation of such numbers in an
	*		interval from 0 - m_x/y_size.
	*	
	*		if the m_boundaries flag is not true and the coordinates are
	*		not in the interval it will print an error message and set
	*		both x and y to 0
	*		example: if x = m_x_size+1 it becomes 0
	*/
	void m_bound_translator(int &x , int &y);


	//! where an animal could move
	/*!	for a determinate species it controll inside near
	*		subsystem if they were full and took a random one of
	*		unfull
	*		\param u_x the x coord animal will move
	*		\param curr_x	the current x coord in which the animal is
	*		\return false if there is no place to move
	*/
	bool m_where_to_move( int 					&u_x 				, 
												int 					&u_y 				, 
									const int 					curr_x 			, 
									const int 					curr_y			, 
									const	unsigned int 	species_id	);

	//! migrate form curr_x and curr_y using m_where_to_move
	/*!	this method is called inside step.
	*		and move the animal from a subsystem to other.
	*
	*
	*		this method were called in step() when:
	*		-	there is no food in the current subsystem
	*		-	there is no one for reproduction
	*		-	there is no space for a newborn
	*
	*		\param curr_x the current x subsystem coordinate
	*		\param to_move	an iterator to the animal to move
	*		\param idx the current id_index in which the animal is.
	*			this parameter is necessary because the animal had to be
	*			removed from the current ecosystem
	*		\returns true if migration occours, else false
	*/
	bool m_migrate	(	const int	curr_x															,
										const int	curr_y															,
										SubsystemContainer::an_id_it 				&to_move 	,
										SubsystemContainer::index_an_by_id	&idx			);

	//! migrate form curr_x and curr_y using m_where_to_move
	/*!	this method is called inside step.
	*		and move the animal from a subsystem to other.
	*
	*
	*		this method were called in step() when:
	*		-	there is no food in the current subsystem
	*		-	there is no one for reproduction
	*		-	there is no space for a newborn
	*
	*		\param curr_x the current x subsystem coordinate
	*		\param to_move	an iterator to the animal to move
	*		\param idx the current id_index in which the animal is.
	*			this parameter is necessary because the animal had to be
	*			removed from the current ecosystem
	*		\param log create a PopulationVariation and insert it in the
	*			the StepLog passed.	
	*		\returns true if migration occours, else false
	*/
	bool m_migrate	(	const int	curr_x															,
										const int	curr_y															,
										SubsystemContainer::an_id_it 				&to_move 	,
										SubsystemContainer::index_an_by_id	&idx			,
										StepLog &log																	);
	

	//!x size of the container
	unsigned int 	m_x_size;
	//!y size of the container
	unsigned int	m_y_size;
	
	//!if the ecosystem have boundaries or not
	bool 					m_boundaries;
	
	//!the ecosystem as ensable of subsystem containers
	/*!	as you can see in ecosys_type this is a bidimensional
	*		boost::multy_array (a matrix). please read the 
	*		boost::multy_array doc befor to edit. 
	*		range goes from [0][0] to [m_x_size-1][m_y_size-1]
	*/
	ecosys_type	m_ecosystem;

	//!the last existance id of a new creature
	/*!	due to the fact that there is an unique id_number all the vivent
	*		inside a subsystem container vegetable or animal set every new
	*		vivent created can not have the same id of another. if this occurs 
	*		the insertion in the subsystem will fail. so every time an animal
	*		is inserted or created by FillRandom() or m_newborn this variable 
	*		is incremented;
	*/
	unsigned long int m_last_existance_id;

	//! the species controller for this ecosys
	/*!	the species controller is initializiated calling
	*		the methods initialize.
	*/
	SpeciesController	m_species_controller;

	//! the clock of the system
	Clock	m_clock;
	
	//! the random number generator used by m_rand_int
	boost::mt19937 m_generator;

};

#endif
