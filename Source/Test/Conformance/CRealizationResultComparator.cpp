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

#include "CRealizationResultComparator.h"

#include <QFile>
#include <QFileInfo>
#include <QDomDocument>
#include <QStringList>


namespace Konclude {

	namespace Test {

		namespace Conformance {


			QString CRealizationResultComparator::resolveIRI(const QString& iriValue, const QHash<QString, QString>& prefixIRIHash) {
				QString iri = iriValue.trimmed();
				if (iri.isEmpty()) {
					return QString();
				}
				int colon = iri.indexOf(':');
				if (colon > 0 && !iri.contains("://")) {
					QString prefix = iri.left(colon);
					QString local = iri.mid(colon + 1);
					if (prefixIRIHash.contains(prefix)) {
						return prefixIRIHash.value(prefix) + local;
					}
					if (prefix == "owl") {
						return QString("http://www.w3.org/2002/07/owl#") + local;
					}
					if (prefix == "rdf") {
						return QString("http://www.w3.org/1999/02/22-rdf-syntax-ns#") + local;
					}
					if (prefix == "rdfs") {
						return QString("http://www.w3.org/2000/01/rdf-schema#") + local;
					}
					if (prefix == "xsd") {
						return QString("http://www.w3.org/2001/XMLSchema#") + local;
					}
				}
				return iri;
			}


			bool CRealizationResultComparator::loadTypeAssertions(const QString& filePath, QSet<QString>& typeAssertions, QString& detailMessage) {
				QFile file(filePath);
				const QString comparatorDomain = QString("::Konclude::Test::Conformance::RealizationResultComparator");
				if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
					LOG(ERROR, comparatorDomain, logTr("Cannot open realization result file '%1'.").arg(filePath), 0);
					detailMessage = QString("cannot open realization result file '%1'").arg(QFileInfo(filePath).fileName());
					return false;
				}
				QDomDocument doc;
				QString errMsg;
				int errLine = 0, errCol = 0;
				if (!doc.setContent(&file, &errMsg, &errLine, &errCol)) {
					file.close();
					LOG(ERROR, comparatorDomain, logTr("Cannot parse realization result file '%1' (%2 at line %3).").arg(filePath).arg(errMsg).arg(errLine), 0);
					detailMessage = QString("cannot parse realization result file '%1'").arg(QFileInfo(filePath).fileName());
					return false;
				}
				file.close();

				QDomElement root = doc.documentElement();
				if (root.isNull()) {
					detailMessage = QString("realization result file '%1' has no root element").arg(QFileInfo(filePath).fileName());
					return false;
				}

				QHash<QString, QString> prefixIRIHash;
				QDomNodeList prefixes = root.elementsByTagName("Prefix");
				for (int i = 0; i < prefixes.count(); ++i) {
					QDomElement prefixElement = prefixes.at(i).toElement();
					QString name = prefixElement.attribute("name");
					QString iri = prefixElement.attribute("IRI");
					if (!name.isEmpty() && !iri.isEmpty()) {
						prefixIRIHash.insert(name, iri);
					}
				}

				QDomNodeList classAssertions = root.elementsByTagName("ClassAssertion");
				for (int i = 0; i < classAssertions.count(); ++i) {
					QDomElement assertion = classAssertions.at(i).toElement();
					QDomElement classElement = assertion.firstChildElement("Class");
					QDomElement individualElement = assertion.firstChildElement("NamedIndividual");
					if (classElement.isNull() || individualElement.isNull()) {
						continue;
					}
					QString classIRI = resolveIRI(classElement.attribute("IRI"), prefixIRIHash);
					if (classIRI.isEmpty()) {
						classIRI = resolveIRI(classElement.attribute("abbreviatedIRI"), prefixIRIHash);
					}
					QString individualIRI = resolveIRI(individualElement.attribute("IRI"), prefixIRIHash);
					if (individualIRI.isEmpty()) {
						individualIRI = resolveIRI(individualElement.attribute("abbreviatedIRI"), prefixIRIHash);
					}
					if (!classIRI.isEmpty() && !individualIRI.isEmpty()) {
						typeAssertions.insert(individualIRI + "\n" + classIRI);
					}
				}
				return true;
			}


			bool CRealizationResultComparator::compareRealizationFiles(const QString& actualFile, const QString& expectedFile, QString& detailMessage) {
				detailMessage.clear();
				QSet<QString> actualAssertions;
				QSet<QString> expectedAssertions;
				if (!loadTypeAssertions(actualFile, actualAssertions, detailMessage)) {
					return false;
				}
				if (!loadTypeAssertions(expectedFile, expectedAssertions, detailMessage)) {
					return false;
				}
				if (actualAssertions == expectedAssertions) {
					return true;
				}

				QSet<QString> missing = expectedAssertions - actualAssertions;
				QSet<QString> unexpected = actualAssertions - expectedAssertions;
				QStringList details;
				details << QString("%1 missing, %2 unexpected type assertion(s)").arg(missing.count()).arg(unexpected.count());
				for (const QString& assertion : missing) {
					QStringList parts = assertion.split('\n');
					details << QString("missing: %1 instance of %2").arg(parts.value(0)).arg(parts.value(1));
				}
				for (const QString& assertion : unexpected) {
					QStringList parts = assertion.split('\n');
					details << QString("unexpected: %1 instance of %2").arg(parts.value(0)).arg(parts.value(1));
				}
				detailMessage = details.join("; ");
				return false;
			}


		}; // end namespace Conformance

	}; // end namespace Test

}; // end namespace Konclude
