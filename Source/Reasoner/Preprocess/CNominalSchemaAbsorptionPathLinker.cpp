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

#include "CNominalSchemaAbsorptionPathLinker.h"


namespace Konclude {

	namespace Reasoner {

		namespace Preprocess {


			CNominalSchemaAbsorptionPathLinker::CNominalSchemaAbsorptionPathLinker() : CLinkerBase<CNominalSchemaAbsorptionPathLinker*,CNominalSchemaAbsorptionPathLinker>(this) {
			}

			CNominalSchemaAbsorptionPathLinker* CNominalSchemaAbsorptionPathLinker::initNominalSchemaAbsorptionPathLinker(CNominalSchemaAbsorptionPathLinker* nomSchAbsorpPathLinker) {
				if (nomSchAbsorpPathLinker) {
					mNomSchBindConceptHash = nomSchAbsorpPathLinker->mNomSchBindConceptHash;
					mAncestorList = nomSchAbsorpPathLinker->mAncestorList;
					mVariableList = nomSchAbsorpPathLinker->mVariableList;
					mLastPathConcept = nomSchAbsorpPathLinker->mLastPathConcept;
				} else {
					mLastPathConcept = nullptr;
					mNomSchBindConceptHash.clear();
					mAncestorList.clear();
					mVariableList.clear();
				}
				return this;
			}

			QHash<CConcept*,CConcept*>* CNominalSchemaAbsorptionPathLinker::getNominalSchemaBindedConceptTriggerHash() {
				return &mNomSchBindConceptHash;
			}

			QList<CConcept*>* CNominalSchemaAbsorptionPathLinker::getAncestorConceptTriggerList() {
				return &mAncestorList;
			}

			QList<CVariable*>* CNominalSchemaAbsorptionPathLinker::getVariableList() {
				return &mVariableList;
			}

			bool CNominalSchemaAbsorptionPathLinker::hasVariables() {
				return !mVariableList.isEmpty();
			}

			CNominalSchemaAbsorptionPathLinker* CNominalSchemaAbsorptionPathLinker::addVariable(CVariable* variable) {
				mVariableList.append(variable);
				return this;
			}


			CVariable* CNominalSchemaAbsorptionPathLinker::getLastVariable() {
				if (hasVariables()) {
					return mVariableList.last();
				} else {
					return nullptr;
				}
			}

			CConcept* CNominalSchemaAbsorptionPathLinker::getLastPathTriggerConcept() {
				return mLastPathConcept;
			}

			CNominalSchemaAbsorptionPathLinker* CNominalSchemaAbsorptionPathLinker::setLastPathTriggerConcept(CConcept* lastPathTriggerConcept) {
				mLastPathConcept = lastPathTriggerConcept;
				return this;
			}

		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude
