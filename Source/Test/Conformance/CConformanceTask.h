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

#ifndef KONCLUDE_TEST_CONFORMANCE_CCONFORMANCETASK_H
#define KONCLUDE_TEST_CONFORMANCE_CCONFORMANCETASK_H

// Libraries includes
#include <QString>
#include <QList>


namespace Konclude {

	namespace Test {

		namespace Conformance {

			/*!
			 *		\class		CConformanceTask
			 *		\brief		Description of the reasoner tasks a conformance test can
			 *					ask for: the task kinds, their canonical names as used in
			 *					control ontologies and reports, the control ontology marker
			 *					class and the verdict vocabulary of each task.
			 *
			 *					A task says WHAT is checked (consistency, classification, an
			 *					OWLlink answer, ...). HOW the reasoner is reached is the
			 *					interface, described separately by CConformanceChannel (file or
			 *					live server). A test case carries both values and
			 *					CConformanceTaskHandlerFactory maps the pair to a handler.
			 *
			 *					This class is the single source of truth for the textual form
			 *					of a task. All other code works with TYPE and VERDICT; strings
			 *					are produced or parsed only at the ontology/report boundary.
			 */
			class CConformanceTask {
				// public types
				public:
					//! Kind of reasoner task a test case asks for.
					enum class TYPE {
						CONSISTENCY,			//!< Is the ontology consistent?
						SATISFIABILITY,			//!< Is the requested class satisfiable?
						CLASS_CLASSIFICATION,	//!< Does the computed class taxonomy match the expected one?
						CLASS_REALIZATION,		//!< Do the computed individual types match the expected ones?
						OWLLINK,		//!< Does the OWLlink BooleanResponse match the expectation?
						SPARQL			//!< Does the SPARQL ASK result match the expectation?
					};

					//! Boolean answer a task can produce. POSITIVE means
					//! consistent/satisfiable/true/match and NEGATIVE means
					//! inconsistent/unsatisfiable/false/mismatch, depending on the task.
					enum class VERDICT {
						UNSET,					//!< No answer available (error, timeout or not specified).
						POSITIVE,
						NEGATIVE
					};

				// public methods
				public:
					//! All known task kinds, in the order used by the reports.
					static QList<TYPE> allTypes();

					//! Canonical lower-case name of a task (e.g. "consistency",
					//! "owllink", "sparql"), as used in control ontologies and reports.
					static QString typeToString(TYPE type);

					//! Parses a canonical task name. Returns false for unknown names.
					static bool typeFromString(const QString& text, TYPE& type);

					//! Name of the control ontology class that declares the task
					//! (e.g. "kt:ConsistencyTest"); empty when the task has no marker class.
					static QString typeToTestClassName(TYPE type);

					//! Maps a control ontology marker class (full IRI or abbreviatedIRI,
					//! e.g. "kt:SatisfiabilityTest") to a task kind. Returns false when the
					//! class is not a task marker.
					static bool typeFromTestClassName(const QString& className, TYPE& type);

					//! Renders a verdict in the vocabulary of the task ("consistent",
					//! "satisfiable", "true", "match", ...). Returns an empty string for UNSET.
					static QString verdictToString(TYPE type, VERDICT verdict);

					//! Parses a verdict literal from a control ontology. Returns false when
					//! the literal is not valid for the task.
					static bool verdictFromString(TYPE type, const QString& text, VERDICT& verdict);

					//! Returns the local part of an IRI or abbreviated IRI
					//! ("kt:ConsistencyTest" and "http://...#ConsistencyTest" both yield
					//! "ConsistencyTest").
					static QString localName(const QString& iri);
			};

		}; // end namespace Conformance

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_CONFORMANCE_CCONFORMANCETASK_H
