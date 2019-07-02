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

#include "CConceptNegationOverRoleTuble.h"


namespace Konclude {

	namespace Reasoner {

		namespace Classifier {


			CConceptNegationOverRoleTuble::CConceptNegationOverRoleTuble(qint64 conceptTag, bool conceptNegation, bool conceptOverRole) {
				conTag = conceptTag;
				conNeg = conceptNegation;
				overRole = conceptOverRole;
			}

			CConceptNegationOverRoleTuble::~CConceptNegationOverRoleTuble() {
			}

			qint64 CConceptNegationOverRoleTuble::getConceptTag() const {
				return conTag;
			}

			bool CConceptNegationOverRoleTuble::isConceptNegation() const {
				return conNeg;
			}

			bool CConceptNegationOverRoleTuble::isConceptOverRole() const {
				return overRole;
			}

			bool CConceptNegationOverRoleTuble::operator==(const CConceptNegationOverRoleTuble& tuple) const {
				return conNeg == tuple.conNeg && conTag == tuple.conTag && overRole == tuple.overRole;
			}


		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude
