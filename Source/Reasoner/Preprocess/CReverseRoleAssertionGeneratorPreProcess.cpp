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

#include "CReverseRoleAssertionGeneratorPreProcess.h"


namespace Konclude {

	namespace Reasoner {

		namespace Preprocess {


			CReverseRoleAssertionGeneratorPreProcess::CReverseRoleAssertionGeneratorPreProcess() {
			}


			CReverseRoleAssertionGeneratorPreProcess::~CReverseRoleAssertionGeneratorPreProcess() {
			}



			bool CReverseRoleAssertionGeneratorPreProcess::isAssertionChanged(CIndividual* indi, CIndividual* prevIndi, CRoleAssertionLinker* roleAssLinker, CPreProcessContext* context) {
				if (!prevIndi) {
					return true;
				} else {
					CIndividual* destIndi = roleAssLinker->getIndividual();
					CRole* role = roleAssLinker->getRole();
					for (CRoleAssertionLinker* prevRoleAssLinkerIt = prevIndi->getAssertionRoleLinker(); prevRoleAssLinkerIt; prevRoleAssLinkerIt = prevRoleAssLinkerIt->getNext()) {
						if (prevRoleAssLinkerIt->getIndividual()->getIndividualID() == destIndi->getIndividualID() && prevRoleAssLinkerIt->getRole() == role) {
							return false;
						}
					}
					return true;
				}
			}



			CConcreteOntology *CReverseRoleAssertionGeneratorPreProcess::preprocess(CConcreteOntology *ontology, CPreProcessContext* context) {


				CTBox* tbox = ontology->getDataBoxes()->getTBox();
				CABox *abox = ontology->getDataBoxes()->getABox();
				CRBox *rbox = ontology->getDataBoxes()->getRBox();
				CMBox *mbox = ontology->getDataBoxes()->getMBox();

				mBoxMemMan = CContext::getMemoryAllocationManager(ontology->getDataBoxes()->getBoxContext());

				mConVec = tbox->getConceptVector();
				cint64 conceptCount = mConVec->getItemCount();

				mRoleVec = rbox->getRoleVector();
				cint64 roleCount = rbox->getRoleCount();


				mIndiVec = abox->getIndividualVector(false);
				if (mIndiVec) {
					cint64 indiCount = mIndiVec->getItemCount();

					if (indiCount > 0) {

						if (ontology->getDataBoxes()->isInitialBuild()) {

							for (qint64 i = 0; i < indiCount; ++i) {
								CIndividual* indi = mIndiVec->getLocalData(i);

								if (indi) {
									for (CRoleAssertionLinker* roleAssLinkerIt = indi->getAssertionRoleLinker(); roleAssLinkerIt; roleAssLinkerIt = roleAssLinkerIt->getNext()) {
										CIndividual* otherIndi = roleAssLinkerIt->getIndividual();
										CReverseRoleAssertionLinker* revAssLinker = CObjectAllocator<CReverseRoleAssertionLinker>::allocateAndConstruct(mBoxMemMan);
										revAssLinker->initReverseRoleAssertionLinker(roleAssLinkerIt,indi);
										otherIndi->addReverseAssertionRoleLinker(revAssLinker);
									}
								}
							}
						} else {
							bool deletedABoxAssertions = ontology->getIncrementalRevisionData()->getAxiomChangeData()->hasDeletedABoxAxioms();
							QSet<cint64> directUpdateIndividualSet;
							QSet<cint64> reverseAssUpdateIndividualSet;
							QSet<CReverseRoleAssertionLinker*> validAssIndividualSet;

							for (qint64 i = 0; i < indiCount; ++i) {
								CIndividual* indi = mIndiVec->getLocalData(i);
								if (indi) {
									directUpdateIndividualSet.insert(i);
									if (deletedABoxAssertions) {
										CIndividual* prevIndividual = mIndiVec->getReferenceData(i);
										for (CRoleAssertionLinker* roleAssLinkerIt = indi->getAssertionRoleLinker(); roleAssLinkerIt; roleAssLinkerIt = roleAssLinkerIt->getNext()) {
											CIndividual* otherIndi = roleAssLinkerIt->getIndividual();
											if (isAssertionChanged(indi,prevIndividual,roleAssLinkerIt,context)) {
												reverseAssUpdateIndividualSet.insert(otherIndi->getIndividualID());
											}
										}
										if (prevIndividual) {
											for (CRoleAssertionLinker* prevRoleAssLinkerIt = prevIndividual->getAssertionRoleLinker(); prevRoleAssLinkerIt; prevRoleAssLinkerIt = prevRoleAssLinkerIt->getNext()) {
												CIndividual* otherIndi = prevRoleAssLinkerIt->getIndividual();
												if (isAssertionChanged(prevIndividual,indi,prevRoleAssLinkerIt,context)) {
													reverseAssUpdateIndividualSet.insert(otherIndi->getIndividualID());
												}
											}
										}
										if (!indi->getIndividualNominalConcept() && prevIndividual->getIndividualNominalConcept()) {
											indi->setIndividualNominalConcept(prevIndividual->getIndividualNominalConcept());											
										}
									}
								}
							}

							for (QSet<cint64>::const_iterator indiIt = directUpdateIndividualSet.constBegin(), indiItEnd = directUpdateIndividualSet.constEnd(); indiIt != indiItEnd; ++indiIt) {
								cint64 indiID = *indiIt;
								CIndividual* individual = mIndiVec->getLocalData(indiID);
								CIndividual* prevIndividual = mIndiVec->getReferenceData(individual->getIndividualID());
								if (prevIndividual) {
									for (CReverseRoleAssertionLinker* prevRevRoleAssLinkerIt = prevIndividual->getReverseAssertionRoleLinker(); prevRevRoleAssLinkerIt; prevRevRoleAssLinkerIt = prevRevRoleAssLinkerIt->getNext()) {
										CIndividual* otherIndi = prevRevRoleAssLinkerIt->getIndividual();
										if (!directUpdateIndividualSet.contains(otherIndi->getIndividualID())) {
											CReverseRoleAssertionLinker* revAssLinker = CObjectAllocator<CReverseRoleAssertionLinker>::allocateAndConstruct(mBoxMemMan);
											revAssLinker->initReverseRoleAssertionLinker(prevRevRoleAssLinkerIt->getRoleAssertion(),otherIndi);
											individual->addReverseAssertionRoleLinker(revAssLinker);
										}
									}
								}
							}



							for (QSet<cint64>::const_iterator indiIt = reverseAssUpdateIndividualSet.constBegin(), indiItEnd = reverseAssUpdateIndividualSet.constEnd(); indiIt != indiItEnd; ++indiIt) {
								cint64 indiID = *indiIt;
								bool local = false;
								CIndividual* individual = mIndiVec->getData(indiID,&local);
								if (!local) {
									CIndividual* locIndividual = CConceptRoleIndividualLocator::getLocatedIndividual(individual,ontology);
									locIndividual->setAssertionRoleLinker(individual->getAssertionRoleLinker());

									for (CReverseRoleAssertionLinker* prevRevRoleAssLinkerIt = individual->getReverseAssertionRoleLinker(); prevRevRoleAssLinkerIt; prevRevRoleAssLinkerIt = prevRevRoleAssLinkerIt->getNext()) {
										CIndividual* otherIndi = prevRevRoleAssLinkerIt->getIndividual();
										if (!directUpdateIndividualSet.contains(otherIndi->getIndividualID())) {
											CReverseRoleAssertionLinker* revAssLinker = CObjectAllocator<CReverseRoleAssertionLinker>::allocateAndConstruct(mBoxMemMan);
											revAssLinker->initReverseRoleAssertionLinker(prevRevRoleAssLinkerIt->getRoleAssertion(),otherIndi);
											locIndividual->addReverseAssertionRoleLinker(revAssLinker);
										}
									}
								}
							}

							for (QSet<cint64>::const_iterator indiIt = directUpdateIndividualSet.constBegin(), indiItEnd = directUpdateIndividualSet.constEnd(); indiIt != indiItEnd; ++indiIt) {
								cint64 indiID = *indiIt;
								CIndividual* individual = mIndiVec->getLocalData(indiID);
								CIndividual* prevIndividual = mIndiVec->getReferenceData(indiID);
								for (CRoleAssertionLinker* roleAssLinkerIt = individual->getAssertionRoleLinker(); roleAssLinkerIt; roleAssLinkerIt = roleAssLinkerIt->getNext()) {
									CIndividual* otherIndi = roleAssLinkerIt->getIndividual();
									if (directUpdateIndividualSet.contains(otherIndi->getIndividualID()) || reverseAssUpdateIndividualSet.contains(otherIndi->getIndividualID())) {

										//CIndividual* locOtherIndi = mIndiVec->getLocalData(otherIndi->getIndividualID());
										CIndividual* locOtherIndi = CConceptRoleIndividualLocator::getLocatedIndividual(otherIndi,ontology);
										CReverseRoleAssertionLinker* revAssLinker = CObjectAllocator<CReverseRoleAssertionLinker>::allocateAndConstruct(mBoxMemMan);
										revAssLinker->initReverseRoleAssertionLinker(roleAssLinkerIt,individual);
										locOtherIndi->addReverseAssertionRoleLinker(revAssLinker);
									}
								}
							}



						}

					}
				}



				return ontology;
			}



		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude
