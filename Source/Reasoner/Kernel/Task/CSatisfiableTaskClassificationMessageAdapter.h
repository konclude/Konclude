/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#ifndef KONCLUDE_REASONER_KERNEL_TASK_CSATISFIABLETASKCLASSIFICATIONMESSAGEADAPTER_H
#define KONCLUDE_REASONER_KERNEL_TASK_CSATISFIABLETASKCLASSIFICATIONMESSAGEADAPTER_H

// Libraries includes


// Namespace includes


// Other includes
#include "Reasoner/Ontology/CConcreteOntology.h"
#include "Reasoner/Ontology/CConceptReferenceLinking.h"

#include "Reasoner/Classifier/CClassificationMessageDataObserver.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;
		using namespace Classifier;

		namespace Kernel {

			namespace Task {

				/*! 
				 *
				 *		\class		CSatisfiableTaskClassificationMessageAdapter
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CSatisfiableTaskClassificationMessageAdapter {
					// public methods
					public:
						//! Constructor
						CSatisfiableTaskClassificationMessageAdapter(CConcept* testingConcept = nullptr, CConcreteOntology* testingOntology = nullptr, CClassificationMessageDataObserver* observer = nullptr, QHash<CConcept*,CConceptReferenceLinking*>* conRefLinkDataHash = nullptr, cint64 extractionFlags = 0);

						CConcept* getTestingConcept();
						CConcreteOntology* getTestingOntology();

						CClassificationMessageDataObserver* getClassificationMessageDataObserver();

						QHash<CConcept*,CConceptReferenceLinking*>* getConceptReferenceLinkingDataHash();

						cint64 getExtractionFlags();
						CSatisfiableTaskClassificationMessageAdapter* setExtractionFlags(cint64 flags);

						bool hasExtractionFlags(cint64 flags);

						const static cint64 EFEXTRACTSUBSUMERSROOTNODE				= 1;
						const static cint64 EFEXTRACTSUBSUMERSOTHERNODES			= 2;
						const static cint64 EFEXTRACTPOSSIBLESUBSUMERSROOTNODE		= 4;
						const static cint64 EFEXTRACTPOSSIBLESUBSUMERSOTHERNODES	= 8;
						const static cint64 EFEXTRACTIDENTIFIERPSEUDOMODEL			= 16;
						const static cint64 EFEXTRACTOTHERNODESSINGLEDEPENDENCY		= 32;
						const static cint64 EFEXTRACTOTHERNODESMULTIPLEDEPENDENCY	= 64;

						const static cint64 EFEXTRACTALL							= EFEXTRACTSUBSUMERSROOTNODE | EFEXTRACTSUBSUMERSOTHERNODES | EFEXTRACTPOSSIBLESUBSUMERSROOTNODE | EFEXTRACTPOSSIBLESUBSUMERSOTHERNODES | EFEXTRACTIDENTIFIERPSEUDOMODEL | EFEXTRACTOTHERNODESMULTIPLEDEPENDENCY;


					// protected methods
					protected:

					// protected variables
					protected:
						CConcept* mTestingCon;
						CConcreteOntology* mOntology;
						CClassificationMessageDataObserver* mMessageObserver;
						cint64 mExtractionFlags;
						QHash<CConcept*,CConceptReferenceLinking*>* mConRefLinkDataHash;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Task

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_TASK_CSATISFIABLETASKCLASSIFICATIONMESSAGEADAPTER_H
