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

				mTopDataPropExpression = mOntoBuild->getTopDataPropertyExpression();
				mBottomDataPropExpression = mOntoBuild->getBottomDataPropertyExpression();


				mClassTermConceptHash = mOntoData->getExpressionDataBoxMapping()->getClassTermConceptMappingHash();
				mConceptClassTermHash = mOntoData->getExpressionDataBoxMapping()->getConceptClassTermMappingHash();

				mObjPropTermRoleHash = mOntoData->getExpressionDataBoxMapping()->getObjectPropertyTermRoleMappingHash();
				mRoleObjPropTermHash = mOntoData->getExpressionDataBoxMapping()->getRoleObjectPropertyTermMappingHash();

				mDataPropTermRoleHash = mOntoData->getExpressionDataBoxMapping()->getDataPropertyTermRoleMappingHash();
				mRoleDataPropTermHash = mOntoData->getExpressionDataBoxMapping()->getRoleDataPropertyTermMappingHash();

				mDataPropTermRoleHash = mOntoData->getExpressionDataBoxMapping()->getDataPropertyTermRoleMappingHash();
				mRoleDataPropTermHash = mOntoData->getExpressionDataBoxMapping()->getRoleDataPropertyTermMappingHash();

				mIndividulTermIndiHash = mOntoData->getExpressionDataBoxMapping()->getIndividulTermIndiMappingHash();
				mIndiIndividulTermHash = mOntoData->getExpressionDataBoxMapping()->getIndiIndividulTermMappingHash();

				mExpressionBuildContainerList = mOntoBuild->getExpressionBuildListContainer();
				mInverseObjectPropertyHash = mOntoBuild->getInverseObjectPropertyHash();


				mExpressionBuildHash = mOntoBuild->getStructuralExpressionBuildHash();

				mClassBuildHash = mOntoBuild->getClassEntityBuildHash();
				mObjectPropertyBuildHash = mOntoBuild->getObjectPropertyEntityBuildHash();
				mIndividualBuildHash = mOntoBuild->getIndividualEntityBuildHash();
				mDataPropertyBuildHash = mOntoBuild->getDataPropertyEntityBuildHash();

				mAbbreviatedNamePrefixMapHash = mOntoStrings->getAbbreviatedNamePrefixHash();
				mNamePrefixMapHash = mOntoStrings->getNamePrefixHash();

				mDatatypeExpDatatypeHash = mOntoData->getExpressionDataBoxMapping()->getDatatypeExpressionDatatypeHash();
				mDatatypeDatatypeExpHash = mOntoData->getExpressionDataBoxMapping()->getDatatypeDatatypeExpessionHash();

				mDatatypeBuildHash = mOntoBuild->getDatatypeIRIBuildHash();

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



			bool CConcreteOntologyBasementBuilder::createDatatype(const QString& datatypeIRI) {
				if (!mDatatypeBuildHash->contains(datatypeIRI)) {
					CDatatypeExpression* datatypeExpression = new CDatatypeExpression(datatypeIRI);
					datatypeExpression->setEntityID(mOntoBuild->getNextEntityNumber(true));
					mDatatypeBuildHash->insert(datatypeIRI,datatypeExpression);
					CDatatype* datatype = new CDatatype();
					datatype->initDatatype(datatypeIRI);
					mDatatypeExpDatatypeHash->insert(datatypeExpression,datatype);
					mDatatypeDatatypeExpHash->insert(datatype,datatypeExpression);
					CDatatypeVector* datatypeVector = tBox->getDatatypeVector(true);
					datatype->setDatatypeTag(datatypeVector->getItemCount());
					datatypeVector->setData(datatype->getDatatypeTag(),datatype);
				}
				return true;
			}


			bool CConcreteOntologyBasementBuilder::buildOntologyBasement() {
				// generate top and bottom class and concept
				addNameAbbreviation(PREFIX_OWL,"owl");
				addNameAbbreviation(PREFIX_RDF,"rdf");
				addNameAbbreviation(PREFIX_XML,"xml");
				addNameAbbreviation(PREFIX_XSD,"xsd");
				addNameAbbreviation(PREFIX_RDFS,"rdfs");

				QString topConceptString = PREFIX_OWL_THING;
				QString bottomConceptString = PREFIX_OWL_NOTHING;

				QString topObjectRoleString = PREFIX_OWL_TOPOBJECTPROPERTY;
				QString bottomObjectRoleString = PREFIX_OWL_BOTTOMOBJECTPROPERTY;

				QString topDataRoleString = PREFIX_OWL_TOPDATAPROPERTY;
				QString bottomDataRoleString = PREFIX_OWL_BOTTOMDATAPROPERTY;

				mActiveEntityCountVector->increaseVectorSize(mOntoBuild->getNextEntityNumber(false)+6+35);
				if (!mTopClassExpression) {
					CClassExpression* topClassExpression = new CClassExpression(topConceptString);
					topClassExpression->setEntityID(mOntoBuild->getNextEntityNumber(true));
					mActiveEntityCountVector->incActiveEntityCount(topClassExpression);
					mExpressionBuildContainerList->append(topClassExpression);
					mClassBuildHash->insert(topConceptString,topClassExpression);
					mTopClassExpression = topClassExpression;
					mOntoBuild->setTopClassExpression(mTopClassExpression);
				}
				if (!mBottomClassExpression) {
					CClassExpression* bottomClassExpression = new CClassExpression(bottomConceptString);
					bottomClassExpression->setEntityID(mOntoBuild->getNextEntityNumber(true));
					mActiveEntityCountVector->incActiveEntityCount(bottomClassExpression);
					mExpressionBuildContainerList->append(bottomClassExpression);
					mClassBuildHash->insert(bottomConceptString,bottomClassExpression);
					mBottomClassExpression = bottomClassExpression;
					mOntoBuild->setBottomClassExpression(mBottomClassExpression);
				}
				if (!mTopObjPropExpression) {
					CObjectPropertyExpression* topObjPropExpression = new CObjectPropertyExpression(topObjectRoleString);
					topObjPropExpression->setEntityID(mOntoBuild->getNextEntityNumber(true));
					mActiveEntityCountVector->incActiveEntityCount(topObjPropExpression);
					mExpressionBuildContainerList->append(topObjPropExpression);
					mObjectPropertyBuildHash->insert(topObjectRoleString,topObjPropExpression);
					mTopObjPropExpression = topObjPropExpression;
					mOntoBuild->setTopObjectPropertyExpression(mTopObjPropExpression);
				}
				if (!mBottomObjPropExpression) {
					CObjectPropertyExpression* bottomObjPropExpression = new CObjectPropertyExpression(bottomObjectRoleString);
					bottomObjPropExpression->setEntityID(mOntoBuild->getNextEntityNumber(true));
					mActiveEntityCountVector->incActiveEntityCount(bottomObjPropExpression);
					mExpressionBuildContainerList->append(bottomObjPropExpression);
					mObjectPropertyBuildHash->insert(bottomObjectRoleString,bottomObjPropExpression);
					mBottomObjPropExpression = bottomObjPropExpression;
					mOntoBuild->setBottomObjectPropertyExpression(mTopObjPropExpression);
				}

				if (!mTopDataPropExpression) {
					CDataPropertyExpression* topDataPropExpression = new CDataPropertyExpression(topDataRoleString);
					topDataPropExpression->setEntityID(mOntoBuild->getNextEntityNumber(true));
					mActiveEntityCountVector->incActiveEntityCount(topDataPropExpression);
					mExpressionBuildContainerList->append(topDataPropExpression);
					mDataPropertyBuildHash->insert(topDataRoleString,topDataPropExpression);
					mTopDataPropExpression = topDataPropExpression;
					mOntoBuild->setTopDataPropertyExpression(mTopDataPropExpression);
				}
				if (!mBottomDataPropExpression) {
					CDataPropertyExpression* bottomDataPropExpression = new CDataPropertyExpression(bottomDataRoleString);
					bottomDataPropExpression->setEntityID(mOntoBuild->getNextEntityNumber(true));
					mActiveEntityCountVector->incActiveEntityCount(bottomDataPropExpression);
					mExpressionBuildContainerList->append(bottomDataPropExpression);
					mDataPropertyBuildHash->insert(bottomDataRoleString,bottomDataPropExpression);
					mBottomDataPropExpression = bottomDataPropExpression;
					mOntoBuild->setBottomDataPropertyExpression(mBottomDataPropExpression);
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
					tBox->setTopConcept(topConcept);
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
					tBox->setBottomConcept(bottomConcept);
				}
				CConcept* indiTriggerConcept = tBox->getIndividualTriggerConcept();
				if (!indiTriggerConcept) {
					indiTriggerConcept = CObjectAllocator<CConcept>::allocateAndConstruct(tBoxMemMan);
					indiTriggerConcept->initConcept();
					indiTriggerConcept->setConceptTag(tBox->getIndividualTriggerConceptIndex());
					indiTriggerConcept->setOperatorCode(CCIMPLTRIG);
					tBox->setIndividualTriggerConcept(indiTriggerConcept);
				}
				CConcept* topDataRangeConcept = tBox->getTopDataRangeConcept();
				if (!topDataRangeConcept) {
					topDataRangeConcept = CObjectAllocator<CConcept>::allocateAndConstruct(tBoxMemMan);
					topDataRangeConcept->initConcept();
					topDataRangeConcept->setConceptTag(tBox->getTopDataRangeConceptIndex());
					topDataRangeConcept->setOperatorCode(CCIMPLTRIG);
					tBox->setTopDataRangeConcept(topDataRangeConcept);
				}

				CMemoryAllocationManager* rBoxMemMan = CContext::getMemoryAllocationManager(rBox->getBoxContext());
				CRole* topObjectRole = rBox->getTopObjectRole();
				if (!topObjectRole) {
					topObjectRole = CObjectAllocator<CRole>::allocateAndConstruct(tBoxMemMan);
					topObjectRole->initRole();
					topObjectRole->setRoleTag(rBox->getTopObjectRoleIndex());
					updateName(topObjectRole,topObjectRoleString);
					mObjPropTermRoleHash->insert(mTopObjPropExpression,topObjectRole);
					mRoleObjPropTermHash->insert(topObjectRole,mTopObjPropExpression);
					rBox->setTopObjectRole(topObjectRole);
				}
				CRole* bottomObjectRole = rBox->getBottomObjectRole();
				if (!bottomObjectRole) {
					bottomObjectRole = CObjectAllocator<CRole>::allocateAndConstruct(tBoxMemMan);
					bottomObjectRole->initRole();
					bottomObjectRole->setRoleTag(rBox->getBottomObjectRoleIndex());
					updateName(bottomObjectRole,bottomObjectRoleString);
					mObjPropTermRoleHash->insert(mBottomObjPropExpression,bottomObjectRole);
					mRoleObjPropTermHash->insert(bottomObjectRole,mBottomObjPropExpression);
					rBox->setBottomObjectRole(bottomObjectRole);
				}
				CRole* topDataRole = rBox->getTopDataRole();
				if (!topDataRole) {
					topDataRole = CObjectAllocator<CRole>::allocateAndConstruct(tBoxMemMan);
					topDataRole->initRole();
					topDataRole->setDataRole(true);
					topDataRole->setRoleTag(rBox->getTopDataRoleIndex());
					updateName(topDataRole,topDataRoleString);
					mDataPropTermRoleHash->insert(mTopDataPropExpression,topDataRole);
					mRoleDataPropTermHash->insert(topDataRole,mTopDataPropExpression);
					rBox->setTopDataRole(topDataRole);
				}
				CRole* bottomDataRole = rBox->getBottomDataRole();
				if (!bottomDataRole) {
					bottomDataRole = CObjectAllocator<CRole>::allocateAndConstruct(tBoxMemMan);
					bottomDataRole->initRole();
					bottomDataRole->setDataRole(true);
					bottomDataRole->setRoleTag(rBox->getBottomDataRoleIndex());
					updateName(bottomDataRole,bottomDataRoleString);
					mDataPropTermRoleHash->insert(mBottomDataPropExpression,bottomDataRole);
					mRoleDataPropTermHash->insert(bottomDataRole,mBottomDataPropExpression);
					rBox->setBottomDataRole(bottomDataRole);
				}



				createDatatype(PREFIX_OWL_TOP_DATATYPE);
				createDatatype(PREFIX_OWL_BOTTOM_DATATYPE);
				createDatatype(PREFIX_OWL_REAL_DATATYPE);
				createDatatype(PREFIX_OWL_RATIONAL_DATATYPE);
				createDatatype(PREFIX_XML_DECIMAL_DATATYPE);
				createDatatype(PREFIX_XML_INTEGER_DATATYPE);

				createDatatype(PREFIX_XML_NONNEGATIVEINTEGER_DATATYPE);
				createDatatype(PREFIX_XML_POSITIVEINTEGER_DATATYPE);
				createDatatype(PREFIX_XML_NONPOSITIVEINTEGER_DATATYPE);
				createDatatype(PREFIX_XML_NEGATIVEINTEGER_DATATYPE);
				createDatatype(PREFIX_XML_LONG_DATATYPE);
				createDatatype(PREFIX_XML_INT_DATATYPE);
				createDatatype(PREFIX_XML_SHORT_DATATYPE);
				createDatatype(PREFIX_XML_BYTE_DATATYPE);
				createDatatype(PREFIX_XML_UNSIGNEDLONG_DATATYPE);
				createDatatype(PREFIX_XML_UNSIGNEDINT_DATATYPE);
				createDatatype(PREFIX_XML_UNSIGNEDSHORT_DATATYPE);
				createDatatype(PREFIX_XML_UNSIGNEDBYTE_DATATYPE);


				createDatatype(PREFIX_XML_PLAINLITERAL_DATATYPE);
				createDatatype(PREFIX_XML_STRING_DATATYPE);
				createDatatype(PREFIX_XML_NORMALIZEDSTRING_DATATYPE);
				createDatatype(PREFIX_XML_TOKEN_DATATYPE);
				createDatatype(PREFIX_XML_NAME_DATATYPE);
				createDatatype(PREFIX_XML_NCNAME_DATATYPE);
				createDatatype(PREFIX_XML_NMTOKEN_DATATYPE);
				createDatatype(PREFIX_XML_LANGUAGE_DATATYPE);


				createDatatype(PREFIX_XML_BOOLEAN_DATATYPE);


				createDatatype(PREFIX_XML_DOUBLE_DATATYPE);
				createDatatype(PREFIX_XML_FLOAT_DATATYPE);


				createDatatype(PREFIX_XML_BINARYHEX_DATATYPE);
				createDatatype(PREFIX_XML_BINARYBASE64_DATATYPE);


				createDatatype(PREFIX_XML_IRI_DATATYPE);


				createDatatype(PREFIX_XML_DATETIME_DATATYPE);
				createDatatype(PREFIX_XML_DATETIMESTAMP_DATATYPE);


				createDatatype(PREFIX_XML_XML_DATATYPE);


				CDatatypeExpression* topDatatypeExpression = mDatatypeBuildHash->value(CStringRefStringHasher(PREFIX_OWL_TOP_DATATYPE));
				mOntoBuild->setTopDataRangeExpression(topDatatypeExpression);

				CDatatypeExpression* bottomDatatypeExpression = mDatatypeBuildHash->value(CStringRefStringHasher(PREFIX_OWL_BOTTOM_DATATYPE));
				mOntoBuild->setBottomDataRangeExpression(bottomDatatypeExpression);
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