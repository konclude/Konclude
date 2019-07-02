/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
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

#include "CBooleanTaskResult.h"


namespace Konclude {

	namespace Scheduler {



		CBooleanTaskResult::CBooleanTaskResult() {
			mValidResult = false;
			mResultValue = false;
		}

		CBooleanTaskResult::~CBooleanTaskResult() {
		}


		bool CBooleanTaskResult::hasResult() {
			return mValidResult;
		}


		bool CBooleanTaskResult::getResultValue() {
			return mResultValue;
		}

		bool CBooleanTaskResult::hasResultValue(bool testValue) {
			return mResultValue == testValue;
		}

		CBooleanTaskResult* CBooleanTaskResult::setResultValue(bool resultValue) {
			mResultValue = resultValue;
			return this;
		}

		CBooleanTaskResult* CBooleanTaskResult::setValidResult(bool validResult) {
			mValidResult = validResult;
			return this;
		}

		CBooleanTaskResult* CBooleanTaskResult::installResult(bool resultValue) {
			mResultValue = resultValue;
			mValidResult = true;
			return this;
		}


	}; // end namespace Scheduler

}; // end namespace Konclude
