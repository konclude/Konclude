/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify it under
 *		the terms of version 2.1 of the GNU Lesser General Public License (LGPL2.1)
 *		as published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details, see GNU Lesser General Public License.
 *
 */

#include "COWLPigeonholePrincipleGenerator.h"


namespace Konclude {

	namespace Test {



		COWLPigeonholePrincipleGenerator::COWLPigeonholePrincipleGenerator() : CLogIdentifier("::Konclude::Test::OWLPigeonholePrincipleGenerator",this) {
			owllinkNS = "http://www.owllink.org/owllink#";
			owlNS = "http://www.w3.org/2002/07/owl#";
		}


		COWLPigeonholePrincipleGenerator::~COWLPigeonholePrincipleGenerator() {
		}



		void COWLPigeonholePrincipleGenerator::generatePigeonholePrinciple(cint64 size, const QString& filename) {

			QFile file(filename);
			if (file.open(QIODevice::WriteOnly)) {


				rootNode = document.createElementNS(owllinkNS,"RequestMessage");
				rootNode.setAttributeNS("http://www.w3.org/2001/XMLSchema-instance","xsi:schemaLocation","http://www.owllink.org/owllink# http://www.owllink.org/owllink-20091116.xsd");
				rootNode.setAttribute("xmlns:owl",owlNS);
				document.appendChild(rootNode);


				QDomElement createOntologyNode = document.createElement("CreateKB");
				createOntologyNode.setAttribute(QString("kb"),QString("http://www.owllink.org/testsuite/PigeonholePrinciple-%1").arg(size));
				rootNode.appendChild(createOntologyNode);

				QDomElement tellOntologyNode = document.createElement("Tell");
				tellOntologyNode.setAttribute(QString("kb"),QString("http://www.owllink.org/testsuite/PigeonholePrinciple-%1").arg(size));
				rootNode.appendChild(tellOntologyNode);



				cint64 pigeonSize = std::log((double)(size+1))/std::log((double)10)+1;
				QStringList pigeonStringList;
				for (cint64 i = 0; i <= size; ++i) {
					QString pigeonString = QString::number(i);
					while (pigeonString.count() < pigeonSize) {
						pigeonString.prepend("0");
					}
					pigeonString = QString("P%1").arg(pigeonString);
					pigeonStringList.append(pigeonString);
				}
				QStringList holeStringList;
				for (cint64 i = 0; i < size; ++i) {
					QString holeString = QString::number(i);
					while (holeString.count() < pigeonSize) {
						holeString.prepend("0");
					}
					holeString = QString("H%1").arg(holeString);
					holeStringList.append(holeString);
				}

				QString satTestClass = QString("CSAT");
				tellOntologyNode.appendChild(getOWLClassDeclaration(satTestClass));


				QList<QDomElement> clauselList;
				// generate step 1
				foreach (const QString& pigeonString, pigeonStringList) {
					QList<QDomElement> atLeastOneHoleList;
					foreach (const QString& holeString, holeStringList) {
						QString ph = pigeonString+holeString;
						tellOntologyNode.appendChild(getOWLClassDeclaration(ph));
						atLeastOneHoleList.append(getOWLClass(ph));
					}
					clauselList.append(getOWLUnion(atLeastOneHoleList));
				}
				// generate step 2
				foreach (const QString& holeString, holeStringList) {
					for (QStringList::const_iterator it = pigeonStringList.constBegin(), itEnd = pigeonStringList.constEnd(); it != itEnd; ++it) {
						QStringList::const_iterator it2 = it;
						for (++it2; it2 != itEnd; ++it2) {
							QString ph1 = *it+holeString;
							QString ph2 = *it2+holeString;
							clauselList.append(getOWLUnion(getOWLComplementClass(ph1),getOWLComplementClass(ph2)));
						}
					}
				}

				tellOntologyNode.appendChild(getOWLEquivalentClass(getOWLClass("CSAT"),getOWLIntersection(clauselList)));


				QDomElement queryOntologyNode = document.createElement("IsClassSatisfiable");
				queryOntologyNode.setAttribute(QString("kb"),QString("http://www.owllink.org/testsuite/PigeonholePrinciple-%1").arg(size));
				queryOntologyNode.setAttribute(QString("timing"),QString("simple"));
				queryOntologyNode.appendChild(getOWLClass("CSAT"));

				QDomElement expectedResultNode = document.createElement("ExpectedResult");
				QDomElement boolResultNode = document.createElement("BooleanResponse");
				boolResultNode.setAttribute("result","false");
				expectedResultNode.appendChild(boolResultNode);
				queryOntologyNode.appendChild(expectedResultNode);

				rootNode.appendChild(queryOntologyNode);

				file.write(document.toByteArray());

				file.close();
			}


		}


		QDomElement COWLPigeonholePrincipleGenerator::getOWLUnion(const QList<QDomElement>& list) {
			QDomElement dmDl = document.createElement("owl:ObjectUnionOf");
			foreach (QDomElement dmEl, list) {
				dmDl.appendChild(dmEl);
			}
			return dmDl;
		}


		QDomElement COWLPigeonholePrincipleGenerator::getOWLUnion(const QDomElement& u1, const QDomElement& u2) {
			QDomElement dmDl = document.createElement("owl:ObjectUnionOf");
			dmDl.appendChild(u1);
			dmDl.appendChild(u2);
			return dmDl;
		}


		QDomElement COWLPigeonholePrincipleGenerator::getOWLIntersection(const QList<QDomElement>& list) {
			QDomElement dmDl = document.createElement("owl:ObjectIntersectionOf");
			foreach (QDomElement dmEl, list) {
				dmDl.appendChild(dmEl);
			}
			return dmDl;
		}


		QDomElement COWLPigeonholePrincipleGenerator::getOWLClass(const QString& className, bool negated) {
			if (negated) {
				return getOWLComplementClass(className);
			} else {
				return getOWLClass(className);
			}
		}


		QDomElement COWLPigeonholePrincipleGenerator::getOWLComplementClass(const QString& className) {
			QDomElement dmDl = document.createElement("owl:ObjectComplementOf");
			dmDl.appendChild(getOWLClass(className));
			return dmDl;
		}


		QDomElement COWLPigeonholePrincipleGenerator::getOWLClassDeclaration(const QString& className) {
			QDomElement dmDl = document.createElement("owl:Declaration");
			dmDl.appendChild(getOWLClass(className));
			return dmDl;
		}

		QDomElement COWLPigeonholePrincipleGenerator::getOWLClass(const QString& className) {
			QDomElement dmEl = document.createElement("owl:Class");
			dmEl.setAttribute("IRI",className);
			return dmEl;
		}


		QDomElement COWLPigeonholePrincipleGenerator::getOWLEquivalentClass(const QDomElement& eq1, const QDomElement& eq2) {
			QDomElement dmEl = document.createElement("owl:EquivalentClasses");
			dmEl.appendChild(eq1);
			dmEl.appendChild(eq2);
			return dmEl;
		}




	}; // end namespace Test

}; // end namespace Konclude
