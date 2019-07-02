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

#include "CConceptReapplyHash.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CConceptReapplyHash::CConceptReapplyHash(CProcessContext* processContext) : CPROCESSHASH<QPair<CConcept*,bool>,CReapplyQueue>(processContext) {
				}

				CConceptReapplyHash::~CConceptReapplyHash() {
				}

				CConceptReapplyHash* CConceptReapplyHash::initConceptReapplyHash(CConceptReapplyHash* prevConceptReapplyHash) {
					if (prevConceptReapplyHash) {
						*this = *prevConceptReapplyHash;
					} else {
						CPROCESSHASH<QPair<CConcept*,bool>,CReapplyQueue>::clear();
					}
					return this;
				}


				CReapplyQueue* CConceptReapplyHash::getConceptReapplyQueue(CConcept *&concept, bool &conceptNegation, bool create) {
					QPair<CConcept*,bool> conNegPair(concept,conceptNegation);
					if (create) {
						return &this->operator[](conNegPair);
					} else {
						if (contains(conNegPair)) {
							return &this->operator[](conNegPair);
						} else {
							return nullptr;
						}
					}
				}


				bool CConceptReapplyHash::containsConceptReapplyQueue(CConcept *&concept, bool &conceptNegation) {
					return contains(QPair<CConcept*,bool>(concept,conceptNegation));
				}

				CReapplyQueueIterator CConceptReapplyHash::getConceptReapplyIterator(CConcept *&concept, bool &conceptNegation, bool clearDynamicReapplyQueue) {
					QPair<CConcept*,bool> conNegPair(concept,conceptNegation);
					CReapplyQueue* reapplyQueue = nullptr;
					if (tryGetValuePointer(conNegPair,reapplyQueue)) {
						return reapplyQueue->getIterator(clearDynamicReapplyQueue);
					}
					return CReapplyQueueIterator(nullptr,nullptr);
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
