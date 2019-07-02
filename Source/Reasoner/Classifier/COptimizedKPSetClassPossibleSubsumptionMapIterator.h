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

#ifndef KONCLUDE_REASONER_CLASSIFIER_COPTIMIZEDKPSETCLASSPOSSIBLESUBSUMPTIONMAPITERATOR_H
#define KONCLUDE_REASONER_CLASSIFIER_COPTIMIZEDKPSETCLASSPOSSIBLESUBSUMPTIONMAPITERATOR_H

// Libraries includes
#include <QMap>

// Namespace includes
#include "ClassifierSettings.h"
#include "CClassPossibleSubsumptionMapIterator.h"
#include "COptimizedKPSetClassPossibleSubsumptionData.h"


// Other includes
#include "Reasoner/Ontology/CConceptTagComparer.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;

		namespace Classifier {


			/*! 
			 *
			 *		\class		COptimizedKPSetClassPossibleSubsumptionMapIterator
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedKPSetClassPossibleSubsumptionMapIterator : public CClassPossibleSubsumptionMapIterator {
				// public methods
				public:
					// Constructor
					COptimizedKPSetClassPossibleSubsumptionMapIterator();
					COptimizedKPSetClassPossibleSubsumptionMapIterator(QMap<CConceptTagComparer,COptimizedKPSetClassPossibleSubsumptionData*>::iterator itBegin, QMap<CConceptTagComparer,COptimizedKPSetClassPossibleSubsumptionData*>::iterator itEnd);

					bool hasNext();

					bool isSubsumptionConfirmed();
					bool isSubsumptionInvalided();
					CConcept* getSubsumptionConcept();

					bool invalidateSubsumption();
					bool confirmSubsumption();

					bool moveNext();

				// protected methods
				protected:

				// protected variables
				protected:
					QMap<CConceptTagComparer,COptimizedKPSetClassPossibleSubsumptionData*>::iterator mItBegin;
					QMap<CConceptTagComparer,COptimizedKPSetClassPossibleSubsumptionData*>::iterator mItEnd;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CLASSIFIER_COPTIMIZEDKPSETCLASSPOSSIBLESUBSUMPTIONMAPITERATOR_H
