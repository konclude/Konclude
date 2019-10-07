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

#include "CConcreteOntologyPreProcess.h"


namespace Konclude {

	namespace Reasoner {

		namespace Preprocess {


			CConcreteOntologyPreProcess::CConcreteOntologyPreProcess() {
			}


			CConcreteOntologyPreProcess::~CConcreteOntologyPreProcess() {
			}

			COntology *CConcreteOntologyPreProcess::preprocess(COntology *ontology) {
				CConcreteOntology* concreteOntology = dynamic_cast<CConcreteOntology*>(ontology);
				if (!concreteOntology) {					
					LOG(INFO,"::Konclude::Reasoner::Preprocess::ConcreteOntologyPreprocess",logTr("Not supported ontology format."),this);
				} else {
					preprocess(concreteOntology);
				}
				return ontology;
			}

		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude
