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

#include "CWritePropertySubsumptionsHierarchyQuery.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CWritePropertySubsumptionsHierarchyQuery::CWritePropertySubsumptionsHierarchyQuery(CConcreteOntology *premisingClassificationOntology, CConfigurationBase *configuration, const QString& outputFileString, bool dataProperties, const QString &superPropertyName, const QString &subPropertyHierarchyQueryName)
					: CClassificationPremisingQuery(premisingClassificationOntology,configuration) {
				mPropertyName = superPropertyName;
				mQueryName = subPropertyHierarchyQueryName;
				if (mPropertyName.isEmpty()) {
					mQueryString = QString("Get Subproperty Hierarchy");
				} else {
					mQueryString = QString("Get Subproperty Hierarchy of property '%1'").arg(mPropertyName);
				}

				mUseAbbreviatedIRIs = CConfigDataReader::readConfigBoolean(configuration,"Konclude.CLI.Output.AbbreviatedIRIs",false);
				mWriteDeclarations = CConfigDataReader::readConfigBoolean(configuration,"Konclude.CLI.Output.WriteDeclarations",false);

				mTaxCalcError = false;
				mQueryConstructError = false;

				mBottomPropertyNameString = QString("http://www.w3.org/2002/07/owl#Nothing");
				mTopPropertyNameString = QString("http://www.w3.org/2002/07/owl#Thing");
				mOutputFileNameString = outputFileString;
				mQueryAnswered = false;
				mDataProperties = dataProperties;

				if (mDataProperties) {
					mRequiresDataRoleClassification = true;
				} else {
					mRequiresObjectRoleClassification = true;
				}				 
			}

			CQueryResult *CWritePropertySubsumptionsHierarchyQuery::getQueryResult() {
				return nullptr;
			}

			CWriteQuery::WRITEQUERYTYPE CWritePropertySubsumptionsHierarchyQuery::getWriteQueryType() {
				if (mDataProperties) {
					return CWriteQuery::WRITESUBDATAPROPERTYHIERARCHY;
				} else {
					return CWriteQuery::WRITESUBOBJECTPROPERTYHIERARCHY;
				}
			}


			CQuery* CWritePropertySubsumptionsHierarchyQuery::addQueryError(CQueryError* queryError) {
				if (CQueryInconsitentOntologyError::hasInconsistentOntologyError(queryError)) {
					// write inconsistency
					if (!writeInconsistentHierarchyResult()) {
						addQueryError(new CQueryUnspecifiedStringError(QString("Could not write property hierarchy to file '%1'.").arg(mOutputFileNameString)));
					}
				}
				CQuery::addQueryError(queryError);
				return this;
			}


			bool CWritePropertySubsumptionsHierarchyQuery::writeInconsistentHierarchyResult() {
				if (startWritingOutput()) {

					writeOntologyStart();

					CRole* topRole = mOntology->getRBox()->getTopObjectRole();
					CRole* bottomRole = mOntology->getRBox()->getBottomObjectRole();
					if (mDataProperties) {
						topRole = mOntology->getRBox()->getTopDataRole();
						bottomRole = mOntology->getRBox()->getBottomDataRole();
					}

					CBOXSET<CRole*>* activePropertyRoleSet = mOntology->getRBox()->getActivePropertyRoleSet();


					if (mWriteDeclarations) {
						for (CBOXSET<CRole*>::const_iterator it = activePropertyRoleSet->constBegin(), itEnd = activePropertyRoleSet->constEnd(); it != itEnd; ++it) {
							CRole* role(*it);
							QString propertyName = CIRIName::getRecentIRIName(role->getPropertyNameLinker());
							if (mUseAbbreviatedIRIs) {
								propertyName = CAbbreviatedIRIName::getRecentAbbreviatedPrefixWithAbbreviatedIRIName(role->getPropertyNameLinker());
							}
							writeDeclaration(propertyName);
						}
					}

					QStringList incPropertyNameList;
					for (CBOXSET<CRole*>::const_iterator it = activePropertyRoleSet->constBegin(), itEnd = activePropertyRoleSet->constEnd(); it != itEnd; ++it) {
						CRole* role(*it);
						QString propertyName = CIRIName::getRecentIRIName(role->getPropertyNameLinker());
						if (mUseAbbreviatedIRIs) {
							propertyName = CAbbreviatedIRIName::getRecentAbbreviatedPrefixWithAbbreviatedIRIName(role->getPropertyNameLinker());
						}
						incPropertyNameList.append(propertyName);
					}
					writeEquivalenceRelations(incPropertyNameList);

					writeOntologyEnd();

					return endWritingOutput();
				}
				return false;
			}


			CQueryResult *CWritePropertySubsumptionsHierarchyQuery::constructResult(CClassification* classification) {
				mQueryAnswered = true;
				CPropertyRoleClassification* roleClassification = classification->getObjectPropertyRoleClassification();
				if (mDataProperties) {
					roleClassification = classification->getDataPropertyRoleClassification();
				}
				if (!roleClassification || !writeSubPropertyHierarchyResult(roleClassification->getRolePropertiesHierarchy())) {
					addQueryError(new CQueryUnspecifiedStringError(QString("Could not write class hierarchy to file '%1'.").arg(mOutputFileNameString)));
				}
				return nullptr;
			}

			bool CWritePropertySubsumptionsHierarchyQuery::writeSubPropertyHierarchyResult(CRolePropertiesHierarchy *hierarchy) {
				if (startWritingOutput()) {
					writeOntologyStart();

					CRolePropertiesHierarchyNode* topNode = hierarchy->getTopHierarchyNode();
					CRolePropertiesHierarchyNode* bottomNode = hierarchy->getBottomHierarchyNode();
					CRolePropertiesHierarchyNode* basicNode = topNode;
					if (!mPropertyName.isEmpty()) {
						CMAPPINGHASH<CStringRefStringHasher,CRole*>* propertyNameConceptMappingHash = mOntology->getStringMapping()->getPropertyNameRoleMappingHash(false);
						CRole* role = propertyNameConceptMappingHash->value(mPropertyName);
						basicNode = hierarchy->getHierarchyNode(role);
					}
					writeDeclarations(bottomNode);
					writeBottomEquivalences(bottomNode);
					QSet<CRolePropertiesHierarchyNode *> processNodeSet;
					processNodeSet.insert(bottomNode);
					if (basicNode) {
						QList<CRolePropertiesHierarchyNode *> processNodeList;
						writeDeclarations(basicNode);
						writeTopEquivalences(basicNode);
						processNodeSet.insert(basicNode);
						processNodeList.append(basicNode);
						while (!processNodeList.isEmpty()) {
							CRolePropertiesHierarchyNode *procNode = processNodeList.takeFirst();
							QSet<CRolePropertiesHierarchyNode *>* childNodeSet(procNode->getChildNodeSet());
							foreach (CRolePropertiesHierarchyNode *childNode, *childNodeSet) {

								if (!processNodeSet.contains(childNode)) {
									writeDeclarations(childNode);
									writeEquivalences(childNode);
									processNodeSet.insert(childNode);
									processNodeList.append(childNode);
								}

								if (childNode != bottomNode) {
									writeSubPropertyRelations(procNode,childNode);
								}
							}
						}
					}
					writeOntologyEnd();
					return endWritingOutput();
				}
				return false;
			}

			void CWritePropertySubsumptionsHierarchyQuery::writeBottomEquivalences(CRolePropertiesHierarchyNode* node) {
				if (node->getEquivalentElementCount() > 1) {
					writeEquivalenceRelations(node);
				}
			}

			void CWritePropertySubsumptionsHierarchyQuery::writeTopEquivalences(CRolePropertiesHierarchyNode* node) {
				if (node->getEquivalentElementCount() > 1) {
					writeEquivalenceRelations(node);
				}
			}




			void CWritePropertySubsumptionsHierarchyQuery::writeEquivalenceRelations(CRolePropertiesHierarchyNode* node) {
				const QStringList& propertyNameList(node->getEquivalentRoleStringList(mUseAbbreviatedIRIs));
				writeEquivalenceRelations(propertyNameList);
			}



			void CWritePropertySubsumptionsHierarchyQuery::writeEquivalences(CRolePropertiesHierarchyNode* node) {
				if (node->getEquivalentElementCount() > 1) {
					writeEquivalenceRelations(node);
				}
			}


			void CWritePropertySubsumptionsHierarchyQuery::writeDeclarations(CRolePropertiesHierarchyNode* node) {
				if (mWriteDeclarations) {
					const QStringList& propertyNameList(node->getEquivalentRoleStringList(mUseAbbreviatedIRIs));
					foreach (const QString& propertyName, propertyNameList) {
						writeDeclaration(propertyName);
					}
				}
			}




			void CWritePropertySubsumptionsHierarchyQuery::writeSubPropertyRelations(CRolePropertiesHierarchyNode* superNode, CRolePropertiesHierarchyNode* subNode) {
				const QStringList& superPropertyNameList(superNode->getEquivalentRoleStringList(mUseAbbreviatedIRIs));
				const QStringList& subPropertyNameList(subNode->getEquivalentRoleStringList(mUseAbbreviatedIRIs));

				writeSubPropertyRelation(subPropertyNameList.first(),superPropertyNameList.first());
			}




			QString CWritePropertySubsumptionsHierarchyQuery::getQueryName() {
				return mQueryName;
			}

			QString CWritePropertySubsumptionsHierarchyQuery::getQueryString() {
				return mQueryString;
			}

			bool CWritePropertySubsumptionsHierarchyQuery::hasAnswer() {
				return mQueryAnswered;
			}

			QString CWritePropertySubsumptionsHierarchyQuery::getAnswerString() {
				return QString("Subproperty Hierarchy written to file '%1'").arg(mOutputFileNameString);
			}


			bool CWritePropertySubsumptionsHierarchyQuery::hasError() {
				return mTaxCalcError || mQueryConstructError || CQuery::hasError();
			}

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
