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

#include "COWLlinkAPIReasonerTestfileTester.h"


namespace Konclude {

	namespace Test {



		COWLlinkAPIReasonerTestfileTester::COWLlinkAPIReasonerTestfileTester(CConfiguration *config) 
					: CLogIdentifier("::Konclude::Test::OWLlinkAPIReasonerTestfileTester",this),mConfig(config) {


			// 2 weeks timeout
			mTransManager = new CQtHttpTransactionManager(1000*60*60*24*14);

			mAddressString = CConfigDataReader::readConfigString(mConfig,"Konclude.OWLlink.OWLlinkAPI.Address","http://127.0.0.1:8080");

			mWriteResponseStdOut = CConfigDataReader::readConfigBoolean(mConfig,"Konclude.OWLlink.WriteResponseToStandardOutput",true);
			mModifyRequest = CConfigDataReader::readConfigBoolean(mConfig,"Konclude.OWLlink.OWLlinkAPI.RequestFileCompatibleModification",true);
			mModifyResponseTiming = CConfigDataReader::readConfigBoolean(mConfig,"Konclude.OWLlink.OWLlinkAPI.ResponseFileTimeExtension",true);
			mModifyRequestAbsolutePaths = CConfigDataReader::readConfigBoolean(mConfig,"Konclude.OWLlink.OWLlinkAPI.RequestFileAbsolutePathsModification",true);
			mExpectedResult = nullptr;
		}


		COWLlinkAPIReasonerTestfileTester::~COWLlinkAPIReasonerTestfileTester() {
			delete mTransManager;
		}




		void COWLlinkAPIReasonerTestfileTester::startTestfileTest(const QString& requestFileName, const QString& responseFileName) {

			QFile file(requestFileName);
			if (file.open(QIODevice::ReadOnly)) {
				QDomDocument document;
				document.setContent(&file,false);
				file.close();
				QDomElement rootEl = document.documentElement();
				if (mModifyRequest) {
					modifyRequestDomStructure(rootEl);
				}
				QByteArray transReqByteArray(document.toByteArray());

				CHttpRequest* webRequest = mTransManager->createRequest(mAddressString,transReqByteArray);
				QTime timing;
				timing.start();
				CHttpResponse* webResponse = mTransManager->getResponse(webRequest);
				LOG(INFO,getLogDomain(),logTr("Sending OWLlink request from file '%1' to '%2'.").arg(requestFileName).arg(mAddressString),this);

				QString* siteText = nullptr;
				siteText = mTransManager->getExtractedText(webResponse);
				cint64 elapsedTime = timing.elapsed();

				if (siteText && !siteText->isEmpty()) {
					LOG(INFO,getLogDomain(),logTr("Recieved OWLlink response in '%1' ms.").arg(elapsedTime),this);
					document.setContent(*siteText,false);
					rootEl = document.documentElement();
					if (mModifyResponseTiming) {						
						modifyResponseDomStructure(rootEl,elapsedTime);
					}

					mTransManager->releaseResponse(webResponse);

					if (mWriteResponseStdOut) {
						QString respString(document.toByteArray());
						cout<<respString.toLocal8Bit().data();
					}


					if (!responseFileName.isEmpty()) {
						QFile wFile(responseFileName);
						if (wFile.open(QIODevice::WriteOnly)) {
							wFile.write(document.toByteArray());
							wFile.close();
							LOG(INFO,getLogDomain(),logTr("Writing OWLlink response to file '%1'.").arg(responseFileName),this);
						} else {
							LOG(INFO,getLogDomain(),logTr("Could not write response to '%1'.").arg(responseFileName),this);
						}
					}
				} else {
					LOG(INFO,getLogDomain(),logTr("Timeout/Error while requesting response from '%1'.").arg(mAddressString),this);
				}
			} else {
				LOG(INFO,getLogDomain(),logTr("File '%1' does not exist or is not readable.").arg(requestFileName),this);
			}		

		}

		void COWLlinkAPIReasonerTestfileTester::modifyResponseDomStructure(QDomElement& el, cint64 respTime) {
			if (el.tagName() == "BooleanResponse") {
				el.setAttribute("response-time",respTime);
			} else if (el.tagName() == "ClassHierarchy") {
				el.setAttribute("response-time",respTime);
				if (mExpectedResult) {
					CClassHierarchyResult* respResult = parseClassHierarchyResultRelocatedTOP(&el);
					if (!mExpectedResult->isResultEquivalentTo(respResult)) {
						LOG(WARNING,getLogDomain(),logTr("Result doesn't match expected result."),this);
						el.setAttribute("warning","Result doesn't match expected result.");
					}
				}
			} else {
				QDomElement child(el.firstChildElement());
				while (!child.isNull()) {
					modifyResponseDomStructure(child,respTime);
					child = child.nextSiblingElement();
				}
			}
		}


		bool COWLlinkAPIReasonerTestfileTester::modifyRequestDomStructure(QDomElement& el) {
			if (el.tagName() == "ExpectedResult") {
				QDomElement classHirNode = el.firstChildElement("ClassHierarchy");
				if (!classHirNode.isNull()) {
					mExpectedResult = parseClassHierarchyResult(&classHirNode);
				}
				return true;
			} else if (mModifyRequestAbsolutePaths && el.tagName() == "OntologyIRI") {
				QString ontoIRI = el.attribute("IRI");
				if (ontoIRI.startsWith("file:") && !ontoIRI.startsWith("file://")) {
					QString relFileString = ontoIRI;
					relFileString = relFileString.remove(0,5);
					QDir currFilePath(QDir::currentPath());
					QString absFileString = currFilePath.absoluteFilePath(relFileString);
					absFileString = QUrl::fromLocalFile(absFileString).toString();
					el.setAttribute("IRI",absFileString);
					LOG(INFO,getLogDomain(),logTr("Changed relative filepath '%1' to absolute filepath '%2'.").arg(ontoIRI).arg(absFileString),this);
				}
			} 
		
			QDomElement child(el.firstChildElement());
			while (!child.isNull()) {
				if (modifyRequestDomStructure(child)) {
					el.removeChild(child);
				}
				child = child.nextSiblingElement();
			}
			return false;
		}


		CClassHierarchyResult* COWLlinkAPIReasonerTestfileTester::parseClassHierarchyResultRelocatedTOP(QDomElement *node) {
			// rename BOTTOM
			QDomElement bottomClassNode = node->firstChildElement("ClassSynset");
			renameAbbreviatedClassSynset(&bottomClassNode,"owl:Nothing","http://www.w3.org/2002/07/owl#Nothing");

			// relocate and rename TOP
			QDomElement subClassNode = node->firstChildElement("ClassSubClassesPair");
			while (!subClassNode.isNull()) {
				QDomElement classSynsetElm(subClassNode.firstChildElement("ClassSynset"));
				if (hasClassSynsetTOP(&classSynsetElm)) {
					renameAbbreviatedClassSynset(&classSynsetElm,"owl:Thing","http://www.w3.org/2002/07/owl#Thing");
					node->removeChild(subClassNode);
					node->insertAfter(subClassNode,bottomClassNode);
					break;
				}
				subClassNode = subClassNode.nextSiblingElement("ClassSubClassesPair");
			}

			return parseClassHierarchyResult(node);
		}

		bool COWLlinkAPIReasonerTestfileTester::hasClassSynsetTOP(QDomElement *node) {
			QDomElement classNode = node->firstChildElement("owl:Class");
			while (!classNode.isNull()) {
				QString className = classNode.attribute("abbreviatedIRI");
				if (!className.isEmpty() && className == "owl:Thing") {
					return true;
				}
				classNode = classNode.nextSiblingElement("owl:Class");
			}
			return false;
		}


		bool COWLlinkAPIReasonerTestfileTester::renameAbbreviatedClassSynset(QDomElement *node, const QString& abbIRI, const QString& replaceIRI) {
			QDomElement classNode = node->firstChildElement("owl:Class");
			while (!classNode.isNull()) {
				QString className = classNode.attribute("abbreviatedIRI");
				if (!className.isEmpty() && className == abbIRI) {
					classNode.removeAttribute("abbreviatedIRI");
					classNode.setAttribute("IRI",replaceIRI);
					return true;
				}
				classNode = classNode.nextSiblingElement("owl:Class");
			}
			return false;
		}



		CClassHierarchyResult* COWLlinkAPIReasonerTestfileTester::parseClassHierarchyResult(QDomElement *node) {
			CClassHierarchyResult *classHierResult = 0;
			QDomElement bottomClassNode = node->firstChildElement("ClassSynset");
			CClassSynsetResult *bottomClassSynset = parseClassSynsetResult(&bottomClassNode);
			CClassSynsetResult *topClassSynset = 0;
			QDomElement subClassNode = node->firstChildElement("ClassSubClassesPair");
			while (!subClassNode.isNull()) {
				CClassSynsetResult *superClass = 0;
				if (!classHierResult) {
					QDomElement classSynsetElm(subClassNode.firstChildElement("ClassSynset"));
					topClassSynset = parseClassSynsetResult(&classSynsetElm);
					classHierResult = new CClassHierarchyResult(bottomClassSynset,topClassSynset);
					superClass = topClassSynset;
				} else {
					QDomElement classSynsetElm(subClassNode.firstChildElement("ClassSynset"));
					QStringList classNames = parseClassNames(&classSynsetElm);
					superClass = classHierResult->getClassSynset(classNames);
				}

				QDomElement subClassSynsetNode = subClassNode.firstChildElement("SubClassSynsets");
				while (!subClassSynsetNode.isNull()) {
					QDomElement classSynsetNode = subClassSynsetNode.firstChildElement("ClassSynset");
					while (!classSynsetNode.isNull()) {
						QStringList classNames = parseClassNames(&classSynsetNode);
						CClassSynsetResult *subClass = classHierResult->getClassSynset(classNames);
						classHierResult->addSubClassRelation(superClass,subClass);
						classSynsetNode = classSynsetNode.nextSiblingElement("ClassSynset");
					}
					subClassSynsetNode = subClassSynsetNode.nextSiblingElement("SubClassSynsets");
				}

				subClassNode = subClassNode.nextSiblingElement("ClassSubClassesPair");
			}
			return classHierResult;
		}


		CClassSynsetResult *COWLlinkAPIReasonerTestfileTester::parseClassSynsetResult(QDomElement *node) {
			QStringList classNameList = parseClassNames(node);
			CClassSynsetResult *classSynset = new CClassSynsetResult(classNameList);
			return classSynset;
		}

		QStringList COWLlinkAPIReasonerTestfileTester::parseClassNames(QDomElement *node) {
			QStringList classNameList;
			bool err = node->isNull();

			QString tagName = node->tagName();

			QDomElement classNode = node->firstChildElement("owl:Class");
			while (!classNode.isNull()) {
				QString className = classNode.attribute("IRI");
				classNameList.append(className);
				classNode = classNode.nextSiblingElement("owl:Class");
			}
			return classNameList;
		}

	}; // end namespace Test

}; // end namespace Konclude
