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

#ifndef KONCLUDE_REASONER_CLASSIFIER_CINCREMENTALKPSETCLASSONTOLOGYCLASSIFICATIONITEM_H
#define KONCLUDE_REASONER_CLASSIFIER_CINCREMENTALKPSETCLASSONTOLOGYCLASSIFICATIONITEM_H

// Libraries includes


// Namespace includes
#include "ClassifierSettings.h"
#include "COptimizedKPSetClassOntologyClassificationItem.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Classifier {


			/*! 
			 *
			 *		\class		CIncrementalKPSetClassOntologyClassificationItem
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CIncrementalKPSetClassOntologyClassificationItem : public COptimizedKPSetClassOntologyClassificationItem {
				// public methods
				public:
					//! Constructor
					CIncrementalKPSetClassOntologyClassificationItem(CConfigurationBase *configuration, CClassifierStatistics *nextClassificationStatistics = 0);

					//! Destructor
					virtual ~CIncrementalKPSetClassOntologyClassificationItem();


					CIncrementalKPSetClassOntologyClassificationItem* setPreviousHierarchyReused(bool reused);
					bool isPreviousHierarchyReused();


					CClassConceptClassification* getPreviousReusedClassClassification();
					CClassConceptClassification* setPreviousReusedClassClassification(CClassConceptClassification* reusedClassClassification);

					cint64 getPreviousReusedClassCalculationCount();
					cint64 getNewInitializedClassCalculationCount();

					CIncrementalKPSetClassOntologyClassificationItem* setPreviousReusedClassCalculationCount(cint64 count);
					CIncrementalKPSetClassOntologyClassificationItem* incPreviousReusedClassCalculationCount(cint64 incCount = 1);

					CIncrementalKPSetClassOntologyClassificationItem* setNewInitializedClassCalculationCount(cint64 count);
					CIncrementalKPSetClassOntologyClassificationItem* incNewInitializedClassCalculationCount(cint64 incCount = 1);

				// protected methods
				protected:

				// protected variables
				protected:
					bool mPreviousHierarchyReused;
					CClassConceptClassification* mReusedClassConClassification;

					cint64 mPreviousReusedClassCalculationCount;
					cint64 mNewInitializedClassCalculationCount;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CLASSIFIER_CINCREMENTALKPSETCLASSONTOLOGYCLASSIFICATIONITEM_H
