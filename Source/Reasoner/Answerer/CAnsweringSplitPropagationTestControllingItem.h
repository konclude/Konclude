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

#ifndef KONCLUDE_REASONER_ANSWERER_CANSWERINGSPLITPROPAGATIONTESTCONTROLLINGITEM_H
#define KONCLUDE_REASONER_ANSWERER_CANSWERINGSPLITPROPAGATIONTESTCONTROLLINGITEM_H

// Libraries includes


// Namespace includes
#include "AnswererSettings.h"
#include "CAnsweringSplitPropagationTestingVariableData.h"
#include "CAnsweringPropagationExtendedSteeringController.h"
#include "COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData.h"


// Other includes
#include "Utilities/UtilitiesSettings.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities;

	namespace Reasoner {

		namespace Answerer {


			/*! 
			 *
			 *		\class		CAnsweringSplitPropagationTestControllingItem
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CAnsweringSplitPropagationTestControllingItem : public CAnsweringPropagationExtendedSteeringController {
				// public methods
				public:
					//! Constructor
					CAnsweringSplitPropagationTestControllingItem(cint64 testingItemId, COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* absorptionData);

					~CAnsweringSplitPropagationTestControllingItem();

					cint64 getTestingItemId();

					CAnsweringSplitPropagationTestControllingItem* setIntegrationLimit(cint64 limit);
					CAnsweringSplitPropagationTestControllingItem* addVariableData(CAnsweringSplitPropagationTestingVariableData* varData, bool splittingBase);


					QList<CAnsweringSplitPropagationTestingVariableData*> getIntegrationOrderedVariableDataList();
					CAnsweringSplitPropagationTestingVariableData* getBaseVariableData();

					cint64 getExpectedIntegrationCount();
					cint64 getTestActualIntegrationCount();
					cint64 getIntendedIntegrationLimit();

					bool reconfigureVariableIntegrationLimits(cint64 intededIntegrationLimit);
					bool disableCheckedVariableIntegrations();

					bool hasCheckedAllVariableIntegrations();


					virtual bool tryExtendVariableIntegrationLimits(CVariable** integrationExtendedVariable = nullptr);
                    virtual bool canIntegrateVariableBinding(CVariable* var, const CIndividualReference& indiRef);
					virtual CAnsweringPropagationExtendedSteeringController* incVariableBindingsRemainingIntegrationRequirementCount(CVariable* var, cint64 count);


					virtual bool finalizeWithClashing();
					virtual bool finalizeWithBindingExtraction();

					virtual bool isPreparationBindingNominalIndividual(CVariable* variable, CIndividual* indi);
					virtual bool isPreparationBindingNominalIndividual(CVariable* variable, cint64 indiId);
					virtual bool isPreparationBindingAllIndividuals(CVariable* variable);


					CAnsweringSplitPropagationTestingVariableData* getVariableData(CVariable* var);

					cint64 getNextTestingId();
					cint64 getNextVariableDataId();

					CAnsweringSplitPropagationTestControllingItem* reset();

				// protected methods
				protected:
					CAnsweringSplitPropagationTestControllingItem* updateIntegrationOrderedVariableDataList();

				// protected variables
				protected:
					cint64 mTestingItemId;
					COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* mAbsorptionData;

					QHash<CVariable*, CAnsweringSplitPropagationTestingVariableData*> mVariableDataHash;
					QHash<CExpressionVariable*, CAnsweringSplitPropagationTestingVariableData*> mVariableExpressionDataHash;
					CAnsweringSplitPropagationTestingVariableData* mBaseSplittingVarData;

					QList<CAnsweringSplitPropagationTestingVariableData*> mVariableDataList;

					cint64 mIntegrationOrderedVariableDataCount;
					QList<CAnsweringSplitPropagationTestingVariableData*> mIntegrationOrderedVariableDataList;
					cint64 mNextIntegrationId;
					cint64 mNextTestingId;

					CAnsweringSplitPropagationTestingVariableData* mLastIntegrationTestedVarData;
					CVariable* mLastIntegrationTestedVar;

					cint64 mExpectedIntegrationCount;
					cint64 mTestActualIntegrationCount;
					cint64 mIntendedIntegrationLimit;

					bool mAllIntegrationTested;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_CANSWERINGSPLITPROPAGATIONTESTCONTROLLINGITEM_H
