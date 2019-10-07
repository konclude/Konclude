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

#ifndef KONCLUDE_REASONER_CLASSIFIER_CSUBSUMPTIONCLASSIFIER_H
#define KONCLUDE_REASONER_CLASSIFIER_CSUBSUMPTIONCLASSIFIER_H

// Libraries includes


// Namespace includes
#include "ClassifierSettings.h"
#include "CClassifierStatistics.h"
#include "CClassificationProgress.h"
#include "CClassificationMessageDataObserver.h"

// Other includes
#include "Reasoner/Ontology/CConcept.h"

#include "Reasoner/Taxonomy/CTaxonomy.h"

#include "Concurrent/Callback/CCallbackData.h"

#include "Config/CConfigurationBase.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Concurrent::Callback;
	using namespace Config;

	namespace Reasoner {

		using namespace Ontology;
		using namespace Taxonomy;

		namespace Classifier {


			/*! 
			 *
			 *		\class		CSubsumptionClassifier
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CSubsumptionClassifier : public CClassificationMessageDataObserver {
				// public methods
				public:
					//! Constructor
					CSubsumptionClassifier();

					//! Destructor
					virtual ~CSubsumptionClassifier();

					virtual bool classify(CConcreteOntology *ontology, CConfigurationBase *config, const QList<COntologyProcessingRequirement*>& requirementList, CCallbackData* callback) = 0;
					virtual bool classify(CConcreteOntology *ontology, CConfigurationBase *config, const QList<COntologyProcessingRequirement*>& requirementList) = 0;
					virtual bool callbackClassified(CConcreteOntology *ontology, CCallbackData *callback) = 0;

					virtual CClassifierStatistics *getClassificationStatistics() = 0;
					virtual QString getStatusString() = 0;

					virtual CClassificationProgress* getClassificationProgress() = 0;

				// protected methods
				protected:

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

#endif // KONCLUDE_REASONER_CLASSIFIER_CSUBSUMPTIONCLASSIFIER_H
