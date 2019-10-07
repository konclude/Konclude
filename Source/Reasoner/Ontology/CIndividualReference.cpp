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

#include "CIndividualReference.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			CIndividualReference::CIndividualReference() : mIdentifier(-1) {
				mIndividual = nullptr;
			}

			CIndividualReference::CIndividualReference(cint64 id) : mIdentifier(id) {
				mIndividual = nullptr;
			}


			CIndividualReference::CIndividualReference(const CIndividualIdentifier& identifier) : mIdentifier(identifier) {
				mIndividual = nullptr;
			}

			CIndividualReference::CIndividualReference(CIndividual* individual) {
				if (individual) {
					mIdentifier = individual->getIndividualID();
					mIndividual = individual;
				} else {
					mIndividual = nullptr;
					mIdentifier = -1;
				}
			}

			CIndividualReference::CIndividualReference(CIndividual* individual, cint64 indiId) : mIdentifier(indiId) {
				mIndividual = individual;
			}

			cint64 CIndividualReference::getIndividualID() const {
				return mIdentifier.getIndividualID();
			}

			CIndividualIdentifier CIndividualReference::getIndividualIdentifier() const {
				return mIdentifier;
			}

			CIndividual* CIndividualReference::getIndividual() const {
				return mIndividual;
			}

			bool CIndividualReference::isEmpty() const {
				return getIndividualID() == -1;
			}

			bool CIndividualReference::isNonEmpty() const {
				return getIndividualID() != -1;
			}


		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude
