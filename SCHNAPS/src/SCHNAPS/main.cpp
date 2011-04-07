/*
 * main.cpp
 *
 * SCHNAPS
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

#include "SCHNAPS/Core.hpp"
#include "SCHNAPS/Simulation.hpp"

#ifdef SCHNAPS_IS_UNIX
#include <unistd.h>
#else
#include <direct.h>
#include "XGetopt.h"
#endif

using namespace std;
using namespace SCHNAPS;

int main(int argc, char* argv[]) {
	try{
		int lOpt;

		// configuration
		std::string lDirectory = "";
		std::string lConfigurationFile = "";
		std::string lParameters = "";
		std::string lScenario = "";

		printf("start\n");

		printf("- arguments parsing\n");

		while ((lOpt = getopt(argc, argv, "d:c:s:p:")) != -1) {
			switch (lOpt) {
			case 'd':
				lDirectory.assign(optarg);
				break;
			case 'c':
				lConfigurationFile.assign(optarg);
				break;
			case 'p':
				lParameters.assign(optarg);
				break;
			case 's':
				lScenario.assign(optarg);
				break;
			case '?':
				fprintf(stderr, "Missing argument of option -%c.\n", optopt);
				break;
			default:
				fprintf(stderr, "Unknown option -%c.\n", optopt);
				break;
			}
		}

		printf("- configuration\n");
		
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

		// set current working directory
		if (lDirectory.empty() == false) {
			int lChdir = chdir(lDirectory.c_str());
			schnaps_AssertM(lChdir == 0);
		}

		// configure simulator from file
		PACC::XML::Document *lDocument = new PACC::XML::Document();
		lDocument->parse(lConfigurationFile);
		lSimulator.read(lDocument->getFirstDataTag());
		delete lDocument;

		// command-line parameters override configuration file.
		if (lParameters.empty() == false) {
			lSimulator.configure(lParameters);
		}

		printf("- Simulation\n");
		// simulate
		lSimulator.simulate(lScenario);

		printf("end\n");

		return 0;
	} catch (Core::Exception& E) {
		E.terminate();
		return -1;
	}
}
