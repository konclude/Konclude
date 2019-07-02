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
				return nullptr;
			}


		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
