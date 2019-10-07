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

#include "CExpressionDataBoxMapping.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			CExpressionDataBoxMapping::CExpressionDataBoxMapping(COntologyContext* ontoContext) : mConstructFlags(ontoContext) {
				mOntoContext = ontoContext;
				CMemoryAllocationManager* memAllocMan = CContext::getMemoryAllocationManager(mOntoContext);

				mActiveEntityCountVector = CObjectParameterizingAllocator< CActiveEntityCountVector,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);

				mClassTermConceptHash = CObjectParameterizingAllocator< CBUILDHASH<CClassTermExpression*,CConcept*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);
				mConceptClassTermHash = CObjectParameterizingAllocator< CBUILDHASH<CConcept*,CClassTermExpression*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);
				mDataRangeTermConceptHash = CObjectParameterizingAllocator< CBUILDHASH<CDataRangeTermExpression*,CConcept*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);
				mConceptDataRangeTermHash = CObjectParameterizingAllocator< CBUILDHASH<CConcept*,CDataRangeTermExpression*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);
				mObjPropTermRoleHash = CObjectParameterizingAllocator< CBUILDHASH<CObjectPropertyTermExpression*,CRole*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);
				mRoleObjPropTermHash = CObjectParameterizingAllocator< CBUILDHASH<CRole*,CObjectPropertyTermExpression*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);
				mDataPropTermRoleHash = CObjectParameterizingAllocator< CBUILDHASH<CDataPropertyTermExpression*,CRole*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);
				mRoleDataPropTermHash = CObjectParameterizingAllocator< CBUILDHASH<CRole*,CDataPropertyTermExpression*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);
				mIndividulTermIndiHash = CObjectParameterizingAllocator< CBUILDHASH<CIndividualTermExpression*,CIndividual*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);
				mIndiIndividulTermHash = CObjectParameterizingAllocator< CBUILDHASH<CIndividual*,CIndividualTermExpression*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);

				mDatatypeExpDatatypeHash = CObjectParameterizingAllocator< CBUILDHASH<CDatatypeExpression*,CDatatype*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);
				mDatatypeDatatypeExpHash = CObjectParameterizingAllocator< CBUILDHASH<CDatatype*,CDatatypeExpression*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);

				mLexicalDataExpDatatypePairDataLiteralHash = CObjectParameterizingAllocator< CBUILDHASH<QPair<CDataLexicalValueExpression*, CDatatype*>, CDataLiteral*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);

				mUpdateClassAxiomHash = CObjectParameterizingAllocator< CBUILDHASH<CClassAxiomExpression*,bool>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);
				mUpdateObjectPropertyAxiomHash = CObjectParameterizingAllocator< CBUILDHASH<CObjectPropertyAxiomExpression*,bool>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);
				mUpdateDataPropertyAxiomHash = CObjectParameterizingAllocator< CBUILDHASH<CDataPropertyAxiomExpression*,bool>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);
				mUpdateAssertionAxiomHash = CObjectParameterizingAllocator< CBUILDHASH<CAssertionAxiomExpression*,bool>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);

				mClassTermClassAxiomSet = CObjectParameterizingAllocator< CBUILDSET< QPair<CClassTermExpression*,CClassAxiomExpression*> >,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);
				mClassTermClassAxiomHash = CObjectParameterizingAllocator< CBUILDHASH<CClassTermExpression*,CClassAxiomExpression*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);

				mObjPropTermObjPropAxiomSet = CObjectParameterizingAllocator< CBUILDSET< QPair<CObjectPropertyTermExpression*,CObjectPropertyAxiomExpression*> >,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);
				mObjPropTermObjPropAxiomHash = CObjectParameterizingAllocator< CBUILDHASH<CObjectPropertyTermExpression*,CObjectPropertyAxiomExpression*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);

				mDataPropTermDataPropAxiomSet = CObjectParameterizingAllocator< CBUILDSET< QPair<CDataPropertyTermExpression*,CDataPropertyAxiomExpression*> >,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);
				mDataPropTermDataPropAxiomHash = CObjectParameterizingAllocator< CBUILDHASH<CDataPropertyTermExpression*,CDataPropertyAxiomExpression*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);

				mIndiTermAssertionAxiomSet = CObjectParameterizingAllocator< CBUILDSET< QPair<CIndividualTermExpression*,CAssertionAxiomExpression*> >,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);
				mIndiTermAssertionAxiomHash = CObjectParameterizingAllocator< CBUILDHASH<CIndividualTermExpression*,CAssertionAxiomExpression*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);

				mTopRebuildClassTermExpressionsSet = CObjectParameterizingAllocator< CBUILDSET<CClassTermExpression*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);
				mTopRebuildObjectPropertyTermExpressionsSet = CObjectParameterizingAllocator< CBUILDSET<CObjectPropertyTermExpression*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);

				mObjPropTermRoleChainHash = CObjectParameterizingAllocator< CBUILDHASH<CObjectPropertyAxiomExpression*,CRoleChain*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);
				mRoleChainObjPropTermHash = CObjectParameterizingAllocator< CBUILDHASH<CRoleChain*,CObjectPropertyAxiomExpression*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);

				mBuildedConceptSet = CObjectParameterizingAllocator< CBUILDSET<CClassTermExpression*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);
				mBuildedObjectRoleSet = CObjectParameterizingAllocator< CBUILDSET<CObjectPropertyTermExpression*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);
				mBuildedDataRoleSet = CObjectParameterizingAllocator< CBUILDSET<CDataPropertyTermExpression*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);
				mBuildedDataRangeSet = CObjectParameterizingAllocator< CBUILDSET<CDataRangeTermExpression*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);
				mBuildedDatatypeSet = CObjectParameterizingAllocator< CBUILDSET<CDatatypeExpression*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);
				mBuildedIndividualSet = CObjectParameterizingAllocator< CBUILDSET<CIndividualTermExpression*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);
			
				mBuildedConceptList = CObjectParameterizingAllocator< CBUILDLIST<CClassTermExpression*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);
				mBuildedObjectRoleList = CObjectParameterizingAllocator< CBUILDLIST<CObjectPropertyTermExpression*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);
				mBuildedDataRoleList = CObjectParameterizingAllocator< CBUILDLIST<CDataPropertyTermExpression*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);
				mBuildedDataRangeList = CObjectParameterizingAllocator< CBUILDLIST<CDataRangeTermExpression*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);
				mBuildedDatatypeList = CObjectParameterizingAllocator< CBUILDLIST<CDatatypeExpression*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);
				mBuildedIndividualList = CObjectParameterizingAllocator< CBUILDLIST<CIndividualTermExpression*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);

				mExpressionBuildHash = CObjectParameterizingAllocator< CBUILDHASH<CExpressionHasher,CBuildExpression*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);
				mExpressionBuildListContainer = CObjectParameterizingAllocator< CBUILDLIST<CBuildExpression*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);
				mInverseObjectPropertyHash = CObjectParameterizingAllocator< CBUILDHASH<CObjectPropertyTermExpression*,CObjectPropertyTermExpression*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);

				mLastActiveCountedEntityAxiom = 0;

				mLastProcessedChangedAxiom = 0;
				mLastProcessedBuildIndividual = 0;
				mLastProcessedBuildConcept = 0;
				mLastProcessedBuildObjectRole = 0;
				mLastProcessedBuildDataRole = 0;
				mLastProcessedBuildDataRange = 0;
				mLastProcessedBuildDatatype = 0;

				mLastProcessedInverseProperty = 0;
				mLastProcessedExpression = 0;

				mLastBuildedIndividual = 0;
				mLastBuildedConcept = 0;
				mLastBuildedObjectRole = 0;
				mLastBuildedDataRole = 0;
				mLastBuildedDataRange = 0;
				mLastBuildedDatatype = 0;
			}

			CExpressionDataBoxMapping::~CExpressionDataBoxMapping() {
				CMemoryAllocationManager* memAllocMan = CContext::getMemoryAllocationManager(mOntoContext);
				COPADestroyAndRelease(mClassTermConceptHash,memAllocMan);
				COPADestroyAndRelease(mConceptClassTermHash,memAllocMan);
				COPADestroyAndRelease(mDataRangeTermConceptHash,memAllocMan);
				COPADestroyAndRelease(mConceptDataRangeTermHash,memAllocMan);
				COPADestroyAndRelease(mObjPropTermRoleHash,memAllocMan);
				COPADestroyAndRelease(mRoleObjPropTermHash,memAllocMan);
				COPADestroyAndRelease(mDataPropTermRoleHash,memAllocMan);
				COPADestroyAndRelease(mRoleDataPropTermHash,memAllocMan);
				COPADestroyAndRelease(mIndividulTermIndiHash,memAllocMan);
				COPADestroyAndRelease(mIndiIndividulTermHash,memAllocMan);

				COPADestroyAndRelease(mDatatypeExpDatatypeHash,memAllocMan);
				COPADestroyAndRelease(mDatatypeDatatypeExpHash,memAllocMan);
				COPADestroyAndRelease(mLexicalDataExpDatatypePairDataLiteralHash, memAllocMan);

				COPADestroyAndRelease(mUpdateClassAxiomHash,memAllocMan);
				COPADestroyAndRelease(mUpdateObjectPropertyAxiomHash,memAllocMan);
				COPADestroyAndRelease(mUpdateDataPropertyAxiomHash,memAllocMan);
				COPADestroyAndRelease(mUpdateAssertionAxiomHash,memAllocMan);

				COPADestroyAndRelease(mObjPropTermObjPropAxiomSet,memAllocMan);
				COPADestroyAndRelease(mObjPropTermObjPropAxiomHash,memAllocMan);

				COPADestroyAndRelease(mDataPropTermDataPropAxiomSet,memAllocMan);
				COPADestroyAndRelease(mDataPropTermDataPropAxiomHash,memAllocMan);

				COPADestroyAndRelease(mIndiTermAssertionAxiomSet,memAllocMan);
				COPADestroyAndRelease(mIndiTermAssertionAxiomHash,memAllocMan);

				COPADestroyAndRelease(mTopRebuildClassTermExpressionsSet,memAllocMan);
				COPADestroyAndRelease(mTopRebuildObjectPropertyTermExpressionsSet,memAllocMan);

				COPADestroyAndRelease(mClassTermClassAxiomSet,memAllocMan);
				COPADestroyAndRelease(mClassTermClassAxiomHash,memAllocMan);

				COPADestroyAndRelease(mObjPropTermRoleChainHash,memAllocMan);
				COPADestroyAndRelease(mRoleChainObjPropTermHash,memAllocMan);

				COPADestroyAndRelease(mBuildedConceptSet,memAllocMan);
				COPADestroyAndRelease(mBuildedObjectRoleSet,memAllocMan);
				COPADestroyAndRelease(mBuildedDataRoleSet,memAllocMan);
				COPADestroyAndRelease(mBuildedDataRangeSet,memAllocMan);
				COPADestroyAndRelease(mBuildedDatatypeSet,memAllocMan);
				COPADestroyAndRelease(mBuildedIndividualSet,memAllocMan);

				COPADestroyAndRelease(mBuildedIndividualList,memAllocMan);
				COPADestroyAndRelease(mBuildedObjectRoleList,memAllocMan);
				COPADestroyAndRelease(mBuildedDataRoleList,memAllocMan);
				COPADestroyAndRelease(mBuildedDataRangeList,memAllocMan);
				COPADestroyAndRelease(mBuildedDatatypeList,memAllocMan);
				COPADestroyAndRelease(mBuildedConceptList,memAllocMan);

				COPADestroyAndRelease(mExpressionBuildHash,memAllocMan);
				COPADestroyAndRelease(mExpressionBuildListContainer,memAllocMan);
				COPADestroyAndRelease(mInverseObjectPropertyHash,memAllocMan);
			}

			CExpressionDataBoxMapping* CExpressionDataBoxMapping::referenceDataBoxMapping(CExpressionDataBoxMapping* dataBoxMapping) {
				*mClassTermConceptHash = *dataBoxMapping->mClassTermConceptHash;
				*mConceptClassTermHash = *dataBoxMapping->mConceptClassTermHash;
				*mDataRangeTermConceptHash = *dataBoxMapping->mDataRangeTermConceptHash;
				*mConceptDataRangeTermHash = *dataBoxMapping->mConceptDataRangeTermHash;
				*mObjPropTermRoleHash = *dataBoxMapping->mObjPropTermRoleHash;
				*mRoleObjPropTermHash = *dataBoxMapping->mRoleObjPropTermHash;
				*mDataPropTermRoleHash = *dataBoxMapping->mDataPropTermRoleHash;
				*mRoleDataPropTermHash = *dataBoxMapping->mRoleDataPropTermHash;
				*mIndividulTermIndiHash = *dataBoxMapping->mIndividulTermIndiHash;
				*mIndiIndividulTermHash = *dataBoxMapping->mIndiIndividulTermHash;

				*mDatatypeExpDatatypeHash = *dataBoxMapping->mDatatypeExpDatatypeHash;
				*mDatatypeDatatypeExpHash = *dataBoxMapping->mDatatypeDatatypeExpHash;
				*mLexicalDataExpDatatypePairDataLiteralHash = *dataBoxMapping->mLexicalDataExpDatatypePairDataLiteralHash;

				mActiveEntityCountVector->initActiveEntityCountVector(dataBoxMapping->mActiveEntityCountVector);

				mUpdateClassAxiomHash->clear();
				mUpdateObjectPropertyAxiomHash->clear();
				mUpdateDataPropertyAxiomHash->clear();
				mUpdateAssertionAxiomHash->clear();

				//*mUpdateClassAxiomHash = *dataBoxMapping->mUpdateClassAxiomHash;
				//*mUpdateObjectPropertyAxiomHash = *dataBoxMapping->mUpdateObjectPropertyAxiomHash;

				*mClassTermClassAxiomSet = *dataBoxMapping->mClassTermClassAxiomSet;
				*mClassTermClassAxiomHash = *dataBoxMapping->mClassTermClassAxiomHash;


				*mObjPropTermObjPropAxiomSet = *dataBoxMapping->mObjPropTermObjPropAxiomSet;
				*mObjPropTermObjPropAxiomHash = *dataBoxMapping->mObjPropTermObjPropAxiomHash;

				*mDataPropTermDataPropAxiomSet = *dataBoxMapping->mDataPropTermDataPropAxiomSet;
				*mDataPropTermDataPropAxiomHash = *dataBoxMapping->mDataPropTermDataPropAxiomHash;

				*mIndiTermAssertionAxiomSet = *dataBoxMapping->mIndiTermAssertionAxiomSet;
				*mIndiTermAssertionAxiomHash = *dataBoxMapping->mIndiTermAssertionAxiomHash;

				*mTopRebuildClassTermExpressionsSet = *dataBoxMapping->mTopRebuildClassTermExpressionsSet;
				*mTopRebuildObjectPropertyTermExpressionsSet = *dataBoxMapping->mTopRebuildObjectPropertyTermExpressionsSet;

				*mObjPropTermRoleChainHash = *dataBoxMapping->mObjPropTermRoleChainHash;
				*mRoleChainObjPropTermHash = *dataBoxMapping->mRoleChainObjPropTermHash;

				mLastActiveCountedEntityAxiom = dataBoxMapping->mLastActiveCountedEntityAxiom;

				mLastProcessedChangedAxiom = dataBoxMapping->mLastProcessedChangedAxiom;
				mLastProcessedBuildIndividual = dataBoxMapping->mLastProcessedBuildIndividual;
				mLastProcessedBuildConcept = dataBoxMapping->mLastProcessedBuildConcept;
				mLastProcessedBuildObjectRole = dataBoxMapping->mLastProcessedBuildObjectRole;
				mLastProcessedBuildDataRole = dataBoxMapping->mLastProcessedBuildDataRole;
				mLastProcessedBuildDataRange = dataBoxMapping->mLastProcessedBuildDataRange;
				mLastProcessedBuildDatatype = dataBoxMapping->mLastProcessedBuildDatatype;

				mLastProcessedInverseProperty = dataBoxMapping->mLastProcessedInverseProperty;
				mLastProcessedExpression = dataBoxMapping->mLastProcessedExpression;

				mLastBuildedIndividual = dataBoxMapping->mLastBuildedIndividual;
				mLastBuildedConcept = dataBoxMapping->mLastBuildedConcept;
				mLastBuildedObjectRole = dataBoxMapping->mLastBuildedObjectRole;
				mLastBuildedDataRole = dataBoxMapping->mLastBuildedDataRole;
				mLastBuildedDataRange = dataBoxMapping->mLastBuildedDataRange;
				mLastBuildedDatatype = dataBoxMapping->mLastBuildedDatatype;

				*mBuildedIndividualList = *dataBoxMapping->mBuildedIndividualList;
				*mBuildedConceptList = *dataBoxMapping->mBuildedConceptList;
				*mBuildedObjectRoleList = *dataBoxMapping->mBuildedObjectRoleList;
				*mBuildedDataRoleList = *dataBoxMapping->mBuildedDataRoleList;
				*mBuildedDataRangeList = *dataBoxMapping->mBuildedDataRangeList;
				*mBuildedDatatypeList = *dataBoxMapping->mBuildedDatatypeList;

				*mBuildedIndividualSet = *dataBoxMapping->mBuildedIndividualSet;
				*mBuildedObjectRoleSet = *dataBoxMapping->mBuildedObjectRoleSet;
				*mBuildedDataRoleSet = *dataBoxMapping->mBuildedDataRoleSet;
				*mBuildedDatatypeSet = *dataBoxMapping->mBuildedDatatypeSet;
				*mBuildedDataRangeSet = *dataBoxMapping->mBuildedDataRangeSet;
				*mBuildedConceptSet = *dataBoxMapping->mBuildedConceptSet;

				*mInverseObjectPropertyHash = *dataBoxMapping->mInverseObjectPropertyHash;
				*mExpressionBuildHash = *dataBoxMapping->mExpressionBuildHash;
				*mExpressionBuildListContainer = *dataBoxMapping->mExpressionBuildListContainer;

				mConstructFlags.referenceBuildConstructFlags(&dataBoxMapping->mConstructFlags);
				return this;
			}


			CActiveEntityCountVector* CExpressionDataBoxMapping::getActiveEntityVector() {
				return mActiveEntityCountVector;
			}


			COntologyBuildConstructFlags* CExpressionDataBoxMapping::getBuildConstructFlags() {
				return &mConstructFlags;
			}

			CBUILDHASH<CClassTermExpression*,CConcept*>* CExpressionDataBoxMapping::getClassTermConceptMappingHash() {
				return mClassTermConceptHash;
			}

			CBUILDHASH<CConcept*,CClassTermExpression*>* CExpressionDataBoxMapping::getConceptClassTermMappingHash() {
				return mConceptClassTermHash;
			}

			CBUILDHASH<CDataRangeTermExpression*,CConcept*>* CExpressionDataBoxMapping::getDataRangeTermConceptMappingHash() {
				return mDataRangeTermConceptHash;
			}

			CBUILDHASH<CConcept*,CDataRangeTermExpression*>* CExpressionDataBoxMapping::getConceptDataRangeTermMappingHash() {
				return mConceptDataRangeTermHash;
			}



			CBUILDHASH<CObjectPropertyTermExpression*,CRole*>* CExpressionDataBoxMapping::getObjectPropertyTermRoleMappingHash() {
				return mObjPropTermRoleHash;
			}

			CBUILDHASH<CRole*,CObjectPropertyTermExpression*>* CExpressionDataBoxMapping::getRoleObjectPropertyTermMappingHash() {
				return mRoleObjPropTermHash;
			}


			CBUILDHASH<CDataPropertyTermExpression*,CRole*>* CExpressionDataBoxMapping::getDataPropertyTermRoleMappingHash() {
				return mDataPropTermRoleHash;
			}

			CBUILDHASH<CRole*,CDataPropertyTermExpression*>* CExpressionDataBoxMapping::getRoleDataPropertyTermMappingHash() {
				return mRoleDataPropTermHash;
			}



			CBUILDHASH<CIndividualTermExpression*,CIndividual*>* CExpressionDataBoxMapping::getIndividulTermIndiMappingHash() {
				return mIndividulTermIndiHash;
			}

			CBUILDHASH<CIndividual*,CIndividualTermExpression*>* CExpressionDataBoxMapping::getIndiIndividulTermMappingHash() {
				return mIndiIndividulTermHash;
			}



			CBUILDHASH<QPair<CDataLexicalValueExpression*, CDatatype*>, CDataLiteral*>* CExpressionDataBoxMapping::getDataLexicalValueExpressionDatatypePairDataLiteralHash() {
				return mLexicalDataExpDatatypePairDataLiteralHash;
			}


			CBUILDHASH<CDatatypeExpression*,CDatatype*>* CExpressionDataBoxMapping::getDatatypeExpressionDatatypeHash() {
				return mDatatypeExpDatatypeHash;
			}

			CBUILDHASH<CDatatype*,CDatatypeExpression*>* CExpressionDataBoxMapping::getDatatypeDatatypeExpessionHash() {
				return mDatatypeDatatypeExpHash;
			}



			CBUILDHASH<CClassAxiomExpression*,bool>* CExpressionDataBoxMapping::getUpdatedClassAxiomTellOrRetractHash() {
				return mUpdateClassAxiomHash;
			}


			CBUILDHASH<CObjectPropertyAxiomExpression*,bool>* CExpressionDataBoxMapping::getUpdateObjectPropertyAxiomHash() {
				return mUpdateObjectPropertyAxiomHash;
			}

			CBUILDHASH<CDataPropertyAxiomExpression*,bool>* CExpressionDataBoxMapping::getUpdateDataPropertyAxiomHash() {
				return mUpdateDataPropertyAxiomHash;
			}

			CBUILDHASH<CAssertionAxiomExpression*,bool>* CExpressionDataBoxMapping::getUpdateAssertionAxiomHash() {
				return mUpdateAssertionAxiomHash;
			}


			CBUILDSET< QPair<CClassTermExpression*,CClassAxiomExpression*> >* CExpressionDataBoxMapping::getClassTermExpressionClassAxiomExpressionSet() {
				return mClassTermClassAxiomSet;
			}

			CBUILDHASH<CClassTermExpression*,CClassAxiomExpression*>* CExpressionDataBoxMapping::getClassTermExpressionClassAxiomExpressionHash() {
				return mClassTermClassAxiomHash;
			}



			CBUILDSET< QPair<CObjectPropertyTermExpression*,CObjectPropertyAxiomExpression*> >* CExpressionDataBoxMapping::getObjectPropertyTermObjectPropertyAxiomSet() {
				return mObjPropTermObjPropAxiomSet;
			}

			CBUILDHASH<CObjectPropertyTermExpression*,CObjectPropertyAxiomExpression*>* CExpressionDataBoxMapping::getObjectPropertyTermObjectPropertyAxiomHash() {
				return mObjPropTermObjPropAxiomHash;
			}





			CBUILDSET< QPair<CDataPropertyTermExpression*,CDataPropertyAxiomExpression*> >* CExpressionDataBoxMapping::getDataPropertyTermDataPropertyAxiomSet() {
				return mDataPropTermDataPropAxiomSet;
			}

			CBUILDHASH<CDataPropertyTermExpression*,CDataPropertyAxiomExpression*>* CExpressionDataBoxMapping::getDataPropertyTermDataPropertyAxiomHash() {
				return mDataPropTermDataPropAxiomHash;
			}






			CBUILDSET< QPair<CIndividualTermExpression*,CAssertionAxiomExpression*> >* CExpressionDataBoxMapping::getIndividualTermAssertionAxiomSet() {
				return mIndiTermAssertionAxiomSet;
			}

			CBUILDHASH<CIndividualTermExpression*,CAssertionAxiomExpression*>* CExpressionDataBoxMapping::getIndividualTermAssertionAxiomHash() {
				return mIndiTermAssertionAxiomHash;
			}



			CBUILDSET<CClassTermExpression*>* CExpressionDataBoxMapping::getTopRebuildClassTermExpressionsSet() {
				return mTopRebuildClassTermExpressionsSet;
			}

			CBUILDSET<CObjectPropertyTermExpression*>* CExpressionDataBoxMapping::getTopRebuildObjectPropertyTermExpressionsSet() {
				return mTopRebuildObjectPropertyTermExpressionsSet;
			}

			CBUILDHASH<CObjectPropertyAxiomExpression*,CRoleChain*>* CExpressionDataBoxMapping::getObjectPropertyTermRoleChainHash() {
				return mObjPropTermRoleChainHash;
			}

			CBUILDHASH<CRoleChain*,CObjectPropertyAxiomExpression*>* CExpressionDataBoxMapping::getRoleChainObjectPropertyTermHash() {
				return mRoleChainObjPropTermHash;
			}



			CBUILDSET<CClassTermExpression*>* CExpressionDataBoxMapping::getBuildedConceptSet() {
				return mBuildedConceptSet;
			}

			CBUILDSET<CObjectPropertyTermExpression*>* CExpressionDataBoxMapping::getBuildedObjectRoleSet() {
				return mBuildedObjectRoleSet;
			}

			CBUILDSET<CDataPropertyTermExpression*>* CExpressionDataBoxMapping::getBuildedDataRoleSet() {
				return mBuildedDataRoleSet;
			}


			CBUILDSET<CDataRangeTermExpression*>* CExpressionDataBoxMapping::getBuildedDataRangeSet() {
				return mBuildedDataRangeSet;
			}

			CBUILDSET<CDatatypeExpression*>* CExpressionDataBoxMapping::getBuildedDatatypeSet() {
				return mBuildedDatatypeSet;
			}

			CBUILDSET<CIndividualTermExpression*>* CExpressionDataBoxMapping::getBuildedIndividualSet() {
				return mBuildedIndividualSet;
			}


			cint64 CExpressionDataBoxMapping::getLastActiveCountedEntityAxiom() {
				return mLastActiveCountedEntityAxiom;
			}

			CExpressionDataBoxMapping* CExpressionDataBoxMapping::setLastActiveCountedEntityAxiom(cint64 lastCounted) {
				mLastActiveCountedEntityAxiom = lastCounted;
				return this;
			}



			cint64 CExpressionDataBoxMapping::getLastProcessedChangedAxiom() {
				return mLastProcessedChangedAxiom;
			}

			cint64 CExpressionDataBoxMapping::getLastBuildedIndividual() {
				return mLastBuildedIndividual;
			}

			cint64 CExpressionDataBoxMapping::getLastBuildedConcept() {
				return mLastBuildedConcept;
			}

			cint64 CExpressionDataBoxMapping::getLastBuildedObjectRole() {
				return mLastBuildedObjectRole;
			}

			cint64 CExpressionDataBoxMapping::getLastBuildedDataRole() {
				return mLastBuildedDataRole;
			}

			cint64 CExpressionDataBoxMapping::getLastBuildedDataRange() {
				return mLastBuildedDataRange;
			}

			cint64 CExpressionDataBoxMapping::getLastBuildedDatatype() {
				return mLastBuildedDatatype;

			}
			cint64 CExpressionDataBoxMapping::getLastProcessedBuildIndividual() {
				return mLastProcessedBuildIndividual;
			}

			cint64 CExpressionDataBoxMapping::getLastProcessedBuildConcept() {
				return mLastProcessedBuildConcept;
			}

			cint64 CExpressionDataBoxMapping::getLastProcessedBuildObjectRole() {
				return mLastProcessedBuildObjectRole;
			}

			cint64 CExpressionDataBoxMapping::getLastProcessedBuildDataRole() {
				return mLastProcessedBuildDataRole;
			}

			cint64 CExpressionDataBoxMapping::getLastProcessedBuildDatatype() {
				return mLastProcessedBuildDatatype;
			}

			cint64 CExpressionDataBoxMapping::getLastProcessedBuildDataRange() {
				return mLastProcessedBuildDataRange;
			}

			cint64 CExpressionDataBoxMapping::getLastProcessedInverseProperty() {
				return mLastProcessedInverseProperty;
			}

			cint64 CExpressionDataBoxMapping::getLastProcessedExpression() {
				return mLastProcessedExpression;
			}

			CExpressionDataBoxMapping* CExpressionDataBoxMapping::setLastProcessedChangedAxiom(cint64 lastBuilded) {
				mLastProcessedChangedAxiom = lastBuilded;
				return this;
			}

			CExpressionDataBoxMapping* CExpressionDataBoxMapping::setLastProcessedBuildIndividual(cint64 lastBuilded) {
				mLastProcessedBuildIndividual = lastBuilded;
				return this;
			}

			CExpressionDataBoxMapping* CExpressionDataBoxMapping::setLastProcessedBuildConcept(cint64 lastBuilded) {
				mLastProcessedBuildConcept = lastBuilded;
				return this;
			}


			CExpressionDataBoxMapping* CExpressionDataBoxMapping::setLastProcessedBuildObjectRole(cint64 lastBuilded) {
				mLastProcessedBuildObjectRole = lastBuilded;
				return this;
			}

			CExpressionDataBoxMapping* CExpressionDataBoxMapping::setLastProcessedBuildDataRole(cint64 lastBuilded) {
				mLastProcessedBuildDataRole = lastBuilded;
				return this;
			}

			CExpressionDataBoxMapping* CExpressionDataBoxMapping::setLastProcessedBuildDatatype(cint64 lastBuilded) {
				mLastProcessedBuildDatatype = lastBuilded;
				return this;
			}

			CExpressionDataBoxMapping* CExpressionDataBoxMapping::setLastProcessedBuildDataRange(cint64 lastBuilded) {
				mLastProcessedBuildDataRange = lastBuilded;
				return this;
			}

			CExpressionDataBoxMapping* CExpressionDataBoxMapping::setLastProcessedInverseProperty(cint64 lastBuilded) {
				mLastProcessedInverseProperty = lastBuilded;
				return this;
			}

			CExpressionDataBoxMapping* CExpressionDataBoxMapping::setLastProcessedExpression(cint64 lastBuilded) {
				mLastProcessedExpression = lastBuilded;
				return this;
			}

			CExpressionDataBoxMapping* CExpressionDataBoxMapping::setLastBuildedIndividual(cint64 lastBuilded) {
				mLastBuildedIndividual = lastBuilded;
				return this;
			}

			CExpressionDataBoxMapping* CExpressionDataBoxMapping::setLastBuildedConcept(cint64 lastBuilded) {
				mLastBuildedConcept = lastBuilded;
				return this;
			}


			CExpressionDataBoxMapping* CExpressionDataBoxMapping::setLastBuildedObjectRole(cint64 lastBuilded) {
				mLastBuildedObjectRole = lastBuilded;
				return this;
			}

			CExpressionDataBoxMapping* CExpressionDataBoxMapping::setLastBuildedDataRole(cint64 lastBuilded) {
				mLastBuildedDataRole = lastBuilded;
				return this;
			}

			CExpressionDataBoxMapping* CExpressionDataBoxMapping::setLastBuildedDataRange(cint64 lastBuilded) {
				mLastBuildedDataRange = lastBuilded;
				return this;
			}

			CExpressionDataBoxMapping* CExpressionDataBoxMapping::setLastBuildedDatatype(cint64 lastBuilded) {
				mLastBuildedDatatype = lastBuilded;
				return this;
			}

			CBUILDLIST<CClassTermExpression*>* CExpressionDataBoxMapping::getBuildedConceptList() {
				return mBuildedConceptList;
			}

			CBUILDLIST<CObjectPropertyTermExpression*>* CExpressionDataBoxMapping::getBuildedObjectRoleList() {
				return mBuildedObjectRoleList;
			}

			CBUILDLIST<CDataPropertyTermExpression*>* CExpressionDataBoxMapping::getBuildedDataRoleList() {
				return mBuildedDataRoleList;
			}


			CBUILDLIST<CDataRangeTermExpression*>* CExpressionDataBoxMapping::getBuildedDataRangeList() {
				return mBuildedDataRangeList;
			}

			CBUILDLIST<CDatatypeExpression*>* CExpressionDataBoxMapping::getBuildedDatatypeList() {
				return mBuildedDatatypeList;
			}

			CBUILDLIST<CIndividualTermExpression*>* CExpressionDataBoxMapping::getBuildedIndividualList() {
				return mBuildedIndividualList;
			}

			CBUILDHASH<CExpressionHasher,CBuildExpression*>* CExpressionDataBoxMapping::getStructuralExpressionBuildHash() {
				return mExpressionBuildHash;
			}

			CBUILDLIST<CBuildExpression*>* CExpressionDataBoxMapping::getExpressionBuildListContainer() {
				return mExpressionBuildListContainer;
			}

			CBUILDHASH<CObjectPropertyTermExpression*,CObjectPropertyTermExpression*>* CExpressionDataBoxMapping::getInverseObjectPropertyHash() {
				return mInverseObjectPropertyHash;
			}


		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude
