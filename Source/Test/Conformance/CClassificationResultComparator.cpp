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

#include "CClassificationResultComparator.h"

#include <QStringList>
#include <QSet>
#include <QList>
#include <QPair>


namespace Konclude {

	namespace Test {

		namespace Conformance {


			QString CClassificationResultComparator::resolveClassIRI(const QDomElement& classElement, const QHash<QString, QString>& prefixIRIHash) {
				QString iri = classElement.attribute("IRI");
				if (!iri.isEmpty()) {
					return iri;
				}
				iri = classElement.attribute("abbreviatedIRI");
				if (iri.isEmpty()) {
					return QString();
				}
				int colon = iri.indexOf(':');
				if (colon > 0) {
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
					if (prefix == "xml") {
						return QString("http://www.w3.org/XML/1998/namespace#") + local;
					}
				}
				return iri;
			}

			CClassHierarchyResult* CClassificationResultComparator::loadClassificationHierarchy(const QString& filePath) {
				QFile file(filePath);
				const QString comparatorDomain = QString("::Konclude::Test::Conformance::ClassificationResultComparator");
				if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
					LOG(ERROR, comparatorDomain, logTr("Cannot open classification result file '%1'.").arg(filePath), 0);
					return nullptr;
				}
				QDomDocument doc;
				QString errMsg;
				int errLine = 0, errCol = 0;
				if (!doc.setContent(&file, &errMsg, &errLine, &errCol)) {
					file.close();
					LOG(ERROR, comparatorDomain, logTr("Cannot parse classification result file '%1' (%2 at line %3).").arg(filePath).arg(errMsg).arg(errLine), 0);
					return nullptr;
				}
				file.close();

				QDomElement root = doc.documentElement();
				if (root.isNull()) {
					return nullptr;
				}

				// Gather declared prefixes (for expanding abbreviatedIRI values).
				QHash<QString, QString> prefixIRIHash;
				QDomNodeList prefixes = root.elementsByTagName("Prefix");
				for (int i = 0; i < prefixes.count(); ++i) {
					QDomElement pe = prefixes.at(i).toElement();
					QString name = pe.attribute("name");
					QString iri = pe.attribute("IRI");
					if (!name.isEmpty() && !iri.isEmpty()) {
						prefixIRIHash.insert(name, iri);
					}
				}

				// Union-find over equivalent classes (from EquivalentClasses axioms).
				QHash<QString, QString> parentHash;
				auto ensureRoot = [&parentHash](const QString& x) {
					if (!parentHash.contains(x)) {
						parentHash.insert(x, x);
					}
				};
				auto findRep = [&parentHash, &ensureRoot](QString x) -> QString {
					ensureRoot(x);
					QString root = x;
					while (parentHash.value(root, root) != root) {
						root = parentHash.value(root, root);
					}
					while (parentHash.value(x, x) != x) {
						QString nxt = parentHash.value(x, x);
						parentHash.insert(x, root);
						x = nxt;
					}
					return root;
				};

				auto collectClassElementList = [&prefixIRIHash](const QDomElement& parentEl) -> QStringList {
					QStringList classIRIs;
					QDomElement child = parentEl.firstChildElement();
					while (!child.isNull()) {
						if (child.tagName() == "Class") {
							QString resolved = CClassificationResultComparator::resolveClassIRI(child, prefixIRIHash);
							if (!resolved.isEmpty()) {
								classIRIs.append(resolved);
							}
						}
						child = child.nextSiblingElement();
					}
					return classIRIs;
				};

				QSet<QString> allClassIRIs;
				QList<QPair<QString, QString> > subClassOfPairs;

				// EquivalentClasses -> union all members.
				QDomNodeList equivalentList = root.elementsByTagName("EquivalentClasses");
				for (int i = 0; i < equivalentList.count(); ++i) {
					QStringList members = collectClassElementList(equivalentList.at(i).toElement());
					QString firstRep;
					for (const QString& member : members) {
						allClassIRIs.insert(member);
						if (firstRep.isEmpty()) {
							firstRep = findRep(member);
						} else {
							parentHash.insert(findRep(member), firstRep);
						}
					}
				}

				// SubClassOf -> collect (sub, super) class-level pairs.
				QDomNodeList subClassList = root.elementsByTagName("SubClassOf");
				for (int i = 0; i < subClassList.count(); ++i) {
					QStringList classes = collectClassElementList(subClassList.at(i).toElement());
					if (classes.count() >= 2) {
						QString sub = classes.at(0);
						QString superc = classes.at(1);
						allClassIRIs.insert(sub);
						allClassIRIs.insert(superc);
						subClassOfPairs.append(QPair<QString, QString>(sub, superc));
					}
				}

				// Declarations -> ensure all atomic classes are known.
				QDomNodeList declarationList = root.elementsByTagName("Declaration");
				for (int i = 0; i < declarationList.count(); ++i) {
					QDomElement decEl = declarationList.at(i).toElement();
					QDomElement classEl = decEl.firstChildElement("Class");
					if (!classEl.isNull()) {
						QString resolved = resolveClassIRI(classEl, prefixIRIHash);
						if (!resolved.isEmpty()) {
							allClassIRIs.insert(resolved);
						}
					}
				}
				// Group member IRIs by their representative.
				QHash<QString, QStringList> repNamesHash;
				for (const QString& cls : allClassIRIs) {
					repNamesHash[findRep(cls)].append(cls);
				}

				QString nothingIRI = "http://www.w3.org/2002/07/owl#Nothing";
				QString thingIRI = "http://www.w3.org/2002/07/owl#Thing";
				QString bottomRep = findRep(nothingIRI);
				QString topRep = findRep(thingIRI);
				if (!repNamesHash.contains(bottomRep)) {
					repNamesHash.insert(bottomRep, QStringList() << nothingIRI);
				}
				if (!repNamesHash.contains(topRep)) {
					repNamesHash.insert(topRep, QStringList() << thingIRI);
				}

				QStringList bottomNames = repNamesHash.value(bottomRep);
				bottomNames.sort();
				QStringList topNames = repNamesHash.value(topRep);
				topNames.sort();

				CClassSynsetResult* bottomSynset = new CClassSynsetResult(bottomNames);
				CClassSynsetResult* topSynset = new CClassSynsetResult(topNames);
				CClassHierarchyResult* hierarchy = new CClassHierarchyResult(bottomSynset, topSynset);

				QHash<QString, CClassSynsetResult*> repSynsetHash;
				repSynsetHash.insert(bottomRep, bottomSynset);
				repSynsetHash.insert(topRep, topSynset);

				for (const QString& rep : repNamesHash.keys()) {
					if (rep == bottomRep || rep == topRep) {
						continue;
					}
					QStringList names = repNamesHash.value(rep);
					names.sort();
					CClassSynsetResult* synset = new CClassSynsetResult(names);
					hierarchy->addClassSynset(synset);
					repSynsetHash.insert(rep, synset);
				}

				for (const QPair<QString, QString>& pair : subClassOfPairs) {
					CClassSynsetResult* subSynset = repSynsetHash.value(findRep(pair.first));
					CClassSynsetResult* superSynset = repSynsetHash.value(findRep(pair.second));
					if (subSynset && superSynset) {
						hierarchy->addSubClassRelation(superSynset, subSynset);
					}
				}

				return hierarchy;
			}


			bool CClassificationResultComparator::compareClassificationFiles(const QString& actualFile, const QString& expectedFile, QString& detailMessage) {
				detailMessage.clear();
				CClassHierarchyResult* actualHierarchy = loadClassificationHierarchy(actualFile);
				CClassHierarchyResult* expectedHierarchy = loadClassificationHierarchy(expectedFile);
				if (!actualHierarchy || !expectedHierarchy) {
					delete actualHierarchy;
					delete expectedHierarchy;
					detailMessage = "classification result files could not be parsed";
					return false;
				}
				bool equivalent = actualHierarchy->isResultEquivalentTo(expectedHierarchy);
				if (!equivalent) {
					detailMessage = "subsumption hierarchies differ";
				}
				delete actualHierarchy;
				delete expectedHierarchy;
				return equivalent;
			}


		}; // end namespace Conformance

	}; // end namespace Test

}; // end namespace Konclude
