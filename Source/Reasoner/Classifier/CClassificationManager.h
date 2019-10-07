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

#ifndef KONCLUDE_REASONER_CLASSIFIER_CCLASSIFICATIONMANAGER_H
#define KONCLUDE_REASONER_CLASSIFIER_CCLASSIFICATIONMANAGER_H

// Libraries includes
#include <QReadWriteLock>

// Namespace includes
#include "ClassifierSettings.h"
#include "CSubsumptionClassifierFactory.h"
#include "CClassificationProgress.h"
#include "CSubsumptionClassifierThread.h"

// Other includes
#include "Reasoner/Ontology/CConcreteOntology.h"

#include "Reasoner/Taxonomy/CTaxonomy.h"

#include "Config/CConfigurationProvider.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Config;

	namespace Reasoner {

		using namespace Ontology;
		using namespace Taxonomy;

		namespace Classifier {


			/*! 
			 *
			 *		\class		CClassificationManager
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CClassificationManager {
				// public methods
				public:
					//! Constructor
					CClassificationManager();

					//! Destructor
					virtual ~CClassificationManager();

					virtual CClassificationManager *initializeManager(CSubsumptionClassifierFactory *takeClassifierFactory, CConfigurationProvider *configurationProvider);

					virtual CSubsumptionClassifier *getClassClassifier(CConcreteOntology *ontology, CConfigurationBase *config, bool create = true, bool backgroundClassification = false);

					virtual CClassifierStatistics *collectClassificationStatistics(CClassifierStatistics *statistics);
					
					virtual QList<CSubsumptionClassifier *> getClassifierList();

					virtual CClassificationProgress* getClassificationProgress();

					virtual cint64 getActiveClassifierCount();


					virtual CSubsumptionClassifier *getDataPropertyClassifier(CConcreteOntology *ontology, CConfigurationBase *config);
					virtual CSubsumptionClassifier *getObjectPropertyClassifier(CConcreteOntology *ontology, CConfigurationBase *config);


				// protected methods
				protected:

				// protected variables
				protected:
					QReadWriteLock readWriteLock;

					CSubsumptionClassifierFactory* mClassifierFac;

					QHash<CConcreteOntology*,CSubsumptionClassifier*> mOntoClassifierHash;
					QHash<CConcreteOntology*,CSubsumptionClassifier*> mOntoBackgroundClassifierHash;

					QSet<CSubsumptionClassifier*> mOntoClassifierSet;

					CClassificationProgress mClassificationProgress;



					QHash<CConcreteOntology*,CSubsumptionClassifier*> mOntoDataPropertyClassifierHash;
					QHash<CConcreteOntology*,CSubsumptionClassifier*> mOntoObjectPropertyClassifierHash;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CLASSIFIER_CCLASSIFICATIONMANAGER_H
