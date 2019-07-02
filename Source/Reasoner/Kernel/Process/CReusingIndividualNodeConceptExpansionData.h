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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CREUSINGINDIVIDUALNODECONCEPTEXPANSIONDATA_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CREUSINGINDIVIDUALNODECONCEPTEXPANSIONDATA_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CConceptDescriptor.h"
#include "CProcessContext.h"
#include "CSignatureBlockingIndividualNodeConceptExpansionData.h"

// Other includes
#include "Utilities/UtilitiesSettings.h"

#include "Reasoner/Kernel/Process/Dependency/CNonDeterministicDependencyTrackPoint.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Utilities;

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				using namespace Dependency;

				/*! 
				 *
				 *		\class		CReusingIndividualNodeConceptExpansionData
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CReusingIndividualNodeConceptExpansionData : public CSignatureBlockingIndividualNodeConceptExpansionData {
					// public methods
					public:
						//! Constructor
						CReusingIndividualNodeConceptExpansionData();

						CReusingIndividualNodeConceptExpansionData* initReusingExpansionData(CReusingIndividualNodeConceptExpansionData* prevData);

						cint64 getReusingTriedCount();
						cint64 getReusingFailedCount();

						CReusingIndividualNodeConceptExpansionData* setReusingTriedCount(cint64 triedCount);
						CReusingIndividualNodeConceptExpansionData* setReusingFailedCount(cint64 failedCount);

						CReusingIndividualNodeConceptExpansionData* incReusingTriedCount(cint64 incCount = 1);
						CReusingIndividualNodeConceptExpansionData* incReusingFailedCount(cint64 incCount = 1);

						CReusingIndividualNodeConceptExpansionData* addReusingFailedSignatureAndIndividual(cint64 conSetSignature, cint64 individualID);

						CNonDeterministicDependencyTrackPoint* getReuseConceptsDependencyTrackPoint();
						CReusingIndividualNodeConceptExpansionData* setReuseConceptsDependencyTrackPoint(CNonDeterministicDependencyTrackPoint* depTrackPoint);
						
						CXLinker<CConceptDescriptor*>* getLastNonDeterministicExpansionLinker();
						CReusingIndividualNodeConceptExpansionData* setLastNonDeterministicExpansionLinker(CXLinker<CConceptDescriptor*>* expLinker);


					// protected methods
					protected:

					// protected variables
					protected:

						cint64 mReusingTriedCount;
						cint64 mReusingFailedCount;
						CPROCESSSET<cint64> mReusedIndiviudals;
						CPROCESSSET<cint64> mReusedConceptSetSignatures;

						CNonDeterministicDependencyTrackPoint* mReuseConceptsDependencyTrackPoint;
						CXLinker<CConceptDescriptor*>* mLastNonDetExpansionLinker;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CREUSINGINDIVIDUALNODECONCEPTEXPANSIONDATA_H
