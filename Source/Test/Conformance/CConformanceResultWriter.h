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

#ifndef KONCLUDE_TEST_CONFORMANCE_CCONFORMANCERESULTWRITER_H
#define KONCLUDE_TEST_CONFORMANCE_CCONFORMANCERESULTWRITER_H

// Libraries includes
#include <QString>
#include <QList>

// Namespace includes
#include "CConformanceTestResult.h"

// Logger includes
#include "Logger/CLogIdentifier.h"
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Logger;

	namespace Test {

		namespace Conformance {

			/*!
			 *		\class		CConformanceResultWriter
			 *		\brief		Serialises the executed conformance results into a result file.
			 *
			 *					The runner only knows this interface; the concrete format is
			 *					selected by CConformanceResultWriterFactory from the file name.
			 *					Adding another output format means adding a subclass and a
			 *					factory case, not editing the runner.
			 */
			class CConformanceResultWriter : public CLogIdentifier {
				// public methods
				public:
					CConformanceResultWriter();
					virtual ~CConformanceResultWriter();

					//! Writes all results to filePath. Returns false and logs the reason
					//! when the file cannot be written.
					virtual bool writeResults(const QString& filePath, const QList<CConformanceTestResult>& results) = 0;
			};

		}; // end namespace Conformance

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_CONFORMANCE_CCONFORMANCERESULTWRITER_H
