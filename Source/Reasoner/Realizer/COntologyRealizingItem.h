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

#ifndef KONCLUDE_REASONER_REALIZER_CONTOLOGYREALIZINGITEM_H
#define KONCLUDE_REASONER_REALIZER_CONTOLOGYREALIZINGITEM_H

// Libraries includes


// Namespace includes
#include "RealizerSettings.h"

// Other includes
#include "Reasoner/Ontology/CConcreteOntology.h"
#include "Reasoner/Ontology/COntologyProcessingRequirement.h"


#include "Concurrent/Callback/CCallbackRememberLinkedExecuter.h"
#include "Concurrent/Callback/CCallbackData.h"

#include "Config/CConfigurationBase.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Concurrent::Callback;
	using namespace Config;

	namespace Reasoner {

		using namespace Ontology;

		namespace Realizer {


			/*! 
			 *
			 *		\class		COntologyRealizingItem
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COntologyRealizingItem {
				// public methods
				public:
					//! Constructor
					COntologyRealizingItem();

					COntologyRealizingItem* initRealizingItem(CConcreteOntology* ontology, CConfigurationBase* config);

					virtual COntologyRealizingItem* addProcessingRequirement(COntologyProcessingRequirement* ontoRequirement);

					bool isRealizingFinished();
					COntologyRealizingItem* setRealizingFinished(bool finished);

					COntologyRealizingItem* doRealizingFinishedCallback(bool doCallbacks = true);

					COntologyRealizingItem* addRealizingFinishedCallback(CCallbackData* callback);

					CConfigurationBase* getConfiguration();
					CConcreteOntology* getOntology();

					QSet<CRealizingTestingItem*>* getRealizingTestingItemSet();
					cint64 getCurrentRealizingTestingCount();
					bool hasCurrentRealizingTestingCount();



				// protected methods
				protected:

				// protected variables
				protected:
					CConcreteOntology* mOntology;
					CConfigurationBase* mConfig;

					bool mRealizingFinishedFlag;
					CCallbackRememberLinkedExecuter mCallbackExecuter;
					QSet<CRealizingTestingItem*> mTestItemSet;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_REALIZER_CONTOLOGYREALIZINGITEM_H
