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

#include "CSuccessorConnectedNominalSet.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CSuccessorConnectedNominalSet::CSuccessorConnectedNominalSet(CProcessContext* processContext) 
						: mProcessContext(processContext),CPROCESSSET<cint64>(processContext) {

				}


				CSuccessorConnectedNominalSet* CSuccessorConnectedNominalSet::initSuccessorConnectedNominalSet(CSuccessorConnectedNominalSet* nominalSet) {
					if (nominalSet) {
						*this = *nominalSet;
					} else {
						CPROCESSSET<cint64>::clear();
					}
					return this;
				}

				CSuccessorConnectedNominalSet* CSuccessorConnectedNominalSet::copySuccessorConnectedNominalSet(CSuccessorConnectedNominalSet* nominalSet) {
					initSuccessorConnectedNominalSet(nominalSet);
					return this;
				}


				bool CSuccessorConnectedNominalSet::addSuccessorConnectedNominal(cint64 nominalNodeID) {
					if (!CPROCESSSET<cint64>::contains(nominalNodeID)) {
						CPROCESSSET<cint64>::insert(nominalNodeID);
						return true;
					}
					return false;
				}


				bool CSuccessorConnectedNominalSet::hasSuccessorConnectedNominal(cint64 nominalNodeID) {
					return CPROCESSSET<cint64>::contains(nominalNodeID);
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
