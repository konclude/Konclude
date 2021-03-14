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

#ifndef KONCLUDE_REASONER_ANSWERER_CANSWERINGSPLITPROPAGATIONTESTINGVARIABLEDATA_H
#define KONCLUDE_REASONER_ANSWERER_CANSWERINGSPLITPROPAGATIONTESTINGVARIABLEDATA_H

// Libraries includes


// Namespace includes
#include "AnswererSettings.h"
#include "CAnsweringSplitPropagationTestingVariableBindingData.h"


// Other includes
#include "Utilities/UtilitiesSettings.h"

#include "Reasoner/Ontology/CVariable.h"
#include "Reasoner/Ontology/CIndividualReference.h"

#include "Parser/Expressions/CExpressionVariable.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities;
	using namespace Parser::Expression;

	namespace Reasoner {

		using namespace Ontology;

		namespace Answerer {


			/*! 
			 *
			 *		\class		CAnsweringSplitPropagationTestingVariableData
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CAnsweringSplitPropagationTestingVariableData {
				// public methods
				public:
					//! Constructor
					CAnsweringSplitPropagationTestingVariableData(cint64 variableDataId, CVariable* variable, CExpressionVariable* variableExpression, QSet<CIndividualReference>* bindingSet);

					QSet<CIndividualReference>* getVariableBindingIndividualSet();

					CAnsweringSplitPropagationTestingVariableData* resetIntegrations();
					CAnsweringSplitPropagationTestingVariableData* disableIntegrations();
					CAnsweringSplitPropagationTestingVariableData* setVariableBindingsIntegrationLimit(cint64 limit);

					CAnsweringSplitPropagationTestingVariableData* addIntegrateableVariableBinding(const CIndividualReference& indiRef);
					CAnsweringSplitPropagationTestingVariableData* addIntegrateableVariableBindings(QSet<CIndividualReference>* bindingSet);
					bool visitIntegrateableVariableBindings(function<bool(const CIndividualReference& indiRef)> visitFunc);
					cint64 getIntegrateableVariableBindingCount();


					cint64 getVariableDataId();
					cint64 getVariableBindingsIntegrationCount();
					cint64 getVariableBindingsIntegrationLimit();
					cint64 getVariableBindingsRemainingIntegrationRequirementCount();

					bool hasVariableBiningsIntegrationLimitReached();


					CVariable* getVariable();
					CExpressionVariable* getVariableExpression();

					QHash<CIndividualReference, CAnsweringSplitPropagationTestingVariableBindingData>* getBindingIndividualDataHash();

					bool canIntegrateBinding(const CIndividualReference& indiRef, cint64* integationCounter);
					CAnsweringSplitPropagationTestingVariableData* incVariableBindingsRemainingIntegrationRequirementCount(cint64 count);


					bool hasVariableDataIntegrationId();
					cint64 getVariableDataIntegrationId();
					CAnsweringSplitPropagationTestingVariableData* setVariableDataIntegrationId(cint64 id);


					cint64 getDisabledVariableBindingsCount();
					bool hasAllVariableBindingsDisabled();
					cint64 getLastVariableBindingsIntegrationCount(bool updateLast = true);

				// protected methods
				protected:

				// protected variables
				protected:
					cint64 mVariableDataIntegrationId;
					cint64 mVariableDataId;
					CVariable* mVariable;
					CExpressionVariable* mVariableExpression;
					bool mDynamicBindings;

					QSet<CIndividualReference>* mBindingSet;
					QHash<CIndividualReference, CAnsweringSplitPropagationTestingVariableBindingData> mBindingIndiDataHash;

					cint64 mDisabledBindingsCount;
					cint64 mLastIntegratedBindingsCount;
					cint64 mIntegratedBindingsCount;
					cint64 mIntegrationBindingsLimit;
					cint64 mRemainingIntegrationRequirementCount;

					CAnsweringSplitPropagationTestingVariableBindingData* mIntegratedDataLinker;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_CANSWERINGSPLITPROPAGATIONTESTINGVARIABLEDATA_H
