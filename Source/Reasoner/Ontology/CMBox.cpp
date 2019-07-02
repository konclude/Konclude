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

#include "CMBox.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			CMBox::CMBox(CBoxContext* boxContext) {
				conceptDep = nullptr;
				roleDep = nullptr;
				indiDep = nullptr;
				mConDataVector = nullptr;
				mBranchTriggVector = nullptr;
				mImpRepVector = nullptr;
				mNomSchemaTemplVector = nullptr;
				mValueSpacesTriggers = nullptr;

				mBoxContext = boxContext;
				mMemMan = CContext::getMemoryAllocationManager(mBoxContext);
				mDeleteBoxContext = nullptr;
			}


			CMBox::~CMBox() {
				delete mDeleteBoxContext;
				COPADestroyAndRelease(conceptDep,mMemMan);
				COPADestroyAndRelease(roleDep,mMemMan);
				COPADestroyAndRelease(indiDep,mMemMan);
				COPADestroyAndRelease(mConDataVector,mMemMan);
				COPADestroyAndRelease(mBranchTriggVector,mMemMan);
				COPADestroyAndRelease(mImpRepVector,mMemMan);
				COPADestroyAndRelease(mNomSchemaTemplVector,mMemMan);
				COPADestroyAndRelease(mValueSpacesTriggers,mMemMan);
			}


			CConceptDependenceVector *CMBox::getConceptDependenceVector(bool create) {
				if (!conceptDep && create) {
					conceptDep = CObjectParameterizingAllocator<CConceptDependenceVector,CBoxContext*>::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mBoxContext),mBoxContext);
				}
				return conceptDep;
			}

			CMBox *CMBox::setConceptDependenceVector(CConceptDependenceVector *conceptDependenceVector) {
				COPADestroyAndRelease(conceptDep,mMemMan);
				conceptDep = conceptDependenceVector;
				return this;
			}



			CRoleDependenceVector *CMBox::getRoleDependenceVector(bool create) {
				if (!roleDep && create) {
					roleDep = CObjectParameterizingAllocator<CRoleDependenceVector,CBoxContext*>::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mBoxContext),mBoxContext);
				}
				return roleDep;
			}

			CMBox *CMBox::setRoleDependenceVector(CRoleDependenceVector *roleDependenceVector) {
				COPADestroyAndRelease(roleDep,mMemMan);
				roleDep = roleDependenceVector;
				return this;
			}


			CIndividualDependenceVector *CMBox::getIndividualDependenceVector(bool create) {
				if (!indiDep && create) {
					indiDep = CObjectParameterizingAllocator<CIndividualDependenceVector,CBoxContext*>::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mBoxContext),mBoxContext);
				}
				return indiDep;
			}

			CMBox *CMBox::setIndividualDependenceVector(CIndividualDependenceVector *indiDependenceVector) {
				COPADestroyAndRelease(indiDep,mMemMan);
				indiDep = indiDependenceVector;
				return this;
			}



			CMBox *CMBox::getMBoxReferenceCopy(CBoxContext *boxContext) {
				CMBox *mBox = CObjectParameterizingAllocator<CMBox,CBoxContext*>::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(boxContext),boxContext);
				mBox->referenceMBox(this);
				return mBox;
			}



			CConceptDataVector *CMBox::getConceptDataVector(bool create) {
				if (!mConDataVector && create) {
					mConDataVector = CObjectParameterizingAllocator<CConceptDataVector,CBoxContext*>::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mBoxContext),mBoxContext);
				}
				return mConDataVector;
			}

			CMBox *CMBox::setConceptDataVector(CConceptDataVector *conceptDataVector) {
				COPADestroyAndRelease(mConDataVector,mMemMan);
				mConDataVector = conceptDataVector;
				return this;
			}


			CBranchingTriggerVector *CMBox::getBranchingTriggerVector(bool create) {
				if (!mBranchTriggVector && create) {
					mBranchTriggVector = CObjectParameterizingAllocator<CBranchingTriggerVector,CBoxContext*>::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mBoxContext),mBoxContext);
				}
				return mBranchTriggVector;
			}

			CMBox *CMBox::setBranchingTriggerVector(CBranchingTriggerVector *branchTriggVector) {
				COPADestroyAndRelease(mBranchTriggVector,mMemMan);
				mBranchTriggVector = branchTriggVector;
				return this;
			}


			CImplicationReplacementVector *CMBox::getImplicationReplacementVector(bool create) {
				if (!mImpRepVector && create) {
					mImpRepVector = CObjectParameterizingAllocator<CImplicationReplacementVector,CBoxContext*>::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mBoxContext),mBoxContext);
				}
				return mImpRepVector;
			}

			CMBox *CMBox::setImplicationReplacementVector(CImplicationReplacementVector* impRepVector) {
				COPADestroyAndRelease(mImpRepVector,mMemMan);
				mImpRepVector = impRepVector;
				return this;
			}


			CNominalSchemaTemplateVector *CMBox::getNominalSchemaTemplateVector(bool create) {
				if (!mNomSchemaTemplVector && create) {
					mNomSchemaTemplVector = CObjectParameterizingAllocator<CNominalSchemaTemplateVector,CBoxContext*>::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mBoxContext),mBoxContext);
				}
				return mNomSchemaTemplVector;
			}


			CDatatypeValueSpacesTriggers* CMBox::getValueSpacesTriggers(bool create) {
				if (!mValueSpacesTriggers && create) {
					mValueSpacesTriggers = CObjectParameterizingAllocator<CDatatypeValueSpacesTriggers,CBoxContext*>::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mBoxContext),mBoxContext);
					mValueSpacesTriggers->initValueSpacesTriggers(nullptr);
				}
				return mValueSpacesTriggers;
			}


			CMBox *CMBox::setNominalSchemaTemplateVector(CNominalSchemaTemplateVector* nomSchTemplVector) {
				COPADestroyAndRelease(mNomSchemaTemplVector,mMemMan);
				mNomSchemaTemplVector = nomSchTemplVector;
				return this;
			}

			CMBox *CMBox::referenceMBox(CMBox *mBox) {
				if (mBox->conceptDep) {
					getConceptDependenceVector();
					conceptDep->referenceVector(mBox->conceptDep);
				} else if (conceptDep) {					
					conceptDep->clear();
				}
				if (mBox->roleDep) {
					getRoleDependenceVector();
					roleDep->referenceVector(mBox->roleDep);
				} else if (roleDep) {					
					roleDep->clear();
				}
				if (mBox->indiDep) {
					getIndividualDependenceVector();
					indiDep->referenceVector(mBox->indiDep);
				} else if (indiDep) {					
					indiDep->clear();
				}
				if (mBox->mConDataVector) {
					getConceptDataVector();
					mConDataVector->referenceVector(mBox->mConDataVector);
				} else if (mConDataVector) {					
					mConDataVector->clear();
				}
				if (mBox->mBranchTriggVector) {
					getBranchingTriggerVector();
					mBranchTriggVector->referenceVector(mBox->mBranchTriggVector);
				} else if (mBranchTriggVector) {					
					mBranchTriggVector->clear();
				}
				if (mBox->mImpRepVector) {
					getImplicationReplacementVector();
					mImpRepVector->referenceVector(mBox->mImpRepVector);
				} else if (mImpRepVector) {					
					mImpRepVector->clear();
				}
				if (mBox->mNomSchemaTemplVector) {
					getNominalSchemaTemplateVector();
					mNomSchemaTemplVector->referenceVector(mBox->mNomSchemaTemplVector);
				} else if (mNomSchemaTemplVector) {					
					mNomSchemaTemplVector->clear();
				}
				if (mBox->mValueSpacesTriggers) {
					getValueSpacesTriggers();
					mValueSpacesTriggers->initValueSpacesTriggers(mBox->mValueSpacesTriggers);
				} else if (mValueSpacesTriggers) {					
					mValueSpacesTriggers->initValueSpacesTriggers(nullptr);
				}
				return this;
			}



			CBoxContext *CMBox::getBoxContext(bool create) {
				if (!mBoxContext && create) {
					mBoxContext = new CConcreteOntologyContextBase();
					mDeleteBoxContext = mBoxContext;
				}
				return mBoxContext;
			}

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude
