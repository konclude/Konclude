/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
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

#include "COWLFileAxiomRemover.h"


namespace Konclude {

	namespace Test {



		COWLFileAxiomRemover::COWLFileAxiomRemover(CConfiguration *config) 
					: CLogIdentifier("::Konclude::Test::OWLFileAxiomRemover",this),mConfig(config) {


			mConfReplaceDatatypeExpressions = false;

			QList<QString> removeTagList;
			if (mConfReplaceDatatypeExpressions) {
				removeTagList.append("DataProperty");
				removeTagList.append("SubDataPropertyOf");
				removeTagList.append("EquivalentDataProperties");
				removeTagList.append("DisjointDataProperties");
				removeTagList.append("DataPropertyDomain");
				removeTagList.append("DataPropertyRange");
				removeTagList.append("FunctionalDataProperty");
				removeTagList.append("DataAllValuesFrom");
				removeTagList.append("DataMaxCardinality");
				removeTagList.append("DataPropertyAssertion");
				removeTagList.append("DatatypeDefinition");
				removeTagList.append("DatatypeRestriction");
				removeTagList.append("Datatype");
				removeTagList.append("HasKey");
				removeTagList.append("DataSomeValuesFrom");
				removeTagList.append("DataMinCardinality");
				removeTagList.append("DataExactCardinality");
				removeTagList.append("DataHasValue");
				removeTagList.append("NegativeDataPropertyAssertion");
			}



			foreach (QString removeTagString, removeTagList) {
				mRemoveTagNameSet.insert(removeTagString);
				mRemoveTagNameSet.insert(QString("owl:")+removeTagString);
			}

			QList<QString> alwRemoveTagList;
			alwRemoveTagList.append("AnnotationAssertion");
			alwRemoveTagList.append("SubAnnotationPropertyOf");
			alwRemoveTagList.append("AnnotationPropertyDomain");
			alwRemoveTagList.append("AnnotationPropertyRange");
			alwRemoveTagList.append("Annotation");
			alwRemoveTagList.append("AnnotationProperty");
			alwRemoveTagList.append("Import");

			foreach (QString removeTagString, alwRemoveTagList) {
				mRemoveTagNameSet.insert(removeTagString);
				mRemoveTagNameSet.insert(QString("owl:")+removeTagString);

				mAlwaysRemoveTagNameSet.insert(removeTagString);
				mAlwaysRemoveTagNameSet.insert(QString("owl:")+removeTagString);
			}

			QList<QString> removeAttributeList;
			removeAttributeList.append("versionIRI");

			foreach (QString removeAttributeString, removeAttributeList) {
				mRemoveAttributeNameSet.insert(removeAttributeString);
			}


		}


		COWLFileAxiomRemover::~COWLFileAxiomRemover() {
		}




		bool COWLFileAxiomRemover::loadRemoveAxiomsAndSave(const QString& requestFileName, const QString& responseFileName) {
			mStatRemovedAxioms = 0;
			mStatReplacedExpressions = 0;

			QFile file(requestFileName);
			if (file.open(QIODevice::ReadOnly)) {
				QDomDocument document;
				document.setContent(&file,false);
				file.close();
				QDomElement rootEl = document.documentElement();

				removeElements(rootEl,true,document);
				removeAttributes(rootEl,document);

				QFile wFile(responseFileName);
				if (wFile.open(QIODevice::WriteOnly)) {
					wFile.write(document.toByteArray());
					wFile.close();
				}
				LOG(INFO,getLogDomain(),logTr("Removed %1 axioms and replaced %2 expressions with TOP in '%3'.").arg(mStatRemovedAxioms).arg(mStatReplacedExpressions).arg(requestFileName),this);
				return true;

			} else {
				LOG(INFO,getLogDomain(),logTr("File '%1' does not exist or is not readable.").arg(requestFileName),this);
			}
			return false;
		}

		void COWLFileAxiomRemover::removeAttributes(QDomElement& domElement, QDomDocument& document) {
			foreach (QString attString, mRemoveAttributeNameSet) {
				domElement.removeAttribute(attString);
			}
			QDomElement nextEl = domElement.firstChildElement();
			while (!nextEl.isNull()) {
				removeAttributes(nextEl,document);
				nextEl = nextEl.nextSiblingElement();
			}
		}

		bool COWLFileAxiomRemover::removeElements(QDomElement& domElement, bool axiomLevel, QDomDocument& document) {
			bool modified = false;
			QDomElement nextEl = domElement.firstChildElement();
			while (!nextEl.isNull()) {
				QDomElement tmpElNode = nextEl;
				nextEl = nextEl.nextSiblingElement();

				QString nodeName = tmpElNode.nodeName();
				if (mRemoveTagNameSet.contains(nodeName)) {
					if (axiomLevel) {
						domElement.removeChild(tmpElNode);
						++mStatRemovedAxioms;
					} else if (mAlwaysRemoveTagNameSet.contains(nodeName)) {
						domElement.removeChild(tmpElNode);
						++mStatRemovedAxioms;
					} else {
						// replace with top element
						QDomElement topElement = document.createElement("Class");
						topElement.setAttribute("IRI","http://www.w3.org/2002/07/owl#Thing");
						domElement.removeChild(tmpElNode);
						domElement.appendChild(topElement);
						++mStatReplacedExpressions;
					}
					modified = true;
				} else {

					if (nodeName == "Declaration" || nodeName == "owl:Declaration") {
						removeElements(tmpElNode,true,document);
						if (tmpElNode.childNodes().isEmpty()) {
							// remove this node too
							domElement.removeChild(tmpElNode);
							++mStatRemovedAxioms;
						}
					}

					modified |= removeElements(tmpElNode,false,document);
				}
			}
			return modified;
		}


	}; // end namespace Test

}; // end namespace Konclude
