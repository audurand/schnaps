/*
 * main.cpp
 *
 *  Created on: 2009-11-12
 *  Author: Audrey Durand
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
	int lOpt;

	// Configuration
	std::string lDirectory = "";
	std::string lConfigurationFile = "";
	std::string lScenario = "";
	char *lParameters = '\0';

	printf("Start\n");

	printf("Parsing arguments\n");

	while ((lOpt = getopt(argc, argv, "d:c:s:p:")) != -1) {
		switch (lOpt) {
		case 'd':
			lDirectory = optarg;
			break;
		case 'c':
			lConfigurationFile = optarg;
			break;
		case 's':
			lScenario = optarg;
			break;
		case 'p':
			lParameters = optarg;
			break;
		case '?':
			fprintf(stderr, "Missing argument of option -%c.\n", optopt);
			break;
		default:
			fprintf(stderr, "Unknown option -%c.\n", optopt);
			break;
		}
	}

	try {
		schnaps_AssertM(lConfigurationFile.empty() == false);
		schnaps_AssertM(lScenario.empty() == false);

		Simulation::Simulator lSimulator;

		if (lDirectory.empty() == false) {
			int lChdir = chdir(lDirectory.c_str());
			schnaps_AssertM(lChdir == 0);
		}

		PACC::XML::Document *lDocument = new PACC::XML::Document();
		lDocument->parse(lConfigurationFile);
		lSimulator.read(lDocument->getFirstDataTag());
		delete lDocument;

		// Command-line parameters override configuration file.
		lSimulator.configure(lParameters);

		printf("Simulating\n");
		// Simulate
		lSimulator.simulate(lScenario);
	} catch (Core::AssertException e) {
		e.explain(std::cerr);
	}

	printf("End\n");

	return 0;
}
