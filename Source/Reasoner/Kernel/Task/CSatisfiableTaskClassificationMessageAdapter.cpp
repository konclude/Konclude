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

#include "CSatisfiableTaskClassificationMessageAdapter.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Task {

				CSatisfiableTaskClassificationMessageAdapter::CSatisfiableTaskClassificationMessageAdapter(CConcept* testingConcept, CConcreteOntology* testingOntology, CClassificationMessageDataObserver* observer, QHash<CConcept*,CConceptReferenceLinking*>* conRefLinkDataHash, cint64 extractionFlags) {
					mTestingCon = testingConcept;
					mOntology = testingOntology;
					mMessageObserver = observer;
					mConRefLinkDataHash = conRefLinkDataHash;
					mExtractionFlags = extractionFlags;
				}

				CConcept *CSatisfiableTaskClassificationMessageAdapter::getTestingConcept() {
					return mTestingCon;
				}

				CConcreteOntology *CSatisfiableTaskClassificationMessageAdapter::getTestingOntology() {
					return mOntology;
				}

				CClassificationMessageDataObserver *CSatisfiableTaskClassificationMessageAdapter::getClassificationMessageDataObserver() {
					return mMessageObserver;
				}

				QHash<CConcept*,CConceptReferenceLinking*>* CSatisfiableTaskClassificationMessageAdapter::getConceptReferenceLinkingDataHash() {
					return mConRefLinkDataHash;
				}

				cint64 CSatisfiableTaskClassificationMessageAdapter::getExtractionFlags() {
					return mExtractionFlags;
				}

				CSatisfiableTaskClassificationMessageAdapter* CSatisfiableTaskClassificationMessageAdapter::setExtractionFlags(cint64 flags) {
					mExtractionFlags = flags;
					return this;
				}

				bool CSatisfiableTaskClassificationMessageAdapter::hasExtractionFlags(cint64 flags) {
					return (mExtractionFlags & flags) != 0;
				}


			}; // end namespace Task

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
