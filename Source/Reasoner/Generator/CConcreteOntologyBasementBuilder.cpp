/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#include "CConcreteOntologyBasementBuilder.h"


namespace Konclude {

	namespace Reasoner {

		namespace Generator {

			CConcreteOntologyBasementBuilder::CConcreteOntologyBasementBuilder(CConcreteOntology* basementBuildConcreteOntology) {
				LOG(NOTICE,"::Konclude::Reasoner::Generator::ConcreteOntologyBasementBuilder",logTr("ConcreteOntologyBasementBuilder initialized."),this);
				mOnto = basementBuildConcreteOntology;
			}

			CConcreteOntologyBasementBuilder::~CConcreteOntologyBasementBuilder() {
			}





			bool CConcreteOntologyBasementBuilder::initializeBuilding() {
				mOntoStrings = mOnto->getStringMapping();
				mOntoStrings = mOnto->getStringMapping();
				mOntoData = mOnto->getDataBoxes();
				mOntoBuild = mOnto->getBuildData();

				tBox = mOntoData->getTBox();
				rBox = mOntoData->getRBox();
				aBox = mOntoData->getABox();

				mOntoData->setBasicBuild(true);

				mMemManager = CContext::getMemoryAllocationManager(mOnto->getOntologyContext());

				mTellAxiomSet = mOntoBuild->getTellAxiomSet();
				mRetractAxiomSet = mOntoBuild->getRetractAxiomSet();

				mTellUpdatedAxiomSet = mOntoBuild->getUpdatedTellAxiomSet();
				mRetractUpdatedAxiomSet = mOntoBuild->getUpdatedRetractAxiomSet();

				mUpdateClassAxiomHash = mOntoData->getExpressionDataBoxMapping()->getUpdatedClassAxiomTellOrRetractHash();
				mUpdateObjectPropertyAxiomHash = mOntoData->getExpressionDataBoxMapping()->getUpdateObjectPropertyAxiomHash();

				mClassTermClassAxiomSet = mOntoData->getExpressionDataBoxMapping()->getClassTermExpressionClassAxiomExpressionSet();
				mClassTermClassAxiomHash = mOntoData->getExpressionDataBoxMapping()->getClassTermExpressionClassAxiomExpressionHash();

				mObjPropTermObjPropAxiomSet = mOntoData->getExpressionDataBoxMapping()->getObjectPropertyTermObjectPropertyAxiomSet();
				mObjPropTermObjPropAxiomHash = mOntoData->getExpressionDataBoxMapping()->getObjectPropertyTermObjectPropertyAxiomHash();

				mTopRebuildClassTermExpressionsSet = mOntoData->getExpressionDataBoxMapping()->getTopRebuildClassTermExpressionsSet();
				mTopRebuildObjectPropertyTermExpressionsSet = mOntoData->getExpressionDataBoxMapping()->getTopRebuildObjectPropertyTermExpressionsSet();


				mActiveEntityCountVector = mOntoData->getExpressionDataBoxMapping()->getActiveEntityVector();


				mTopClassExpression = mOntoBuild->getTopClassExpression();
				mBottomClassExpression = mOntoBuild->getBottomClassExpression();
				mTopObjPropExpression = mOntoBuild->getTopObjectPropertyExpression();
				mBottomObjPropExpression = mOntoBuild->getBottomObjectPropertyExpression();


				mClassTermConceptHash = mOntoData->getExpressionDataBoxMapping()->getClassTermConceptMappingHash();
				mConceptClassTermHash = mOntoData->getExpressionDataBoxMapping()->getConceptClassTermMappingHash();

				mObjPropTermRoleHash = mOntoData->getExpressionDataBoxMapping()->getObjectPropertyTermRoleMappingHash();
				mRoleObjPropTermHash = mOntoData->getExpressionDataBoxMapping()->getRoleObjectPropertyTermMappingHash();

				mIndividulTermIndiHash = mOntoData->getExpressionDataBoxMapping()->getIndividulTermIndiMappingHash();
				mIndiIndividulTermHash = mOntoData->getExpressionDataBoxMapping()->getIndiIndividulTermMappingHash();

				mExpressionBuildContainerList = mOntoBuild->getExpressionBuildListContainer();
				mInverseObjectPropertyHash = mOntoBuild->getInverseObjectPropertyHash();


				mExpressionBuildHash = mOntoBuild->getStructuralExpressionBuildHash();

				mClassBuildHash = mOntoBuild->getClassEntityBuildHash();
				mObjectPropertyBuildHash = mOntoBuild->getObjectPropertyEntityBuildHash();
				mIndividualBuildHash = mOntoBuild->getIndividualEntityBuildHash();

				mAbbreviatedNamePrefixMapHash = mOntoStrings->getAbbreviatedNamePrefixHash();
				mNamePrefixMapHash = mOntoStrings->getNamePrefixHash();

				return true;
			}



			bool CConcreteOntologyBasementBuilder::updateName(CNamedItem* item, const QString& name) {
				bool nameAdded = false;
				if (!CIRIName::hasIRIName(item->getNameLinker(),name)) {
					cint64 lastNameID = CName::getLastNameVersionID(item->getNameLinker());
					if (lastNameID < 0) {
						lastNameID = 0;
					}
					FOREACHHASHIT (QString prefix, CNamePrefix* namePrefix, *mNamePrefixMapHash) {
						if (name.startsWith(prefix)) {
							CAbbreviatedIRIName* newName = CObjectAllocator<CAbbreviatedIRIName>::allocateAndConstruct(mMemManager);
							QString abbString = name.mid(prefix.length());
							newName->init(namePrefix,abbString);
							newName->setNameVersionID(lastNameID);
							CLinker<CName*>* newNameLinker = CObjectAllocator< CLinker<CName*> >::allocateAndConstruct(mMemManager);
							newNameLinker->init(newName);
							item->addNameLinker(newNameLinker);
							nameAdded = true;
						}
					}
					if (!nameAdded) {
						CIRIName* newName = CObjectAllocator<CIRIName>::allocateAndConstruct(mMemManager);
						newName->init(name);
						newName->setNameVersionID(lastNameID);
						CLinker<CName*>* newNameLinker = CObjectAllocator< CLinker<CName*> >::allocateAndConstruct(mMemManager);
						newNameLinker->init(newName);
						item->addNameLinker(newNameLinker);
					}
				}	
				return nameAdded;
			}


			bool CConcreteOntologyBasementBuilder::buildOntologyBasement() {
				// generate top and bottom class and concept
				addNameAbbreviation("http://www.w3.org/2002/07/owl#","owl");

				QString topConceptString = "http://www.w3.org/2002/07/owl#Thing";
				QString bottomConceptString = "http://www.w3.org/2002/07/owl#Nothing";
				QString topRoleString = "http://www.w3.org/2002/07/owl#topObjectProperty";
				QString bottomRoleString = "http://www.w3.org/2002/07/owl#bottomObjectProperty";
				mActiveEntityCountVector->increaseVectorSize(mOntoBuild->getNextEntityNumber(false)+4);
				if (!mTopClassExpression) {
					CClassExpression* topClassExpression = new CClassExpression(topConceptString);
					topClassExpression->setEntityID(mOntoBuild->getNextEntityNumber(true));
					mActiveEntityCountVector->incActiveEntityCount(topClassExpression);
					mExpressionBuildContainerList->append(topClassExpression);
					mClassBuildHash->insert(topConceptString,topClassExpression);
					mTopClassExpression = topClassExpression;
				}
				if (!mBottomClassExpression) {
					CClassExpression* bottomClassExpression = new CClassExpression(bottomConceptString);
					bottomClassExpression->setEntityID(mOntoBuild->getNextEntityNumber(true));
					mActiveEntityCountVector->incActiveEntityCount(bottomClassExpression);
					mExpressionBuildContainerList->append(bottomClassExpression);
					mClassBuildHash->insert(bottomConceptString,bottomClassExpression);
					mBottomClassExpression = bottomClassExpression;
				}
				if (!mTopObjPropExpression) {
					CObjectPropertyExpression* topObjPropExpression = new CObjectPropertyExpression(topRoleString);
					topObjPropExpression->setEntityID(mOntoBuild->getNextEntityNumber(true));
					mActiveEntityCountVector->incActiveEntityCount(topObjPropExpression);
					mExpressionBuildContainerList->append(topObjPropExpression);
					mObjectPropertyBuildHash->insert(topRoleString,topObjPropExpression);
					mTopObjPropExpression = topObjPropExpression;
				}
				if (!mBottomObjPropExpression) {
					CObjectPropertyExpression* bottomObjPropExpression = new CObjectPropertyExpression(bottomRoleString);
					bottomObjPropExpression->setEntityID(mOntoBuild->getNextEntityNumber(true));
					mActiveEntityCountVector->incActiveEntityCount(bottomObjPropExpression);
					mExpressionBuildContainerList->append(bottomObjPropExpression);
					mObjectPropertyBuildHash->insert(bottomRoleString,bottomObjPropExpression);
					mBottomObjPropExpression = bottomObjPropExpression;
				}
				CMemoryAllocationManager* tBoxMemMan = CContext::getMemoryAllocationManager(tBox->getBoxContext());
				CConcept* topConcept = tBox->getTopConcept();
				if (!topConcept) {
					topConcept = CObjectAllocator<CConcept>::allocateAndConstruct(tBoxMemMan);
					topConcept->initConcept();
					topConcept->setConceptTag(tBox->getTopConceptIndex());
					topConcept->setOperatorCode(CCTOP);
					updateName(topConcept,topConceptString);
					mClassTermConceptHash->insert(mTopClassExpression,topConcept);
					mConceptClassTermHash->insert(topConcept,mTopClassExpression);
				}
				CConcept* bottomConcept = tBox->getBottomConcept();
				if (!bottomConcept) {
					bottomConcept = CObjectAllocator<CConcept>::allocateAndConstruct(tBoxMemMan);
					bottomConcept->initConcept();
					bottomConcept->setConceptTag(tBox->getBottomConceptIndex());
					bottomConcept->setOperatorCode(CCBOTTOM);
					updateName(bottomConcept,bottomConceptString);
					mClassTermConceptHash->insert(mBottomClassExpression,bottomConcept);
					mConceptClassTermHash->insert(bottomConcept,mBottomClassExpression);
				}
				CConcept* indiTriggerConcept = tBox->getIndividualTriggerConcept();
				if (!indiTriggerConcept) {
					indiTriggerConcept = CObjectAllocator<CConcept>::allocateAndConstruct(tBoxMemMan);
					indiTriggerConcept->initConcept();
					indiTriggerConcept->setConceptTag(tBox->getIndividualTriggerConceptIndex());
					indiTriggerConcept->setOperatorCode(CCIMPLTRIG);
				}
				tBox->setTopConcept(topConcept);
				tBox->setIndividualTriggerConcept(indiTriggerConcept);
				tBox->setBottomConcept(bottomConcept);


				CMemoryAllocationManager* rBoxMemMan = CContext::getMemoryAllocationManager(rBox->getBoxContext());
				CRole* topRole = rBox->getTopRole();
				if (!topRole) {
					topRole = CObjectAllocator<CRole>::allocateAndConstruct(tBoxMemMan);
					topRole->initRole();
					topRole->setRoleTag(1);
					updateName(topRole,topRoleString);
					mObjPropTermRoleHash->insert(mTopObjPropExpression,topRole);
					mRoleObjPropTermHash->insert(topRole,mTopObjPropExpression);
				}
				CRole* bottomRole = rBox->getBottomRole();
				if (!bottomRole) {
					bottomRole = CObjectAllocator<CRole>::allocateAndConstruct(tBoxMemMan);
					bottomRole->initRole();
					bottomRole->setRoleTag(0);
					updateName(bottomRole,bottomRoleString);
					mObjPropTermRoleHash->insert(mBottomObjPropExpression,bottomRole);
					mRoleObjPropTermHash->insert(bottomRole,mBottomObjPropExpression);
				}
				rBox->setTopRole(topRole);
				rBox->setBottomRole(bottomRole);


				mOntoBuild->setTopClassExpression(mTopClassExpression);
				mOntoBuild->setBottomClassExpression(mBottomClassExpression);
				mOntoBuild->setTopObjectPropertyExpression(mTopObjPropExpression);
				mOntoBuild->setBottomObjectPropertyExpression(mBottomObjPropExpression);
				return true;
			}


			bool CConcreteOntologyBasementBuilder::completeBuilding() {
				return true;
			}



			bool CConcreteOntologyBasementBuilder::addNameAbbreviation(const QString &name, const QString &abbreviatedName) {
				if (!mAbbreviatedNamePrefixMapHash->contains(abbreviatedName)) {
					CNamePrefix* namePrefix = new CNamePrefix(abbreviatedName,name);
					mAbbreviatedNamePrefixMapHash->insert(abbreviatedName,namePrefix);
					mNamePrefixMapHash->insert(name,namePrefix);
					return true;
				} else {
					return false;
				}
			}


		}; // end namespace Generator

	}; // end namespace Reasoner

}; // end namespace Konclude