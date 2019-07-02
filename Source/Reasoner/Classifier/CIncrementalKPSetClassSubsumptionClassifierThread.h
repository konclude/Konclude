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

#ifndef KONCLUDE_REASONER_CLASSIFIER_CINCREMENTALKPSETCLASSSUBSUMPTIONCLASSIFIERTHREAD_H
#define KONCLUDE_REASONER_CLASSIFIER_CINCREMENTALKPSETCLASSSUBSUMPTIONCLASSIFIERTHREAD_H

// Libraries includes


// Namespace includes
#include "ClassifierSettings.h"
#include "COptimizedKPSetClassSubsumptionClassifierThread.h"
#include "CIncrementalKPSetClassOntologyClassificationItem.h"
#include "COptimizedSubClassOntologyClassificationItem.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"
#include "Logger/CLogIdentifier.h"



namespace Konclude {

	namespace Reasoner {

		namespace Classifier {



			/*! 
			 *
			 *		\class		CIncrementalKPSetClassSubsumptionClassifierThread
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CIncrementalKPSetClassSubsumptionClassifierThread : public COptimizedKPSetClassSubsumptionClassifierThread {
				// public methods
				public:
					//! Constructor
					CIncrementalKPSetClassSubsumptionClassifierThread(CReasonerManager *reasonerManager);

					//! Destructor
					virtual ~CIncrementalKPSetClassSubsumptionClassifierThread();


				// protected methods
				protected:
					virtual void readCalculationConfig(CCalculationConfigurationExtension *config);
					virtual CSubsumptionClassifierThread* scheduleOntologyClassification(CConcreteOntology *ontology, CClassificationCalculationSupport *classificationSupport, CConfigurationBase *config);
					virtual COptimizedKPSetClassOntologyClassificationItem* createOntologyClassificationItem(CConcreteOntology *ontology, CConfigurationBase *config);

					virtual bool addClassificationStatistics(COntologyClassClassificationItem *ontClassItem, CClassConceptClassification* classConClassification);

					bool tryReusePreviousClassesCalculations(CIncrementalKPSetClassOntologyClassificationItem *ontClassItem, CClassConceptClassification* prevClassConClassification);

				// protected variables
				protected:

				
				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CLASSIFIER_CINCREMENTALKPSETCLASSSUBSUMPTIONCLASSIFIERTHREAD_H
