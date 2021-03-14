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

#include "CAnsweringSplitPropagationTestingVariableBindingData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			CAnsweringSplitPropagationTestingVariableBindingData::CAnsweringSplitPropagationTestingVariableBindingData() {
				setData(false);
				mDisabled = false;
			}


			bool CAnsweringSplitPropagationTestingVariableBindingData::isIntegrated() {
				return getData();
			}


			bool CAnsweringSplitPropagationTestingVariableBindingData::isDisabled() {
				return mDisabled;
			}

			CAnsweringSplitPropagationTestingVariableBindingData* CAnsweringSplitPropagationTestingVariableBindingData::setIntegrated() {
				setData(true);
				return this;
			}

			CAnsweringSplitPropagationTestingVariableBindingData* CAnsweringSplitPropagationTestingVariableBindingData::resetIntegration() {
				setData(false);
				mDisabled = false;
				return this;
			}

			CAnsweringSplitPropagationTestingVariableBindingData* CAnsweringSplitPropagationTestingVariableBindingData::disableIntegration() {
				setData(false);
				mDisabled = true;
				return this;
			}

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
