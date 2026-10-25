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

#ifndef KONCLUDE_TEST_CONFORMANCE_CCLASSIFICATIONRESULTCOMPARATOR_H
#define KONCLUDE_TEST_CONFORMANCE_CCLASSIFICATIONRESULTCOMPARATOR_H

// Libraries includes
#include <QString>
#include <QHash>
#include <QDir>
#include <QFile>
#include <QDomDocument>

// Namespace includes
#include "Reasoner/Query/CClassHierarchyResult.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Logger;
	using namespace Reasoner::Query;

	namespace Test {

		namespace Conformance {

			/*!
			 *		\class		CClassificationResultComparator
			 *		\brief		Compares two OWL/XML classification results (the taxonomy
			 *					written by `Konclude classification -i <ont> -o <out.owl.xml>`)
			 *					semantically and order-independently. Both files are parsed into
			 *					the existing order-independent CClassHierarchyResult model
			 *					(where EquivalentClasses become synsets and SubClassOf define the
			 *					subsumption relations) and compared via isResultEquivalentTo().
			 *					Reusable for any OWL/XML taxonomy difference check.
			 */
			class CClassificationResultComparator {
				// public methods
				public:
					//! Compares two OWL/XML classification result files. Returns true when the
					//! taxonomies are equivalent (same classes, same equivalences, same subsumptions),
					//! independent of the order of assertions. On mismatch, detailMessage receives a
					//! short human-readable hint. Returns false (with a message) if either file is
					//! unreadable or does not contain a parseable taxonomy.
					static bool compareClassificationFiles(const QString& actualFile, const QString& expectedFile, QString& detailMessage);

				// protected methods
				protected:
					//! Parses an OWL/XML classification result file into a semantic,
					//! order-independent class hierarchy (equivalence classes become synsets).
					//! Returns null if the file cannot be read or contains no taxonomy.
					static CClassHierarchyResult* loadClassificationHierarchy(const QString& filePath);

					//! Resolves the IRI of a <Class> element, expanding abbreviatedIRI against the
					//! declared prefixes and falling back to the built-in OWL/RDF/RDFS/XSD/XML prefixes.
					static QString resolveClassIRI(const QDomElement& classElement, const QHash<QString, QString>& prefixIRIHash);

			};

		}; // end namespace Conformance

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_CONFORMANCE_CCLASSIFICATIONRESULTCOMPARATOR_H
