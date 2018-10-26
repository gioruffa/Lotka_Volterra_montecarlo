//-----------------------------------------------------------------------
//
//	File Name:	ecosystem.cpp
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

#include <string>	//string (insert whith a string)	

#include "boost/multi_array.hpp"	//multi_array (subsyst matrix)
#include "boost/tokenizer.hpp"		//used in insert
#include "boost/lexical_cast.hpp"	//print numbers whith root
#include "boost/tuple/tuple.hpp"


#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <set>
#include <algorithm>//copy , back_insert , to pase options

#include "ecosystem.h"
#include "fieldchangers.hpp"	//change animal field


#include "boost/shared_ptr.hpp"	//pointers to TH2
#include "boost/shared_array.hpp"
#include "boost/tokenizer.hpp"		

#include <cstdlib>	//system -> sleep
#include <time.h>	//random seeding


typedef EcosystemContainer Eco;

Eco::EcosystemContainer
			(	unsigned int 	u_x_size					,
				unsigned int 	u_y_size					,
				bool					u_bound						,
				unsigned int	u_species_number	,
				bool					u_random_seed			)
	:	m_x_size							(	u_x_size	)			,
		m_y_size							(	u_y_size	)			,
		m_boundaries					(	u_bound		)			,
		m_species_controller	(	u_species_number)	

		
{
	//size the ecoystem
	Eco::ecosys_type::extent_gen extents;
	m_ecosystem.resize(extents[u_x_size][u_y_size]);
	typedef Eco::ecosys_type::index index;
	
	for (index i = 0 ; i < u_x_size ; i++)
	{
		for(index y = 0 ; y < u_y_size ; y++)
		{
			m_ecosystem[i][y].x_position() = i;
			m_ecosystem[i][y].y_position() = y;
		}
	}

	m_last_existance_id = 0;

	if (u_random_seed)
	{
		m_generator = boost::mt19937(std::time(NULL));
	}
}

Eco::~EcosystemContainer()
{
}


bool Eco::is_full()
{	
	//=============================//
	//	for every subsystem run is_full
	//	for every species
	//=============================//
	
	for (unsigned int i = 0 ; i < m_x_size ; i++)
	{
		for (unsigned int j = 0 ; j < m_y_size ; j++)
		{
			//for every species

			const SpeciesController::infos_tp infos = 
				m_species_controller.get_infos();

			for (	SpeciesController::infos_const_it info_it =	
					infos.begin()	;
				info_it != infos.end()	;
				info_it ++
			)
			{
				const SpeciesInfo &info = info_it->second;
				if (subsystem(i,j).is_full(info.species_id) == false)	\
					return false;
			}
		}
	}//end for every subs
	
	return true;
}


void Eco::initialize(std::istream &in)
{
	/******************************
	*         INTRODUCTION        *
	******************************/

	std::cout	<<	std::endl
						<<	"#############################"
						<<	"#INITIALIZATION OF ECOSYSTEM#"
						<<	"#    using standard input   #"
						<<	"#############################"
						<<	std::endl;
	

	/******************************
	*			COMPLETING THE SPECIES	*
	*	 					CONTROLLER				*
	******************************/

	std::cout	<<	std::endl
						<<	"#INITIALIZING THE SPECIES CONTROLLER"
						<< 	std::endl;
	
	in >> m_species_controller;

	if (!m_species_controller.check())
	{
		std::cerr	<<	std::endl
							<<	"ERROR: the initialization of the speciescontroller "
							<<	"is uncompleted! the programm will try to run but "
							<<	"no proper behavior is guaranteed."
							<<	std::endl;

		std::cerr	<<	std::endl
							<<	"WARNING: SPECIES CONTROLLER INITIALIZATION "
							<<	"UNCOMPLETE!"
							<<	std::endl;
	}
	else
	{
		std::cerr	<<	std::endl
							<<	"SPECIES CONTROLLER INITIALIZATION "
							<<	"COMPLETED WITHOUT ERROR"
							<<	std::endl;
	}

}

void Eco::initialize(std::ifstream &infs)
{
	/******************************
	*         INTRODUCTION        *
	******************************/

	std::cout	<<	std::endl
						<<	"#############################"
						<<	"#INITIALIZATION OF ECOSYSTEM#"
						<<	"#    using file stream			 #"
						<<	"#############################"
						<<	std::endl;
	

	/******************************
	*			COMPLETING THE SPECIES	*
	*	 					CONTROLLER				*
	******************************/

	std::cout	<<	std::endl
						<<	"#INITIALIZING THE SPECIES CONTROLLER"
						<< 	std::endl;
	if(!infs.is_open())\
	{
		std::cerr	<<	std::endl
							<<	"WARNING: no source file indicated "
							<<	"using \"source.dat\" "
							<<	std::endl;
		infs.open("source.dat");
	}

	while(infs.good())
	{
		infs >> m_species_controller;
	}

	//check is it's well formed
	if (!m_species_controller.check())
	{
		std::cerr	<<	std::endl
							<<	"ERROR: the initialization of the speciescontroller "
							<<	"is uncompleted! the programm will try to run but "
							<<	"no proper behavior is guaranteed."
							<<	std::endl;

		std::cerr	<<	std::endl
							<<	"WARNING: SPECIES CONTROLLER INITIALIZATION "
							<<	"UNCOMPLETE!"
							<<	std::endl;
	}
	else
	{
		std::cerr	<<	std::endl
							<<	"SPECIES CONTROLLER INITIALIZATION "
							<<	"COMPLETED WITHOUT ERROR"
							<<	std::endl;
	}
	#ifdef RUN_DBG
		std::cerr	<< std::endl
							<< m_species_controller
							<< std::endl;
	#endif

	infs.close();
}





bool Eco::insert(	IndividualAnimal&		u_an	,
									const int 					u_x		,
									const int						u_y		)
{
	int x=0 , y=0 ;
	
	/*	this is not the smartest way to do it but
	*		is the most compresible one
	*		the problem is that our system could have boundaries
	*		and someone could enter a negative coordinate for the
	*		subsystem or could enter a bigger coordinate than the
	*		real boarder.
	*/	

	if (m_boundaries)
	{
		//get the resto-intero
		int appo_x = u_x % m_x_size;
		int appo_y = u_y % m_y_size;
	
		//u_x or u_y could be negative	
		if (appo_x < 0) x = m_x_size + appo_x;
		else	x = appo_x;
		if (appo_y < 0) y = m_y_size + appo_y;
 		else	y = appo_y;
	}
	else
	{
		if(	static_cast<unsigned int>(u_x) > m_x_size 
					or 
				static_cast<unsigned int>(u_y) > m_y_size	)
		{
			std::cerr	<<	std::endl
								<<	"ERROR: the specified subsystem container does not "
								<<	"exists."	<<	std::endl;
			std::cerr	<<	"WARNING: using default coordinates 0 , 0"	
								<<	std::endl;
			//i do this because the program must run as possible
			x	=	0;
			y	=	0;
		}
		else
		{
			x = u_x;
			y = u_y;
		}
	}	//controll the logs of insert and is_full
	
	#ifdef DEBUG
		std::cerr	<<	std::endl	<< "x: " << x	;
		std::cerr	<<	std::endl	<< "y: " << y	;
	#endif
	if (m_ecosystem[x][y].insert(u_an) == true)	return true;
	else return false;
}


bool Eco::insert(	IndividualVegetable&	u_veg	,
									const int 						u_x		,
									const int							u_y		)
{
	int x=0 , y=0 ;
	
	/*	this is not the smartest way to do it but
	*		is the most compresible one
	*		the problem is that our system could have boundaries
	*		and someone could enter a negative coordinate for the
	*		subsystem or could enter a bigger coordinate than the
	*		real boarder.
	*/	

	if (m_boundaries)
	{
		//get the resto-intero
		int appo_x = u_x % m_x_size;
		int appo_y = u_y % m_y_size;
	
		//u_x or u_y could be negative	
		if (appo_x < 0) x = m_x_size + appo_x;
		else	x = appo_x;
		if (appo_y < 0) y = m_y_size + appo_y;
 		else	y = appo_y;
	}
	else
	{
		if(	static_cast<unsigned int>(u_x) > m_x_size 
					or 
				static_cast<unsigned int>(u_y) > m_y_size)
		{
			std::cerr	<<	std::endl
								<<	"ERROR: the specified subsystem container does not "
								<<	"exists."	<<	std::endl;
			std::cerr	<<	"WARNING: using default coordinates 0 , 0"	
								<<	std::endl;
			//i do this because the program must run as possible
			x	=	0;
			y	=	0;
		}
		else
		{
			x = u_x;
			y = u_y;
		}
	}	//controll the logs of insert and is_full
	
	#ifdef DEBUG
		std::cerr	<<	std::endl	<< "x: " << x	;
		std::cerr	<<	std::endl	<< "y: " << y	;
	#endif
	if (m_ecosystem[x][y].insert(u_veg) == true)	return true;
	else return false;
}


void Eco::FillRandom()
{
	//get infos
	const SpeciesController::infos_tp infos = 
		m_species_controller.get_infos();
	for (	SpeciesController::infos_const_it info_it =	
					infos.begin()	;
				info_it != infos.end()	;
				info_it ++
			)
	{
		//need to know how many animals can be stored
		//for subsystem
			//division round for defect so no prob
			//static_cast will not do so...
			//insertion wont fail if we insert more animals
			//than permitted but will print a Warning

		//leave the key
		const SpeciesInfo &info = info_it->second;

		int tot_vivent_store = 100 / info.life_space;
		
			//please controll whith gdb
		
		//now for each subsystem
		for (unsigned int x = 0 ; x < m_x_size ; x ++)
		{
			for (unsigned int y = 0 ; y < m_y_size ; y ++)
			{
				//how many vivents
				unsigned int tot_vivents = m_rand_int (0,tot_vivent_store);
				
				#ifdef RUN_DBG
					//for debug purposes we need a vegetable in each subs
					if (info.is_animal == false)	
					{
						tot_vivents = 1;
					}
				#endif

				for (unsigned int i = 0 ; i < tot_vivents ; i++  )
				{
				//now we have to create an Vivent whith randhp
				//but befor we need to know if is an animal or not

					if(info.is_animal)
					{
						IndividualAnimal	cash_animal;
						//need random hp
						//please not dead animals
						cash_animal.hp() = m_rand_int(1,100);

						cash_animal.id_number() = m_last_existance_id + 1;	
	
			//vivent
						if(m_rand_int(0,1) == 0)	\
							cash_animal.gender() = Gender("male");
						else \
							cash_animal.gender() = Gender("female");

						//specied
						cash_animal.species_id() 		= info.species_id;
						cash_animal.species_name() 	= info.species_name;
						cash_animal.life_coast() 		= info.life_coast;
						cash_animal.health_status() = info.health_status;
						cash_animal.calorie() 			= info.calorie;
						cash_animal.life_space() 		= info.life_space;

						//animal
						cash_animal.life_coast()		= info.life_coast;
	
						//individual animal
						cash_animal.appetite()		=	50;
						cash_animal.libido()			= 50;
	
						//now our animal is formed
						if (subsystem(x,y).insert(cash_animal))
						{	
							//increment the reference number
							m_last_existance_id ++;
							#ifdef RUN_DBG
								std::cerr	<<	std::endl
													<<	"NEWBORN INSERTED"
													<<	std::endl
													<<	cash_animal
													<<	std::endl;
							#endif
						}
							else //insertion fail
							{
								std::cerr	<<	std::endl
													<<	"ERROR: FillRandom "
													<<	std::endl
													<<	"insertion fail of "
													<<	std::endl
													<<	cash_animal
													<<	std::endl;
								break;
							}
						}
					else	//is vegetable
					{
						IndividualVegetable	cash_vegetable;
						cash_vegetable.hp() = 100;
	
						cash_vegetable.id_number() = m_last_existance_id + 1;	

						//cash_vegetable.gender() = Gender("asexual");
						//specied
						cash_vegetable.species_id() 		= info.species_id;
						cash_vegetable.species_name() 	= info.species_name;
						cash_vegetable.life_coast() 		= info.life_coast;
						cash_vegetable.health_status() 	= info.health_status;
						cash_vegetable.calorie() 				= info.calorie;
						cash_vegetable.life_space() 		= info.life_space;				

						if (subsystem(x,y).insert(cash_vegetable))
						{
							//increment the reference number
							m_last_existance_id ++;
							#ifdef RUN_DBG
								std::cerr	<<	std::endl
													<<	"VEGETABLE INSERTED"
													<<	std::endl
													<<	cash_vegetable
													<<	std::endl;
							#endif						
						}
						else
						{
							std::cerr	<<	std::endl
												<<	"ERROR: FillRandom "
												<<	std::endl
												<<	"insertion fail of "
												<<	std::endl
												<<	cash_vegetable
												<<	std::endl;
							break;
						}
					}
				}//end for i
			}
		}//end for all subs
		
	}//end for all species

}


bool Eco::fill(std::ifstream &ifs)
{	
	if (!m_species_controller.check())
	{
		std::cerr	<<	std::endl
							<<	"ERROR: before filling the ecosystem please"
							<<	"complete the initialization"
							<<	std::endl;
		return false;
	}
	if (!ifs.is_open())
	{
		std::cerr	<<	std::endl
							<<	"WARNING: file stream is not open. it will look for"
							<<	"fill_source.dat"
							<<	std::endl;
		ifs.open("fill_source.dat");
	}
	if (!ifs.good())
	{
		std::cerr	<<	std::endl
							<<	"ERROR: the file you entered is not present"
							<<	"or corrupted. please check"
							<<	std::endl;
		return false;
	}

	while (ifs.good())
	{
		ifs >> *this;
	}

	ifs.close();
	return true;
}


/************/
/*	set-get	*/
/************/


	//set

Eco::ecosys_type	&Eco::ecosystem()
{
	return m_ecosystem;
}

SubsystemContainer	&Eco::subsystem(const int u_x,const int u_y)
{
	int x = 0 , y = 0;

	/*	this is not the smartest way to do it but
	*		is the most compresible one
	*		the problem is that our system could have boundaries
	*		and someone could enter a negative coordinate for the
	*		subsystem or could enter a bigger coordinate than the
	*		real boarder.
	*/	
	if (m_boundaries)
	{
		//get the resto-intero
		int appo_x = u_x % m_x_size;
		int appo_y = u_y % m_y_size;
	
		//u_x or u_y could be negative	
		if (appo_x < 0) x = m_x_size + appo_x;
		else	x = appo_x;
		if (appo_y < 0) y = m_y_size + appo_y;
 		else	y = appo_y;
	}
	else
	{
		if(	static_cast<unsigned int>(u_x) > m_x_size 
					or 
				static_cast<unsigned int>(u_y) > m_y_size)
		{
			std::cerr	<<	std::endl
								<<	"ERROR: the specified subsystem container does not "
								<<	"exists."	<<	std::endl;
			std::cerr	<<	"WARNING: using default coordinates 0 , 0"	
								<<	std::endl;
			x	=	0;
			y	=	0;
		}
		else
		{
			x = u_x;
			y = u_y;
		}
	}

	#ifdef DEBUG
		std::cerr	<<	std::endl	<< "x: " << x	;
		std::cerr	<<	std::endl	<< "y: " << y	;
	#endif

	return m_ecosystem[x][y];
}

	//get


const Eco::ecosys_type	&Eco::ecosystem()	const
{
	return m_ecosystem;
}


const SubsystemContainer	
	&Eco::subsystem(const int u_x,const int u_y)
		const
{
	int x = 0 , y = 0;

	/*	this is not the smartest way to do it but
	*		is the most compresible one
	*		the problem is that our system could have boundaries
	*		and someone could enter a negative coordinate for the
	*		subsystem or could enter a bigger coordinate than the
	*		real boarder.
	*/	
	if (m_boundaries)
	{
		//get the resto-intero
		int appo_x = u_x % m_x_size;
		int appo_y = u_y % m_y_size;
	
		//u_x or u_y could be negative	
		if (appo_x < 0) x = m_x_size + appo_x;
		else	x = appo_x;
		if (appo_y < 0) y = m_y_size + appo_y;
 		else	y = appo_y;
	}
	else
	{
		if(	static_cast<unsigned int>(u_x) > m_x_size 
					or 
				static_cast<unsigned int>(u_y) > m_y_size)
		{
			std::cerr	<<	std::endl
								<<	"ERROR: the specified subsystem container does not "
								<<	"exists."	<<	std::endl;
			std::cerr	<<	"WARNING: using default coordinates 0 , 0"	
								<<	std::endl;
			x	=	0;
			y	=	0;
		}
		else
		{
			x = u_x;
			y = u_y;
		}
	}

	#ifdef DEBUG
		std::cerr	<<	std::endl	<< "x: " << x	;
		std::cerr	<<	std::endl	<< "y: " << y	;
	#endif

	return m_ecosystem[x][y];
}

int	Eco::m_rand_int	(int a, int b)
{
	boost::uniform_int<> dist(a,b);
	boost::variate_generator<
		boost::mt19937&, 
		boost::uniform_int<> 
	> die(m_generator, dist);

	return die();
}


void Eco::m_dead(	SubsystemContainer::an_eat_it dead_an 							,
									SubsystemContainer::index_an_by_eat	&eat_index			)
{
	std::cout	<<	std::endl
						<<	"The animal "
						<<	*dead_an
						<<	"is no longer living; "
						<<	"will be removed."
						<< 	std::endl;

	eat_index.erase(dead_an);
}

void Eco::m_dead(	SubsystemContainer::an_eat_it 			dead_an 				,
									SubsystemContainer::index_an_by_eat	&eat_index			,
									StepLog															&log						,
									int x , int y																				)
{
	std::cout	<<	std::endl
						<<	"The animal "
						<<	*dead_an
						<<	"is no longer living; "
						<<	"will be removed."
						<< 	std::endl;

	eat_index.erase(dead_an);
	PopulationVariation cash	(		PopulationVariation::coord_tp
																	(x , y)						 					,
																dead_an->species_id()					,
																	-1													,
																m_clock.abs()									,
																m_clock.rel()									);
	log.variations.push_back(cash);
}


void Eco::m_dead	(	SubsystemContainer::an_id_it dead_an 							,
										SubsystemContainer::index_an_by_id	&id_index			)
{	
	std::cout	<<	std::endl
						<<	"The animal "
						<<	*dead_an
						<<	"is no longer living; "
						<<	"will be removed."
						<< 	std::endl;

	id_index.erase(dead_an);
}

void Eco::m_dead	(	SubsystemContainer::an_id_it 				dead_an 			,
										SubsystemContainer::index_an_by_id	&id_index			,
										StepLog															&log					,
										int x ,	int y																			)
{	
	std::cout	<<	std::endl
						<<	"The animal "
						<<	*dead_an
						<<	"is no longer living; "
						<<	"will be removed."
						<< 	std::endl;
	
	id_index.erase(dead_an);
	PopulationVariation cash	(	PopulationVariation::coord_tp
																(x , y)						 					,
															dead_an->species_id()					,
																-1													,
															m_clock.abs()									,
															m_clock.rel()									);

	log.variations.push_back(cash);
}

void Eco::m_dead (SubsystemContainer::an_reproduce_it 			dead_an			,
									SubsystemContainer::index_an_by_reproduce &repr_index	)
{
	std::cout	<<	std::endl
						<<	"The animal "
						<<	*dead_an
						<<	"is no longer living; "
						<<	"will be removed."
						<< 	std::endl;
	
	repr_index.erase(dead_an);
}


void Eco::m_dead (	SubsystemContainer::an_reproduce_it 			dead_an			,
								SubsystemContainer::index_an_by_reproduce &repr_index	,
								StepLog																		&log				,
								int x , int y																					)
{
	std::cout	<<	std::endl
						<<	"The animal "
						<<	*dead_an
						<<	"is no longer living; "
						<<	"will be removed."
						<< 	std::endl;

	repr_index.erase(dead_an);
	
	PopulationVariation cash	(	PopulationVariation::coord_tp
																(x , y)						 					,
															dead_an->species_id()					,
																-1													,
															m_clock.abs()									,
															m_clock.rel()									);

	log.variations.push_back(cash);
}


void Eco::m_bound_translator(int &x , int &y)
{
	if (!m_boundaries)
	{
		//get the resto-intero
		int appo_x = x % m_x_size;
		int appo_y = y % m_y_size;
	
		//u_x or u_y could be negative	
		if (appo_x < 0) x = m_x_size + appo_x;
		else	x = appo_x;
		if (appo_y < 0) y = m_y_size + appo_y;
 		else	y = appo_y;
	}
	else
	{
		if(x < 0 or y < 0)
		{
			std::cerr	<<	std::endl
								<<	"ERROR: the specified subsystem container "
								<<	x	<< "," << y
								<<	" does not exists. one of the two coordinates is < 0"	
								<<	std::endl;
			std::cerr	<<	"WARNING: using default coordinates 0 , 0"	
								<<	std::endl;
			//i do this because the program must run as possible
			x	=	0;
			y	=	0;

		}
		if(	static_cast<unsigned int>(x) > m_x_size 
					or 
				static_cast<unsigned int>(y) > m_y_size	)
		{
			std::cerr	<<	std::endl
								<<	"ERROR: the specified subsystem container does not "
								<<	"exists."	<<	std::endl;
			std::cerr	<<	"WARNING: using default coordinates 0 , 0"	
								<<	std::endl;
			//i do this because the program must run as possible
			x	=	0;
			y	=	0;
		}
		else
		{
			x = x;
			y = y;
		}
	}

}

std::pair<bool , bool> Eco::m_newborn(	const unsigned int species_id	,
																			const unsigned int 	subs_x			,
																			const unsigned int 	subs_y			,
																			unsigned int 				u_hp				)
{
	//we need the species info
	//all features will be taken from the species_info
	SpeciesController::infos_const_it info_it =
		m_species_controller.get_info( species_id);
	
	//controll if species exists
	if (info_it == m_species_controller.get_infos().end()
				or 
			species_id == 0	)
	{
		std::cerr	<<	std::endl
							<<	"ERROR: newborn: this species id is not contained "
							<<	"in the ecoysstem SpeciesController."
							<<	std::endl;
		return std::pair<bool , bool>(false , false);
	}

	//controll if there is space in this subspace
	int x=subs_x , y=subs_y;
	m_bound_translator(x,y);
	if	(subsystem(x,y).is_full(species_id))
	{
		std::cerr	<< 	std::endl
							<<	"WARNING: there is no space for the newborn of species "
							<<	species_id	<< "in subsystem "	<< x << "," << y
							<<	std::endl;
		return std::pair<bool , bool>(true , false);
	}

	//leave the key
	const SpeciesInfo &info = info_it->second;
	IndividualAnimal newborn;
	
	//now we set the fields
		//existance
	newborn.birth_date() = DateOfBirth(m_clock);
	newborn.id_number() = m_last_existance_id + 1;	//not increment it yet
	
		//vivent
	//too much	
	//newborn.hp() = info.health_status;
	newborn.hp()	=	u_hp;
			//gender -> run die 50%
	if(m_rand_int(0,1) == 0)	\
		newborn.gender() = Gender("male");
	else \
		newborn.gender() = Gender("female");

		//specied
	newborn.species_id() 		= info.species_id;
	newborn.species_name() 	= info.species_name;
	newborn.life_coast() 		= info.life_coast;
	newborn.health_status() = info.health_status;
	newborn.calorie() 			= info.calorie;
	newborn.life_space() 		= info.life_space;
		
		//animal
	newborn.life_coast()		= info.life_coast;
	
		//individual animal
	newborn.appetite()		=	50;
	newborn.libido()			= 50;

	//now our animal is formed
	if (subsystem(x,y).insert(newborn))
	{	
		//increment the reference number
		m_last_existance_id ++;
		#ifdef RUN_DBG
			std::cerr	<<	std::endl
								<<	"NEWBORN INSERTED"
								<<	std::endl
								<<	newborn
								<<	std::endl;
		#endif
		return std::pair<bool , bool>(true , true);
	}
	else return std::pair<bool , bool>(false , true);

}


std::pair<bool , bool> Eco::m_newborn(	const unsigned int 	species_id	,
																				const unsigned int 	subs_x			,
																				const unsigned int 	subs_y			,
																				unsigned int				u_hp				,
																				StepLog &log										)
{
	//we need the species info
	//all features will be taken from the species_info
	SpeciesController::infos_const_it info_it =
		m_species_controller.get_info( species_id);
	
	//controll if species exists
	if (info_it == m_species_controller.get_infos().end()
				or 
			species_id == 0	)
	{
		std::cerr	<<	std::endl
							<<	"ERROR: newborn: this species id is not contained "
							<<	"in the ecoysstem SpeciesController."
							<<	std::endl;
		return std::pair<bool , bool>(false , false);
	}

	//controll if there is space in this subspace
	int x=subs_x , y=subs_y;
	m_bound_translator(x,y);
	if	(subsystem(x,y).is_full(species_id))
	{
		std::cerr	<< 	std::endl
							<<	"WARNING: there is no space for the newborn of species "
							<<	species_id	<< "in subsystem "	<< x << "," << y
							<<	std::endl;
		return std::pair<bool , bool>(true , false);
	}

	//leave the key
	const SpeciesInfo &info = info_it->second;
	IndividualAnimal newborn;
	
	//now we set the fields
		//existance
	newborn.birth_date() = DateOfBirth(m_clock);
	newborn.id_number() = m_last_existance_id + 1;	//not increment it yet
	
		//vivent
	//too much	
	//newborn.hp() = info.health_status;
	newborn.hp()	=	u_hp;
			//gender -> run die 50%
	if(m_rand_int(0,1) == 0)	\
		newborn.gender() = Gender("male");
	else \
		newborn.gender() = Gender("female");

		//specied
	newborn.species_id() 		= info.species_id;
	newborn.species_name() 	= info.species_name;
	newborn.life_coast() 		= info.life_coast;
	newborn.health_status() = info.health_status;
	newborn.calorie() 			= info.calorie;
	newborn.life_space() 		= info.life_space;
		
		//animal
	newborn.life_coast()		= info.life_coast;
	
		//individual animal
	newborn.appetite()		=	50;
	newborn.libido()			= 50;

	//now our animal is formed
	if (subsystem(x,y).insert(newborn))
	{	
		//increment the reference number
		m_last_existance_id ++;
		#ifdef RUN_DBG
			std::cerr	<<	std::endl
								<<	"NEWBORN INSERTED"
								<<	std::endl
								<<	newborn
								<<	std::endl;
		#endif
		//log
		PopulationVariation cash	(	PopulationVariation::coord_tp
																	(x , y)						 					,
																species_id										,
																1															,
																m_clock.abs()									,
																m_clock.rel()									);
		log.variations.push_back(cash);

		return std::pair<bool , bool>(true , true);
	}
	else return std::pair<bool , bool>(false , true);

}



bool Eco::m_where_to_move(int 					&u_x 				, 
													int 					&u_y 				, 
										const int 					curr_x 			, 
										const int 					curr_y			, 
										const	unsigned int 	species_id	)
{
	//std::vector<SubsystemContainer*> places;
	//coordinate container
	typedef	std::pair<int, int> coord_cont;
	std::vector<coord_cont>	places;

	//translate the positions
	int trans_x = curr_x , trans_y = curr_y;
	m_bound_translator (trans_x , trans_y);

	unsigned int tr_curr_x 	=	static_cast<unsigned int>	(trans_x);
	unsigned int tr_curr_y	= static_cast<unsigned int>	(trans_y);


	
	//fill the vector whith places
	if(m_boundaries)
	{
		//SubsystemContainer *pnt=NULL;
		//right
		if (tr_curr_x != m_x_size - 1) 
		{
			if (!m_ecosystem[tr_curr_x + 1][tr_curr_y].is_full(species_id))
			{
				places.push_back(coord_cont(tr_curr_x + 1, tr_curr_y));
			}
		}
		//left
		if (tr_curr_x > 0)
		{
			if (!m_ecosystem[tr_curr_x - 1][tr_curr_y].is_full(species_id))
			{
				places.push_back(coord_cont(tr_curr_x - 1, tr_curr_y));
			}
		}
		//up
		if (tr_curr_y != m_y_size - 1)
		{
			if (!m_ecosystem[tr_curr_x ][tr_curr_y + 1].is_full(species_id))
			{
				places.push_back(coord_cont(tr_curr_x, tr_curr_y + 1));
			}
		}
		//down
		if (tr_curr_y > 0)
		{
			if (!m_ecosystem[tr_curr_x][tr_curr_y - 1].is_full(species_id))
			{
				places.push_back(coord_cont(tr_curr_x, tr_curr_y - 1));
			}
		}
	}
	else
	{
	}

	if (places.size() == 0) 
	{
		#ifdef RUN_DBG
			std::cerr	<<	std::endl
								<<	"NO PLACE TO MOVE"
								<<	std::endl;
		#endif
		return false;
	}

	coord_cont to_move = places[m_rand_int(0,places.size() - 1 )];
	u_x = to_move.first;
	u_y = to_move.second;
	

	return true;
}


bool Eco::m_migrate	(	const int	curr_x															,
											const int	curr_y															,
											SubsystemContainer::an_id_it 				&to_move 	,
											SubsystemContainer::index_an_by_id	&idx			)
{
	int x_move = 0 , y_move = 0;
	if(	m_where_to_move(x_move , y_move , 
											curr_x , curr_y , 
											to_move->species_id())
		)
	{
		#ifdef RUN_DBG
			std::cerr	<<	std::endl
								<<	"###### DEBUG ##### "
								<<	"# PRINTING SUBSYSTEM TO MOVE #"
								<<	std::endl
								<<	subsystem(x_move,y_move)
								<<	std::endl;
		#endif

		IndividualAnimal predator = *to_move;
	
		SubsystemContainer &subs_to_move =	subsystem(x_move , y_move);

		SubsystemContainer::animal_set
			&an_set_move = subs_to_move.animal_sub_ecosystem();
			
		SubsystemContainer::index_an_by_id& an_id_index_move =
			an_set_move.get<SubsystemContainer::id>();

		std::pair<SubsystemContainer::an_id_it, bool>	returned
			=	an_id_index_move.insert(predator);
		if( returned.second == true)
		{
			#ifdef RUN_DBG
				std::cerr	<<	std::endl
									<<	"INSERTION DONE UNDER MIGRATION"
									<<	std::endl;
			#endif
			//now erase the old element
			idx.erase(to_move);
			//now erase should point to end() or to the element
			//after the old animal pointed

			#ifdef RUN_DBG
				std::cerr	<<	std::endl
									<<	"###### DEBUG ######"
									<<	"# PRINTING CURRENT SUBSYSTEM AFTER DELETE #"
									<<	std::endl
									<<	subsystem(curr_x , curr_y)
									<<	std::endl;
			#endif

			//repoint to move to the new position
			to_move = returned.first;
			
			#ifdef RUN_DBG
			//do some checkes
				std::cerr	<<	std::endl
									<<	"INSIDE MIGRATE: repointing of to_move done."
									<<	" to_move now pointing to"
									<<	std::endl
									<<	*to_move
									<<	std::endl;

				std::cerr	<<	std::endl
									<<	"###### DEBUG ##### "
									<<	"# PRINTING SUBSYSTEM TO MOVE AFTER INSERTION #"
									<<	std::endl
									<<	subs_to_move
									<<	std::endl;

			#endif

			return true;
		}
		else
		{
			#ifdef RUN_DBG
				std::cerr	<<	std::endl
									<<	"ERROR: INSERTION FAILS UNDER MIGRATION"
									<<	" this is due to animal"
									<<	*(returned.first)
									<<	std::endl;
			#endif
			to_move = returned.first;
			return false;
		}

	}
	//no place to move but we don't have mem errors
	else 
	{
		#ifdef RUN_DBG
			std::cerr	<<	std::endl
								<<	"M_MIGRATE NO PLACE TO MOVE"
								<<	std::endl;
		#endif
		return true;
	}
}


	
bool Eco::m_migrate	(	const int	curr_x															,
											const int	curr_y															,
											SubsystemContainer::an_id_it 				&to_move 	,
											SubsystemContainer::index_an_by_id	&idx			,
											StepLog	&log																	)
{
	int x_move = 0 , y_move = 0;
	if(	m_where_to_move(x_move , y_move , 
											curr_x , curr_y , 
											to_move->species_id())
		)
	{
		#ifdef RUN_DBG
			std::cerr	<<	std::endl
								<<	"###### DEBUG ##### "
								<<	"# PRINTING SUBSYSTEM TO MOVE #"
								<<	std::endl
								<<	subsystem(x_move,y_move)
								<<	std::endl;
		#endif

		IndividualAnimal predator = *to_move;

		SubsystemContainer &subs_to_move =	subsystem(x_move , y_move);

		SubsystemContainer::animal_set
			&an_set_move = subs_to_move.animal_sub_ecosystem();
			
		SubsystemContainer::index_an_by_id& an_id_index_move =
			an_set_move.get<SubsystemContainer::id>();

		std::pair<SubsystemContainer::an_id_it, bool>	returned
			=	an_id_index_move.insert(predator);
		if( returned.second == true)
		{
			#ifdef RUN_DBG
				std::cerr	<<	std::endl
									<<	"INSERTION DONE UNDER MIGRATION"
									<<	std::endl;
			#endif
			//now erase the old element
			idx.erase(to_move);
			//now erase should point to end() or to the element
			//after the old animal pointed

			#ifdef RUN_DBG
				std::cerr	<<	std::endl
									<<	"###### DEBUG ######"
									<<	"# PRINTING CURRENT SUBSYSTEM AFTER DELETE #"
									<<	std::endl
									<<	subsystem(curr_x , curr_y)
									<<	std::endl;
			#endif

			//repoint to move to the new position
			to_move = returned.first;
			
			#ifdef RUN_DBG
			//do some checkes
				std::cerr	<<	std::endl
									<<	"INSIDE MIGRATE: repointing of to_move done."
									<<	" to_move now pointing to"
									<<	std::endl
									<<	*to_move
									<<	std::endl;

				std::cerr	<<	std::endl
									<<	"###### DEBUG ##### "
									<<	"# PRINTING SUBSYSTEM TO MOVE AFTER INSERTION #"
									<<	std::endl
									<<	subs_to_move
									<<	std::endl;

			#endif
			PopulationVariation cash	(	PopulationVariation::coord_tp
																		(curr_x , curr_y) 					,
																	to_move->species_id()					,
																	-1														,
																	m_clock.abs()									,
																	m_clock.rel()									);

			log.variations.push_back(cash);

			cash.subs_coord = PopulationVariation::coord_tp 
												(x_move, y_move);
			cash.species_id = to_move->species_id()	;
			cash.variation	=	1;
			log.variations.push_back(cash);


			return true;
		}
		else
		{
			#ifdef RUN_DBG
				std::cerr	<<	std::endl
									<<	"ERROR: INSERTION FAILS UNDER MIGRATION"
									<<	" this is due to animal"
									<<	*(returned.first)
									<<	std::endl;
			#endif
			to_move = returned.first;
			return false;
		}

	}
	//no place to move but we don't have mem errors
	else 
	{
		#ifdef RUN_DBG
			std::cerr	<<	std::endl
								<<	"M_MIGRATE NO PLACE TO MOVE"
								<<	std::endl;
		#endif
		return true;
	}
}


std::pair< unsigned int , bool > Eco::specied_population
		(	const unsigned int u_spec_id 	)
{
	unsigned int tot = 0;
	bool all_good = true;

	//get the info
	SpeciesController::infos_const_it spi_it =
		m_species_controller.get_info(u_spec_id);
				
	const SpeciesInfo &spi = spi_it->second;

	for (unsigned int i = 0 ; i < m_x_size ; i++)
	{
		for (unsigned int j = 0 ; j < m_y_size ; j ++)
		{
			std::pair <unsigned int, bool> returned;
			returned = subsystem(i,j).count_vivents(spi);
			tot += returned.first;
			if (!returned.second)	all_good = false;
		}
	}

	return std::pair <unsigned int ,bool> (tot , all_good);

}


bool Eco::evolv(unsigned int steps)
{
	for (unsigned int i = 0 ; ; i++)
	{
		std::cout	<<	std::endl
							<<	"###### STEP "
							<<	i
							<<	"######"
							<<	std::endl;
		if (!step())
		{
			std::cerr	<<	std::endl
								<<	"ERROR: step returned exit status false;"
								<<	"exiting by EcosystemContainer::evolv()"
								<<	std::endl;
			return false;
		}
	}
	return true;
}


bool Eco::evolv(unsigned int steps , std::vector<StepLog> &logs)
{
	for (unsigned int i = 0 ; ; i++)
	{
		std::cout	<<	std::endl
							<<	"###### STEP "
							<<	i
							<<	"######"
							<<	std::endl;
		StepLog cash_log;
		if (!step(cash_log))
		{
			std::cerr	<<	std::endl
								<<	"ERROR: step returned exit status false;"
								<<	"exiting by EcosystemContainer::evolv()"
								<<	std::endl;
			return false;
		}
		else
		{
			logs.push_back(cash_log);
		}
	}
	return true;
}


bool Eco::evolv (	unsigned int steps					,
									std::ofstream &ofs					)
{
	//our data type is composed for a certain species
	//	-population
	//	-abs_time
	//	-rel_time
	//	-step
	

	//=============================//
	//	PASSAGES:
	//	-create the container
	//	-create a first picture
	//	-make a step
	//	-parse the log
	//	-insert in the container
	//=============================//

	//=============================//
	//	1)	boost::tuple give us
	//		lots of flexibility if we 
	//		want to add or remove
	//		fields
	//=============================//

	typedef boost::tuple	<	unsigned int	,	// 0 species_id
													std::string		,	// 1 species_name
													unsigned int	,	// 2 population
													long int			,	// 3 abs_time
													float					,	// 4 rel_time
													unsigned int	>	// 5 step
		point_tp;

	typedef std::map<	unsigned int 					, 
										std::vector<point_tp> >
		datas_tp;
	
	//create our container
	datas_tp data;

	//=============================//
	//	2) to create the first
	//		pic we do like in 
	//		draw_evolv
	//=============================//

	//get the infos reference
	const std::map<unsigned int, SpeciesInfo> &infos =
		m_species_controller.get_infos();

	//for each species
	for (	SpeciesController::infos_const_it it = infos.begin()	;
				it != infos.end()	;
				it ++	)
	{
		const SpeciesInfo &info = it->second;
		
		point_tp	first_point;

		first_point.get<0>()	=	info.species_id;
		first_point.get<1>()	=	info.species_name;
		
		//compute population
		std::pair<unsigned int , bool> tot_pop_ret =
			specied_population(info.species_id);

		//skip wrong species id
		if (!tot_pop_ret.second)	continue;

		else first_point.get<2>()	= tot_pop_ret.first;

		first_point.get<3>()	= m_clock.abs();
		first_point.get<4>()	=	m_clock.rel();

		first_point.get<5>()	= 0;
		
		//now the first point should be formed
		//insert it
		
			//create a cash vector to insert
		std::vector<point_tp>	cash_vect;
		cash_vect.push_back(first_point);

			//create a member of datas_tp 
		std::pair<unsigned int, std::vector<point_tp> >
			first_data(info.species_id , cash_vect);
	
		data.insert(first_data);

	}


	//=============================//
	//	start the step cicle
	//=============================//

	for (unsigned int i = 0 ; i < steps ; i ++)
	{
		StepLog log;
		
		std::cout	<<	std::endl
							<<	"###### STEP "
							<<	i +1
							<<	"######"
							<<	std::endl;

		if (!step(log))
		{
			std::cerr	<<	std::endl
								<<	"ERROR: "
								<<	"step retourned false"
								<<	std::endl;
			return false;
		}
		
		//now we parse the log
		for (	std::vector<PopulationVariation>::iterator it = 
						log.variations.begin()	;
					it != log.variations.end()	;
					it	++	)
		{
			//we need the species data to know initial pop
			
			std::vector<point_tp>	&points =
				(data.find(it->species_id))->second;

			//get first_point
			const point_tp &last_point = points.back();

			point_tp cash_point;
		
			//set id
			cash_point.get<0>() = it->species_id;
			//set name
			cash_point.get<1>()	= last_point.get<1>();
			//set new population which is the
			//last pop entered.
			cash_point.get<2>()	=	last_point.get<2>() + 
														it->variation					;
			//set time
			cash_point.get<3>()	= it->abs_time;
			cash_point.get<4>()	=	it->rel_time;
			//set step
			cash_point.get<5>()	=	i + 1 ;
		
			//cash point formed, insert it
			points.push_back(cash_point);
		
		}
	
	}//steps ended

	
	//=============================//
	//	now data is formed
	//	we have to write all down
	//=============================//

	if (!ofs.is_open())	
	{
		ofs.open("result_default.dat");
	}
	if (!ofs.good())
	{
		std::cerr	<<	std::endl
							<<	"ERROR: the output file stream you entered"
							<<	"is not good. exiting evolv returning false"
							<<	std::endl;
		return false;
	}


	//write comments on formatting
	std::list<std::string> header;

	std::string head;
	head = 	"#	RESULT OF A "	+
					boost::lexical_cast<std::string>(steps)	+
					" EVOLUTION";
	header.push_back(head);

	std::string format;
	format =std::string("# format is: species_id|species_name|population")
				+	std::string("|absolute_time|relative_time|number of steps");
	header.push_back(format);

	header.push_back("#");


	//insert the speciesinfo strings

	header.push_back("#species infos: ");

	std::map<unsigned int, SpeciesInfo> &infos_non_const =
		m_species_controller.get_infos();

	for (	SpeciesController::infos_it it = infos_non_const.begin()	;
				it != infos_non_const.end()	;
				it ++	)
	{
		SpeciesInfo &info = it->second;
		std::string info_string("#");
		info_string +=	info.get_info_string();

		header.push_back(info_string);
	}


	ofs << "#"	<< std::endl;

	for (	std::list<std::string>::iterator header_it =
					header.begin();
				header_it != header.end();
				header_it ++	)
	{
		ofs <<	*header_it
				<<	std::endl;
	}


	//we want to write all dat for each species
	for(	datas_tp::iterator data_it = data.begin();
				data_it != data.end();
				data_it ++	)
	{
		//get the species id
		unsigned int spec_id = data_it->first;
		//get the vector
		std::vector<point_tp>	&points =
				data_it->second;
		
		//write intestation
		ofs << 	std::endl	
				<<	"SPECIES_ID: "
				<<	spec_id
				<<	std::endl;

		//from the vector to the file
		for (	std::vector<point_tp>::iterator point_it =
						points.begin();
					point_it != points.end();
					point_it ++	)
		{
			// we form a string and then we write it
			std::string to_write;
			//spec_id
			to_write 	+= 	boost::lexical_cast<std::string>(point_it->get<0>());
			to_write 	+= 	"|";
			//spec_name
			to_write 	+= 	point_it->get<1>();
			to_write	+=	"|";
			//populations
			to_write	+=	boost::lexical_cast<std::string>(point_it->get<2>());
			to_write	+=	"|";
			//abs_time
			to_write	+=	boost::lexical_cast<std::string>(point_it->get<3>());
			to_write	+=	"|";
			//rel_tim
			to_write	+=	boost::lexical_cast<std::string>(point_it->get<4>());
			to_write	+=	"|";
			//step
			to_write	+=	boost::lexical_cast<std::string>(point_it->get<5>());
			to_write	+=	"|";

			ofs	<<	to_write	<<	std::endl;
		}

	}

	ofs.close();
	return true;
}

std::ifstream& operator>>(	std::ifstream 			&is	,
														EcosystemContainer 	&eco)
{
	bool input_done = false;
	
	while(!input_done)
  {
     std::stringbuf rsbuf;
     std::string    line;

     is.get(rsbuf);
     line=rsbuf.str();

		 //controll for comments
		 if (*line.begin() == '#')\
		 	continue;
			
		 if (!line.empty())
     {
      is.clear(); // We may be at EOF after reading a valid record.

			//parse the line
			typedef boost::tokenizer<boost::char_separator<char> > 
    	tokenizer_tp;
			boost::char_separator<char> sep("|");

			tokenizer_tp tokenizer(line, sep);
	
			//this is a line whith no information
			if(tokenizer.begin() == tokenizer.end())	continue;
	
	
			typedef std::vector<std::string> tokens_container;
			tokens_container tk_cont;	//contain all tokens separed by "|"

			//we copy the content of the string iside the
			//tk container to better handle it
			std::copy(tokenizer.begin() 					, 
								tokenizer.end() 						, 
								std::back_inserter(tk_cont)	);

			//controll the number of tokens

			if (tk_cont.size() - 1 > eco.m_species_controller.species_number())
			{
				std::cerr	<<	std::endl
									<<	"WARNING: line skypped for excedent species number"
									<<	std::endl;
				continue;
			}

			//now tk_cont contains all the tokens
				//first has to be very good is the subsystem
				//of the for "subs_x,subs_y"
			unsigned int subs_x = 0 , subs_y = 0;
			
			boost::char_separator<char> subs_sep(",");
			tokenizer_tp subs_tok(tk_cont[0] , subs_sep);			
		
			std::vector<std::string> subs_cont;
		
			std::copy(	subs_tok.begin()							,
									subs_tok.end()								,
									std::back_inserter(subs_cont)	);
			if (subs_cont.size()!=2)
			{
				std::cerr	<<	std::endl
									<<	"WARNING: subsystem token uncorrect. "
									<<	"skypping line"
									<<	std::endl;
				continue;
			}

			try
			{
				subs_x	= 
					lexical_cast<unsigned int>(subs_cont[0]);
				subs_y 	= 
					lexical_cast<unsigned int>(subs_cont[1]);
 			}
	 		catch(bad_lexical_cast &)
 	 		{
				std::cerr	<< std::endl 
									<< "WARNING: "
									<< "invalid field for subsystem coordinate"
									<< std::endl;
				continue;
  		}
			
			if (	subs_x >= eco.m_x_size or
						subs_x < 0 or
						subs_y >= eco.m_y_size or
						subs_y < 0	)
			{
				std::cerr	<<	std::endl
									<<	"WARNING: ecosystem coordinate out of boundaries. "
									<<	"skipping line"
									<<	std::endl;
				continue;	
			}
			
			//we have a subsystem coord now
			
			//now we have to pars consecutive tokens and do it good
			
			//need infos
			const SpeciesController::infos_tp &infos = 
				eco.m_species_controller.get_infos();
			
			for ( tokens_container::iterator tok_it = tk_cont.begin() + 1;
						tok_it != tk_cont.end();
						tok_it ++	)
			{
				//parse it like for subs coordinate
				unsigned int species_id = 0;
				unsigned int population = 0;

				boost::char_separator<char> pop_sep("-");
				tokenizer_tp pop_tok(*tok_it , pop_sep);			

				std::vector<std::string> pop_cont;

				std::copy(	pop_tok.begin()								,
										pop_tok.end()									,
										std::back_inserter(pop_cont)	);

				if (pop_cont.size()!=2)
				{
								std::cerr	<<	std::endl
												<<	"WARNING:  population entry oversize"
												<<	"skypping entry"
												<<	std::endl;
								continue;
				}

				try
				{
					species_id	= 
						lexical_cast<unsigned int>(pop_cont[0]);
					population 	= 
						lexical_cast<unsigned int>(pop_cont[1]);
				}
				catch(bad_lexical_cast &)
				{
					std::cerr	<< std::endl 
										<< "WARNING: "
										<< "invalid species population, skipping entry"
										<< std::endl;
					continue;
				}

				//now we have shurely 2 numbers
					//rest to check if there is a species int he controller
				
				SpeciesController::infos_const_it info_it =
					infos.find(species_id);
				
				const SpeciesInfo &info = info_it->second;

				if (info_it == infos.end())
				{
					std::cerr	<<	std::endl
										<<	"WARNING: no info finded whith this species id. "
										<<	"entry skipped"
										<<	std::endl;
					continue;
				}
				

				//now we have to form the population
				for (unsigned int i = 0 ; i < population ; i++)	
				{	
					//for animal we can use m_newborn
					if (info.is_animal)
					{
						eco.m_newborn(	species_id 						,
														subs_x 								,
														subs_y 								, 
														eco.m_rand_int(1,100)	);
					}
					//vegetable cant so we insert manually
					else
					{
						if	(eco.m_ecosystem[subs_x][subs_y].is_full(species_id))
						{
							std::cerr	<< 	std::endl
												<<	"WARNING: there is no space for the "
												<<	"newborn of species "
												<<	species_id	<< "in subsystem "	
												<< 	subs_x << "," << subs_y
												<<	std::endl;
							continue;
						}
						IndividualVegetable veg;
						//remember that in this realize veg can't die
						veg.hp() = 100;

						veg.birth_date() = DateOfBirth(eco.m_clock);
						veg.id_number() = eco.m_last_existance_id + 1;

						veg.species_id() 		= info.species_id;
						veg.species_name() 	= info.species_name;
						veg.life_coast() 		= info.life_coast;
						veg.health_status() = info.health_status;
						veg.calorie() 			= info.calorie;
						veg.life_space() 		= info.life_space;

						veg.life_coast()		= info.life_coast;


						if (eco.m_ecosystem[subs_x][subs_y].insert(veg))
						{	
							//increment the reference number
							eco.m_last_existance_id ++;
							#ifdef RUN_DBG
								std::cerr	<<	std::endl
													<<	"NEWBORN INSERTED"
													<<	std::endl
													<<	veg
													<<	std::endl;
							#endif
						}
						else continue;
					
					}
				}
				//should_be_done

			}
			
	
			input_done = true;
		 }
	   if (is.rdstate() == std::ios::failbit) is.clear();
     if (! is.good()) input_done = true; 
     else
     {
  	 	is.get();   // Skip past delimiter (newline). 
	   	is.clear(); // Be happy if it doesn't succeed.
  	 }
	}

	return is;
}

