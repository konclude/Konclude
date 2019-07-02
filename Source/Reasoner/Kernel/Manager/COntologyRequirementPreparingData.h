/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
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

#ifndef KONCLUDE_REASONER_KERNEL_MANAGER_CONTOLOGYREQUIREMENTPREPARINGDATA_H
#define KONCLUDE_REASONER_KERNEL_MANAGER_CONTOLOGYREQUIREMENTPREPARINGDATA_H

// Libraries includes
#include <QReadWriteLock>
#include <QHash>

// Namespace includes
#include "ManagerSettings.h"
#include "COntologyRequirementPreparingData.h"

// Other includes
#include "Reasoner/Ontology/CConcreteOntology.h"
#include "Reasoner/Ontology/COntologyProcessingRequirement.h"
#include "Reasoner/Ontology/COntologyProcessingStepRequirement.h"
#include "Reasoner/Ontology/COntologyProcessingConceptSaturationRequirement.h"

#include "Reasoner/Query/CQuery.h"

#include "Concurrent/Callback/CCallbackData.h"


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Concurrent::Callback;

	namespace Reasoner {

		using namespace Query;
		using namespace Ontology;

		namespace Kernel {

			namespace Manager {


				/*! 
				 *
				 *		\class		COntologyRequirementPreparingData
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class COntologyRequirementPreparingData {
					// public methods
					public:
						//! Constructor
						COntologyRequirementPreparingData(CConcreteOntology* ontology);

						virtual ~COntologyRequirementPreparingData();

						COntologyRequirementPreparingData* addOntologyRequirement(COntologyProcessingRequirement* requirement);


						bool mRequirementsProcessed;
						CConcreteOntology* mOntology;
						QList<COntologyProcessingRequirement*> mPreprocessorReqList;
						QList<COntologyProcessingRequirement*> mPrecomputorReqList;
						QList<COntologyProcessingRequirement*> mClassifierReqList;
						QList<COntologyProcessingRequirement*> mRealizerReqList;

						QList<COntologyProcessingRequirement*> mCheckingReqList;
						QList<COntologyProcessingRequirement*> mFailedReqList;

						QList<COntologyProcessingRequirement*> mAllReqList;


						COntologyProcessingStep::PROCESSORTYPE mCheckingProcessorType;
						QTime mCheckingTimer;

					// protected methods
					protected:

					// protected variables
					protected:

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Manager

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_MANAGER_CONTOLOGYREQUIREMENTPREPARINGDATA_H
