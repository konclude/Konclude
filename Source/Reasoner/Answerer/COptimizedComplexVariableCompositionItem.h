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

#ifndef KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLECOMPOSITIONITEM_H
#define KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLECOMPOSITIONITEM_H

// Libraries includes


// Namespace includes
#include "AnswererSettings.h"
#include "COptimizedComplexVariableIndividualMapping.h"
#include "COptimizedComplexVariableJoiningBindingPositionMapping.h"



// Other includes
#include "Reasoner/Ontology/CRole.h"

#include "Utilities/CVariantTrible.hpp"
#include "Utilities/CVariantQuad.hpp"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities;

	namespace Reasoner {

		using namespace Ontology;

		namespace Answerer {


#ifndef KONCLUDE_FORCE_ALL_DEBUG_DEACTIVATED

			#define OPTIMIZED_ANSWERER_DEBUG_STRINGS

#endif

			/*! 
			 *
			 *		\class		COptimizedComplexVariableCompositionItem
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedComplexVariableCompositionItem {
				// public methods
				public:
					//! Constructor
					COptimizedComplexVariableCompositionItem();
					virtual ~COptimizedComplexVariableCompositionItem();

					enum COMPOSITION_TYPE {
						CONCEPT_BASE, ROLE_PROPAGATION, JOINING, DATA_LITERAL_BASE, DATA_LITERAL_EXTENSION, ABSOROPTION_BASED_EXTENSION, BINDING_REDUCTION, ROLE_PROPAGATION_JOIN, ROLE_PROPAGATION_REPLACEMENT
					};


					virtual COMPOSITION_TYPE getCompositionType() = 0;
					


					bool isVariableMappingComputed();
					COptimizedComplexVariableCompositionItem* setVariableMappingComputed(bool computed);

					bool isComputationQueued();
					COptimizedComplexVariableCompositionItem* setComputationQueued(bool queued);


					QHash<CVariantTrible<CRole*, bool, cint64>, COptimizedComplexVariableRolePropagationItem*>* getRoleInverseItemHash();
					QHash<COptimizedComplexVariableJoiningBindingPositionMapping, COptimizedComplexVariableJoiningItem*>* getJoiningItemHash();

					
					COptimizedComplexVariableRolePropagationItem*& getRoleInversePropagationItem(CRole* role, bool inversed, cint64 varIdx);
					COptimizedComplexVariableRolePropagationJoiningItem*& getRoleInversePropagationJoiningItem(CRole* role, bool inversed, cint64 varIdx, cint64 joinVarIdx);
					COptimizedComplexVariableRolePropagationReplacementItem*& getRoleInversePropagationReplacementItem(CRole* role, bool inversed, cint64 varIdx, cint64 replaceVarIdx);


					COptimizedComplexVariableJoiningItem*& getJoiningItem(const COptimizedComplexVariableJoiningBindingPositionMapping& bindingPosMapping);

					bool hasJoiningItem(const COptimizedComplexVariableJoiningBindingPositionMapping& bindingPosMapping);
					bool hasRoleInversePropagationItem(CRole* role, bool inversed, cint64 varIdx);


					COptimizedComplexVariableIndividualMapping* getVariableMapping();


					COptimizedComplexVariableCompositionItem* setVariableMapping(COptimizedComplexVariableIndividualMapping* variableMapping);


					COptimizedComplexVariableDataLiteralExtensionItem*& getDataLiteralExtensionItem(CRole* dataRole, cint64 varIdx);
					COptimizedComplexVariableReductionItem*& getReductionItem(cint64 varIdx);


#ifdef OPTIMIZED_ANSWERER_DEBUG_STRINGS
					QStringList debugVariableNameStringList;
					QString debugCreationString;
#endif

				// protected methods
				protected:

				// protected variables
				protected:
					COptimizedComplexVariableIndividualMapping* mVariableMapping;

					bool mVariableMappingComputed;
					bool mComputationQueued;



					QHash<CVariantQuad<CRole*, bool, cint64, cint64>, COptimizedComplexVariableRolePropagationJoiningItem*> mRoleInverseJoinItemHash;
					QHash<CVariantQuad<CRole*, bool, cint64, cint64>, COptimizedComplexVariableRolePropagationReplacementItem*> mRoleInverseReplaceItemHash;

					QHash<CVariantTrible<CRole*, bool, cint64>, COptimizedComplexVariableRolePropagationItem*> mRoleInverseItemHash;
					QHash<COptimizedComplexVariableJoiningBindingPositionMapping, COptimizedComplexVariableJoiningItem*> mJoiningItemHash;
					QHash< QPair<CRole*, cint64>, COptimizedComplexVariableDataLiteralExtensionItem* > mDataLiteralExtensionItemHash;

					QHash< cint64, COptimizedComplexVariableReductionItem* > mReductionItemHash;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLECOMPOSITIONITEM_H