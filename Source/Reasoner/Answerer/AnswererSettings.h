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

#ifndef KONCLUDE_REASONER_ANSWERER_ANSWERERSETTINGS
#define KONCLUDE_REASONER_ANSWERER_ANSWERERSETTINGS

// Libraries includes
#include <QtGlobal>
#include <QEvent>

// Namespace includes


// Other includes
#include "Utilities/Container/CQtManagedRestrictedModificationList.h"
#include "Utilities/Container/CQtManagedRestrictedModificationMap.h"
#include "Utilities/Container/CQtManagedRestrictedModificationHash.h"

// Logger includes


namespace Konclude {

	using namespace Utilities::Container;

	namespace Reasoner {

		namespace Answerer {

			/*! 
			 *
			 *		\file		ClassificationSettings
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */



			// forward declarations

			class CComplexQueryExpressionProcessingData;
			class COptimizedComplexConceptItem;
			class CAnsweringHandler;
			class CAnswererThread;
			class CAnsweringMessageData;
			class CAnsweringMessageDataCalculationCompleted;
			class CAnsweringCalculationHandler;
			class CAnswererContext;
			class COntologyAnsweringItem;
			class COptimizedComplexVariableCompositionItem;
			class COptimizedComplexBuildingIndividualVariableCompositionsItem;
			class COptimizedComplexVariableRolePropagationItem;
			class COptimizedComplexVariableRolePropagationJoiningItem;
			class COptimizedComplexVariableRolePropagationReplacementItem;
			class COptimizedComplexVariableConceptBaseItem;
			class CRedlandRasqalSubQueryBoundNodeResolvingHash;
			class COptimizedComplexVariableJoiningItem;
			class COptimizedComplexVariableDataLiteralExtensionItem;
			class COptimizedComplexVariableReductionItem;
			class COptimizedComplexVariableExtractionItem;
			class COptimizedComplexVariableCompositionItemVariableIndexMapping;
			class CRedlandRasqalSubQueryTripleResolvingData;
			class CRedlandRasqalSubQueryProcessingData;
			class CRedlandRasqalSubQueryBindingPositionResolvingHash;
			class CAnsweringPropagationSteeringAbsorptionExtensionItemController;
			class COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem;
			class COptimizedComplexVariableCompositionItemDependence;
			class COptimizedComplexBuildingVariableCompositionsItem;
			class COptimizedComplexBuildingClassVariableCompositionsItem;
			class COptimizedComplexVariableConceptSubSuperItem;
			class COptimizedComplexVariableRoleSubSuperItem;
			class COptimizedComplexVariableSubSuperClassesPropagationItem;
			class COptimizedComplexVariableSubSuperPropertiesPropagationItem;
			class COptimizedComplexVariableIndividualTypeExtensionItem;
			class COptimizedComplexVariableIndividualNeighboringPropertyExtensionItem;

			// Custom Events >= 2000
			const QEvent::Type EVENTANSWERINGINITIALIZING			= (QEvent::Type)2001;
			const QEvent::Type EVENTANSWERINGMESSAGE				= (QEvent::Type)2002;
			const QEvent::Type EVENTCALCULATIONCOMPLETED			= (QEvent::Type)2003;
			const QEvent::Type EVENTANSWERCOMPLEXQUERY				= (QEvent::Type)2004;
			const QEvent::Type EVENTANSWRINGCOMPLEXQUERYCOMPLETED	= (QEvent::Type)2005;
			const QEvent::Type EVENTREQUIREMENTCOMPLETED			= (QEvent::Type)2006;
			const QEvent::Type EVENTANSWERINGPREPARATION			= (QEvent::Type)2007;
			const QEvent::Type EVENTANSWERINGPREPARED				= (QEvent::Type)2008;




#define CCLASSSUBSUMPTIONMESSAGELIST CQtManagedRestrictedModificationList
#define CCLASSPOSSIBLESUBSUMPTIONMESSAGELIST CQtManagedRestrictedModificationList
#define CCLASSPSEUDOMODELHASH CQtManagedRestrictedModificationHash
#define CCLASSPSEUDOMODELMAP CQtManagedRestrictedModificationMap

#define CPROPERTYSUBSUMPTIONMESSAGELIST CQtManagedRestrictedModificationList
#define CPROPERTYPOSSIBLESUBSUMPTIONMESSAGELIST CQtManagedRestrictedModificationList


#define CVARIABLEBINDINGMESSAGELIST CQtManagedRestrictedModificationList


#ifndef KONCLUDE_FORCE_ALL_DEBUG_DEACTIVATED

	#define OPTIMIZED_ANSWERER_DEBUG_STRINGS

#endif



#ifndef KONCLUDE_FORCE_ALL_DEBUG_DEACTIVATED

#define CONCURRENT_JOIN_COMPUTATION_PERFORMANCE_MEASUREMENT

#endif


		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude


#endif // end KONCLUDE_REASONER_ANSWERER_ANSWERERSETTINGS