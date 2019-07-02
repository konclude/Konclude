/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
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

#ifndef KONCLUDE_REASONER_CLASSIFIER_CCONFIGDEPENDEDSUBSUMPTIONCLASSIFIERFACTORY_H
#define KONCLUDE_REASONER_CLASSIFIER_CCONFIGDEPENDEDSUBSUMPTIONCLASSIFIERFACTORY_H

// Libraries includes


// Namespace includes
#include "ClassifierSettings.h"
#include "CSubsumptionClassifier.h"
#include "CSubsumptionClassifierFactory.h"
#include "COptimizedSubClassSubsumptionClassifierThread.h"
#include "COptimizedKPSetClassSubsumptionClassifierThread.h"
#include "COptimizedClassExtractedSaturationSubsumptionClassifierThread.h"
#include "CIncrementalKPSetClassSubsumptionClassifierThread.h"

// Other includes
#include "Config/CConfigurationBase.h"
#include "Config/CConfigDataReader.h"

#include "Reasoner/Ontology/CConcreteOntology.h"

#include "Reasoner/Kernel/Manager/CReasonerManager.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Config;

	namespace Reasoner {

		using namespace Ontology;
		using namespace Kernel;
		using namespace Manager;

		namespace Classifier {


			/*! 
			 *
			 *		\class		CConfigDependedSubsumptionClassifierFactory
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CConfigDependedSubsumptionClassifierFactory : public CSubsumptionClassifierFactory {
				// public methods
				public:
					//! Constructor
					CConfigDependedSubsumptionClassifierFactory(CReasonerManager *reasonerManager);

					//! Destructor
					virtual ~CConfigDependedSubsumptionClassifierFactory();


					virtual CSubsumptionClassifier *createClassifier(CConcreteOntology *ontology, CConfigurationBase *config);
					virtual CSubsumptionClassifier *createBackgroundClassifier(CConcreteOntology *ontology, CConfigurationBase *config);


				// protected methods
				protected:
					bool isClassificationBySatisfiableCalculationSufficient(CConcreteOntology *ontology, CConfigurationBase *config);
					bool isClassificationBySaturationCalculationSufficient(CConcreteOntology *ontology, CConfigurationBase *config);

				// protected variables
				protected:
					CReasonerManager *reasonerMan;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CLASSIFIER_CCONFIGDEPENDEDSUBSUMPTIONCLASSIFIERFACTORY_H
