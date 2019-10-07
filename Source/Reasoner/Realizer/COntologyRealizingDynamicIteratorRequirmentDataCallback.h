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

#ifndef KONCLUDE_REASONER_REALIZER_CONTOLOGYREALIZINGDYNAMICITERATORREQUIRMENTDATACALLBACK_H
#define KONCLUDE_REASONER_REALIZER_CONTOLOGYREALIZINGDYNAMICITERATORREQUIRMENTDATACALLBACK_H

// Libraries includes


// Namespace includes
#include "RealizerSettings.h"
#include "COntologyRealizingDynamicRequirmentProcessingData.h"

// Other includes

#include "Concurrent/Callback/CCallbackData.h"

#include "Config/CConfigurationBase.h"

#include "Reasoner/Ontology/COntologyProcessingIteratorRealizationRequirement.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Concurrent::Callback;

	namespace Reasoner {

		using namespace Ontology;

		namespace Realizer {


			/*! 
			 *
			 *		\class		COntologyRealizingDynamicIteratorRequirmentDataCallback
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COntologyRealizingDynamicIteratorRequirmentDataCallback : public CCallbackData, public COntologyProcessingDynamicRealizationRequirement {
				// public methods
				public:
					//! Constructor
					COntologyRealizingDynamicIteratorRequirmentDataCallback(COntologyRealizingDynamicRequirmentProcessingData* procData, COptimizedRepresentativeKPSetOntologyRealizingItem* realizationItem, COntologyProcessingIteratorRealizationRequirement* iteratorRequirement);


					virtual void doCallback();


					COntologyRealizingDynamicRequirmentProcessingData* getRequirementProcessingData();
					COptimizedRepresentativeKPSetOntologyRealizingItem* getRealizationItem();
					COntologyProcessingIteratorRealizationRequirement* getRealizationIteratorRequirement();


					cint64 getFoundInstancesCount();
					COntologyRealizingDynamicIteratorRequirmentDataCallback* incFoundInstancesCount(cint64 count);


					bool hasRealizedAllInstances();
					COntologyRealizingDynamicIteratorRequirmentDataCallback* setRealizedAllInstances(bool realizedAll);

					virtual COntologyProcessingDynamicRealizationRequirement* submitRequirementUpdate(cint64 processingFlags, cint64 errorFlags);



					cint64 getRoleInitializationCount();
					cint64 getRoleRealizationCount();
					cint64 getConceptRealizationCount();

					COntologyRealizingDynamicIteratorRequirmentDataCallback* incRoleInitializationCount();
					COntologyRealizingDynamicIteratorRequirmentDataCallback* incRoleRealizationCount();
					COntologyRealizingDynamicIteratorRequirmentDataCallback* incConceptRealizationCount();

				// protected methods
				protected:

				// protected variables
				protected:
					COntologyRealizingDynamicRequirmentProcessingData* mProcData;
					COptimizedRepresentativeKPSetOntologyRealizingItem* mRealizationItem;
					COntologyProcessingIteratorRealizationRequirement* mIteratorRequirement;


					cint64 mRoleInitializationCount;
					cint64 mRoleRealizationCount;
					cint64 mConceptRealizationCount;


					cint64 mFoundInstancesCount;
					bool mRealizedAllInstances;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_REALIZER_CONTOLOGYREALIZINGDYNAMICITERATORREQUIRMENTDATACALLBACK_H
