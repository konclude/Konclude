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

#include "CNominalSchemaAbsorptionBranchLinker.h"


namespace Konclude {

	namespace Reasoner {

		namespace Preprocess {


			CNominalSchemaAbsorptionBranchLinker::CNominalSchemaAbsorptionBranchLinker() : CLinkerBase<CNominalSchemaAbsorptionBranchLinker*,CNominalSchemaAbsorptionBranchLinker>(this) {
			}

			CNominalSchemaAbsorptionBranchLinker* CNominalSchemaAbsorptionBranchLinker::initNominalSchemaAbsorptionPathLinker(CNominalSchemaAbsorptionBranchLinker* nomSchAbsorpPathLinker) {
				if (nomSchAbsorpPathLinker) {
					mNomSchBindConceptHash = nomSchAbsorpPathLinker->mNomSchBindConceptHash;
					mVariableSet = nomSchAbsorpPathLinker->mVariableSet;
					mLastBranchTriggerConcept = nomSchAbsorpPathLinker->mLastBranchTriggerConcept;
					mBackPropagationBranchTriggerConcept = nomSchAbsorpPathLinker->mBackPropagationBranchTriggerConcept;
				} else {
					mBackPropagationBranchTriggerConcept = nullptr;
					mLastBranchTriggerConcept = nullptr;
					mNomSchBindConceptHash.clear();
					mVariableSet.clear();
				}
				return this;
			}


			CNominalSchemaAbsorptionBranchLinker* CNominalSchemaAbsorptionBranchLinker::joinNominalSchemaAbsorptionPathLinker(CNominalSchemaAbsorptionBranchLinker* nomSchAbsorpPathLinker1, CNominalSchemaAbsorptionBranchLinker* nomSchAbsorpPathLinker2) {
				mNomSchBindConceptHash = nomSchAbsorpPathLinker1->mNomSchBindConceptHash;
				for (QHash<CConcept*,CConcept*>::const_iterator it = nomSchAbsorpPathLinker2->mNomSchBindConceptHash.constBegin(), itEnd = nomSchAbsorpPathLinker2->mNomSchBindConceptHash.constEnd(); it != itEnd; ++it) {
					mNomSchBindConceptHash.insertMulti(it.key(),it.value());
				}
				mVariableSet = nomSchAbsorpPathLinker1->mVariableSet;
				mVariableSet += nomSchAbsorpPathLinker2->mVariableSet;
				mLastBranchTriggerConcept = nullptr;
				mBackPropagationBranchTriggerConcept = nullptr;
				return this;
			}


			QHash<CConcept*,CConcept*>* CNominalSchemaAbsorptionBranchLinker::getNominalSchemaBindedConceptTriggerHash() {
				return &mNomSchBindConceptHash;
			}

			QSet<CVariable*>* CNominalSchemaAbsorptionBranchLinker::getVariableSet() {
				return &mVariableSet;
			}

			bool CNominalSchemaAbsorptionBranchLinker::hasVariables() {
				return !mVariableSet.isEmpty();
			}

			CNominalSchemaAbsorptionBranchLinker* CNominalSchemaAbsorptionBranchLinker::addVariable(CVariable* variable) {
				mVariableSet.insert(variable);
				return this;
			}



			CConcept* CNominalSchemaAbsorptionBranchLinker::getLastBranchTriggerConcept() {
				return mLastBranchTriggerConcept;
			}

			CNominalSchemaAbsorptionBranchLinker* CNominalSchemaAbsorptionBranchLinker::setLastBranchTriggerConcept(CConcept* lastBranchTriggerConcept) {
				mLastBranchTriggerConcept = lastBranchTriggerConcept;
				return this;
			}



			CConcept* CNominalSchemaAbsorptionBranchLinker::getBackPropagationBranchTriggerConcept() {
				return mBackPropagationBranchTriggerConcept;
			}

			CNominalSchemaAbsorptionBranchLinker* CNominalSchemaAbsorptionBranchLinker::setBackPropagationBranchTriggerConcept(CConcept* backPropagationBranchTriggerConcept) {
				mBackPropagationBranchTriggerConcept = backPropagationBranchTriggerConcept;
				return this;
			}


		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude
