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

#include "CNamedItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			CNamedItem::CNamedItem() {
				mNameLinker = 0;
			}




			bool CNamedItem::hasName() {
				return mNameLinker;
			}


			CNamedItem *CNamedItem::setNameLinker(CLinker<CName *> *nameLinker) {
				mNameLinker = nameLinker;
				return this;
			}

			CNamedItem *CNamedItem::addNameLinker(CLinker<CName *> *nameLinker) {
				if (nameLinker) {
					nameLinker->append(mNameLinker);
					mNameLinker = nameLinker;
				}
				return this;
			}

			CLinker<CName *> *CNamedItem::getNameLinker() {
				return mNameLinker;
			}




		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude
