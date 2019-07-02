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

#include "CReplacementData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			CReplacementData::CReplacementData() {
				mCommDisjConLinker = nullptr;
				mImplicationReplacementConcept = nullptr;
			}


			CConcept* CReplacementData::getImplicationReplacementConcept() {
				return mImplicationReplacementConcept;
			}

			CXNegLinker<CConcept*>* CReplacementData::getCommonDisjunctConceptLinker() {
				return mCommDisjConLinker;
			}

			CReplacementData* CReplacementData::setImplicationReplacementConcept(CConcept* implConcept) {
				mImplicationReplacementConcept = implConcept;
				return this;
			}

			CReplacementData* CReplacementData::setCommonDisjunctConceptLinker(CXNegLinker<CConcept*>* conLinker) {
				mCommDisjConLinker = conLinker;
				return this;
			}

			CReplacementData* CReplacementData::addCommonDisjunctConceptLinker(CXNegLinker<CConcept*>* conLinker) {
				if (conLinker) {
					mCommDisjConLinker = conLinker->append(mCommDisjConLinker);
				}
				return this;
			}

			CReplacementData* CReplacementData::initReplacementData(CReplacementData* replData) {
				if (replData) {
					mImplicationReplacementConcept = replData->mImplicationReplacementConcept;
					mCommDisjConLinker = replData->mCommDisjConLinker;
				} else {
					mImplicationReplacementConcept = nullptr;
					mCommDisjConLinker = nullptr;
				}
				return this;
			}

			CReplacementData* CReplacementData::clearCommonDisjunctConceptLinker() {
				mCommDisjConLinker = nullptr;
				return this;
			}


		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude
