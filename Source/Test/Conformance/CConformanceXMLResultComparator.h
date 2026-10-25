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

#ifndef KONCLUDE_TEST_CONFORMANCE_CCONFORMANCEXMLRESULTCOMPARATOR_H
#define KONCLUDE_TEST_CONFORMANCE_CCONFORMANCEXMLRESULTCOMPARATOR_H

// Libraries includes
#include <QString>
#include <QStringList>
#include <QList>
#include <QMap>
#include <QDomDocument>
#include <QDomElement>


namespace Konclude {

	namespace Test {

		namespace Conformance {

			/*!
			 *		\class		CConformanceXMLResultComparator
			 *		\brief		Compares two XML answer documents semantically instead of
			 *					byte by byte.
			 *
			 *					Element prefixes and namespace declarations are ignored,
			 *					volatile attributes (xml:base, response-time, xmlns…) are
			 *					ignored, attribute order does not matter and text is
			 *					trimmed. Elements that model sets (SPARQL <results>, OWLlink
			 *					SetOf…) are compared order-independently, because neither
			 *					SPARQL result rows nor OWLlink set members have a meaningful
			 *					order.
			 *
			 *					Used for OWLlink and SPARQL tests that declare an expected
			 *					result file (kt:expectedFile).
			 */
			class CConformanceXMLResultComparator {
				// public methods
				public:
					//! Compares the actual answer document with the expected one.
					//! Returns false and fills detailMessage with the first difference.
					static bool compareXMLFiles(const QString& actualFile, const QString& expectedFile, QString& detailMessage);

				// protected methods
				protected:
					static bool loadDocument(const QString& filePath, QDomDocument& document, QString& detailMessage);

					//! Recursive comparison that reports the first difference.
					static bool compareElements(const QDomElement& actualElement, const QDomElement& expectedElement, const QString& path, QString& detailMessage);

					//! Canonical string of a subtree, used for order-independent sets.
					static QString canonicalElement(const QDomElement& element);

					//! Local element name without namespace prefix.
					static QString elementName(const QDomElement& element);

					//! True for elements whose children are a set, not a sequence.
					static bool isUnorderedContainer(const QString& elementName);

					static bool isIgnoredAttribute(const QString& attributeName);
					static QMap<QString, QString> relevantAttributes(const QDomElement& element);
					static QString directText(const QDomElement& element);
					static QList<QDomElement> childElements(const QDomElement& element);
			};

		}; // end namespace Conformance

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_CONFORMANCE_CCONFORMANCEXMLRESULTCOMPARATOR_H
