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

#include "CTBox.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			CTBox::CTBox(CBoxContext* boxContext) {
				mBoxContext = boxContext;
				mMemMan = CContext::getMemoryAllocationManager(mBoxContext);
				concepts = nullptr;
				mDeleteBoxContext = nullptr;
				mTriggerImpHash = nullptr;
				mEquivConCandidateHash = nullptr;
				mCandidateEquivConHash = nullptr;
				mEquivConNonCandidateSet = nullptr;
				mRoleDomainTriggerConceptHash = nullptr;
				mIndividualTriggerConceptHash = nullptr;
				mUnivConnNomValueCon = nullptr;
				gciConceptSet = 0;
				activeConceptSet = 0;
				mBottomConceptIndex = 0;
				mTopConceptIndex = 1;
				mIndividualTriggerConceptIndex = 2;
				mTopDataRangeConceptIndex = 3;
				mDatatypeVector = nullptr;
			}


			CTBox::~CTBox() {
				delete mDeleteBoxContext;
				COPADestroyAndRelease(gciConceptSet,mMemMan);
				COPADestroyAndRelease(activeConceptSet,mMemMan);
				COPADestroyAndRelease(concepts,mMemMan);
				COPADestroyAndRelease(mTriggerImpHash,mMemMan);
				COPADestroyAndRelease(mEquivConCandidateHash,mMemMan);
				COPADestroyAndRelease(mCandidateEquivConHash,mMemMan);
				COPADestroyAndRelease(mEquivConNonCandidateSet,mMemMan);
				COPADestroyAndRelease(mRoleDomainTriggerConceptHash,mMemMan);
				COPADestroyAndRelease(mIndividualTriggerConceptHash,mMemMan);
				COPADestroyAndRelease(mDatatypeVector,mMemMan);

			}

			cint64 CTBox::getTopConceptIndex() {
				return mTopConceptIndex;
			}

			cint64 CTBox::getBottomConceptIndex() {
				return mBottomConceptIndex;
			}

			cint64 CTBox::getIndividualTriggerConceptIndex() {
				return mIndividualTriggerConceptIndex;
			}


			cint64 CTBox::getTopDataRangeConceptIndex() {
				return mTopDataRangeConceptIndex;
			}

			CConcept* CTBox::getTopConcept() {
				CConcept* concept = nullptr;
				if (concepts) {
					concept = concepts->getData(mTopConceptIndex);
				}
				return concept;
			}



			CConcept* CTBox::getTopDataRangeConcept() {
				CConcept* concept = nullptr;
				if (concepts) {
					concept = concepts->getData(mTopDataRangeConceptIndex);
				}
				return concept;
			}

			CConcept* CTBox::getIndividualTriggerConcept() {
				CConcept* concept = nullptr;
				if (concepts) {
					concept = concepts->getData(mIndividualTriggerConceptIndex);
				}
				return concept;
			}

			CConcept* CTBox::getBottomConcept() {
				CConcept* concept = nullptr;
				if (concepts) {
					concept = concepts->getData(mBottomConceptIndex);
				}
				return concept;
			}

			CTBox* CTBox::setTopConcept(CConcept* topConcept) {
				getConceptVector(true)->setData(mTopConceptIndex,topConcept);
				return this;
			}

			CTBox* CTBox::setBottomConcept(CConcept* bottomConcept) {
				getConceptVector(true)->setData(mBottomConceptIndex,bottomConcept);
				return this;
			}

			CTBox* CTBox::setIndividualTriggerConcept(CConcept* indTrigConcept) {
				getConceptVector(true)->setData(mIndividualTriggerConceptIndex,indTrigConcept);
				return this;
			}


			CTBox* CTBox::setTopDataRangeConcept(CConcept* topDataRangeConcept) {
				getConceptVector(true)->setData(mTopDataRangeConceptIndex,topDataRangeConcept);
				return this;
			}

			CBOXSET<CConcept*>* CTBox::getGCIConceptSet() {
				return gciConceptSet;
			}

			CTBox *CTBox::setGCIConceptSet(CBOXSET<CConcept*>* takeContainsSet) {
				COPADestroyAndRelease(gciConceptSet,mMemMan);
				gciConceptSet = takeContainsSet;
				return this;
			}


			CBOXSET<CConcept*> *CTBox::getActiveClassConceptSet(bool create) {
				if (!activeConceptSet && create) {
					activeConceptSet = CObjectParameterizingAllocator< CBOXSET<CConcept*>,CContext* >::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mBoxContext),mBoxContext);
				}
				return activeConceptSet;
			}

			CTBox *CTBox::setActiveClassConceptSet(CBOXSET<CConcept*>* takeActiveConceptSet) {
				COPADestroyAndRelease(activeConceptSet,mMemMan);
				activeConceptSet = takeActiveConceptSet;
				return this;
			}


			CBOXHASH<CConcept*,CConcept*>* CTBox::getTriggerImplicationHash(bool create) {
				if (!mTriggerImpHash && create) {
					mTriggerImpHash = CObjectParameterizingAllocator< CBOXHASH<CConcept*,CConcept*>,CContext* >::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mBoxContext),mBoxContext);
				}
				return mTriggerImpHash;
			}

			CBOXHASH<CConcept*,CConcept*>* CTBox::getEquivalentConceptCandidateHash(bool create) {
				if (!mEquivConCandidateHash && create) {
					mEquivConCandidateHash = CObjectParameterizingAllocator< CBOXHASH<CConcept*,CConcept*>,CContext* >::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mBoxContext),mBoxContext);
				}
				return mEquivConCandidateHash;
			}

			CBOXHASH<CConcept*,CConcept*>* CTBox::getCandidateEquivalentConceptHash(bool create) {
				if (!mCandidateEquivConHash && create) {
					mCandidateEquivConHash = CObjectParameterizingAllocator< CBOXHASH<CConcept*,CConcept*>,CContext* >::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mBoxContext),mBoxContext);
				}
				return mCandidateEquivConHash;
			}

			CBOXSET<CConcept*>* CTBox::getEquivalentConceptNonCandidateSet(bool create) {
				if (!mEquivConNonCandidateSet && create) {
					mEquivConNonCandidateSet = CObjectParameterizingAllocator< CBOXSET<CConcept*>,CContext* >::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mBoxContext),mBoxContext);
				}
				return mEquivConNonCandidateSet;
			}


			CBOXHASH<CRole*,CConcept*>* CTBox::getRoleDomainTriggerConceptHash(bool create) {
				if (!mRoleDomainTriggerConceptHash && create) {
					mRoleDomainTriggerConceptHash = CObjectParameterizingAllocator< CBOXHASH<CRole*,CConcept*>,CContext* >::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mBoxContext),mBoxContext);
				}
				return mRoleDomainTriggerConceptHash;
			}

			CBOXHASH<cint64,CConcept*>* CTBox::getIndividualTriggerConceptHash(bool create) {
				if (!mIndividualTriggerConceptHash && create) {
					mIndividualTriggerConceptHash = CObjectParameterizingAllocator< CBOXHASH<cint64,CConcept*>,CContext* >::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mBoxContext),mBoxContext);
				}
				return mIndividualTriggerConceptHash;
			}

			CTBox* CTBox::setTriggerImplicationHash(CBOXHASH<CConcept*,CConcept*>* takeTriggerImplHash) {
				COPADestroyAndRelease(mTriggerImpHash,mMemMan);
				mTriggerImpHash = takeTriggerImplHash;
				return this;
			}



			CTBox *CTBox::setConceptVector(CConceptVector* takeConceptVec) {
				COPADestroyAndRelease(concepts,mMemMan);
				concepts = takeConceptVec;
				return this;
			}


			CConceptVector* CTBox::getConceptVector(bool create) {
				if (!concepts && create) {
					concepts = CObjectParameterizingAllocator<CConceptVector,CContext*>::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mBoxContext),mBoxContext);
				}
				return concepts;
			}


			qint64 CTBox::getConceptCount() {
				cint64 conCount = 0;
				if (concepts) {
					conCount = concepts->getItemCount();
				}
				return conCount;
			}

			

			CTBox *CTBox::getTBoxReferenceCopy(CBoxContext* boxContext) {
				CTBox *tBox = CObjectParameterizingAllocator<CTBox,CBoxContext*>::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(boxContext),boxContext);
				tBox->referenceTBox(this);
				return tBox;
			}


			CDatatypeVector *CTBox::getDatatypeVector(bool create) {
				if (!mDatatypeVector && create) {
					mDatatypeVector = CObjectParameterizingAllocator<CDatatypeVector,CBoxContext*>::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mBoxContext),mBoxContext);
				}
				return mDatatypeVector;
			}


			CTBox *CTBox::setDatatypeVector(CDatatypeVector* datatypeVector) {
				COPADestroyAndRelease(mDatatypeVector,mMemMan);
				mDatatypeVector = datatypeVector;
				return this;
			}


			CTBox *CTBox::referenceTBox(CTBox *tBox) {
				if (tBox->concepts) {
					getConceptVector();
					concepts->referenceVector(tBox->concepts);
				} else if (concepts) {					
					concepts->clear();
				}
				if (tBox->mDatatypeVector) {
					getDatatypeVector();
					mDatatypeVector->referenceVector(tBox->mDatatypeVector);
				} else if (mDatatypeVector) {					
					mDatatypeVector->clear();
				}
				if (tBox->gciConceptSet) {
					if (!gciConceptSet) {
						gciConceptSet = CObjectParameterizingAllocator< CBOXSET<CConcept*>,CContext* >::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mBoxContext),mBoxContext);
					}
					gciConceptSet->init(tBox->gciConceptSet,mBoxContext);
				} else if (gciConceptSet) {
					gciConceptSet->clear();
				}
				if (tBox->activeConceptSet) {
					if (!activeConceptSet) {
						activeConceptSet = CObjectParameterizingAllocator< CBOXSET<CConcept*>,CContext* >::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mBoxContext),mBoxContext);
					}
					activeConceptSet->init(tBox->activeConceptSet,mBoxContext);
				} else if (activeConceptSet) {
					activeConceptSet->clear();
				}
				mUnivConnNomValueCon = tBox->mUnivConnNomValueCon;
				if (tBox->mIndividualTriggerConceptHash) {
					if (!mIndividualTriggerConceptHash) {
						mIndividualTriggerConceptHash = CObjectParameterizingAllocator< CBOXHASH<cint64,CConcept*>,CContext* >::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mBoxContext),mBoxContext);
					}
					mIndividualTriggerConceptHash->init(tBox->mIndividualTriggerConceptHash,mBoxContext);
				} else if (mIndividualTriggerConceptHash) {
					mIndividualTriggerConceptHash->clear();
				}
				if (tBox->mEquivConCandidateHash) {
					if (!mEquivConCandidateHash) {
						mEquivConCandidateHash = CObjectParameterizingAllocator< CBOXHASH<CConcept*,CConcept*>,CContext* >::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mBoxContext),mBoxContext);
					}
					mEquivConCandidateHash->init(tBox->mEquivConCandidateHash,mBoxContext);
				} else if (mEquivConCandidateHash) {
					mEquivConCandidateHash->clear();
				}
				if (tBox->mCandidateEquivConHash) {
					if (!mCandidateEquivConHash) {
						mCandidateEquivConHash = CObjectParameterizingAllocator< CBOXHASH<CConcept*,CConcept*>,CContext* >::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mBoxContext),mBoxContext);
					}
					mCandidateEquivConHash->init(tBox->mCandidateEquivConHash,mBoxContext);
				} else if (mCandidateEquivConHash) {
					mCandidateEquivConHash->clear();
				}
				if (tBox->mEquivConNonCandidateSet) {
					if (!mEquivConNonCandidateSet) {
						mEquivConNonCandidateSet = CObjectParameterizingAllocator< CBOXSET<CConcept*>,CContext* >::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mBoxContext),mBoxContext);
					}
					mEquivConNonCandidateSet->init(tBox->mEquivConNonCandidateSet,mBoxContext);
				} else if (mEquivConNonCandidateSet) {
					mEquivConNonCandidateSet->clear();
				}
				return this;
			}



			CBoxContext *CTBox::getBoxContext(bool create) {
				if (!mBoxContext && create) {
					mBoxContext = new CConcreteOntologyContextBase();
					mDeleteBoxContext = mBoxContext;
				}
				return mBoxContext;
			}

			QString CTBox::getTBoxString() {
				QString tboxString;
				if (concepts) {
					for (qint64 i = 0; i < concepts->getItemCount(); ++i) {
						CConcept *concept = concepts->getData(i);
						if (concept) {
							QString conceptString = QString::number(i);
							while (conceptString.length() <= 5) {
								conceptString.insert(0," ");
							}
							conceptString += ",";
							qint64 conOpCode = concept->getOperatorCode();
							QString iriClassNameString = CIRIName::getRecentIRIName(concept->getClassNameLinker());
							if (!iriClassNameString.isEmpty()) {
								conceptString += iriClassNameString + " = ";
							} else {
								conceptString += "  = ";
							}
							CRole *role = concept->getRole();
							QString paramString = QString::number(concept->getParameter());
							QString roleString;
							if (role) {
								roleString = QString::number(role->getRoleTag());
							}
							CIndividual* indi = concept->getNominalIndividual();
							QString indiString;
							if (indi) {
								indiString = QString::number(indi->getIndividualID());
							}

							if (conOpCode == CCNONE) {	
								conceptString += "( ATOM )";
							} else if (conOpCode == CCTOP) {	
								conceptString += "( TOP "+getConceptOperandString(concept->getOperandList())+")";
							} else if (conOpCode == CCBOTTOM) {	
								conceptString += "( BOTTOM )";
							} else if (conOpCode == CCAND) {
								conceptString += "( AND "+getConceptOperandString(concept->getOperandList())+")";
							} else if (conOpCode == CCAQAND) {
								conceptString += "( AQAND "+getConceptOperandString(concept->getOperandList())+")";
							} else if (conOpCode == CCIMPLAQAND) {
								conceptString += "( IMPLAQAND "+getConceptOperandString(concept->getOperandList())+")";
							} else if (conOpCode == CCBRANCHAQAND) {
								conceptString += "( BRANCHAQAND "+getConceptOperandString(concept->getOperandList())+")";
							} else if (conOpCode == CCPBINDAQAND) {
								conceptString += "( PBINDAQAND "+getConceptOperandString(concept->getOperandList())+")";
							} else if (conOpCode == CCVARBINDAQAND) {
								conceptString += "( VARBINDAQAND "+getConceptOperandString(concept->getOperandList())+")";
							} else if (conOpCode == CCVARPBACKAQAND) {
								conceptString += "( VARPBACKAQAND "+getConceptOperandString(concept->getOperandList())+")";
							} else if (conOpCode == CCEQ) {
								conceptString += "( EQ "+getConceptOperandString(concept->getOperandList())+")";
							} else if (conOpCode == CCSUB) {
								conceptString += "( SUB "+getConceptOperandString(concept->getOperandList())+")";
							} else if (conOpCode == CCIMPLTRIG) {
								conceptString += "( IMPTRIGGER "+getConceptOperandString(concept->getOperandList())+")";
							} else if (conOpCode == CCBRANCHTRIG) {
								conceptString += "( BRANCHTRIG "+getConceptOperandString(concept->getOperandList())+")";
							} else if (conOpCode == CCPBINDTRIG) {
								conceptString += "( PBINDTRIG "+getConceptOperandString(concept->getOperandList())+")";
							} else if (conOpCode == CCVARBINDTRIG) {
								conceptString += "( VARBINDTRIG "+getConceptOperandString(concept->getOperandList())+")";
							} else if (conOpCode == CCVARPBACKTRIG) {
								conceptString += "( VARPBACKTRIG "+getConceptOperandString(concept->getOperandList())+")";
							} else if (conOpCode == CCBACKACTIVTRIG) {
								conceptString += "( BACKACTIVTRIG "+getConceptOperandString(concept->getOperandList())+")";
							} else if (conOpCode == CCOR) {
								conceptString += "( OR "+getConceptOperandString(concept->getOperandList())+")";
							} else if (conOpCode == CCAQCHOOCE) {
								conceptString += "( AQCHOOCE "+getConceptOperandString(concept->getOperandList())+")";
							} else if (conOpCode == CCSOME) {
								conceptString += QString("( SOME %1. ").arg(roleString)+getConceptOperandString(concept->getOperandList())+")";
							} else if (conOpCode == CCSELF) {
								conceptString += QString("( SELF %1. ").arg(roleString)+")";
							} else if (conOpCode == CCNOMINAL) {
								conceptString += QString("( NOMINAL %1. ").arg(indiString)+")";
							} else if (conOpCode == CCAQSOME) {
								conceptString += QString("( AQSOME %1. ").arg(roleString)+getConceptOperandString(concept->getOperandList())+")";
							} else if (conOpCode == CCATLEAST) {
								conceptString += QString("( ATLEAST %1. [%2] ").arg(roleString).arg(paramString)+getConceptOperandString(concept->getOperandList())+")";
							} else if (conOpCode == CCATMOST) {
								conceptString += QString("( ATMOST %1. [%2] ").arg(roleString).arg(paramString)+getConceptOperandString(concept->getOperandList())+")";
							} else if (conOpCode == CCALL) {
								conceptString += QString("( ALL %1. ").arg(roleString)+getConceptOperandString(concept->getOperandList())+")";
							} else if (conOpCode == CCIMPLALL) {
								conceptString += QString("( IMPLALL %1. ").arg(roleString)+getConceptOperandString(concept->getOperandList())+")";
							} else if (conOpCode == CCBRANCHALL) {
								conceptString += QString("( BRANCHALL %1. ").arg(roleString)+getConceptOperandString(concept->getOperandList())+")";
							} else if (conOpCode == CCPBINDALL) {
								conceptString += QString("( PBINDALL %1. ").arg(roleString)+getConceptOperandString(concept->getOperandList())+")";
							} else if (conOpCode == CCVARBINDALL) {
								conceptString += QString("( VARBINDALL %1. ").arg(roleString)+getConceptOperandString(concept->getOperandList())+")";
							} else if (conOpCode == CCVARPBACKALL) {
								conceptString += QString("( VARPBACKALL %1. ").arg(roleString)+getConceptOperandString(concept->getOperandList())+")";
							} else if (conOpCode == CCIMPLAQALL) {
								conceptString += QString("( IMPLAQALL %1. ").arg(roleString)+getConceptOperandString(concept->getOperandList())+")";
							} else if (conOpCode == CCAQALL) {
								conceptString += QString("( AQALL %1. ").arg(roleString)+getConceptOperandString(concept->getOperandList())+")";
							} else if (conOpCode == CCBRANCHAQALL) {
								conceptString += QString("( BRANCHAQALL %1. ").arg(roleString)+getConceptOperandString(concept->getOperandList())+")";
							} else if (conOpCode == CCPBINDAQALL) {
								conceptString += QString("( PBINDAQALL %1. ").arg(roleString)+getConceptOperandString(concept->getOperandList())+")";
							} else if (conOpCode == CCVARBINDAQALL) {
								conceptString += QString("( VARBINDAQALL %1. ").arg(roleString)+getConceptOperandString(concept->getOperandList())+")";
							} else if (conOpCode == CCVARPBACKAQALL) {
								conceptString += QString("( VARPBACKAQALL %1. ").arg(roleString)+getConceptOperandString(concept->getOperandList())+")";
							} else if (conOpCode == CCNOT) {
								conceptString += "( NOT "+getConceptOperandString(concept->getOperandList())+")";
							}
							conceptString += "\r\n";
							tboxString += conceptString;
						}
					}
				}
				return tboxString;
			}


			QString CTBox::getConceptOperandString(CSortedNegLinker<CConcept *> *ops) {
				QString conceptString;
				while (ops) {
					if (ops->isNegated()) {
						conceptString += "-";
					}
					conceptString += QString::number(ops->getData()->getConceptTag()) + " ";
					ops = ops->getNext();
				}
				return conceptString;
			}



			CConcept* CTBox::getUniversalConnectionNominalValueConcept() {
				return mUnivConnNomValueCon;
			}

			CTBox* CTBox::setUniversalConnectionNominalValueConcept(CConcept* valueCon) {
				mUnivConnNomValueCon = valueCon;
				return this;
			}


		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude
