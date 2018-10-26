//-----------------------------------------------------------------------
//
//	File Name:	draw_evolv.cpp
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

#include "TApplication.h" //the app
#include "TCanvas.h" 	//canvas
#include "TH2.h"			//draw
#include "THStack.h"	//draw
#include "TPaveLabel.h"	//root labels
#include "TLatex.h"
#include "TPad.h"
#include "TPaveText.h"
#include "TGraph.h"
#include "TMultiGraph.h"

#include "boost/shared_ptr.hpp"	//pinters to TH2
#include "boost/shared_array.hpp"
#include "boost/tokenizer.hpp"		

#include <cstdlib>	//system -> sleep
#include <time.h>

typedef EcosystemContainer Eco;

void Eco::draw_evolv(const unsigned int steps)
{

	#ifdef RUN_DBG
		std::cerr	<<	std::endl
							<<	"###### TESTING DRAW_EVOLV #####"
							<<	std::endl;
	#endif
	
	TApplication app("App",0,0);
	TCanvas *c1 = new TCanvas("c1","Ecosystem", 500, 400);
	

	//get infos
	const std::map<unsigned int, SpeciesInfo> &infos =
		m_species_controller.get_infos();

	typedef boost::shared_ptr<TH2F>	shared_th2;
	typedef std::map<unsigned int , shared_th2 > histograms_tp;

	histograms_tp	histograms;

	//population map
	std::map <unsigned int , unsigned int>	populations;

	unsigned int i_color = 1;
	for (	SpeciesController::infos_const_it it = infos.begin()	;
				it != infos.end()	;
				it ++	)
	{
		const SpeciesInfo &cash_info = it->second;

		shared_th2 cash_th2( 
			new TH2F (	cash_info.species_name.c_str() 	,
									cash_info.species_name.c_str()	,
									m_x_size	,	0	,	m_x_size				,
									m_y_size	,	0	,	m_y_size				)
		);

		cash_th2->SetFillColor(i_color);
		i_color++;

		histograms.insert	(	std::pair<unsigned int , shared_th2>
													(	cash_info.species_id	,
														cash_th2							) 
											);
		//fill populations whith 0's
		populations.insert	(	std::pair< unsigned int , unsigned int> 
														(	cash_info.species_id	,
															0											)			
												);

	}
	
	//create the stack
	THStack stack("ecosystem" , "ecosystem");

	//compute the tatal number of vivent
	//subsystem to set the stack maximum
	unsigned int total_vivents = 0;

	for (	SpeciesController::infos_const_it info_it =	
					m_species_controller.get_infos().begin()	;
				info_it != infos.end()	;
				info_it ++
			)
	{
		const SpeciesInfo &info = info_it->second;

		int tot_vivent_store = 100 / info.life_space;

		total_vivents += tot_vivent_store;

	}


	stack.SetMinimum(0);
	stack.SetMaximum(total_vivents);
	//fill the stack, hope it works
	for	(	histograms_tp::iterator it 
					= histograms.begin()	;
				it != histograms.end()	;
				it	++	)
	{
		stack.Add( it->second.get() );
	}

	//first we need to make a starting picture
	//and fill all the hists
	for	(	histograms_tp::iterator it 
					= histograms.begin()	;
				it != histograms.end()	;
				it	++	)
	{
		//get hystogram reference
		shared_th2 &hist = it->second;
		
		#ifdef RUN_DBG
			std::cerr	<<	std::endl
								<<	"WORKING ON HIST:  "
								<<	hist->GetName()
								<<	std::endl;
		#endif

		//get spec_info
		SpeciesController::infos_const_it spi_it =
			m_species_controller.get_info(it->first);
				
		const SpeciesInfo &spi = spi_it->second;

		#ifdef RUN_DBG
			std::cerr	<<	std::endl
								<<	"SPECIES INFO"
								<<	std::endl
								<<	spi
								<<	std::endl;
		#endif

		for (unsigned int i = 0 ; i < m_x_size  ; i++ )
		{
			for (unsigned int j = 0 ; j < m_y_size  ; j++ )
			{
	
				std::pair <unsigned int , bool>	returned_count =
					subsystem(i,j).count_vivents(spi);

				#ifdef RUN_DBG
					std::cerr	<<	std::endl
										<<	"SPECIES_ID: "
										<<	it->first
										<<	std::endl
										<<	"SUBSYSTEM: "
										<<	i << "," << j
										<<	std::endl
										<<	"COUNTED ANIMALS: "
										<<	returned_count.first
										<<	std::endl
										<<	"TRUE o FALSE: "
										<<	returned_count.second
										<<	std::endl;
				#endif

				if (!returned_count.second)
				{
					std::cerr	<<	std::endl
										<<	"ERROR: "
										<<	"in Ecosystem::evolv_draw: "
										<<	"bad species number "
										<<	it->first
										<<	std::endl;
					break;
				}
				
				hist->Fill(i , j , returned_count.first);

			}
		}

	}
	//first picture should be done
	//now draw it

	stack.Draw("lego1");
	

	//first pick made now evolving

	for (unsigned int i = 0 ; i < steps ; i++)
	{
		//evolv
		#ifdef RUN_DBG
			std::cerr	<<	std::endl
								<<	"############## STEP "
								<<	i
								<<	" ##############"
								<<	std::endl;
		#endif
		step();

		//pick
		for	(	histograms_tp::iterator it 
						= histograms.begin()	;
					it != histograms.end()	;
					it	++	)
		{
			//get hystogram reference
			shared_th2 &hist = it->second;

			//clear all
			//not smart but works
			hist->Reset();

			std::map<unsigned int , unsigned int>::iterator
				pop_it = populations.find(it->first);

			if (pop_it == populations.end())
			{
				std::cerr	<<	std::endl
									<<	"ERROR:"
									<<	std::endl;
			}
		
			//reset the pop.count
			pop_it->second = 0;

			#ifdef RUN_DBG
				std::cerr	<<	std::endl
									<<	"WORKING ON HIST:  "
									<<	hist->GetName()
									<<	std::endl;
			#endif

			//get spec_info
			SpeciesController::infos_const_it spi_it =
				m_species_controller.get_info(it->first);
				
			const SpeciesInfo &spi = spi_it->second;

			#ifdef RUN_DBG
				std::cerr	<<	std::endl
									<<	"SPECIES INFO"
									<<	std::endl
									<<	spi
									<<	std::endl;
			#endif

			for (unsigned int i = 0 ; i < m_x_size  ; i++ )
			{
				for (unsigned int j = 0 ; j < m_y_size  ; j++ )
				{
					//get specied_number and controll
	
					std::pair <unsigned int , bool>	returned_count =
						subsystem(i,j).count_vivents(spi);

					#ifdef RUN_DBG
						std::cerr	<<	std::endl
											<<	"SPECIES_ID: "
											<<	it->first
											<<	std::endl
											<<	"SUBSYSTEM: "
											<<	i << "," << j
											<<	std::endl
											<<	"COUNTED ANIMALS: "
											<<	returned_count.first
											<<	std::endl
											<<	"TRUE o FALSE: "
											<<	returned_count.second
											<<	std::endl;
					#endif

					if (!returned_count.second)
					{
						std::cerr	<<	std::endl
											<<	"ERROR: "
											<<	"in Ecosystem::evolv_draw: "
											<<	"bad species number "
											<<	it->first
											<<	std::endl;
						break;
					}
				
					hist->Fill(i , j , returned_count.first);
					pop_it->second += returned_count.first;

				}
			}	

		}

		
		
		std::cerr	<<	std::endl << "DRAW"	<< std::endl;
		
		stack.Draw("lego1");
		
		//text
			//prepare tokens
		TLatex text;
		std::string time;
		time = boost::lexical_cast<std::string>(m_clock.abs());	
		text.DrawLatex(0,0.5,time.c_str());

		c1->Modified();
		c1->Update();
	}
	
	app.Run(); 
}


void Eco::draw_evolv_fast(const unsigned int steps , std::string options)
{
	/////////////////////////////
	//	drawing oprions are:
	//	-refresh: refresh during evolution
	//	-populatons:	print a population graph
	//	-phase:	print the phase diagram 
	//		only possible in debug
	//		whith 2 species
	/////////////////////////////

	//-----------------------------//
	//	parse the options
	//-----------------------------//

	bool 	refresh_opt 			= false;
	bool	refresh_finded		=	false;
	bool 	populations_opt		= false;
	bool 	populations_finded= false;

	#ifdef RUN_DBG
		bool 	phase_opt					=	false;
		bool 	phase_finded			= false;
	#endif

	typedef boost::tokenizer<boost::char_separator<char> > 
    tokenizer_tp;
	boost::char_separator<char> sep("_");
	
	tokenizer_tp tokenizer(options, sep);

	typedef std::set<std::string> tokens_container;
	tokens_container opt_cont;	//contain all tokens separed by "|"

	//we copy the content of the string iside the
	//tk container to better handle it
	std::copy(tokenizer.begin() 									, 
						tokenizer.end() 										, 
						inserter(opt_cont,opt_cont.begin())	);

	#ifdef RUN_DBG
		for (	tokens_container::iterator it = opt_cont.begin();
					it	!= opt_cont.end();
					it ++	)
		{
			std::cerr	<< std::endl	
								<< *it;
		}
	#endif
	
	//look for "refresh"
	if (	opt_cont.find(std::string("refresh"))  
				!=		opt_cont.end()	)	
	{		
		refresh_opt 	= true;
		refresh_finded= true;
	}
	
	if (	opt_cont.find(std::string("populations")) != 
							opt_cont.end()	)	
	{
		populations_opt 	= true;
		populations_finded=	true;
	}
	if (opt_cont.find(std::string("phase")) != 
							opt_cont.end() )	
	{
		#ifdef RUN_DBG
			phase_opt = true;
			phase_opt = true;
		#endif
		#ifndef RUN_DBG
			std::cerr	<<	std::endl
								<<	"WARNING: phase option only available in debug_mode"
								<<	std::endl;
		#endif
	}
	if(!refresh_finded and !populations_finded)
	{
		std::cerr	<<	std::endl
							<<	"WARNING: bad options inserted "
							<<	"set default ones"
							<<	std::endl;
		refresh_opt 		= true;
		populations_opt = true;
	}

	
	
	#ifdef RUN_DBG
		std::cerr	<<	std::endl
							<<	"###### TESTING DRAW_EVOLV #####"
							<<	std::endl;
	#endif
	
	TApplication app("App",0,0);
	TCanvas *c1 = new TCanvas("c1","Ecosystem", 800,600);
	//TCanvas	*c2	=	new TCanvas("c2","Populations",500,400);

	c1->Divide(2,2);
	c1->cd(1);

	//get infos
	const std::map<unsigned int, SpeciesInfo> &infos =
		m_species_controller.get_infos();

	typedef boost::shared_ptr<TH2F>		shared_th2;

	typedef boost::shared_ptr<TGraph>	shared_tgraph;

	typedef unsigned int population_number ;


	typedef boost::shared_array<int> i_array;
	
	typedef std::pair	<	int ,	//time
											int	>	//total_population(t)
		pop_point;

	typedef std::vector<pop_point> pop_vect;
	typedef	boost::tuple<	shared_th2				,	//0 Thistogram
												shared_tgraph			,	//1 Tgraph
												population_number	,	//2 tot animals in ecosys
												std::string				,	//3 species_name
												pop_vect 					> 
		drawings;
	//typedef std::map<unsigned int , shared_th2 > histograms_tp;
	typedef	std::map<unsigned int , drawings>	draw_map_tp;

	//histograms_tp	histograms;
	draw_map_tp	draw_map;

	
	

	//population map
//	std::map <unsigned int , unsigned int>	populations;

	unsigned int i_color = 1;
	for (	SpeciesController::infos_const_it it = infos.begin()	;
				it != infos.end()	;
				it ++	)
	{
		const SpeciesInfo &cash_info = it->second;

		shared_th2 cash_th2( 
			new TH2F (	cash_info.species_name.c_str() 	,
									cash_info.species_name.c_str()	,
									m_x_size	,	0	,	m_x_size				,
									m_y_size	,	0	,	m_y_size				)
		);
			
//		cash_th2->GetZaxis()->SetLimits(0.,10.);
		cash_th2->SetFillColor(i_color);
		
		shared_tgraph	cash_graph(
			new	TGraph()
		);

		cash_graph->SetLineColor(i_color);

		i_color++;

		draw_map.insert	(	std::pair<unsigned int, drawings>(
												cash_info.species_id		,
												drawings(	cash_th2 		, 
																	cash_graph 	,
																	0						,
																	cash_info.species_name	,
																	pop_vect()
												)	
											)
		);
	}
	
	//create the stack
	THStack stack("ecosystem" , "ecosystem");

	//now we have to compute the maximum number of vivent
	//reachable in a subsystem in order to set a maximum
	//hight of the stack
	unsigned int total_vivents = 0;

	for (	SpeciesController::infos_const_it info_it =	
					m_species_controller.get_infos().begin()	;
				info_it != infos.end()	;
				info_it ++
			)
	{
		const SpeciesInfo &info = info_it->second;

		int tot_vivent_store = 100 / info.life_space;

		total_vivents += tot_vivent_store;

	}


	stack.SetMinimum(0);
	stack.SetMaximum(total_vivents);


	for (	draw_map_tp::iterator it =
					draw_map.begin()		;
					it != draw_map.end();
					it	++)
	{
		//get the shared_th2
		shared_th2 &hist_ptr = it->second.get<0>();
		stack.Add( hist_ptr.get() );
	}

	//first we need to make a starting picture
	//and fill all the hists
		
	for (	draw_map_tp::iterator it =
					draw_map.begin()		;
					it != draw_map.end();
					it	++)
	{
		//get hystogram reference

		//shared_th2 &hist = it->second;
		shared_th2 &hist = it->second.get<0>();
		
		#ifdef RUN_DBG
			std::cerr	<<	std::endl
								<<	"WORKING ON HIST:  "
								<<	hist->GetName()
								<<	std::endl;
		#endif

		//get spec_info
		SpeciesController::infos_const_it spi_it =
			m_species_controller.get_info(it->first);
				
		const SpeciesInfo &spi = spi_it->second;

		#ifdef RUN_DBG
			std::cerr	<<	std::endl
								<<	"SPECIES INFO"
								<<	std::endl
								<<	spi
								<<	std::endl;
		#endif

		for (unsigned int i = 0 ; i < m_x_size  ; i++ )
		{
			for (unsigned int j = 0 ; j < m_y_size  ; j++ )
			{
				//get specied_number and controll
	
				std::pair <unsigned int , bool>	returned_count =
					subsystem(i,j).count_vivents(spi);

				#ifdef RUN_DBG
					std::cerr	<<	std::endl
										<<	"SPECIES_ID: "
										<<	it->first
										<<	std::endl
										<<	"SUBSYSTEM: "
										<<	i << "," << j
										<<	std::endl
										<<	"COUNTED ANIMALS: "
										<<	returned_count.first
										<<	std::endl
										<<	"TRUE o FALSE: "
										<<	returned_count.second
										<<	std::endl;
				#endif

				if (!returned_count.second)
				{
					std::cerr	<<	std::endl
										<<	"ERROR: "
										<<	"in Ecosystem::evolv_draw: "
										<<	"bad species number "
										<<	it->first
										<<	std::endl;
					break;
				}
				
				hist->Fill(i , j , returned_count.first);

			}
		}

		//get total animal
		unsigned int tot_animals = 
			specied_population(it->first).first;
		
		//set tot population
		it->second.get<2>() = tot_animals;
		
		it->second.get<4>().push_back(
			std::pair<int, int>(0 , tot_animals)
		);

	}
	//first picture should be done
	//now draw it
	c1->cd(1);
	stack.Draw("lego1");

	//first pick made now evolving

	for (unsigned int i = 0 ; i < steps ; i++)
	{
		//evolv
		#ifdef RUN_DBG
			std::cerr	<<	std::endl
								<<	"############## STEP "
								<<	i
								<<	" ##############"
								<<	std::endl;
		#endif
		//logs
		StepLog log;
		if (!step(log))
		{
			std::cerr	<<	std::endl
								<<	"ERROR: "
								<<	"step retourned false"
								<<	std::endl;
		}
		

		//now the log is formed
		//parse it and update the 
		for (	std::vector<PopulationVariation>::iterator it = 
						log.variations.begin()	;
					it != log.variations.end()	;
					it	++	)
		{
			//get the hist
			draw_map_tp::iterator finded =
				draw_map.find(it->species_id);

			if (finded == draw_map.end())
			{
				std::cerr	<<	std::endl
									<<	"ERROR: "
									<<	"inside DRAW_EVOLV_FAST: "
									<<	"species_id not matching"
									<<	std::endl;
				continue;
			}

			c1->cd(1);
			//dump the hist
			shared_th2& h2 = finded->second.get<0>();

			h2->Fill(	it->subs_coord.first , 
								it->subs_coord.second,
								it->variation	);

			c1->cd(3);
	//		shared_tgraph	&graph = finded->second.get<1>();
			unsigned int nw_total_animals = 
				finded->second.get<2>() + it->variation;

			finded->second.get<2>() = nw_total_animals;

			finded->second.get<4>().push_back(	
				std::pair<int , int> (	m_clock.abs() 		,
																nw_total_animals	)
			);
		}

		
		
		std::cerr	<<	std::endl << "DRAW"	<< std::endl;

			//DRAW LEGO
		c1->cd(1);	
		
		if (refresh_opt)	stack.Draw("lego1");
		
		//text
			//prepare tokens
		
		c1->cd(2);


		TPaveText time_infos(0,0,1,1,"NDC");

		std::string abs_time("Absolute time: ");
		abs_time += boost::lexical_cast<std::string>(m_clock.abs());	
	
		std::string rel_time("Relative time: ");
		rel_time	+= boost::lexical_cast<std::string>(m_clock.rel());

		std::string stepping("Steps: ");
		stepping += boost::lexical_cast<std::string>(i);

		time_infos.AddText("TIME:");
		time_infos.AddText(abs_time.c_str());
		time_infos.AddText(rel_time.c_str());
		time_infos.AddText(stepping.c_str());

		//DRAW TIME
		if (refresh_opt) time_infos.Draw();

		
		//POPULATIONS
		c1->cd(4);
		TPaveText	populations_info(0,0,1,1,"NDC");
		
		for (	draw_map_tp::iterator it =
					draw_map.begin()		;
					it != draw_map.end();
					it	++)
		{
			std::string cash_pop_info("Species ");
			//get drawing
			drawings&	species_draw = it->second;
			cash_pop_info += species_draw.get<3>();
			cash_pop_info += " ";
			cash_pop_info += boost::lexical_cast<std::string>
												(species_draw.get<2>());
			
			//pop_info is formed
			populations_info.AddText(cash_pop_info.c_str());
		}
		
		if (refresh_opt) populations_info.Draw();

		//REFRESH CANVAS
		c1->Modified();
		c1->Update();

	}
		
	//once finished the evolution one can print out
	//the populations graphs
	
	TCanvas *c2 = new TCanvas("c2","Populations", 800,600);
	
		//create a TGraph vector
	std::vector<shared_tgraph>	graphs;
	
		//fill it
	
	//this method leaves the zeros
	for (	draw_map_tp::iterator it =
					draw_map.begin()		;
					it != draw_map.end();
					it	++)
	{
		//need to build array to pass to root
			//take pop_vect reference
		const pop_vect	&population_vector	=
			it->second.get<4>()	;
		
		i_array	time_arr (new int [population_vector.size()] );
		i_array	pop_arr	 (new int [population_vector.size()] );
		
		//fill the arrays
		unsigned int i = 0;
		for (	pop_vect::const_iterator pop_it =
						population_vector.begin()	;
					pop_it != population_vector.end()	;
					pop_it++)
		{
			time_arr[i] = pop_it->first;
			pop_arr[i]	=	pop_it->second;
			i++;
		}
		#ifdef RUN_DBG
			std::cerr	<<	std::endl
								<<	"ARRAYS: "
								<<	std::endl;
			
			std::cerr	<<	std::endl
								<<	it->second.get<3>()
								<<	std::endl;
			for (unsigned int i = 0 ; i < population_vector.size(); i ++)
			{
				std::cerr	<<	"#"	<<	i		<< ": "
									<<	"t: "	<<	time_arr[i]	<< " , "
									<<	"p: "	<<	pop_arr[i]	
									<<	std::endl ;

									
			}
			
		#endif

		shared_tgraph cash_graph( 
			new TGraph (	population_vector.size(),	
										time_arr.get()	,
										pop_arr.get()		)
		);

		cash_graph->SetMarkerColor(it->second.get<0>()->GetFillColor());
		cash_graph->SetLineColor(it->second.get<0>()->GetFillColor());
		graphs.push_back(cash_graph);
	}
	

	#ifdef RUN_DBG
		std::cerr	<<	std::endl
							<<	"VECTORS: "
							<<	std::endl;

		for (	draw_map_tp::iterator it =
						draw_map.begin()		;
					it != draw_map.end();
					it	++)
		{
			//get vector
			pop_vect &population_vector =
				it->second.get<4>();
			
			std::cerr	<<	std::endl
								<<	it->second.get<3>()
								<<	std::endl;

			for (	pop_vect::const_iterator pop_it =
							population_vector.begin()	;
						pop_it != population_vector.end()	;
						pop_it++)
			{
				std::cerr	<<	"t: "	<<	pop_it->first	<< " , "
									<<	"p: "	<<	pop_it->second
									<<	std::endl ;
			}	
		}
	#endif

	TMultiGraph multi_graph;

	for (	std::vector<shared_tgraph>::iterator it	=
					graphs.begin();
				it != graphs.end();
				it++	)
	{
		multi_graph.Add(it->get(),"L*");
	}



	
	if (populations_opt) multi_graph.Draw("A");

	c2->Modified();
	c2->Update();


	app.Run(); 
	delete c1;
	delete c2;
}
