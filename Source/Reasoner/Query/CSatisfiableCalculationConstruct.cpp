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

#include "CSatisfiableCalculationConstruct.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CSatisfiableCalculationConstruct::CSatisfiableCalculationConstruct() : CLinkerBase<CSatisfiableCalculationConstruct*,CSatisfiableCalculationConstruct>(this) {
				mIndividual = nullptr;
				mIndiID = -1;
				mRelNodeID = 0;
			}


			CSatisfiableCalculationConstruct::~CSatisfiableCalculationConstruct() {
			}

			CSatisfiableCalculationConstruct* CSatisfiableCalculationConstruct::getNextConstruct() {
				return getNext();
			}



			CSatisfiableCalculationConstruct* CSatisfiableCalculationConstruct::setIndividual(CIndividual* individual) {
				mIndividual = individual;
				mIndiID = individual->getIndividualID();
				return this;
			}

			CIndividual* CSatisfiableCalculationConstruct::getIndividual() {
				return mIndividual;
			}

			CSatisfiableCalculationConstruct* CSatisfiableCalculationConstruct::setIndividualID(cint64 individualID) {
				mIndiID = individualID;
				return this;
			}

			cint64 CSatisfiableCalculationConstruct::getIndividualID() {
				return mIndiID;
			}

			CSatisfiableCalculationConstruct* CSatisfiableCalculationConstruct::setRelativeNewNodeID(cint64 nodeID) {
				return this;
			}

			cint64 CSatisfiableCalculationConstruct::getRelativeNewNodeID() {
				return mRelNodeID;
			}


		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
