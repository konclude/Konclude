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

#ifndef KONCLUDE_REASONER_PREPROCESS_CONTOLOGYPREPROCESSINGITEM_H
#define KONCLUDE_REASONER_PREPROCESS_CONTOLOGYPREPROCESSINGITEM_H

// Libraries includes


// Namespace includes
#include "PreprocessSettings.h"

// Other includes
#include "Reasoner/Ontology/CConcreteOntology.h"
#include "Reasoner/Ontology/COntologyProcessingRequirement.h"


#include "Concurrent/Callback/CCallbackListExecuter.h"
#include "Concurrent/Callback/CCallbackData.h"

#include "Config/CConfigurationBase.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Concurrent::Callback;
	using namespace Config;

	namespace Reasoner {

		using namespace Ontology;

		namespace Preprocess {


			/*! 
			 *
			 *		\class		COntologyPreprocessingItem
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COntologyPreprocessingItem {
				// public methods
				public:
					//! Constructor
					COntologyPreprocessingItem();

					COntologyPreprocessingItem* initPreprocessingItem(CConcreteOntology* ontology, CConfigurationBase* config);

					virtual COntologyPreprocessingItem* addProcessingRequirement(COntologyProcessingRequirement* ontoRequirement);

					bool isPreprocessingFinished();
					COntologyPreprocessingItem* setPreprocessingFinished(bool finished);

					COntologyPreprocessingItem* doPreprocessingFinishedCallback(bool doCallbacks = true);

					COntologyPreprocessingItem* addPreprocessingFinishedCallback(CCallbackData* callback);

					CConfigurationBase* getConfiguration();
					CConcreteOntology* getOntology();

					QSet<CPreprocessingTestingItem*>* getPreprocessingTestingItemSet();
					cint64 getCurrentPreprocessingTestingCount();
					bool hasCurrentPreprocessingTestingCount();



				// protected methods
				protected:

				// protected variables
				protected:
					CConcreteOntology* mOntology;
					CConfigurationBase* mConfig;

					bool mPreprocessingFinishedFlag;
					CCallbackListExecuter mCallbackExecuter;
					QSet<CPreprocessingTestingItem*> mTestItemSet;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_PREPROCESS_CONTOLOGYPREPROCESSINGITEM_H
