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

#include "CConfigurationAnsweringHandlerFactory.h"

namespace Konclude {

	namespace Reasoner {

		namespace Answerer {

			
			CConfigurationAnsweringHandlerFactory::CConfigurationAnsweringHandlerFactory(CConfiguration* config) {
				mConfig = config;
			}


			CConfigurationAnsweringHandlerFactory::~CConfigurationAnsweringHandlerFactory() {
			}
			


			CAnsweringHandler* CConfigurationAnsweringHandlerFactory::createAnsweringHandler(CConcreteOntology* ontology, bool composedQuery) {
				CAnsweringHandler* answeringHandler = nullptr;
				if (!composedQuery) {
					QString answererString = CConfigDataReader::readConfigString(mConfig, "Konclude.Answering.DefaultComplexConceptAnswerer", "Konclude.Answering.OptimizedComplexConceptAnswerer");
					if (answererString == "Konclude.Answering.OptimizedComplexConceptAnswerer") {
						CConfigurationBase* config = ontology->getConfiguration();
						COptimizedComplexExpressionOntologyAnsweringItem* ontoAnsweringItem = new COptimizedComplexExpressionOntologyAnsweringItem(ontology, config);
						answeringHandler = new COptimizedComplexExpressionAnsweringHandler(ontoAnsweringItem);
					}
				} else {
					CConfigurationBase* config = ontology->getConfiguration();
					COptimizedComplexCompositionOntologyAnsweringItem* ontoAnsweringItem = new COptimizedComplexCompositionOntologyAnsweringItem(ontology, config);
#ifdef KONCLUDE_REDLAND_INTEGRATION
					answeringHandler = new COptimizedComplexCompositionRedlandRasqalAnsweringHandler(ontoAnsweringItem);
#else
					answeringHandler = new COptimizedComplexCompositionAnsweringHandler(ontoAnsweringItem);
#endif // KONCLUDE_REDLAND_INTEGRATION
				}
				return answeringHandler;
			}



		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
