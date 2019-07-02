/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
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

#include "CQueryInconsitentOntologyError.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CQueryInconsitentOntologyError::CQueryInconsitentOntologyError(CConcreteOntology* onto) {
				mOnto = onto;
				mOntologyString = mOnto->getOntologyName();
			}

			QString CQueryInconsitentOntologyError::getErrorString() {
				return QString("Ontology '%1' is inconsistent.").arg(mOntologyString);
			}


			bool CQueryInconsitentOntologyError::hasInconsistentOntologyError(CQueryError* queryErrorLinker) {
				for (CQueryError* queryErrorLinkerIt = queryErrorLinker; queryErrorLinkerIt; queryErrorLinkerIt = queryErrorLinkerIt->getNext()) {
					if (dynamic_cast<CQueryInconsitentOntologyError*>(queryErrorLinkerIt)) {
						return true;
					}
				}
				return false;
			}

			CQueryInconsitentOntologyError* CQueryInconsitentOntologyError::getInconsistentOntologyError(CQueryError* queryErrorLinker) {
				for (CQueryError* queryErrorLinkerIt = queryErrorLinker; queryErrorLinkerIt; queryErrorLinkerIt = queryErrorLinkerIt->getNext()) {
					CQueryInconsitentOntologyError* inOnEr = dynamic_cast<CQueryInconsitentOntologyError*>(queryErrorLinkerIt);
					if (inOnEr) {
						return inOnEr;
					}
				}
				return false;
			}


		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
