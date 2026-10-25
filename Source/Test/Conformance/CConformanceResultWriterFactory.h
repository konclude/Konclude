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

#ifndef KONCLUDE_TEST_CONFORMANCE_CCONFORMANCERESULTWRITERFACTORY_H
#define KONCLUDE_TEST_CONFORMANCE_CCONFORMANCERESULTWRITERFACTORY_H

// Namespace includes
#include "CConformanceResultWriter.h"


namespace Konclude {

	namespace Test {

		namespace Conformance {

			/*!
			 *		\class		CConformanceResultWriterFactory
			 *		\brief		Creates the result writer that matches a result file name.
			 *
			 *					The suffix selects the format: '*.owl.xml' and '*.xml' are
			 *					written as OWL/XML. Adding another format means adding a
			 *					writer subclass and a suffix case here.
			 */
			class CConformanceResultWriterFactory {
				// public methods
				public:
					//! Creates the writer for filePath. The caller owns the returned
					//! object. Returns nullptr when the format of filePath is unsupported.
					static CConformanceResultWriter* createWriter(const QString& filePath);
			};

		}; // end namespace Conformance

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_CONFORMANCE_CCONFORMANCERESULTWRITERFACTORY_H
