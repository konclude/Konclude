/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public
 *		License (LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU (Lesser) General Public License for more details.
 *
 *		You should have received a copy of the GNU (Lesser) General Public
 *		License along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "CCommandLineTranlatorHelpLoader.h"


namespace Konclude {

	namespace Control {

		namespace Loader {


			CCommandLineTranlatorHelpLoader::CCommandLineTranlatorHelpLoader() {
			}



			CCommandLineTranlatorHelpLoader::~CCommandLineTranlatorHelpLoader() {
			}


			CLoader *CCommandLineTranlatorHelpLoader::init(CLoaderFactory *loaderFactory, CConfiguration *config) {

				return this;
			}




			CLoader *CCommandLineTranlatorHelpLoader::load() {
				
				CLogger::getInstance()->waitSynchronization();
				LOG(INFO, "::Konclude::Loader::OWLlinkTranslationHelpLoader", logTr(
						"Usage: 'Konclude' <command> [parameters]\n"
						"\n"
						"Available commands:\n"
						"\towllinkfile : Processes an OWLlink request file, which has to be specified with the parameter -i.\n"
						"\towllinkserver : Listening on the specified port (default 8080) for OWLlink requests sent over HTTP.\n"
						"\tclassification : Classifies the ontology that is specified with the parameter -i.\n"
						"\tconsistency : Checks the consistency of the ontology that is specified with the parameter -i.\n"
						"\tsatisfiability : Checks the satisfiability of the class that is specified with -x for the ontology specified with -i.\n"
						"\trealization : Realizes the ontology that is specified with -i (or only a certain individual that is specified with -x).\n"
						"\tsparqlfile : Processes a SPARQL request file, which has to be specified with the parameter -s, results are written to the file specified by -o.\n"
						"\t\t\t(an ontology can be given with -i or via a SPARQL Update LOAD command in the SPARQL file)\n"
						"\tsparqlserver : Listening on the specified port (default 8080) for SPARQL requests sent via HTTP POST.\n"
						"\n"
						"Configurable parameter:\n"
						"\t-w X : Configures the number of processing threads. X has to be an integer greater than 0 or 'AUTO' for\n"
						"\t\t\tscaling the number of processing threads to the number of computer cores.\n"
						"\t-c FILEPATH : Path and file name of the config file.\n"
						"\t-i FILEPATH : Path and file name of the ontology input file / OWLlink request file.\n"
						"\t-o FILEPATH : Path and file name of the output / (SPARQL) result / (OWLlink) response file.\n"
						"\t-s FILEPATH : Path and file name of the SPARQL request/query file.\n"
						"\t-x IRI : IRI of the input entity.\n"
						"\t-p PORT: Specifies PORT as the listening port for the OWLlink/SPARQL server.\n"
						"\t-a : Periodically prints the progress of the current activities.\n"
						"\t-v : Shows loading and processing times in more detail.\n"
						"\t-h : Shows this help (overwrites all other arguments/parameters).\n"
						"\n"
						"Examples:\n"
						"\tKonclude owllinkfile -i Tests/test-request.xml -o test-response.xml\n"
						"\tKonclude owllinkfile -c Configs/default-config.xml -i Tests/test-request.xml -o test-response.xml\n"
						"\tKonclude owllinkserver -w 2 -c Configs/default-config.xml -p 8080\n"
						"\tKonclude satisfiability -i Tests/galen.owl.xml -x http://ex.test/galen#ExternalIliacVein\n"
						"\tKonclude classification -w AUTO -i Tests/roberts-family-full-D.owl.xml -o Tests/roberts-family-full-D-class.owl.xml\n"
						"\tKonclude realization -w AUTO -i Tests/roberts-family-full-D.owl.xml -o Tests/roberts-family-full-D-real.owl.xml\n"
						"\tKonclude sparqlfile -s Tests/sparql-load-and-query-test.sparql -o Tests/query-answers.xml\n"
						"\tKonclude sparqlfile -s Tests/sparql-existential-variables-query-test.sparql -i Tests/roberts-family-full-D.owl.xml\n"
						"\tKonclude sparqlserver -p 8080 -c Configs/querying-config.xml\n"
						"\n"
						),this);

				CLogger::getInstance()->waitSynchronization();
				QCoreApplication::exit();

				return this;
			}

			CLoader *CCommandLineTranlatorHelpLoader::exit() {
				return this;
			}



		}; // end namespace Loader

	}; // end namespace Control

}; // end namespace Konclude
