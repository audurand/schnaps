/*
 * main.cpp
 *
 * Learning using SCHNAPS
 * Copyright (C) 2009-2011 by Audrey Durand
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
 
#include "SCHNAPS/SCHNAPS.hpp"
#include "SCHNAPS/Plugins/Learning/Learning.hpp"
#include "PACC/Util.hpp"
 
#ifdef SCHNAPS_IS_UNIX
#include <sys/stat.h>
#include <unistd.h>
#else
#include <direct.h>
#include "XGetopt.h"
#endif

#include <cstdio>

using namespace SCHNAPS;
using namespace Plugins;
using namespace Learning;

int main(int argc, char* argv[]) {
	try{
		int lOpt;

		// configuration
		std::string lDirectory = "";
		std::string lConfigurationFile = "";
		std::string lInputLearning = "";
		std::string lParameters = "";
		std::string lScenario = "";

#ifdef SCHNAPS_FULL_DEBUG
		std::cout << "Arguments parsing\n";
#endif

		while ((lOpt = getopt(argc, argv, "d:c:s:p:i:")) != -1) {
			switch (lOpt) {
			case 'd':
				lDirectory.assign(optarg);
				break;
			case 'c':
				lConfigurationFile.assign(optarg);
				break;
			case 'i':
				lInputLearning.assign(optarg);
				break;
			case 'p':
				lParameters.assign(optarg);
				break;
			case 's':
				lScenario.assign(optarg);
				break;
			case '?':
				std::cerr << "Missing argument of option -" << optopt << ".\n";
				break;
			default:
				std::cerr << "Unknown option -" << optopt << ".\n";
				break;
			}
		}

#ifdef SCHNAPS_FULL_DEBUG
		std::cout << "Configuration\n";
#endif
		
		if (lConfigurationFile.empty()) {
			std::stringstream lOSS;
			lOSS << "The configuration file is empty; ";
			lOSS << "could not parse it.";
			schnaps_RunTimeExceptionM(lOSS.str());
		}
		
		if (lScenario.empty()) {
			std::stringstream lOSS;
			lOSS << "The given scenario is empty; ";
			lOSS << "could not simulate it.";
			schnaps_RunTimeExceptionM(lOSS.str());
		}

		Simulation::Simulator lSimulator;
		
		// add learning parameters (with default values)
		lSimulator.getSystem().getParameters().insertParameter("learning.episodes", new Core::UInt(50));
		lSimulator.getSystem().getParameters().insertParameter("learning.evaluate", new Core::Bool(false));
		lSimulator.getSystem().getParameters().insertParameter("learning.folders", new Core::Bool(false));
		lSimulator.getSystem().getParameters().insertParameter("learning.geas.alpha", new Core::Double(1));
		lSimulator.getSystem().getParameters().insertParameter("learning.learn", new Core::Bool(true));

		// set current working directy
		if (lDirectory.empty() == false) {
			int lChdir = chdir(lDirectory.c_str());
			schnaps_AssertM(lChdir == 0);
		}

		// configure simulator from file
#ifdef SCHNAPS_FULL_DEBUG
		std::cout << "Configure from file\n";
#endif
		PACC::XML::Document *lDocument = new PACC::XML::Document();
		lDocument->parse(lConfigurationFile);
		lSimulator.read(lDocument->getFirstDataTag());
		delete lDocument;

		// command-line parameters override configuration file.
		if (lParameters.empty() == false) {
#ifdef SCHNAPS_FULL_DEBUG
			std::cout << "Configure from command line\n";
#endif
			lSimulator.configure(lParameters);
		}
		
		// setup learning
		LearningModule::Handle lLearningModule = Core::castHandleT<LearningModule>(lSimulator.getSystem().getComponentHandle("Learning_LearningModule"));
		lLearningModule->setSystem(lSimulator.getSystemHandle());
		lLearningModule->setEnvironment(lSimulator.getEnvironmentHandle());
		lLearningModule->setClock(lSimulator.getClockHandle());
		
		// retrieve learning parameters
		unsigned int lEpisodes = Core::castHandleT<Core::UInt>(lSimulator.getSystem().getParameters().getParameterHandle("learning.episodes"))->getValue();
		bool lEvaluate = Core::castHandleT<Core::Bool>(lSimulator.getSystem().getParameters().getParameterHandle("learning.evaluate"))->getValue();
		bool lFolders = Core::castHandleT<Core::Bool>(lSimulator.getSystem().getParameters().getParameterHandle("learning.folders"))->getValue();
		
		// backup learning configuration
		std::string lLearningConf = lSimulator.getConfiguration();
		
		// backup evaluation input
		std::string lInputEvaluation = lSimulator.getPopulationManager().writeStr();
		
		// backup basic print parameters
		std::string lPrintPrefix_Basic = Core::castHandleT<Core::String>(lSimulator.getSystem().getParameters().getParameterHandle("print.prefix"))->getValue();
		bool lPrintConf = Core::castHandleT<Core::Bool>(lSimulator.getSystem().getParameters().getParameterHandle("print.conf"))->getValue();

		// declare useful streams
		std::ofstream lOFS;
		std::stringstream lSS;
		PACC::XML::Streamer *lOStreamer;
		
		// redirect stdout and stderr to file (for Colosse output)
		lSS.str("");
		lSS << lPrintPrefix_Basic << "out.txt";
		freopen(lSS.str().c_str(), "w", stdout);
		lSS.str("");
		lSS << lPrintPrefix_Basic << "err.txt";
		freopen(lSS.str().c_str(), "w", stderr);
		
		// learn	
		for (unsigned int i = 0; i < lEpisodes; i++) {
#ifdef SCHNAPS_FULL_DEBUG
			std::cout << "Episode " << i << "\n";
#endif
			// reset randomizers to random values
			lSimulator.clearRandomizer();
			lSimulator.getPopulationManager().getGenerator().clearRandomizer();
			
			// reset configuration for learning
			lSimulator.configure(lLearningConf);
			
			// set population to learning input
			lSimulator.getPopulationManager().clear();
			lSimulator.getPopulationManager().readStr(lInputLearning);
			
			// set print prefix for episode i
			if (lFolders) {
				// create folder for episode i
				lSS.str("");
				lSS << lPrintPrefix_Basic << i << "/";
#ifdef SCHNAPS_IS_UNIX
				mkdir(lSS.str().c_str(), 0777);
#else
				mkdir(lSS.str().c_str());
#endif
				lSS.str("");
				lSS << "print.prefix=" << lPrintPrefix_Basic << i << "/";
			} else {
				lSS.str("");
				lSS << "print.prefix=" << lPrintPrefix_Basic << i << "_";
			}
			lSimulator.configure(lSS.str().c_str());
			
			// open learning log files
			lSS.str("");
			lSS << lPrintPrefix_Basic << "LearningLog";
			lLearningModule->open(lSS.str());
			
			// run simulation (for learning)
			lSimulator.simulate(lScenario);
			
			// close log files
			lLearningModule->close();
			
			// update state-action values
			lLearningModule->update(lSS.str());
			
			if (lPrintConf) {
				// print configuration file
				lSS.str("");
				if (lFolders) {
					lSS << lPrintPrefix_Basic << i << "/configuration.xml";
				} else {
					lSS << lPrintPrefix_Basic << i << "_configuration.xml";
				}
				lOFS.open(lSS.str().c_str());
				lOStreamer = new PACC::XML::Streamer(lOFS);
				lSimulator.write(*lOStreamer);
				lOFS.close();
				delete lOStreamer;
			}
			
			if (lEvaluate) {
#ifdef SCHNAPS_FULL_DEBUG
				std::cout << "Evaluating\n";
#endif
				// evaluate the current performance
				// reset randomizers to configuration values
				lSimulator.resetRandomizer();
				lSimulator.getPopulationManager().getGenerator().resetRandomizer();
				
				// simulate
				lSimulator.configure("print.input=false,print.output=true,print.log=true,learning.learn=false");
				lSimulator.getPopulationManager().clear();
				lSimulator.getPopulationManager().readStr(lInputEvaluation);
				lSimulator.simulate(lScenario);
			}
		}
		
#ifdef SCHNAPS_FULL_DEBUG
		std::cout << "Learning done\n";
#endif
		
		if (!lPrintConf) {
			// print final configuration file only
			lSS.str(lPrintPrefix_Basic);
			lSS << "configuration.xml";
			lOFS.open(lSS.str().c_str());
			lOStreamer = new PACC::XML::Streamer(lOFS);
			lSimulator.write(*lOStreamer);
			lOFS.close();
			delete lOStreamer;
		}
		
		if (!lEvaluate) {
#ifdef SCHNAPS_FULL_DEBUG
				std::cout << "Evaluating\n";
#endif
			// evaluate the final performance only
			// reset randomizers to configuration values
			lSimulator.resetRandomizer();
			lSimulator.getPopulationManager().getGenerator().resetRandomizer();
			
			// simulate
			lSS.str("");
			lSS << "print.prefix=" << lPrintPrefix_Basic << ",print.input=false,print.output=true,print.log=true,learning.learn=false";
			lSimulator.configure(lSS.str().c_str());
			lSimulator.getPopulationManager().clear();
			lSimulator.getPopulationManager().readStr(lInputEvaluation);
			lSimulator.simulate(lScenario);
		}
		
		fclose(stdout);
		fclose(stderr);

		return 0;
	} catch (Core::Exception& E) {
		E.terminate();
		
		fclose(stdout);
		fclose(stderr);
		
		return -1;
	}
}
