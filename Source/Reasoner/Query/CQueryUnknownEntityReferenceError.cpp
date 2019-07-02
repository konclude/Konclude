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

#include "CQueryUnknownEntityReferenceError.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CQueryUnknownEntityReferenceError::CQueryUnknownEntityReferenceError(CConcreteOntology* onto, const QString& unknownEntity) {
				mOnto = onto;
				mOntologyString = mOnto->getOntologyName();
				mUnknownEntityString = unknownEntity;
			}

			QString CQueryUnknownEntityReferenceError::getErrorString() {
				if (!mUnknownEntityString.isEmpty()) {
					return QString("Query references unknown entity '%1' in ontology '%2'.").arg(mUnknownEntityString).arg(mOntologyString);
				} else {
					return QString("Query references unknown entity in ontology '%1'.").arg(mOntologyString);
				}
			}


			bool CQueryUnknownEntityReferenceError::hasUnknownEntityReferenceError(CQueryError* queryErrorLinker) {
				for (CQueryError* queryErrorLinkerIt = queryErrorLinker; queryErrorLinkerIt; queryErrorLinkerIt = queryErrorLinkerIt->getNext()) {
					if (dynamic_cast<CQueryUnknownEntityReferenceError*>(queryErrorLinkerIt)) {
						return true;
					}
				}
				return false;
			}

			CQueryUnknownEntityReferenceError* CQueryUnknownEntityReferenceError::getUnknownEntityReferenceError(CQueryError* queryErrorLinker) {
				for (CQueryError* queryErrorLinkerIt = queryErrorLinker; queryErrorLinkerIt; queryErrorLinkerIt = queryErrorLinkerIt->getNext()) {
					CQueryUnknownEntityReferenceError* inOnEr = dynamic_cast<CQueryUnknownEntityReferenceError*>(queryErrorLinkerIt);
					if (inOnEr) {
						return inOnEr;
					}
				}
				return nullptr;
			}


		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
