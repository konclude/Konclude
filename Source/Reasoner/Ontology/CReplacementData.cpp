/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
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
