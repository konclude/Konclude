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

					virtual CSubsumptionClassifier *getClassifier(CConcreteOntology *ontology, CConfigurationBase *config, bool create = true, bool backgroundClassification = false);

					virtual CClassifierStatistics *collectClassificationStatistics(CClassifierStatistics *statistics);
					
					virtual QList<CSubsumptionClassifier *> getClassifierList();

					virtual CClassificationProgress* getClassificationProgress();

					virtual cint64 getActiveClassifierCount();

				// protected methods
				protected:

				// protected variables
				protected:
					QReadWriteLock readWriteLock;

					CSubsumptionClassifierFactory *classifierFac;

					QHash<CConcreteOntology *, CSubsumptionClassifier *> ontoClassifierHash;
					QHash<CConcreteOntology *, CSubsumptionClassifier *> ontoBackgroundClassifierHash;

					QSet<CSubsumptionClassifier *> ontoClassifierSet;

					CClassificationProgress mClassificationProgress;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CLASSIFIER_CCLASSIFICATIONMANAGER_H
