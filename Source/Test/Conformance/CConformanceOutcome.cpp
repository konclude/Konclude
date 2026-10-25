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

#include "CConformanceOutcome.h"


namespace Konclude {

	namespace Test {

		namespace Conformance {


			QList<CConformanceOutcome::TYPE> CConformanceOutcome::allTypes() {
				QList<TYPE> types;
				types << TYPE::PASS << TYPE::KNOWN << TYPE::FAIL << TYPE::FIXED << TYPE::TIMEOUT << TYPE::FLAKY;
				return types;
			}


			QString CConformanceOutcome::typeToString(TYPE type) {
				switch (type) {
					case TYPE::PASS: return QString("PASS");
					case TYPE::KNOWN: return QString("KNOWN");
					case TYPE::FAIL: return QString("FAIL");
					case TYPE::FIXED: return QString("FIXED");
					case TYPE::TIMEOUT: return QString("TIMEOUT");
					case TYPE::FLAKY: return QString("FLAKY");
				}
				return QString();
			}


			QString CConformanceOutcome::typeToDescription(TYPE type) {
				switch (type) {
					case TYPE::PASS: return QString("passed (PASS)");
					case TYPE::KNOWN: return QString("with known issues (KNOWN)");
					case TYPE::FAIL: return QString("failed (FAIL)");
					case TYPE::FIXED: return QString("known issues now fixed (FIXED)");
					case TYPE::TIMEOUT: return QString("timed out (TIMEOUT)");
					case TYPE::FLAKY: return QString("flaky (FLAKY)");
				}
				return QString();
			}


			bool CConformanceOutcome::typeIsFailing(TYPE type) {
				return type == TYPE::FAIL || type == TYPE::FIXED || type == TYPE::TIMEOUT || type == TYPE::FLAKY;
			}


		}; // end namespace Conformance

	}; // end namespace Test

}; // end namespace Konclude
