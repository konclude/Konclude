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

#ifndef KONCLUDE_REASONER_KERNEL_ALGORITHM_CDATATYPEDEPENDENCYCOLLECTION_H
#define KONCLUDE_REASONER_KERNEL_ALGORITHM_CDATATYPEDEPENDENCYCOLLECTION_H

// Libraries includes


// Namespace includes
#include "AlgorithmSettings.h"
#include "CCalculationAlgorithmContext.h"
#include "CDependencyFactory.h"

// Other includes
#include "Reasoner/Kernel/Process/CIndividualProcessNode.h"

#include "Reasoner/Kernel/Process/Dependency/CDependencyTrackPoint.h"
#include "Reasoner/Kernel/Process/Dependency/CDATATYPETRIGGERDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CDATATYPECONNECTIONDependencyNode.h"


#include "Context/CContextBase.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Context;

	namespace Reasoner {

		namespace Kernel {

			using namespace Process;
			using namespace Dependency;

			namespace Algorithm {

				/*! 
				 *
				 *		\class		CDatatypeDependencyCollection
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CDatatypeDependencyCollection : public CDatatypeValueSpaceDependencyCollector {
					// public methods
					public:
						//! Constructor
						CDatatypeDependencyCollection(CCalculationAlgorithmContext* collectionCalcAlgContext);
						CDatatypeDependencyCollection(CDatatypeDependencyCollection* depCollection, CCalculationAlgorithmContext* collectionCalcAlgContext);

						bool addDependency(CDependencyTrackPoint* depTrackPoint);


						bool hasDependencies();

						CDependencyTrackPoint* getDatatypeTriggerDependencyTrackPoint(bool create = true);

					// protected methods
					protected:
						CDATATYPETRIGGERDependencyNode* createDATATYPETRIGGERDependency(CDependencyTrackPoint*& expContinueDepTrackPoint, CDependencyTrackPoint* prevDepTrackPoint, CDependency* prevOtherDependencies, CCalculationAlgorithmContext* calcAlgContext);
						CDATATYPECONNECTIONDependencyNode* createDATATYPECONNECTIONDependency(CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext);


					// protected variables
					protected:

						CPROCESSINGSET<CDependencyTrackPoint*>* mTestDepTrackPointSet;
						CPROCESSINGSET<CDependencyTrackPoint*>* mDepTrackPointSet;

						CDependencyTrackPoint* mCollectedFirstDependency;
						CDependency* mCollectedAdditionalDependencies;
						CCalculationAlgorithmContext* mCollectionCalcAlgContext;

						bool mCollectionDatatypeDepCreated;
						CDependencyTrackPoint* mCollectionDataTriggerDepTrackPoint;


					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_ALGORITHM_CDATATYPEDEPENDENCYCOLLECTION_H
