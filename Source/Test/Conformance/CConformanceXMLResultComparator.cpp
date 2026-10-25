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

#include "CConformanceXMLResultComparator.h"

#include <QFile>
#include <QFileInfo>


namespace Konclude {

	namespace Test {

		namespace Conformance {


			bool CConformanceXMLResultComparator::compareXMLFiles(const QString& actualFile, const QString& expectedFile, QString& detailMessage) {
				detailMessage.clear();

				QDomDocument actualDocument;
				if (!loadDocument(actualFile, actualDocument, detailMessage)) {
					detailMessage = QString("actual result: %1").arg(detailMessage);
					return false;
				}
				QDomDocument expectedDocument;
				if (!loadDocument(expectedFile, expectedDocument, detailMessage)) {
					detailMessage = QString("expected result: %1").arg(detailMessage);
					return false;
				}

				QDomElement actualRoot = actualDocument.documentElement();
				QDomElement expectedRoot = expectedDocument.documentElement();
				if (actualRoot.isNull() || expectedRoot.isNull()) {
					detailMessage = QString("empty XML document");
					return false;
				}

				if (canonicalElement(actualRoot) == canonicalElement(expectedRoot)) {
					return true;
				}

				compareElements(actualRoot, expectedRoot, QString("/") + elementName(expectedRoot), detailMessage);
				if (detailMessage.isEmpty()) {
					detailMessage = QString("answer differs from expected file '%1'").arg(QFileInfo(expectedFile).fileName());
				}
				return false;
			}


			bool CConformanceXMLResultComparator::loadDocument(const QString& filePath, QDomDocument& document, QString& detailMessage) {
				QFile file(filePath);
				if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
					detailMessage = QString("cannot read '%1'").arg(QFileInfo(filePath).fileName());
					return false;
				}
				QString parseError;
				int errorLine = 0;
				int errorColumn = 0;
				bool parsed = document.setContent(&file, &parseError, &errorLine, &errorColumn);
				file.close();
				if (!parsed) {
					detailMessage = QString("cannot parse '%1' (%2 at line %3)").arg(QFileInfo(filePath).fileName()).arg(parseError).arg(errorLine);
					return false;
				}
				return true;
			}


			bool CConformanceXMLResultComparator::compareElements(const QDomElement& actualElement, const QDomElement& expectedElement, const QString& path, QString& detailMessage) {
				QString actualName = elementName(actualElement);
				QString expectedName = elementName(expectedElement);
				if (actualName != expectedName) {
					detailMessage = QString("%1: expected element '%2', got '%3'").arg(path).arg(expectedName).arg(actualName);
					return false;
				}

				QMap<QString, QString> actualAttributes = relevantAttributes(actualElement);
				QMap<QString, QString> expectedAttributes = relevantAttributes(expectedElement);
				for (auto expectedIt = expectedAttributes.constBegin(); expectedIt != expectedAttributes.constEnd(); ++expectedIt) {
					if (!actualAttributes.contains(expectedIt.key())) {
						detailMessage = QString("%1: missing attribute '%2'").arg(path).arg(expectedIt.key());
						return false;
					}
					if (actualAttributes.value(expectedIt.key()) != expectedIt.value()) {
						detailMessage = QString("%1: attribute '%2' expected '%3', got '%4'")
								.arg(path).arg(expectedIt.key()).arg(expectedIt.value()).arg(actualAttributes.value(expectedIt.key()));
						return false;
					}
				}
				for (auto actualIt = actualAttributes.constBegin(); actualIt != actualAttributes.constEnd(); ++actualIt) {
					if (!expectedAttributes.contains(actualIt.key())) {
						detailMessage = QString("%1: unexpected attribute '%2'").arg(path).arg(actualIt.key());
						return false;
					}
				}

				QList<QDomElement> actualChildren = childElements(actualElement);
				QList<QDomElement> expectedChildren = childElements(expectedElement);

				if (isUnorderedContainer(expectedName)) {
					QStringList actualEntries;
					QStringList expectedEntries;
					for (const QDomElement& child : actualChildren) {
						actualEntries << canonicalElement(child);
					}
					for (const QDomElement& child : expectedChildren) {
						expectedEntries << canonicalElement(child);
					}
					actualEntries.sort();
					expectedEntries.sort();
					if (actualEntries != expectedEntries) {
						for (const QString& expectedEntry : expectedEntries) {
							if (!actualEntries.contains(expectedEntry)) {
								detailMessage = QString("%1: missing entry '%2'").arg(path).arg(expectedEntry);
								return false;
							}
						}
						detailMessage = QString("%1: unexpected entry '%2'").arg(path).arg(actualEntries.first());
						return false;
					}
					return true;
				}

				if (actualChildren.count() != expectedChildren.count()) {
					detailMessage = QString("%1: expected %2 child element(s), got %3")
							.arg(path).arg(expectedChildren.count()).arg(actualChildren.count());
					return false;
				}
				for (int i = 0; i < expectedChildren.count(); ++i) {
					QString childPath = path + "/" + elementName(expectedChildren.at(i));
					if (!compareElements(actualChildren.at(i), expectedChildren.at(i), childPath, detailMessage)) {
						return false;
					}
				}

				QString actualText = directText(actualElement);
				QString expectedText = directText(expectedElement);
				if (actualText != expectedText) {
					detailMessage = QString("%1: expected text '%2', got '%3'").arg(path).arg(expectedText).arg(actualText);
					return false;
				}
				return true;
			}


			QString CConformanceXMLResultComparator::canonicalElement(const QDomElement& element) {
				QString name = elementName(element);
				QString canonical = name;

				QMap<QString, QString> attributes = relevantAttributes(element);
				for (auto attributeIt = attributes.constBegin(); attributeIt != attributes.constEnd(); ++attributeIt) {
					canonical += QString("[%1=%2]").arg(attributeIt.key()).arg(attributeIt.value());
				}

				QList<QDomElement> children = childElements(element);
				if (!children.isEmpty()) {
					QStringList childEntries;
					for (const QDomElement& child : children) {
						childEntries << canonicalElement(child);
					}
					if (isUnorderedContainer(name)) {
						childEntries.sort();
					}
					canonical += QString("{%1}").arg(childEntries.join("|"));
				} else {
					QString text = directText(element);
					if (!text.isEmpty()) {
						canonical += QString("=%1").arg(text);
					}
				}
				return canonical;
			}


			QString CConformanceXMLResultComparator::elementName(const QDomElement& element) {
				QString localName = element.localName();
				if (!localName.isEmpty()) {
					return localName;
				}
				QString tagName = element.tagName();
				int colonIndex = tagName.lastIndexOf(':');
				return colonIndex >= 0 ? tagName.mid(colonIndex + 1) : tagName;
			}


			bool CConformanceXMLResultComparator::isUnorderedContainer(const QString& elementName) {
				// SPARQL result rows and OWLlink set members have no meaningful order.
				return elementName == "results" || elementName.startsWith("SetOf");
			}


			bool CConformanceXMLResultComparator::isIgnoredAttribute(const QString& attributeName) {
				return attributeName == "xml:base"
						|| attributeName == "response-time"
						|| attributeName.startsWith("xmlns");
			}


			QMap<QString, QString> CConformanceXMLResultComparator::relevantAttributes(const QDomElement& element) {
				QMap<QString, QString> attributes;
				QDomNamedNodeMap attributeMap = element.attributes();
				for (int i = 0; i < attributeMap.count(); ++i) {
					QDomAttr attribute = attributeMap.item(i).toAttr();
					if (!attribute.isNull() && !isIgnoredAttribute(attribute.name())) {
						attributes.insert(attribute.name(), attribute.value());
					}
				}
				return attributes;
			}


			QString CConformanceXMLResultComparator::directText(const QDomElement& element) {
				QString text;
				for (QDomNode node = element.firstChild(); !node.isNull(); node = node.nextSibling()) {
					if (node.isText() || node.isCDATASection()) {
						text += node.nodeValue();
					}
				}
				return text.trimmed();
			}


			QList<QDomElement> CConformanceXMLResultComparator::childElements(const QDomElement& element) {
				QList<QDomElement> children;
				for (QDomElement child = element.firstChildElement(); !child.isNull(); child = child.nextSiblingElement()) {
					children.append(child);
				}
				return children;
			}

		}; // end namespace Conformance

	}; // end namespace Test

}; // end namespace Konclude

