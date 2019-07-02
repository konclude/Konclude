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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CSATURATIONSUCCESSORALLCONCEPTEXTENSIONDATA_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CSATURATIONSUCCESSORALLCONCEPTEXTENSIONDATA_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CProcessContext.h"
#include "CSaturationSuccessorConceptExtensionMap.h"
#include "CConceptSaturationDescriptor.h"


// Other includes
#include "Utilities/Container/CLinker.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities;
	using namespace Container;

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				/*! 
				 *
				 *		\class		CSaturationSuccessorALLConceptExtensionData
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CSaturationSuccessorALLConceptExtensionData : public CLinkerBase<bool,CSaturationSuccessorALLConceptExtensionData> {
					// public methods
					public:
						//! Constructor
						CSaturationSuccessorALLConceptExtensionData(CProcessContext* processContext);

						CSaturationSuccessorALLConceptExtensionData* initSuccessorConceptExtensionData(CRole* role, CIndividualSaturationProcessNode* indiProcSatNode);

						bool isExtensionProcessingQueued();
						CSaturationSuccessorALLConceptExtensionData* setExtensionProcessingQueued(bool queued);

						CSaturationSuccessorConceptExtensionMap* getSuccessorConceptExtensionMap();
						bool addExtensionConcept(CConcept* concept, bool negation);

						CRole* getRole();
						CIndividualSaturationProcessNode* getIndividualNode();
						CIndividualSaturationProcessNode* getLastResolvedIndividualNode();
						CSaturationSuccessorALLConceptExtensionData* setLastResolvedIndividualNode(CIndividualSaturationProcessNode* indiNode);

						cint64 getRequiredSuccessorCardinality();
						CSaturationSuccessorALLConceptExtensionData* setRequiredSuccessorCardinality(cint64 succCard);
						cint64 getLastConnectedSuccessorCardinality();
						CSaturationSuccessorALLConceptExtensionData* setLastConnectedSuccessorCardinality(cint64 succCard);

						bool addRequiredSuccessorCardinality(cint64 succCard);

						bool hasSuccessorCardinalityUpdatedFlag();
						bool hasConceptsUpdatedFlag();
						CSaturationSuccessorALLConceptExtensionData* clearUpdatedFlags();

					// protected methods
					protected:

					// protected variables
					protected:
						CProcessContext* mProcessContext;
						CRole* mRole;
						bool mConceptsUpdatedFlag;
						bool mSuccessorCardinalityUpdatedFlag;
						cint64 mRequiredSuccCount;
						cint64 mLastConnectedSuccCount;
						CIndividualSaturationProcessNode* mIndiProcSatNode;
						CIndividualSaturationProcessNode* mLastResolvedIndiProcSatNode;
						CConceptSaturationDescriptor* mLastExaminedConDes;
						CSaturationSuccessorConceptExtensionMap mConceptExtensionMap;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CSATURATIONSUCCESSORALLCONCEPTEXTENSIONDATA_H
