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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CSATURATIONINDIVIDUALNODESUCCESSOREXTENSIONDATA_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CSATURATIONINDIVIDUALNODESUCCESSOREXTENSIONDATA_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CProcessContext.h"
#include "CRoleSaturationProcessLinker.h"
#include "CSaturationIndividualNodeExtensionResolveData.h"
#include "CSaturationIndividualNodeALLConceptsExtensionData.h"
#include "CSaturationIndividualNodeFUNCTIONALConceptsExtensionData.h"

// Other includes
#include "Reasoner/Ontology/CConcept.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;

		namespace Kernel {

			namespace Process {

				/*! 
				 *
				 *		\class		CSaturationIndividualNodeSuccessorExtensionData
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CSaturationIndividualNodeSuccessorExtensionData {
					// public methods
					public:
						//! Constructor
						CSaturationIndividualNodeSuccessorExtensionData(CProcessContext* processContext);

						CSaturationIndividualNodeSuccessorExtensionData* initExtensionData(CIndividualSaturationProcessNode* indiProcessNode);

						CSaturationIndividualNodeExtensionResolveData* getAncestorSuccessorMergeResolveData(bool create = true);
						CSaturationIndividualNodeExtensionResolveData* getBaseExtensionResolveData(bool create = true);
						CSaturationIndividualNodeExtensionResolveData* getExtensionResolveData();
						CSaturationIndividualNodeSuccessorExtensionData* setExtensionResolveData(CSaturationIndividualNodeExtensionResolveData* resolveData);

						bool isExtensionProcessingQueued();
						CSaturationIndividualNodeSuccessorExtensionData* setExtensionProcessingQueued(bool queued);

						CSaturationIndividualNodeALLConceptsExtensionData* getALLConceptsExtensionData(bool create = true);
						CSaturationIndividualNodeFUNCTIONALConceptsExtensionData* getFUNCTIONALConceptsExtensionData(bool create = true);

					// protected methods
					protected:

					// protected variables
					protected:
						CProcessContext* mProcessContext;
						bool mExtensionProcessingQueued;
						CIndividualSaturationProcessNode* mIndiProcessNode;
						CSaturationIndividualNodeExtensionResolveData* mExtensionResolveData;
						CSaturationIndividualNodeExtensionResolveData* mAncSuccMergeResolveData;
						CSaturationIndividualNodeALLConceptsExtensionData* mALLConceptsExtensionData;
						CSaturationIndividualNodeFUNCTIONALConceptsExtensionData* mFUNCTIONALConceptsExtensionData;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CSATURATIONINDIVIDUALNODESUCCESSOREXTENSIONDATA_H
