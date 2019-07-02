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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CINDIVIDUALNODEANALIZEDCONCEPTEXPANSIONDATA_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CINDIVIDUALNODEANALIZEDCONCEPTEXPANSIONDATA_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CConceptDescriptor.h"
#include "CProcessContext.h"
#include "CAnalizedConceptExpansionLinker.h"

// Other includes
#include "Utilities/UtilitiesSettings.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Utilities;

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				/*! 
				 *
				 *		\class		CIndividualNodeAnalizedConceptExpansionData
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CIndividualNodeAnalizedConceptExpansionData {
					// public methods
					public:
						//! Constructor
						CIndividualNodeAnalizedConceptExpansionData();

						CIndividualNodeAnalizedConceptExpansionData* initBlockingExplorationData(CIndividualNodeAnalizedConceptExpansionData* prevData);

						CAnalizedConceptExpansionLinker* getReverseAnalizedConceptExpansionLinker();
						CConceptDescriptor* getLastConceptDescriptor();
						cint64 getLastConceptSignature();
						cint64 getLastConceptCount();
						cint64 getExpansionConceptCount();
						cint64 getMinimalValidConceptCountLimit();
						bool isInvalidBlocker();

						CIndividualNodeAnalizedConceptExpansionData* setLastConceptDescriptor(CConceptDescriptor* conDes);
						CIndividualNodeAnalizedConceptExpansionData* setLastConceptSignature(cint64 signature);
						CIndividualNodeAnalizedConceptExpansionData* setLastConceptCount(cint64 conCount);
						CIndividualNodeAnalizedConceptExpansionData* setMinimalValidConceptCountLimit(cint64 conCount);
						CIndividualNodeAnalizedConceptExpansionData* setInvalidBlocker(bool invalid);


						CIndividualNodeAnalizedConceptExpansionData* addAnalizedConceptExpansionLinker(CAnalizedConceptExpansionLinker *linker);


						CXLinker<CConceptDescriptor*>* getAnalysedNonDeterministicConceptExpansionLinker();
						CIndividualNodeAnalizedConceptExpansionData* setAnalysedNonDeterministicConceptExpansionLinker(CXLinker<CConceptDescriptor*>* linker);
						CIndividualNodeAnalizedConceptExpansionData* addAnalysedNonDeterministicConceptExpansionLinker(CXLinker<CConceptDescriptor*>* linker);

					// protected methods
					protected:

					// protected variables
					protected:
						CAnalizedConceptExpansionLinker* mRevAnaConExpLinker;
						CConceptDescriptor* mLastConDes;
						cint64 mLastConceptSignature;
						cint64 mLastConceptCount;
						cint64 mMinValidConceptCountLimit;
						cint64 mExpCount;
						bool mInvalidBlocker;
						CXLinker<CConceptDescriptor*>* mNonDetExpansionLinker;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CINDIVIDUALNODEANALIZEDCONCEPTEXPANSIONDATA_H
