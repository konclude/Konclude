/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify it under
 *		the terms of version 2.1 of the GNU Lesser General Public License (LGPL2.1)
 *		as published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details, see GNU Lesser General Public License.
 *
 */

#ifndef KONCLUDE_REASONER_KERNEL_TASK_CSATISFIABLETASKREALIZATIONMARKEDCANDIDATESMESSAGEADAPTER_H
#define KONCLUDE_REASONER_KERNEL_TASK_CSATISFIABLETASKREALIZATIONMARKEDCANDIDATESMESSAGEADAPTER_H

// Libraries includes


// Namespace includes


// Other includes
#include "Reasoner/Ontology/CConcreteOntology.h"
#include "Reasoner/Ontology/CConceptReferenceLinking.h"

#include "Reasoner/Realizer/CRealizationMessageObserver.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;
		using namespace Realizer;

		namespace Kernel {

			namespace Task {

				/*! 
				 *
				 *		\class		CSatisfiableTaskRealizationMarkedCandidatesMessageAdapter
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CSatisfiableTaskRealizationMarkedCandidatesMessageAdapter {
					// public methods
					public:
						//! Constructor
						CSatisfiableTaskRealizationMarkedCandidatesMessageAdapter(CIndividual* testingIndividual = nullptr, CConcreteOntology* testingOntology = nullptr, CRealizationMessageObserver* observer = nullptr);

						CIndividual* getTestingIndividual();
						CConcreteOntology* getTestingOntology();

						CRealizationMessageObserver* getRealizationMessageDataObserver();


					// protected methods
					protected:

					// protected variables
					protected:
						CIndividual* mTestingCon;
						CConcreteOntology* mOntology;
						CRealizationMessageObserver* mMessageObserver;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Task

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_TASK_CSATISFIABLETASKREALIZATIONMARKEDCANDIDATESMESSAGEADAPTER_H
