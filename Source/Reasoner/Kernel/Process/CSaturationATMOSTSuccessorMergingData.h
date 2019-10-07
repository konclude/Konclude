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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CSATURATIONATMOSTSUCCESSORMERGINGDATA_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CSATURATIONATMOSTSUCCESSORMERGINGDATA_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CProcessContext.h"
#include "CConceptSaturationDescriptor.h"
#include "CIndividualSaturationProcessNodeLinker.h"
#include "CSaturationATMOSTSuccessorMergingHash.h"
#include "CConceptSaturationProcessLinker.h"
#include "CSaturationATMOSTSuccessorMergingData.h"
#include "CLinkedRoleSaturationSuccessorHash.h"


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
				 *		\class		CSaturationATMOSTSuccessorMergingData
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CSaturationATMOSTSuccessorMergingData {
					// public methods
					public:
						//! Constructor
						CSaturationATMOSTSuccessorMergingData(CProcessContext* processContext);

						CSaturationATMOSTSuccessorMergingData* initSuccessorMergingData(CIndividualSaturationProcessNode* indiProcessNode);

						CIndividualSaturationProcessNodeLinker* getMergingIndividualProcessLinker();
						bool isMergingProcessingQueued();

						CConceptSaturationProcessLinker* getMergingConceptLinker();
						CSaturationATMOSTSuccessorMergingData* setMergingConceptLinker(CConceptSaturationProcessLinker* conceptLinker);
						CSaturationATMOSTSuccessorMergingData* addMergingProcessingConcept(CConceptSaturationDescriptor* conSatDes);
						CConceptSaturationProcessLinker* takeNextMergingConceptLinker();

						CSaturationATMOSTSuccessorMergingHash* getATMOSTConceptMergingDataHash(bool create = true);

						CLinkedRoleSaturationSuccessorHash* getMergedLinkedRoleSaturationSuccessorHash(bool create = true);


						CSaturationATMOSTSuccessorMergingHashData& getATMOSTConceptMergingData(CConceptSaturationDescriptor* conSatDes);



						CPROCESSHASH<CSaturationSuccessorData*,cint64>* getRemainingMergeableCardinalityHash(bool create = true);

						CPROCESSHASH<CSaturationSuccessorData*,CSaturationSuccessorData*>* getMergingDistintHash(bool create = true);
						CPROCESSSET< QPair<CSaturationSuccessorData*,CSaturationSuccessorData*> >* getMergingDistintSet(bool create = true);


					// protected methods
					protected:

					// protected variables
					protected:
						CProcessContext* mProcessContext;
						CLinkedRoleSaturationSuccessorHash* mNewSuccessorHash;
						CIndividualSaturationProcessNodeLinker mMergingIndiProcessLinker;
						CConceptSaturationProcessLinker* mMergingConceptLinker;

						CSaturationATMOSTSuccessorMergingHash* mConceptMergingDataHash;


						CPROCESSHASH<CSaturationSuccessorData*,cint64>* mRemainMergeableCardHash;

						CPROCESSHASH<CSaturationSuccessorData*,CSaturationSuccessorData*>* mMergeDistintHash;
						CPROCESSSET< QPair<CSaturationSuccessorData*,CSaturationSuccessorData*> >* mMergeDistintSet;




					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CSATURATIONATMOSTSUCCESSORMERGINGDATA_H
