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

#ifndef KONCLUDE_TEST_CONFORMANCE_CCONFORMANCEOWLXMLRESULTWRITER_H
#define KONCLUDE_TEST_CONFORMANCE_CCONFORMANCEOWLXMLRESULTWRITER_H

// Libraries includes
#include <QTextStream>

// Namespace includes
#include "CConformanceResultWriter.h"


namespace Konclude {

	namespace Test {

		namespace Conformance {

			/*!
			 *		\class		CConformanceOWLXMLResultWriter
			 *		\brief		Writes the conformance results as an OWL/XML ontology: one
			 *					kt:TestResult individual per executed test, linked to the test
			 *					individual via kt:resultOf and annotated with the outcome,
			 *					duration, expected/actual verdict, explanation and output log
			 *					file.
			 */
			class CConformanceOWLXMLResultWriter : public CConformanceResultWriter {
				// public methods
				public:
					CConformanceOWLXMLResultWriter();
					virtual ~CConformanceOWLXMLResultWriter();

					virtual bool writeResults(const QString& filePath, const QList<CConformanceTestResult>& results);

				// protected methods
				protected:
					//! Writes one kt:TestResult individual for result.
					void writeResult(QTextStream& stream, const CConformanceTestResult& result, int resultIndex);

					//! Writes a NamedIndividual reference, choosing IRI or abbreviatedIRI
					//! depending on the shape of the given identifier.
					void writeIndividual(QTextStream& stream, const QString& individualIRI);

					//! Writes an ObjectPropertyAssertion between two named individuals.
					void writeObjectPropertyAssertion(QTextStream& stream, const QString& objectPropertyIRI, const QString& subjectIRI, const QString& objectIRI);

					//! Writes a DataPropertyAssertion with a plain literal value.
					void writeLiteralAssertion(QTextStream& stream, const QString& dataPropertyIRI, const QString& subjectIRI, const QString& value);
			};

		}; // end namespace Conformance

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_CONFORMANCE_CCONFORMANCEOWLXMLRESULTWRITER_H
