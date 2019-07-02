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

#include "COntologyBuildData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			COntologyBuildData::COntologyBuildData(COntologyContext* ontoContext) {
				mOntoContext = ontoContext;
				
				CMemoryAllocationManager* memAllocMan = CContext::getMemoryAllocationManager(mOntoContext);
				mTellAxiomSet = CObjectParameterizingAllocator< CONTOLOGYAXIOMSET<CAxiomExpression*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);
				mRetractAxiomSet = CObjectParameterizingAllocator< CONTOLOGYAXIOMSET<CAxiomExpression*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);
				
				mTellUpdatedAxiomSet = CObjectParameterizingAllocator< CONTOLOGYAXIOMSET<CAxiomExpression*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);
				mRetractUpdatedAxiomSet = CObjectParameterizingAllocator< CONTOLOGYAXIOMSET<CAxiomExpression*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);
	
				mChangeAxiomList = CObjectParameterizingAllocator< CONTOLOGYAXIOMLIST< QPair<CAxiomExpression*,bool> >,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);
				mChangeUpdatedAxiomList = CObjectParameterizingAllocator< CONTOLOGYAXIOMLIST< QPair<CAxiomExpression*,bool> >,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);
				
				mDeclarationAxiomSet = CObjectParameterizingAllocator< CBUILDSET<CDeclarationAxiomExpression*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);

				mTopClassExpression = nullptr;
				mBottomClassExpression = nullptr;

				mTopObjPropExpression = nullptr;
				mBottomObjPropExpression = nullptr;

				mExpressionBuildHash = CObjectParameterizingAllocator< CBUILDHASH<CExpressionHasher,CBuildExpression*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);
				mClassBuildHash = CObjectParameterizingAllocator< CBUILDHASH<CStringRefStringHasher,CClassExpression*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);
				mObjectPropertyBuildHash = CObjectParameterizingAllocator< CBUILDHASH<CStringRefStringHasher,CObjectPropertyExpression*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);
				mIndividualBuildHash = CObjectParameterizingAllocator< CBUILDHASH<CStringRefStringHasher,CNamedIndividualExpression*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);
				mAnoIndividualBuildHash = CObjectParameterizingAllocator< CBUILDHASH<QPair<CStringRefStringHasher,CStringRefStringHasher>,CAnonymousIndividualExpression*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);
				
				mExpressionBuildListContainer = CObjectParameterizingAllocator< CBUILDLIST<CBuildExpression*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);

				mInverseObjectPropertyHash = CObjectParameterizingAllocator< CBUILDHASH<CObjectPropertyTermExpression*,CObjectPropertyTermExpression*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);
				mInverseObjectPropertyList = CObjectParameterizingAllocator< CBUILDLIST< QPair<CObjectPropertyTermExpression*,CObjectPropertyTermExpression*> >,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);
				
				mIndividualVariableIDHash = CObjectParameterizingAllocator< CBUILDHASH<CObjectIndividualVariableExpression*,cint64>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);
				mIndividualVariableBuildHash = CObjectParameterizingAllocator< CBUILDHASH<QPair<CStringRefStringHasher,cint64>,CObjectIndividualVariableExpression*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);
				
				mBuildConceptSet = CObjectParameterizingAllocator< CBUILDSET<CClassTermExpression*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);
				mBuildRoleSet = CObjectParameterizingAllocator< CBUILDSET<CObjectPropertyTermExpression*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);
				mBuildIndividualSet = CObjectParameterizingAllocator< CBUILDSET<CIndividualTermExpression*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);
				
				mBuildConceptList = CObjectParameterizingAllocator< CBUILDLIST<CClassTermExpression*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);
				mBuildRoleList = CObjectParameterizingAllocator< CBUILDLIST<CObjectPropertyTermExpression*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);
				mBuildIndividualList = CObjectParameterizingAllocator< CBUILDLIST<CIndividualTermExpression*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);

				mAxiomNumber = 0;
				mEntityNumber = 0;
			}



			COntologyBuildData::~COntologyBuildData() {
				CMemoryAllocationManager* memAllocMan = CContext::getMemoryAllocationManager(mOntoContext);
				// TODO: delete all
				//cDeleteAll(*mExpressionBuildListContainer);
				COPADestroyAndRelease(mTellAxiomSet,memAllocMan);
				COPADestroyAndRelease(mRetractAxiomSet,memAllocMan);
				COPADestroyAndRelease(mTellUpdatedAxiomSet,memAllocMan);
				COPADestroyAndRelease(mRetractUpdatedAxiomSet,memAllocMan);

				COPADestroyAndRelease(mChangeAxiomList,memAllocMan);
				COPADestroyAndRelease(mChangeUpdatedAxiomList,memAllocMan);


				COPADestroyAndRelease(mDeclarationAxiomSet,memAllocMan);

				COPADestroyAndRelease(mExpressionBuildHash,memAllocMan);
				COPADestroyAndRelease(mClassBuildHash,memAllocMan);
				COPADestroyAndRelease(mObjectPropertyBuildHash,memAllocMan);
				COPADestroyAndRelease(mIndividualBuildHash,memAllocMan);
				COPADestroyAndRelease(mAnoIndividualBuildHash,memAllocMan);
				COPADestroyAndRelease(mExpressionBuildListContainer,memAllocMan);

				COPADestroyAndRelease(mInverseObjectPropertyHash,memAllocMan);
				COPADestroyAndRelease(mInverseObjectPropertyList,memAllocMan);
				COPADestroyAndRelease(mIndividualVariableIDHash,memAllocMan);
				COPADestroyAndRelease(mIndividualVariableBuildHash,memAllocMan);

				COPADestroyAndRelease(mBuildConceptSet,memAllocMan);
				COPADestroyAndRelease(mBuildRoleSet,memAllocMan);
				COPADestroyAndRelease(mBuildIndividualSet,memAllocMan);

				COPADestroyAndRelease(mBuildConceptList,memAllocMan);
				COPADestroyAndRelease(mBuildRoleList,memAllocMan);
				COPADestroyAndRelease(mBuildIndividualList,memAllocMan);
			}



			COntologyBuildData* COntologyBuildData::referenceBuildData(COntologyBuildData* buildData) {
				*mTellAxiomSet = *buildData->mTellAxiomSet;
				*mRetractAxiomSet = *buildData->mRetractAxiomSet;

				mTellUpdatedAxiomSet->clear();
				mRetractUpdatedAxiomSet->clear();

				*mChangeAxiomList = *buildData->mChangeAxiomList;
				mChangeUpdatedAxiomList->clear();

				//*mTellUpdatedAxiomSet = *buildData->mTellUpdatedAxiomSet;
				//*mRetractUpdatedAxiomSet = *buildData->mRetractUpdatedAxiomSet;
				
				*mDeclarationAxiomSet = *buildData->mDeclarationAxiomSet;


				mTopClassExpression = buildData->mTopClassExpression;
				mBottomClassExpression = buildData->mBottomClassExpression;

				mTopObjPropExpression = buildData->mTopObjPropExpression;
				mBottomObjPropExpression = buildData->mBottomObjPropExpression;

				*mExpressionBuildHash = *buildData->mExpressionBuildHash;
				*mClassBuildHash = *buildData->mClassBuildHash;
				*mObjectPropertyBuildHash = *buildData->mObjectPropertyBuildHash;
				*mIndividualBuildHash = *buildData->mIndividualBuildHash;
				*mAnoIndividualBuildHash = *buildData->mAnoIndividualBuildHash;

				*mExpressionBuildListContainer = *buildData->mExpressionBuildListContainer;

				*mInverseObjectPropertyHash = *buildData->mInverseObjectPropertyHash;
				*mInverseObjectPropertyList = *buildData->mInverseObjectPropertyList;

				*mIndividualVariableIDHash = *buildData->mIndividualVariableIDHash;
				*mIndividualVariableBuildHash = *buildData->mIndividualVariableBuildHash;

				*mBuildIndividualList = *buildData->mBuildIndividualList;
				*mBuildConceptList = *buildData->mBuildConceptList;
				*mBuildRoleList = *buildData->mBuildRoleList;

				mAxiomNumber = buildData->mAxiomNumber;
				mEntityNumber = buildData->mEntityNumber;

				return this;
			}





			CONTOLOGYAXIOMSET<CAxiomExpression*>* COntologyBuildData::getTellAxiomSet() {
				return mTellAxiomSet;
			}

			CONTOLOGYAXIOMSET<CAxiomExpression*>* COntologyBuildData::getRetractAxiomSet() {
				return mRetractAxiomSet;
			}

			CONTOLOGYAXIOMSET<CAxiomExpression*>* COntologyBuildData::getUpdatedTellAxiomSet() {
				return mTellUpdatedAxiomSet;
			}

			CONTOLOGYAXIOMSET<CAxiomExpression*>* COntologyBuildData::getUpdatedRetractAxiomSet() {
				return mRetractUpdatedAxiomSet;
			}

			CONTOLOGYAXIOMLIST< QPair<CAxiomExpression*,bool> >* COntologyBuildData::getChangeAxiomList() {
				return mChangeAxiomList;
			}

			CONTOLOGYAXIOMLIST< QPair<CAxiomExpression*,bool> >* COntologyBuildData::getUpdatedChangeAxiomList() {
				return mChangeUpdatedAxiomList;
			}




			CBUILDSET<CDeclarationAxiomExpression*>* COntologyBuildData::getDeclarationAxiomSet() {
				return mDeclarationAxiomSet;
			}


			CClassTermExpression* COntologyBuildData::getTopClassExpression() {
				return mTopClassExpression;
			}

			CClassTermExpression* COntologyBuildData::getBottomClassExpression() {
				return mBottomClassExpression;
			}

			CObjectPropertyTermExpression* COntologyBuildData::getTopObjectPropertyExpression() {
				return mTopObjPropExpression;
			}

			CObjectPropertyTermExpression* COntologyBuildData::getBottomObjectPropertyExpression() {
				return mBottomObjPropExpression;
			}



			COntologyBuildData* COntologyBuildData::setTopClassExpression(CClassTermExpression* topClassExp) {
				mTopClassExpression = topClassExp;
				return this;
			}

			COntologyBuildData* COntologyBuildData::setBottomClassExpression(CClassTermExpression* bottomClassExp) {
				mBottomClassExpression = bottomClassExp;
				return this;
			}


			COntologyBuildData* COntologyBuildData::setTopObjectPropertyExpression(CObjectPropertyTermExpression* topObjectPropertyExp) {
				mTopObjPropExpression = topObjectPropertyExp;
				return this;
			}

			COntologyBuildData* COntologyBuildData::setBottomObjectPropertyExpression(CObjectPropertyTermExpression* bottomObjectPropertyExp) {
				mBottomObjPropExpression = bottomObjectPropertyExp;
				return this;
			}




			CBUILDHASH<CExpressionHasher,CBuildExpression*>* COntologyBuildData::getStructuralExpressionBuildHash() {
				return mExpressionBuildHash;
			}

			CBUILDHASH<CStringRefStringHasher,CClassExpression*>* COntologyBuildData::getClassEntityBuildHash() {
				return mClassBuildHash;
			}

			CBUILDHASH<CStringRefStringHasher,CObjectPropertyExpression*>* COntologyBuildData::getObjectPropertyEntityBuildHash() {
				return mObjectPropertyBuildHash;
			}

			CBUILDHASH<CStringRefStringHasher,CNamedIndividualExpression*>* COntologyBuildData::getIndividualEntityBuildHash() {
				return mIndividualBuildHash;
			}


			CBUILDHASH<QPair<CStringRefStringHasher,CStringRefStringHasher>,CAnonymousIndividualExpression*>* COntologyBuildData::getAnonymousIndividualBuildHash() {
				return mAnoIndividualBuildHash;
			}



			
			CBUILDLIST<CBuildExpression*>* COntologyBuildData::getExpressionBuildListContainer() {
				return mExpressionBuildListContainer;
			}


			CBUILDHASH<CObjectPropertyTermExpression*,CObjectPropertyTermExpression*>* COntologyBuildData::getInverseObjectPropertyHash() {
				return mInverseObjectPropertyHash;
			}

			
			CBUILDLIST< QPair<CObjectPropertyTermExpression*,CObjectPropertyTermExpression*> >* COntologyBuildData::getInverseObjectPropertyList() {
				return mInverseObjectPropertyList;
			}


			CBUILDHASH<CObjectIndividualVariableExpression*,cint64>* COntologyBuildData::getIndividualVariableIDHash() {
				return mIndividualVariableIDHash;
			}

			CBUILDHASH<QPair<CStringRefStringHasher,cint64>,CObjectIndividualVariableExpression*>* COntologyBuildData::getIndividualVariableBuildHash() {
				return mIndividualVariableBuildHash;
			}

			CBUILDSET<CClassTermExpression*>* COntologyBuildData::getBuildConceptSet() {
				return mBuildConceptSet;
			}

			CBUILDSET<CObjectPropertyTermExpression*>* COntologyBuildData::getBuildRoleSet() {
				return mBuildRoleSet;
			}

			CBUILDSET<CIndividualTermExpression*>* COntologyBuildData::getBuildIndividualSet() {
				return mBuildIndividualSet;
			}


			CBUILDLIST<CClassTermExpression*>* COntologyBuildData::getBuildConceptList() {
				return mBuildConceptList;
			}

			CBUILDLIST<CObjectPropertyTermExpression*>* COntologyBuildData::getBuildRoleList() {
				return mBuildRoleList;
			}

			CBUILDLIST<CIndividualTermExpression*>* COntologyBuildData::getBuildIndividualList() {
				return mBuildIndividualList;
			}

			cint64 COntologyBuildData::getNextAxiomNumber(bool moveNext) {
				cint64 nextNumber = mAxiomNumber;
				if (moveNext) {
					++mAxiomNumber;
				}
				return nextNumber;
			}	

			COntologyBuildData* COntologyBuildData::setNextAxiomNumber(cint64 axiomNumber) {
				mAxiomNumber = axiomNumber;
				return this;
			}


			cint64 COntologyBuildData::getNextEntityNumber(bool moveNext) {
				cint64 nextNumber = mEntityNumber;
				if (moveNext) {
					++mEntityNumber;
				}
				return nextNumber;
			}	

			COntologyBuildData* COntologyBuildData::setNextEntityNumber(cint64 entityNumber) {
				mEntityNumber = entityNumber;
				return this;
			}

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude
