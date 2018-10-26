//-----------------------------------------------------------------------
//
//	File Name:	step.cpp
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

#include <iostream>
#include <fstream>
#include <vector>
#include <list>

#include "ecosystem.h"
#include "fieldchangers.hpp" //change animal field
#include "classcompares.hpp"	//LikeRefCmp

typedef EcosystemContainer Eco;

bool Eco::step()
{	
	/******************************
	*			GET RANDOM SUBSYSTEM		*
	******************************/
	unsigned int subs_x	= m_rand_int(0, m_x_size -1);
	unsigned int subs_y	= m_rand_int(0, m_y_size -1);
	
	SubsystemContainer &subs =	subsystem(subs_x , subs_y);
	
	/******************************
	*	GET ANIMAL AND VEGETABLE SET*
	******************************/
	
	SubsystemContainer::animal_set
		&an_set = subs.animal_sub_ecosystem();
	
		//it's not really important to declare it here
		//but is more clean for the reader
	SubsystemContainer::vegetable_set
		&veg_set = subs.vegetable_sub_ecosystem();

	/******************************
	*		GET RANDOM ANIMAL:				*
	*                             *
	* unfortunatly this operation	*
	*	is linear in complexity			*
	*	fee to pay for the multindex*
	*	container										*
	*                             *
	******************************/


	unsigned int total_animals = an_set.size();
	if (total_animals > 0)
	{
		unsigned int rand_position = m_rand_int(0,total_animals -1);

			//appo index_reference
		SubsystemContainer::index_an_by_id& an_id_index =
			an_set.get<SubsystemContainer::id>();

			//predator is intended as an animal that will try to eat
			//also a bunny is considered a predator of grass
		SubsystemContainer::index_an_by_id::iterator it_predator = 
			an_id_index.begin();
		
		//here could be done like
		//if rand_pos is > total/2 start from begin
		//else strart from end() and decrease;

	
		//alternative method
		//create a vector of iterators
		std::vector< SubsystemContainer::index_an_by_id::iterator >
			iterators_vect;

		for (	SubsystemContainer::index_an_by_id::iterator it 
						= an_id_index.begin();
					it != an_id_index.end();
					it ++	)
		{
			iterators_vect.push_back(it);
		}

		#ifdef VERBOSE_DBG
			//size check
				std::cerr	<<	std::endl
								<<	"PRINTING THE CONTAINER"
								<<	std::endl
								<<	subs
								<<	std::endl;
		
			if(	iterators_vect.size() != an_set.size()	)
			{
				std::cerr	<<	std::endl
									<<	"ERROR: CASH CONTAINER SIZE NOT MATCHING"
									<<	std::endl;
				std::cerr	<<	"sub:" 	<<	iterators_vect.size()
									<<	std::endl
									<<	"vect:"	<<	an_set.size()
									<<	std::endl;
			
			

				std::cerr	<<	std::endl
									<<	"PRINTING THE VECTOR"
									<<	std::endl;
			
				for (	std::vector< SubsystemContainer::index_an_by_id::iterator >::iterator 
								vect_it = iterators_vect.begin();
							vect_it != iterators_vect.end();
							vect_it ++ )
				{
					std::cerr	<<	*(*vect_it)
										<<	std::endl;
				}

				typedef std::vector<SubsystemContainer::index_an_by_id::iterator>::iterator
					vect_it_tp;

				typedef	SubsystemContainer::index_an_by_id::iterator
					an_it_tp;
			
				vect_it_tp 	vect_ck = iterators_vect.begin();
				an_it_tp		an_ck		=	an_id_index.begin();

				//making a 1on1 check
				bool checker = true;
				while (	vect_ck != iterators_vect.end()
									and
								an_ck		!= an_id_index.end()		
							)
				{
					if(	(*vect_ck)->id_number()	!= an_ck->id_number()	)
					{
						std::cerr	<<	std::endl
											<<	"TWO DISEGUAL ANIMALS"
											<<	std::endl
											<<	*(*vect_ck)
											<<	std::endl
											<<	*an_ck
											<<	std::endl;
					}
					checker = false;
					vect_ck ++;
					an_ck	++;
				}
			}


		#endif

		rand_position = m_rand_int	(0 , iterators_vect.size() -1);			
		it_predator = iterators_vect[rand_position];



		#ifdef RUN_DBG
			std::cerr	<<	"PRINTING THE *IT_PREDATOR"
								<<	std::endl 
								<<	*it_predator
								<<	std::endl;
		#endif
		
		#ifdef VERBOSE
			std::cerr	<<	"THE ANIMAL SELECTED IS: "
								<<	std::endl 
								<<	*it_predator
								<<	std::endl;
		#endif
		

		if (it_predator->species_id() == 0)
		{
			std::cerr	<<	std::endl
								<<	"ERROR: CRITICAL ERROR! there is an animal whith"
								<<	"species id 0 in this subsystem."
								<<	subs_x	<<	","	<< subs_y
								<<	std::endl;
			#ifdef RUN_DBG
				std::cerr	<<	"PRINTING THE SUBSYSTEM"
									<<	std::endl << std::endl
									<<	subs
									<<	std::endl;
			#endif
			return false;
		}

		//now we should have an animal

		//due to the fact that the animal had been selected
		//it will interact whith the word so the clock has to
		//tick and the life had to be payed
		//in this way we do it once for all
		m_clock.tick();
		//get hp predator

		#ifdef SAFE
			unsigned int predator_hp = it_predator->hp();
		#endif
		#ifndef SAFE
			int predator_hp = it_predator->hp();
		#endif

		//pay life_coast
		#ifdef SAFE
			int discrimine_1 = predator_hp - it_predator->life_coast();
			if (discrimine_1 <= 0 )	predator_hp = 0;
			else predator_hp -= it_predator->life_coast();
		#endif
		#ifndef SAFE
			predator_hp -= it_predator->life_coast();
		#endif

		//write down the life coast subtrection
		if 	(!an_id_index.modify	(	it_predator 									, 
																change_animal_hp(predator_hp)	)
				)
		{
			std::cerr	<<	std::endl
								<<	"ERROR: modify fails at the end of reproduction "
								<<	"cicle."
								<<	std::endl;
			return false;
		}

		//-----------------------------//
		//	CONTROLL IF THE ANIMAL		 //
		//				IS ALIVE						 //
		//-----------------------------//
		
		if (it_predator->hp() <= 0)
		{
			m_dead( it_predator , an_id_index);
			#ifdef RUN_DBG
				std::cerr	<<	std::endl
									<<	"#################"
									<<	"#  DEBUG DEBUG  #"
									<<	"#################"
									<<	std::endl;
				std::cerr	<<	"CONTAINER SIZE : "
									<<	an_set.size();
				std::cerr	<<	std::endl
									<<	std::endl
									<<	subs
									<<	std::endl;
			#endif
			return true;
		}

	/********************************
	*	CALCULATE LIBIDO AND APPETITE	*
	*********************************/
	//get the health_status
	#ifdef SAFE
		unsigned int predator_hs = it_predator->health_status();
	#endif
	#ifndef SAFE
		int predator_hs = it_predator->health_status();
	#endif


	float predator_libido 		= 0;
	float predator_appetite 	= 0;

	#ifdef SAFE
		float range = static_cast<int>(predator_hp) - 
									static_cast<int>(predator_hs);
	#endif
	#ifndef SAFE
		float range = predator_hp - predator_hs;
	#endif
	float sex_range 	= 100 - predator_hs;
	float eat_range 	= predator_hs;
	
	float eat_prob = 0;
	//if hp > hs gave relevancy to sex
	if (range > 0)
	{
		predator_libido	=	((range / sex_range) * 50) + 50;
		predator_appetite		=	100 - predator_libido;
		eat_prob = predator_appetite;
	}
	//else give revelance to eat
	else
	{
		predator_appetite	=	(-1 * (range /eat_range) * 50) + 50;
		predator_libido	=	100	- predator_appetite;
		eat_prob= predator_appetite;
	}

	#ifdef VERBOSE
		std::cerr	<<	std::endl
							<<	"eat probability: "
							<<	eat_prob;
	#endif
	#ifdef COMPLETE
		//those step were not really necessary
		//cause appetito and libido will be calculated
		//indipendently to their values inside the IndividualAnimal
		//but for completenes and for statistical calculation
		//it's better to include them
		

		an_id_index.modify	(	it_predator										, 
													change_animal_libido(
														static_cast<unsigned int>(predator_libido)
													)	
												);
		an_id_index.modify	(	it_predator										, 
													change_animal_appetite(
														static_cast<unsigned int>(predator_appetite)
													)	
												);
	#endif

	//roll the 100 face die
	//if the number is < to the eat_prob -> eat
	//else look for reproduction

	//>>>>>>>>>>>>>>>>>>>|<<<<<<<<<<<<<<<<<<<<//
	//>>>>>>>>>>>>>>>  	EAT	  <<<<<<<<<<<<<<<<//
	//>>>>>>>>>>>>>>>>>>>|<<<<<<<<<<<<<<<<<<<<//

	int rand_num = m_rand_int(0,100);
	if(rand_num < eat_prob)
	{	
		/******************************
		*	GET THE BEST VIVENT  TO EAT *
		******************************/
		
			//-----------------------------//
			//	-first we need the species
			//	info for our predator
			//-----------------------------//

		#ifdef VERBOSE
			std::cerr	<<	std::endl
								<<	"EAT"
								<<	std::endl;
		#endif

		const SpeciesInfo	&predator_info = 
			m_species_controller.get_info_ref(it_predator->species_id());

		#ifdef RUN_DBG
			std::cerr	<<	std::endl
								<<	"PRINTING SPECIES_INFO FOR PREDATOR"
								<<	std::endl
								<<	predator_info
								<<	std::endl;
		#endif
			//------------------------------------------//
			//	-in SpeciesInfo::m_likings_by_lk_factor
			//		animal were sorted by decrescent like
			//		facotor. so we have to look if there is an
			//		animal whith the highest like factor
			//		and if it is alive!
			//		if not try whith the second and so on.
			//		if none finded the iterator remanins
			//		pointing to .end()
			//------------------------------------------//


		//animal index by eat
		SubsystemContainer::index_an_by_eat&	an_eat_index	=
			an_set.get<SubsystemContainer::eat>();
		//vegetable index by eat
		SubsystemContainer::index_veg_by_eat&	veg_eat_index	=
			veg_set.get<SubsystemContainer::eat>();
		//finded animal
		//if none -> end()
		SubsystemContainer::an_eat_it finded_animal =
			an_eat_index.end();
		//finded vegetable
		//if none	-> end()
		SubsystemContainer::veg_eat_it finded_vegetable =
			veg_eat_index.end();
		


		for (	SpeciesInfo::likings_by_lk_factor_iterator lk_it =
						predator_info.likings_by_lk_factor.begin()			;
					lk_it != predator_info.likings_by_lk_factor.end()	;
					lk_it ++	
				)
		{	
			//controll if the like_factor is <=0
			//break because they're sorted by descendent
			//like_factor
			if( lk_it->like_factor <= 0 ) break;
			//get best_spec_id
			const int best_spec_id = lk_it->liked_spec_id ;		

			//is he animal or vegetable?
			//we havee to know where to look for
			//so we need his species_info
			if ((m_species_controller.get_info(best_spec_id))->second.is_animal)
			{
				//controll if thre's an animal whith this spec_id in the
				//animal_set, they were just sorted by best hp so the first
				//is the better
				finded_animal = an_eat_index.find(best_spec_id);
				if(finded_animal == an_eat_index.end())\
					continue;	//no animal finded
				//animal could be dead!
				else if	(finded_animal->hp() <= 0)
				{
					//to die make the clock tick
					m_clock.tick();
					//remove the animal
					m_dead(finded_animal, an_eat_index);

					#ifdef RUN_DBG
						std::cerr	<<	std::endl
											<<	"#################"
											<<	"#  DEBUG DEBUG  #"
											<<	"#################"
											<<	std::endl;

						std::cerr	<<	std::endl
											<<	std::endl
											<<	subs
											<<	std::endl;
					#endif

					//continue because they were sorted
					//by descendent hp so the next one, 
					//if there is, is already dead
					finded_animal = an_eat_index.end();
					continue;
				}
				else	
				{	
					//WE HAVE A PREY ALIVE
					//tick the clock once for all for the prey
					m_clock.tick();
					break;	//we have an animal to eat!!
				}
			}
			else
			{
				//controll if thre's an vegtable whith this spec_id in the
				//vegetable_set, they were just sorted by best hp so the first
				//is the better. in this realize vegetable always have 100hp
				//so we don't need to check if it's alive
				finded_vegetable	=	veg_eat_index.find(best_spec_id);
				if(finded_vegetable == veg_eat_index.end())\
					continue;	//no vegetable finded
				else \
					break;	//we have a vegetable to eat!
			}

		}//end the for 

		//at this point if we have someone to eat one of the 2
		//iterators (finded_animal or finded_vegetable) shouldn't
		//be the end. otherwise we haven't finded to eat an it's better
		//to move the animal in another SubsystemContainer;

		/******************************
		*						FIGHT!						*
		******************************/
		if(finded_animal != an_eat_index.end())
		{
		//pay life_coast
		#ifdef VERBOSE
			std::cerr	<<	std::endl
								<<	"FIGHT"
								<<	std::endl;
		#endif
	
		//get finded_animal's hp
		#ifdef SAFE
			unsigned int prey_hp = finded_animal->hp();
		#endif
		#ifndef SAFE
			int prey_hp = finded_animal->hp();
		#endif

				//-----------------------------//
				//	Steps:
				//	-fight
				//	-if predator fails animals
				//		pay fight_coast 
				//		and life_coast
				//	-if predator succeed
				//		eat
				//		remove eated animal
				//		pays fight_coast and
				//			life_cost
				//	-tick the clok 2 times
				//-----------------------------//
			
			//fight
			//the difference between the predator hp
			//and prey hp goes from -99 to 99
			//then we roll a die betwneed -99 and 99
			//if the number is under pred_hp-prey_hp
			//the predator win

			//cause we just have predator_hp
			int range = predator_hp - prey_hp;
			
			#ifdef VERBOSE
				std::cerr	<< std::endl
									<< "PREDATOR: "
									<< it_predator->id_number()
									<< std::endl
									<< "HAS CIRCA  "
									<< ((range + 100) /2) 
									<< " CHANCE TO WIN:"
									<< std::endl;
			#endif
			
			//predator wins
			if (m_rand_int(-99,99) < range)
			{
				#ifdef VERBOSE
					std::cerr	<<	std::endl
										<< 	"PREDATOR WINS"
										<< 	std::endl;
				#endif
				//now eat
				float float_gain = 
					prey_hp * finded_animal->calorie()/100;
				unsigned int hp_gain = static_cast<unsigned int>(float_gain);
				if (hp_gain + predator_hp > 100)\
					predator_hp = 100;
				else	predator_hp += hp_gain;
				//remove the eated animal
				m_dead(finded_animal , an_eat_index);
				#ifdef RUN_DBG
					std::cerr	<<	std::endl
										<<	"#################"
										<<	"#  DEBUG DEBUG  #"
										<<	"#################"
										<<	std::endl;

					std::cerr	<<	std::endl
										<<	std::endl
										<<	subs
										<<	std::endl;
				#endif

				//pay fight_coast
				#ifdef SAFE
					int discrimine_2 = predator_hp - it_predator->fight_coast();
					if (discrimine_2 <= 0) predator_hp = 0;
					else predator_hp -= it_predator->fight_coast();
				#endif
				#ifndef SAFE
					predator_hp -= it_predator->fight_coast();
				#endif
				//now write down the new predator features
				
				//ATTENTION! it_predator is a an_id_index iterator
				an_id_index.modify(	it_predator										, 
														change_animal_hp(predator_hp)	);

				//exit
				return true;
			}
			else	//predator loose
			{
				#ifdef VERBOSE
					std::cerr	<<	std::endl
										<< 	"PREDATOR LOOSE"
										<< 	std::endl;
				#endif
				
				//prey is alive an pay life_coast
				#ifdef SAFE
					int discrimine_5 = prey_hp - finded_animal->life_coast();
					if (discrimine_5 <= 0)	prey_hp = 0;
					else prey_hp -= finded_animal->life_coast();
				#endif
				#ifndef SAFE
					prey_hp -= finded_animal->life_coast();
				#endif
		
				//prey and predator prey fight_coast
				#ifdef SAFE
					int discrimine_3 = predator_hp - it_predator->fight_coast();
					if (discrimine_3 <= 0) predator_hp = 0;
					else predator_hp -= it_predator->fight_coast();

					int discrimine_4 = prey_hp - finded_animal->fight_coast();
					if (discrimine_4 <= 0) prey_hp = 0;
					else prey_hp -= finded_animal->fight_coast();
				#endif
				#ifndef SAFE
					predator_hp -= it_predator->fight_coast();
					prey_hp 		-= finded_animal->fight_coast();
				#endif
				
			
				
				//--------------------------------//
				//	if the system is too static
				//	in his evolution, here
				//	predator can search in the
				//	adiacent subsystems if there
				//	is a prey whith less hp		
				//--------------------------------//
				
				//write all down
				an_id_index.modify(	it_predator										, 
														change_animal_hp(predator_hp)	);
				an_eat_index.modify(finded_animal							,
														change_animal_hp(prey_hp)	);
				return true;
			}
			
		}
		else if(finded_vegetable != veg_eat_index.end())
		{
			//eat the vegetable
			#ifdef VERBOSE
				std::cerr	<<	std::endl
									<<	"EAT A VEGETABLE"
									<<	std::endl;
			#endif

			//vegetable have 100 hp
			//always
			//predator eat 10hp * calorie
			float  float_gain = (10 * finded_vegetable->calorie())/100;
			unsigned int gain = static_cast<unsigned int>(float_gain);
			if (gain + it_predator->hp() >= 100)	\
				predator_hp = 100;
			else	predator_hp += gain;
			
			//write down
			an_id_index.modify(	it_predator										, 
													change_animal_hp(predator_hp)	);
			return true;
		}

		////////////////////
		//	NONE TO EAT		//
		////////////////////

		else  //move away
		{
			#ifdef VERBOSE
				std::cerr	<<	std::endl
									<<	"TRY TO MIGRATE CAUSE NONE FINDED TO EAT"
									<<	std::endl;
			#endif
			if(!m_migrate(	subs_x ,subs_y 							, 
				 							it_predator		, an_id_index )
			)
			{
				std::cerr	<<	std::endl
									<<	"ERROR: failed migration for animal "
									<<	*it_predator
									<<	std::endl;
				return false;
			}
		
			#ifdef VERBOSE
				std::cerr	<<	std::endl
									<<	"MOGRATION DONE"
									<<	std::endl;
			#endif

			#ifdef VERBOSE
				std::cerr	<<	std::endl
									<<	"STEP CONCLUDED WITHOUT ERRORS"
									<<	std::endl
									<<	"EXITING FROM STEP."
									<<	std::endl;
			#endif

			//exit from step
			return true;
		}

	}

	//>>>>>>>>>>>>>>>>>>>|<<<<<<<<<<<<<<<<<<<<//
	//>>>>>>>>>>>>>  REPRODUCE  <<<<<<<<<<<<<<//
	//>>>>>>>>>>>>>>>>>>>|<<<<<<<<<<<<<<<<<<<<//
	else	
	{

		#ifdef VERBOSE
			std::cerr	<<	std::endl
								<<	"REPRODUCTION"
								<<	std::endl;
		#endif
		SubsystemContainer::index_an_by_reproduce &an_repr_index =
			an_set.get<SubsystemContainer::reproduction>();

		SubsystemContainer::an_reproduce_it	to_repr	=
			an_repr_index.end();

		//we have to know if we want to search a male or a female
		if (it_predator->gender().is_male())
		{
			//look for a FEMALE
			to_repr=an_repr_index.find(
				boost::make_tuple(
					it_predator->species_id(),
					Gender("female")
				)
			);
			#ifdef VERBOSE
				std::cerr	<<	std::endl
									<<	"PREDATOR IS FEMALE"
									<<	std::endl;
			#endif
		}
		else if(it_predator->gender().is_female())
		{
			//look for  MALE
			to_repr=an_repr_index.find(
				boost::make_tuple(
					it_predator->species_id(),
					Gender("male")
				)
			);
			#ifdef VERBOSE
				std::cerr	<<	std::endl
									<<	"PREDATOR IS MALE"
									<<	std::endl;
			#endif
		}
		else if(	it_predator->gender().is_hermaphrodite()
								or
							it_predator->gender().is_asexual())
		{
			//DO NOTHING and return false
			std::cerr	<<	std::endl
								<<	"ERROR: in this realise no asexual or "
								<<	"heraphrodite reproduction is modelled; "
								<<	"EcosystemContainer::step() will return "
								<<	"false."
								<<	std::endl;

			return false;
		}
	
		//at this point our iterator should point to some
		//or if not our predator should look for a better place

		//there's no one -> move away
		if(to_repr == an_repr_index.end())
		{
			#ifdef VERBOSE
				std::cerr	<<	std::endl
									<<	"TRY MIGRATION CAUSE NONE FINDED TO REPRODUCE"
									<<	std::endl;
			#endif
			
			if(!m_migrate(	subs_x ,subs_y 							, 
				 							it_predator		, an_id_index )
			)
			{
				std::cerr	<<	std::endl
									<<	"ERROR: failed migration for animal "
									<<	*it_predator
									<<	std::endl;
				return false;
			}
						
			//migration occours
			#ifdef VERBOSE
				std::cerr	<<	std::endl
									<<	"MIGRATION COMPLETED."
									<<	std::endl;
			#endif

			#ifdef VERBOSE
				std::cerr	<<	std::endl
									<<	"STEP CONCLUDED WITHOUT ERRORS"
									<<	std::endl
									<<	"EXITING FROM STEP."
									<<	std::endl;
			#endif			
			
			return true;
		}
		//the one there is is dead
		else if (to_repr->hp() <= 0)
		{
			m_clock.tick();

			m_dead(to_repr , an_repr_index );

			//MOVE AWAY
			#ifdef VERBOSE
				std::cerr	<<	std::endl
									<<	"TRY TO MIGRATE CAUSE NONE FINDED TO REPRODUCE"
									<<	std::endl;
			#endif
			//MIGRATE!
			if(!m_migrate(	subs_x ,subs_y 							, 
				 							it_predator		, an_id_index )
				)
			{
				std::cerr	<<	std::endl
									<<	"ERROR: failed migration for animal "
									<<	*it_predator
									<<	std::endl;
				return false;
			}
			
			#ifdef VERBOSE
				std::cerr	<<	std::endl
									<<	"MIGRATION DONE."
									<<	std::endl;
			#endif

			#ifdef VERBOSE
				std::cerr	<<	std::endl
									<<	"STEP CONCLUDED WITHOUT ERRORS"
									<<	std::endl
									<<	"EXITING FROM STEP."
									<<	std::endl;
			#endif

			
			return true;
		}
		//no place for the newborn -> migrate
		else if (subs.is_full(it_predator->species_id()))
		{
			//MOVE AWAY
			#ifdef VERBOSE
				std::cerr	<<	std::endl
									<<	"TRY TO MIGRATE CAUSE NO SPACE FOR NEWBORN"
									<<	std::endl;
			#endif
			//MIGRATE!
			if(!m_migrate(	subs_x ,subs_y 							, 
				 							it_predator		, an_id_index )
				)
			{
				std::cerr	<<	std::endl
									<<	"ERROR: failed migration for animal "
									<<	*it_predator
									<<	std::endl;
				return false;
			}
			
			#ifdef VERBOSE
				std::cerr	<<	std::endl
									<<	"MIGRATION DONE."
									<<	std::endl;
			#endif

			#ifdef VERBOSE
				std::cerr	<<	std::endl
									<<	"STEP CONCLUDED WITHOUT ERRORS"
									<<	std::endl
									<<	"EXITING FROM STEP."
									<<	std::endl;
			#endif			

			return true;
		}
		else //we have someone to reproduce
		{	
			m_clock.tick();
			#ifdef SAFE
				unsigned int to_repr_hp = to_repr->hp();
			#endif
			#ifndef SAFE
				int to_repr_hp = to_repr->hp();
			#endif
			int range = 	to_repr_hp	 +
									 	it_predator->hp();
			//run the die
			//range goes from 2 to 200;
				//reproduction succeed
			if( m_rand_int(2,200) <= range)
			{
				//the female pay an additonal 
				//life coast for the gravidance;
				unsigned int newborn_hp = 0 ;
				if(to_repr->gender().is_female())
				{
					#ifdef VERBOSE
						std::cerr	<<	std::endl
											<<	"TO_REPR IS FEMALE"
											<<	std::endl;
					#endif
					#ifdef SAFE
						int discrimine_6 = to_repr_hp - to_repr->life_coast();
						if (discrimine_6 <= 0) to_repr_hp = 0;
						else to_repr_hp -= to_repr->life_coast();
					#endif
					#ifndef SAFE
						to_repr_hp -= to_repr->life_coast();
					#endif
					newborn_hp = to_repr->health_status()/2;
				}
				else	//the female is the predator
				{
					#ifdef VERBOSE
						std::cerr	<<	std::endl
											<<	"TO_REPR IS MALE"
											<<	std::endl;
					#endif
					#ifdef SAFE
						int discrimine_7 = predator_hp - it_predator->life_coast();
						if (discrimine_7 <= 0) predator_hp = 0;
						else predator_hp -= it_predator->life_coast();
					#endif
					#ifndef SAFE
						predator_hp -= it_predator->life_coast();
					#endif
					newborn_hp = it_predator->health_status()/2;
				}


				//NEW PUPPY!
				std::pair<bool , bool> newborn_return = 
					m_newborn(to_repr->species_id() , subs_x , subs_y , newborn_hp);

				if (	newborn_return == std::pair<bool , bool>(true,true)	)
				{
					std::cout	<<	std::endl
										<<	"A new animal of species "
										<<	to_repr->species_id()
										<<	" is born;"
										<<	std::endl;
					#ifdef RUN_DBG
						std::cerr	<<	std::endl
											<<	"########## DEBUG ########### "
											<<	"### PRINTING CONTAINER AFTER NEWBORN ###"
											<<	subs
											<<	std::endl;
					#endif
				}

				//insertion fail or unpresent species info
				else if (newborn_return == std::pair<bool , bool>(false,false))
				{
					std::cerr	<<	std::endl
										<<	"ERROR: failed insertion of newborn of species "
										<<	to_repr->species_id()
										<<	std::endl;
					
					#ifdef VERBOSE
						std::cerr	<<	std::endl
											<<	"ERROR: AN ERROR OCCOURED IN STEP() FUNCTION"
											<<	std::endl
											<<	"EXITING FROM STEP. RETURNING FALSE"
											<<	std::endl;
					#endif	
					
					//step fails
					return false;
				}
			}
			else //no newborn cause the female is not pregnant
			{
				#ifdef VERBOSE
					std::cerr	<<	std::endl
										<<	"FEMALE IS NOT PREGNANT"
										<<	std::endl;
				#endif
			}
			//pay life coast
			#ifdef SAFE
				int discrimine_8 = to_repr_hp - to_repr->life_coast();
				if (discrimine_8 <= 0) to_repr_hp = 0;
				else to_repr_hp -= to_repr->life_coast();
			#endif
			#ifndef SAFE
				to_repr_hp -= to_repr->life_coast();
			#endif

			//write down all the infos relatives to
			//predator and to_repr
			if 	(!an_repr_index.modify (	to_repr 										, 
																		change_animal_hp(to_repr_hp))
					)
			{
				std::cerr	<<	std::endl
									<<	"ERROR: modify fails at the end of reproduction "
									<<	"cicle."
									<<	std::endl;
				return false;
			}
			if 	(!an_id_index.modify	(	it_predator 									, 
																	change_animal_hp(predator_hp)	)
					)
			{
				std::cerr	<<	std::endl
									<<	"ERROR: modify fails at the end of reproduction "
									<<	"cicle."
									<<	std::endl;
				return false;
			}

			#ifdef VERBOSE
				std::cerr	<<	std::endl
									<<	"STEP CONCLUDED WITHOUT ERRORS"
									<<	std::endl
									<<	"EXITING FROM STEP."
									<<	std::endl;
			#endif			

			return true;
		}
	}


	}//end if total_animal>0
	else
	{
		std::cerr	<<	std::endl
							<<	"This Subsystem "
							<<	subs_x << "," << subs_y
							<< 	"contains no animal;"
							<<	std::endl;

		#ifdef VERBOSE
			std::cerr	<<	std::endl
								<<	"STEP CONCLUDED WITHOUT ERRORS"
								<<	std::endl
								<<	"EXITING FROM STEP."
								<<	std::endl;
		#endif		

		return true;
	}


}
