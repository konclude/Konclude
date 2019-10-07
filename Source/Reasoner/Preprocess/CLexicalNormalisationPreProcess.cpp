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

#include "CLexicalNormalisationPreProcess.h"


namespace Konclude {

	namespace Reasoner {

		namespace Preprocess {


			CLexicalNormalisationPreProcess::CLexicalNormalisationPreProcess() {
				mOntology = nullptr;
			}


			CLexicalNormalisationPreProcess::~CLexicalNormalisationPreProcess() {
			}


			void CLexicalNormalisationPreProcess::negateOperandConcepts(CConcept *concept, int firstNegationOperand) {

				CSortedNegLinker<CConcept *> *operands = concept->getOperandList();
				int operand = 0;
				while (operands) {
					if (operand++ >= firstNegationOperand) {
						operands->setNegated(!operands->isNegated());
					}
					operands = operands->getNext();
				}
			}




			CConcreteOntology *CLexicalNormalisationPreProcess::preprocess(CConcreteOntology *ontology, CPreProcessContext* context) {

				bool skipForELOntologies = CConfigDataReader::readConfigBoolean(context->getConfiguration(),"Konclude.Calculation.Preprocessing.LexicalNormalization.SkipForELFragment",true);
				bool nonELConstructsUsed = ontology->getDataBoxes()->getExpressionDataBoxMapping()->getBuildConstructFlags()->isNonELConstructUsed();

				mOntology = nullptr;

				if (nonELConstructsUsed || !skipForELOntologies) {

					mOntology = ontology;

					CTBox* tbox = ontology->getDataBoxes()->getTBox();
					CABox *abox = ontology->getDataBoxes()->getABox();
					CRBox *rbox = ontology->getDataBoxes()->getRBox();
					CMBox *mbox = ontology->getDataBoxes()->getMBox();

					mBoxMemMan = CContext::getMemoryAllocationManager(ontology->getDataBoxes()->getBoxContext());
					CNominalSchemaTemplateVector* nomSchTemplVector = mbox->getNominalSchemaTemplateVector(false);


					concepts = tbox->getConceptVector();
					qint64 conceptCount = concepts->getItemCount();

					roles = rbox->getRoleVector();
					qint64 roleCount = rbox->getRoleCount();


					for (qint64 i = 0; i < conceptCount; ++i) {
						CConcept *concept = concepts->getLocalData(i);

						if (concept) {

							qint64 cOpCode = concept->getDefinitionOperatorTag();

							if (cOpCode == CCOR) {
								concept->setMappingNegation(true);
								concept->setOperatorTag(CCAND);
								negateOperandConcepts(concept,0);
							} else if (cOpCode == CCSOME) {
								concept->setMappingNegation(true);
								concept->setOperatorTag(CCALL);
								negateOperandConcepts(concept,0);
							}
						}
					}

					topConcept = concepts->getData(1);

					for (qint64 i = 0; i < conceptCount; ++i) {
						CConcept *concept = concepts->getLocalData(i);

						if (concept) {

							CSortedNegLinker<CConcept *> *operands = concept->getOperandList();
							while (operands) {
								CConcept *opConcept = operands->getData();
								if (opConcept->hasMappingNegation()) {
									operands->setNegated(!operands->isNegated());
								} 
								if (opConcept->getDefinitionOperatorTag() == CCBOTTOM) {
									operands->setData(topConcept);
									operands->setNegated(!operands->isNegated());
								}
								if (nomSchTemplVector) {
									if (opConcept->getOperatorCode() == CCNOMTEMPLREF || opConcept->getOperatorCode() == CCPBINDGROUND || opConcept->getOperatorCode() == CCVARBINDGROUND) {
										CNominalSchemaTemplate* nsTemplate = nomSchTemplVector->getData(opConcept->getParameter());
										if (nsTemplate) {
											if (nsTemplate->getTemplateConcept()->hasMappingNegation()) {
												operands->setNegated(!operands->isNegated());
											}
										}
									}
								}
								operands = operands->getNext();
							}
						}
					}
					for (qint64 i = 0; i < roleCount; ++i) {
						CRole *role = roles->getLocalData(i);
						if (role) {
							CSortedNegLinker<CConcept *> *domainConIt = role->getDomainConceptList();
							while (domainConIt) {
								CConcept *domainConcept = domainConIt->getData();
								if (domainConcept->hasMappingNegation()) {
									domainConIt->setNegated(!domainConIt->isNegated());
								}
								domainConIt = domainConIt->getNext();
							}
							CSortedNegLinker<CConcept *> *rangeConIt = role->getRangeConceptList();
							while (rangeConIt) {						
								CConcept *rangeConcept = rangeConIt->getData();
								if (rangeConcept->hasMappingNegation()) {
									rangeConIt->setNegated(!rangeConIt->isNegated());
								}
								rangeConIt = rangeConIt->getNext();
							}
						}
					}


					if (abox) {
						individuals = abox->getIndividualVector();
						qint64 individualCount = abox->getIndividualCount();
						for (qint64 i = 0; i < individualCount; ++i) {
							CIndividual *indi = individuals->getLocalData(i);
							if (indi) {
								CConceptAssertionLinker* assertionConLinker = indi->getAssertionConceptLinker();
								while (assertionConLinker) {
									if (assertionConLinker->getData()->hasMappingNegation()) {
										assertionConLinker->setNegation(!assertionConLinker->isNegated());
									}
									assertionConLinker = assertionConLinker->getNext();
								}
							}
						}
					}


					replacementHash = new QHash<CConceptUnambiguousHasher,CConcept*>();
					processedSet = new QSet<CConcept*>();
					conceptReplaceHash = new QHash<CConcept*,QPair<bool,CConcept*> >();
					processingList = new QList<CConcept*>();

					for (qint64 i = 0; i < conceptCount; ++i) {
						CConcept *concept = concepts->getLocalData(i);
						if (concept) {
							if (!processedSet->contains(concept)) {
								//QString iriClassNameString2 = CIRIName::getRecentIRIName(concept->getClassNameLinker());
								//if (iriClassNameString2 == "http://www.owllink.org/testsuite/particle-D#Proton") {
								//	bool bug = true;
								//}
								addNormalizeConcept(concept);
								processedSet->insert(concept);
								normalizeListedConcepts();
							}
						}
					}

					for (qint64 i = 0; i < roleCount; ++i) {
						CRole *role = roles->getLocalData(i);
						if (role) {
							CSortedNegLinker<CConcept *> *domainConIt = role->getDomainConceptList();
							bool domainModified = false;
							while (domainConIt) {
								CConcept *domainConcept = domainConIt->getData();
								if (conceptReplaceHash->contains(domainConcept)) {
									QPair<bool,CConcept *> negRepConcept(conceptReplaceHash->value(domainConcept));
									domainConcept = negRepConcept.second;
									if (negRepConcept.first) {
										domainConIt->setNegated(!domainConIt->isNegated());
									}
									domainConIt->setData(domainConcept);
									domainModified = true;
								}
								domainConIt = domainConIt->getNext();
							}
							if (domainModified) {
								domainConIt = role->getDomainConceptList();
								domainConIt->resortLinkerData();
							}
							CSortedNegLinker<CConcept *> *rangeConIt = role->getRangeConceptList();
							bool rangeModified = false;
							while (rangeConIt) {
								CConcept *rangeConcept = rangeConIt->getData();
								if (conceptReplaceHash->contains(rangeConcept)) {
									QPair<bool,CConcept *> negRepConcept(conceptReplaceHash->value(rangeConcept));
									rangeConcept = negRepConcept.second;
									if (negRepConcept.first) {
										rangeConIt->setNegated(!rangeConIt->isNegated());
									}
									rangeConIt->setData(rangeConcept);
									domainModified = true;
								}
								rangeConIt = rangeConIt->getNext();
							}
							if (rangeModified) {
								rangeConIt = role->getRangeConceptList();
								rangeConIt->resortLinkerData();
							}

						}
					}

					if (abox) {
						individuals = abox->getIndividualVector();
						qint64 individualCount = abox->getIndividualCount();
						for (qint64 i = 0; i < individualCount; ++i) {
							CIndividual *indi = individuals->getLocalData(i);
							if (indi) {

								CConceptAssertionLinker* assertionConLinker = indi->getAssertionConceptLinker();
								while (assertionConLinker) {
									if (conceptReplaceHash->contains(assertionConLinker->getData())) {
										QPair<bool,CConcept *> negRepConcept(conceptReplaceHash->value(assertionConLinker->getData()));
										assertionConLinker->setData(negRepConcept.second);
										assertionConLinker->setNegation(negRepConcept.first);
									}
									assertionConLinker = assertionConLinker->getNext();
								}

							}
						}
					}

					delete replacementHash;
					delete processedSet;
					delete conceptReplaceHash;
					delete processingList;
				}


				return ontology;
			}


			CConcreteOntology* CLexicalNormalisationPreProcess::continuePreprocessing() {
				if (mOntology) {
				}
				return mOntology;
			}

			CSortedNegLinker<CConcept*>* CLexicalNormalisationPreProcess::resortConceptLinkersByTagging(CSortedNegLinker<CConcept*>* conceptLinkers) {
				CSortedNegLinker<CConcept*>* newConceptLinkers = nullptr;
				while (conceptLinkers) {
					CSortedNegLinker<CConcept*>* tmpConceptLinker = conceptLinkers;
					conceptLinkers = conceptLinkers->getNext();
					tmpConceptLinker->setNext(nullptr);
					if (!newConceptLinkers) {
						newConceptLinkers = tmpConceptLinker;
					} else {
						newConceptLinkers = newConceptLinkers->insertSortedNextSorted(tmpConceptLinker);
					}
				}
				return newConceptLinkers;
			}

			void CLexicalNormalisationPreProcess::addNormalizeConcept(CConcept *concept) {
				processingList->prepend(concept);
			}


			void CLexicalNormalisationPreProcess::normalizeListedConcepts() {
				while (!processingList->isEmpty()) {
					CConcept* concept = processingList->first();
					cint64 conceptTag = concept->getConceptTag();
					bool isLocal = concepts->hasLocalData(conceptTag);
					bool conceptNormalizationAdded = false;
					CSortedNegLinker<CConcept*>* opLinkerIt = concept->getOperandList();
					if (isLocal) {
						while (opLinkerIt) {
							CConcept* opConcept = concepts->getData(opLinkerIt->getData()->getConceptTag());
							if (!processedSet->contains(opConcept)) {
								conceptNormalizationAdded = true;
								addNormalizeConcept(opConcept);
								processedSet->insert(opConcept);
							}
							opLinkerIt = opLinkerIt->getNext();
						}
					}

					if (!conceptNormalizationAdded) {
						processingList->removeFirst();
						normalizeConcept(concept,isLocal);
					}
				}
			}


			void CLexicalNormalisationPreProcess::normalizeConcept(CConcept *concept, bool isLocal) {
				CSortedNegLinker<CConcept *> *opList = 0;

				cint64 conceptTag = concept->getConceptTag();
				bool implicTriggers = concept->getOperatorCode() == CCIMPL || concept->getOperatorCode() == CCIMPLALL;
				bool nonReplace = false;
				if (implicTriggers) {
					nonReplace = true;
				}

				if (isLocal) {

					CSortedNegLinker<CConcept *> *conceptAddList = 0;

					CSortedNegLinker<CConcept *> *lastOpList = 0;
					bool opConTagsModified = false;
					opList = concept->getOperandList();
					while (opList) {
						CConcept *opConcept = concepts->getData(opList->getData()->getConceptTag());

						if (!nonReplace && conceptReplaceHash->contains(opConcept)) {
							QPair<bool,CConcept *> negRepConcept(conceptReplaceHash->value(opConcept));
							opConcept = negRepConcept.second;
							if (negRepConcept.first) {
								opList->setNegated(!opList->isNegated());
							}
							opList->setData(opConcept);
							opConTagsModified = true;
						}

						bool removeThisOp = false;

						cint64 opCode = concept->getOperatorCode();


						if (opCode == CCAND) {

							cint64 operandOpCode = opConcept->getOperatorCode();

							if ((operandOpCode == CCAND) && !opList->isNegated()) {
								// remove concept from operand list and copy operand concepts from removed concept to this concept as operands
								CSortedNegLinker<CConcept *> *opOpList = opConcept->getOperandList();
								while (opOpList) {
									concept->incOperandCount();
									conceptAddList = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mBoxMemMan)->init(opOpList->getData(),opOpList->isNegated())->insertSortedNextSorted(conceptAddList);
									opOpList = opOpList->getNext();
								}
								removeThisOp = true;
							} else if (opConcept->getDefinitionOperatorTag() == CCTOP && !opList->isNegated()) {
								removeThisOp = true;
							}

						}

						if (removeThisOp) {
							concept->incOperandCount(-1);
							if (!lastOpList) {
								concept->setOperandList(opList->getNext());
								opList = opList->getNext();
							} else {
								opList = opList->getNext();
								lastOpList->removeNext();
							}
						} else {
							lastOpList = opList;
							opList = opList->getNext();
						}
						
					}
					if (conceptAddList) {
						concept->setOperandList(conceptAddList->insertSortedNextSorted(concept->getOperandList()));
					}
					if (opConTagsModified) {
						concept->setOperandList(resortConceptLinkersByTagging(concept->getOperandList()));
					}

				}
				
				bool isClash = false;

				CSortedNegLinker<CConcept *> *lastOpList = 0;
				opList = concept->getOperandList();
				while (opList) {
					CConcept *opConcept = concepts->getData(opList->getData()->getConceptTag());

					if (isLocal) {
						if (!nonReplace && conceptReplaceHash->contains(opConcept)) {
							QPair<bool,CConcept *> negRepConcept(conceptReplaceHash->value(opConcept));
							opConcept = negRepConcept.second;
							if (negRepConcept.first) {
								opList->setNegated(!opList->isNegated());
							}
							opList->setData(opConcept);
						}
					}

					qint64 opCCode = opConcept->getDefinitionOperatorTag();
					if (concept->getProcessingOperatorTag() == CCAND && opCCode == CCTOP && opList->isNegated()) {
						isClash = true;
					}

					if (concept->getProcessingOperatorTag() == CCSOME && opCCode == CCTOP && opList->isNegated()) {
						isClash = true;
					}
					if (concept->getProcessingOperatorTag() == CCATLEAST && opCCode == CCTOP && opList->isNegated()) {
						isClash = true;
					}

					if (concept->getProcessingOperatorTag() == CCAND && lastOpList && lastOpList->getData()->getConceptTag() == opList->getData()->getConceptTag()) {
						if (lastOpList->isNegated() != opList->isNegated()) {
							isClash = true;
						}
					}
					
					lastOpList = opList;
					opList = opList->getNext();
				}

				if (isClash) {
					conceptReplaceHash->insert(concept,QPair<bool,CConcept *>(true,topConcept));
				} else {
					CConceptUnambiguousHasher conUnHasher(getConceptHasher(concept));
					if (!replacementHash->contains(conUnHasher)) {
						replacementHash->insert(conUnHasher,concept);
					} else {
						conceptReplaceHash->insert(concept,QPair<bool,CConcept *>(false,replacementHash->value(conUnHasher)));
					}
				}
			}



			CConceptUnambiguousHasher CLexicalNormalisationPreProcess::getConceptHasher(CConcept *concept) {
				cint64 opCode = concept->getOperatorCode();
				CConceptOperator* conOperator = concept->getConceptOperator();
				if (opCode == CCATOM || opCode == CCSUB || opCode == CCEQ || opCode == CCDATATYPE || opCode == CCDATALITERAL || opCode == CCDATARESTRICTION || conOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_TRIG_TYPE)) {
					return CConceptUnambiguousHasher(concept,true);
				} else {
					return CConceptUnambiguousHasher(concept,false);
				}
			}



		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude
