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

#ifndef KONCLUDE_TEST_CONFORMANCE_CREALIZATIONRESULTCOMPARATOR_H
#define KONCLUDE_TEST_CONFORMANCE_CREALIZATIONRESULTCOMPARATOR_H

// Libraries includes
#include <QString>
#include <QHash>
#include <QSet>

// Namespace includes
#include "CConformanceTask.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Logger;

	namespace Test {

		namespace Conformance {

			/*!
			 *		\class		CRealizationResultComparator
			 *		\brief		Semantic comparison of two OWL/XML realization results.
			 *
			 *					A realization result is a set of class assertions
			 *					(individual, class); the order of the assertions and the order
			 *					of the declarations are irrelevant. The comparator reads both
			 *					files, expands abbreviated IRIs with the declared prefixes and
			 *					compares the assertion sets.
			 */
			class CRealizationResultComparator {
				// public methods
				public:
					//! Compares the realization result written by a run with the expected
					//! result. Returns true when both files describe the same set of types.
					static bool compareRealizationFiles(const QString& actualFile, const QString& expectedFile, QString& detailMessage);

				// protected methods
				protected:
					//! Reads the class assertions of a realization result file.
					static bool loadTypeAssertions(const QString& filePath, QSet<QString>& typeAssertions, QString& detailMessage);

					//! Expands an IRI/abbreviatedIRI attribute with the declared prefixes.
					static QString resolveIRI(const QString& iriValue, const QHash<QString, QString>& prefixIRIHash);
			};

		}; // end namespace Conformance

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_CONFORMANCE_CREALIZATIONRESULTCOMPARATOR_H
