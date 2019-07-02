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

#include "CPreviousIndividualIDSet.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CPreviousIndividualIDSet::CPreviousIndividualIDSet(CProcessContext* context) : CPROCESSSET<cint64>(context) {
				}

				CPreviousIndividualIDSet::CPreviousIndividualIDSet(const CPreviousIndividualIDSet &disHash, CContext* context) : CPROCESSSET<cint64>(disHash,context) {
				}


				CPreviousIndividualIDSet::~CPreviousIndividualIDSet() {
				}

				CPreviousIndividualIDSet* CPreviousIndividualIDSet::initPreviousIndividualIDSet(CPreviousIndividualIDSet* prevIDSet) {
					if (prevIDSet) {
						*this = *prevIDSet;
					} else {
						clear();
					}
					return this;
				}

				bool CPreviousIndividualIDSet::hasPreviousIndividualID(CIndividualProcessNode *indi) {
					if (indi) {
						qint64 indiID = indi->getIndividualID();
						return CPROCESSSET<cint64>::contains(indiID);
					} else {
						return false;
					}
				}

				bool CPreviousIndividualIDSet::hasPreviousIndividualID(cint64 indiID) {
					return CPROCESSSET<cint64>::contains(indiID);
				}

				CPreviousIndividualIDSet *CPreviousIndividualIDSet::insertPreviousIndividualID(cint64 indiID) {
					CPROCESSSET<cint64>::insert(indiID);
					return this;
				}

				CPreviousIndividualIDSet *CPreviousIndividualIDSet::insertPreviousIndividualID(CIndividualProcessNode *indi) {
					if (indi) {
						qint64 indiID = indi->getIndividualID();
						CPROCESSSET<cint64>::insert(indiID);
					} 
					return this;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
