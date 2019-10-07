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

#include "CVariableBinding.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CVariableBinding::CVariableBinding() {
				}


				CVariableBinding* CVariableBinding::initVariableBinding(CDependencyTrackPoint* dependencyTrackPoint, CIndividualProcessNode* indi, CVariable* variable) {
					initDependencyTracker(dependencyTrackPoint);
					mVariable = variable;
					mIndiNode = indi;
					return this;
				}

				CVariable* CVariableBinding::getBindedVariable() const {
					return mVariable;
				}

				CVariableBinding* CVariableBinding::setBindedVariable(CVariable* variable) {
					mVariable = variable;
					return this;
				}

				CIndividualProcessNode* CVariableBinding::getBindedIndividual() {
					return mIndiNode;
				}

				CVariableBinding* CVariableBinding::setBindedIndividual(CIndividualProcessNode* indi) {
					mIndiNode = indi;
					return this;
				}



				bool CVariableBinding::operator<=(const CVariableBinding& beforeData) {
					return getBindedVariable() <= beforeData.getBindedVariable();
				}

				bool CVariableBinding::operator<=(const CVariableBinding*& beforeData) {
					return getBindedVariable() <= beforeData->getBindedVariable();
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
