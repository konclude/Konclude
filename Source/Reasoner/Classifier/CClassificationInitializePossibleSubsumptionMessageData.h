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

#ifndef KONCLUDE_REASONER_CLASSIFIER_CCLASSIFICATIONINITIALIZEPOSSIBLESUBSUMPTIONMESSAGEDATA_H
#define KONCLUDE_REASONER_CLASSIFIER_CCLASSIFICATIONINITIALIZEPOSSIBLESUBSUMPTIONMESSAGEDATA_H

// Libraries includes


// Namespace includes
#include "ClassifierSettings.h"
#include "CClassificationMessageData.h"
#include "CClassificationInitializePossibleSubsumptionData.h"

// Other includes
#include "Reasoner/Ontology/CConcept.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;

		namespace Classifier {


			/*! 
			 *
			 *		\class		CClassificationInitializePossibleSubsumptionMessageData
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CClassificationInitializePossibleSubsumptionMessageData : public CClassificationMessageData {
				// public methods
				public:
					//! Constructor
					CClassificationInitializePossibleSubsumptionMessageData();

					CClassificationInitializePossibleSubsumptionMessageData* initClassificationPossibleSubsumptionMessageData(CConcept* subsumedConcept, CCLASSPOSSIBLESUBSUMPTIONMESSAGELIST<CClassificationInitializePossibleSubsumptionData*>* possSubsumerList);

					CCLASSPOSSIBLESUBSUMPTIONMESSAGELIST<CClassificationInitializePossibleSubsumptionData*>* getClassPossibleSubsumerList();
					CConcept* getSubsumedConcept();

				// protected methods
				protected:

				// protected variables
				protected:
					CConcept* mSubsumedConcept;
					// list has to be sorted by concept tag
					CCLASSPOSSIBLESUBSUMPTIONMESSAGELIST<CClassificationInitializePossibleSubsumptionData*>* mPossSubsumerList;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CLASSIFIER_CCLASSIFICATIONINITIALIZEPOSSIBLESUBSUMPTIONMESSAGEDATA_H
