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

#include "CConceptDescriptor.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				CConceptDescriptor::CConceptDescriptor() {
				}


				CConceptDescriptor* CConceptDescriptor::initConceptDescriptor(CConcept* concept, bool negated, CDependencyTrackPoint* depTrackPoint) {
					initNegLinker(concept,negated);
					initDependencyTracker(depTrackPoint);
					return this;
				}


				bool CConceptDescriptor::isEqualsToBOTTOM(bool negated) {
					return getData()->isEqualsToBOTTOM(isNegated()^negated);
				}

				bool CConceptDescriptor::isEqualsToTOP(bool negated) {
					return getData()->isEqualsToTOP(isNegated()^negated);
				}

				CConcept *CConceptDescriptor::getConcept() {
					return getData();
				}

				qint64 CConceptDescriptor::getConceptTag() {
					return getData()->getConceptTag();
				}

				qint64 CConceptDescriptor::getTerminologyTag() {
					return getData()->getTerminologyTag();
				}

				QPair<qint64,qint64> CConceptDescriptor::getTerminologyConceptTagPair() {
					return getData()->getTerminologyConceptTagPair();
				}


				bool CConceptDescriptor::isClashWith(CConceptDescriptor *conDesNegLinker) {
					if (conDesNegLinker->isNegated() == isNegated()) {
						return false;
					}
					if (getConceptTag() == conDesNegLinker->getConceptTag()) {
						if (getTerminologyTag() == conDesNegLinker->getTerminologyTag()) {
							return true;
						}
					}
					return false;
				}

				CConceptDescriptor *CConceptDescriptor::getNextConceptDesciptor() {
					return getNext();
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
