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

#include "CSaturationInfluencedNominalSet.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CSaturationInfluencedNominalSet::CSaturationInfluencedNominalSet(CProcessContext* processContext) 
						: mProcessContext(processContext) {

				}


				CSaturationInfluencedNominalSet* CSaturationInfluencedNominalSet::initInfluencedNominalSet(CSaturationInfluencedNominalSet* nominalSet) {
					if (nominalSet) {
						*this = *nominalSet;
					} else {
						CPROCESSSET<cint64>::clear();
					}
					return this;
				}


				bool CSaturationInfluencedNominalSet::setNominalInfluenced(cint64 nominalID) {
					if (!CPROCESSSET<cint64>::contains(nominalID)) {
						CPROCESSSET<cint64>::insert(nominalID);
						return true;
					}
					return false;
				}

				bool CSaturationInfluencedNominalSet::isNominalInfluenced(cint64 nominalID) {
					return CPROCESSSET<cint64>::contains(nominalID);
				}



			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
