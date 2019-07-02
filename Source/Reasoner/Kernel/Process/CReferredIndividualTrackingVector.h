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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CREFERREDINDIVIDUALTRACKINGVECTOR_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CREFERREDINDIVIDUALTRACKINGVECTOR_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CReferredIndividualTrackingData.h"

// Other includes
#include "Reasoner/Ontology/CIndividualDependenceTracking.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;

		namespace Kernel {

			namespace Process {

				/*! 
				 *
				 *		\class		CReferredIndividualTrackingVector
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CReferredIndividualTrackingVector : public CIndividualDependenceTracking {
					// public methods
					public:
						//! Constructor
						CReferredIndividualTrackingVector();
						virtual ~CReferredIndividualTrackingVector();

						CReferredIndividualTrackingVector* initReferredIndividualTrackingVector(cint64 indiCount, cint64 indiOffset);
						CReferredIndividualTrackingVector* initReferredIndividualTrackingVector(CReferredIndividualTrackingVector* indiTrackVec);

						CReferredIndividualTrackingData* getReferredIndividualTrackingData(cint64 indiID);

						virtual bool mergeGatheredTrackedIndividualDependences(CIndividualDependenceTracking* indiDepTracking);

						CReferredIndividualTrackingVector* setIndividualReferred(cint64 indiID);
						CReferredIndividualTrackingVector* setIndividualReferredAndExtended(cint64 indiID);


						virtual bool areIndividualsAffected(QSet<cint64>* indirectlyChangedIndividualSet, QSet<cint64>* changedCompatibleSet);
						virtual bool areIndividualsAffected(CConsistenceData* consData);

						virtual CIndividualDependenceTracking* getCopiedIndividualDependencyTracking();
						
						virtual cint64 getDependenceSize();

					// protected methods
					protected:

					// protected variables
					protected:
						CReferredIndividualTrackingData* mIndiTrackVector;
						cint64 mIndiTrackCount;
						cint64 mIndiTrackOffset;

					// private methods
					private:

					// private variables
					private:


				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CREFERREDINDIVIDUALTRACKINGVECTOR_H
