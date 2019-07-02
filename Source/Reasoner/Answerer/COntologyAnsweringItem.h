/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU General Public License
 *		(LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU General Public License for more details.
 *
 *		You should have received a copy of the GNU General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef KONCLUDE_REASONER_ANSWERER_CONTOLOGYANSWERINGITEM_H
#define KONCLUDE_REASONER_ANSWERER_CONTOLOGYANSWERINGITEM_H

// Libraries includes


// Namespace includes
#include "AnswererSettings.h"


// Other includes
#include "Reasoner/Ontology/CConcreteOntology.h"
#include "Reasoner/Ontology/COntologyProcessingStep.h"
#include "Reasoner/Ontology/COntologyProcessingSteps.h"
#include "Reasoner/Ontology/COntologyProcessingStepData.h"

#include "Reasoner/Kernel/Task/CCalculationConfigurationExtension.h"

#include "Concurrent/Callback/CCallbackListExecuter.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Concurrent::Callback;

	namespace Reasoner {

		using namespace Ontology;

		using namespace Kernel::Task;

		namespace Answerer {


			/*! 
			 *
			 *		\class		COntologyAnsweringItem
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COntologyAnsweringItem {
				// public methods
				public:
					//! Constructor
					COntologyAnsweringItem(CConcreteOntology* ontology, CConfigurationBase* configuration);

					//! Destructor
					virtual ~COntologyAnsweringItem();


					virtual CConcreteOntology* getOntology();

					virtual CCalculationConfigurationExtension* getCalculationConfiguration();



				// protected methods
				protected:

				// protected variables
				protected:
					CConcreteOntology* mOntology;
					CCalculationConfigurationExtension* mCalcConfig;



				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_CONTOLOGYANSWERINGITEM_H
