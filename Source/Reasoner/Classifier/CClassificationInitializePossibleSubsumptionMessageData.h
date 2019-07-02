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

					CClassificationInitializePossibleSubsumptionMessageData* initClassificationPossibleSubsumptionMessageData(CConcept* subsumedConcept, CCLASSPOSSIBLESUBSUMPTIONMESSAGELIST<CClassificationInitializePossibleSubsumptionData*>* possSubsumerList, bool eqConceptsNonCandidatePossSubsumers = false, CCLASSPOSSIBLESUBSUMPTIONMESSAGELIST<CConcept*>* eqConNonCandpossSubsumerList = nullptr);

					CCLASSPOSSIBLESUBSUMPTIONMESSAGELIST<CClassificationInitializePossibleSubsumptionData*>* getClassPossibleSubsumerList();
					CCLASSPOSSIBLESUBSUMPTIONMESSAGELIST<CConcept*>* getClassEqConceptNonCandidatePossibleSubsumerList();
					CConcept* getSubsumedConcept();

					bool hasEqConceptsNonCandidatePossSubsumers();

				// protected methods
				protected:

				// protected variables
				protected:
					CConcept* mSubsumedConcept;
					bool mEqConceptsNonCandidatePossSubsumers;
					// list has to be sorted by concept tag
					CCLASSPOSSIBLESUBSUMPTIONMESSAGELIST<CClassificationInitializePossibleSubsumptionData*>* mPossSubsumerList;
					CCLASSPOSSIBLESUBSUMPTIONMESSAGELIST<CConcept*>* mEqConNonCandPossSubsumerList;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CLASSIFIER_CCLASSIFICATIONINITIALIZEPOSSIBLESUBSUMPTIONMESSAGEDATA_H
