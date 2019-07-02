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

#include "CConceptNominalSchemaGroundingHandler.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Algorithm {

				CConceptNominalSchemaGroundingHandler::CConceptNominalSchemaGroundingHandler() {
					mConfReuseGroundedNominalSchemaConcepts = true;
				}



				CSortedNegLinker<CConcept*>* CConceptNominalSchemaGroundingHandler::getGroundingConceptLinker(CIndividualProcessNode* processNode, CPropagationBindingSet* propBindSet, CConcept* concept, bool negated, CPROCESSINGHASH<CConcept*,CPropagationBindingDescriptor*>*& groundedConPropBindDesHash, CPROCESSINGHASH<CVariable*,CPropagationBindingDescriptor*>*& additionalPropBindDesHash, CCalculationAlgorithmContext* calcAlgContext) {
					CSortedNegLinker<CConcept*>* newLinker = nullptr;

					mDataBox = calcAlgContext->getUsedProcessingDataBox();
					CProcessContext* context = calcAlgContext->getUsedProcessContext();
					mMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
					CMBox* mBox = mDataBox->getOntology()->getDataBoxes()->getMBox();
					CTBox* tBox = mDataBox->getOntology()->getDataBoxes()->getTBox();
					mConceptVec = mDataBox->getExtendedConceptVector(false);
					mReplaceHash = mDataBox->getConceptNominalSchemaGroundingHash(false);
					CNominalSchemaTemplateVector* nomSchVec = mBox->getNominalSchemaTemplateVector(false);
					mLocalizedExtensions = false;

					CPROCESSINGHASH<CConcept*,TConceptPropagationBindingPair> nominalSchemaVarBindedNominalHash(calcAlgContext->getUsedTaskProcessorContext());
					CPROCESSINGSET<CConcept*>* allNominalConceptSet = nullptr;

					cint64 nomSchTemplID = concept->getParameter();


					if (nomSchVec) {
						CNominalSchemaTemplate* nomSchTempl = nomSchVec->getData(nomSchTemplID);
						CConcept* templateConcept = nomSchTempl->getTemplateConcept();
						CBOXHASH<CConcept*,CConcept*>* templateConceptNomSchVarHash = nomSchTempl->getTemplateConceptNominalSchemaConceptHash();
						CBOXSET<CConcept*>* templateConceptNomSchVarSet = nomSchTempl->getNominalSchemaConceptSet();

						CPropagationBindingMap* propBindMap = propBindSet->getPropagationBindingMap();
						for (CPropagationBindingMap::iterator it = propBindMap->begin(), itEnd = propBindMap->end(); it != itEnd; ++it) {
							CPropagationBindingMapData& propBindData = it.value();
							CPropagationBindingDescriptor* propBindDes = propBindData.getPropagationBindingDescriptor();
							CPropagationBinding* propBinding = propBindDes->getPropagationBinding();
							CVariable* variable = propBinding->getBindedVariable();
							CIndividualProcessNode* bindedIndiNode = propBinding->getBindedIndividual();
							if (variable->isNominalVariable()) {
								CConcept* nomSchVarConcept = variable->getNominalVariableConcept();
								if (templateConceptNomSchVarSet->contains(nomSchVarConcept)) {
									CConcept* nominalConcept = getNominalConcept(bindedIndiNode,true,calcAlgContext);
									if (nominalConcept) {
										nominalSchemaVarBindedNominalHash.insertMulti(nomSchVarConcept,TConceptPropagationBindingPair(nominalConcept,propBindDes));
									}
								}
							}
						}

						for (CBOXSET<CConcept*>::iterator it = templateConceptNomSchVarSet->begin(), itEnd = templateConceptNomSchVarSet->end(); it != itEnd; ++it) {
							CConcept* nomSchVarConcept = *it;
							if (!nominalSchemaVarBindedNominalHash.contains(nomSchVarConcept)) {
								collectAllNominalConcepts(allNominalConceptSet,calcAlgContext);
								for (CPROCESSINGSET<CConcept*>::const_iterator nomIt = allNominalConceptSet->constBegin(), nomItEnd = allNominalConceptSet->constEnd(); nomIt != nomItEnd; ++nomIt) {
									CConcept* nominalConcept(*nomIt);
									nominalSchemaVarBindedNominalHash.insertMulti(nomSchVarConcept,TConceptPropagationBindingPair(nominalConcept,nullptr));
								}
							}
						}

						newLinker = createNominalSchemaGroundingConcepts(processNode,nomSchTempl->getTemplateConcept(),negated,&nominalSchemaVarBindedNominalHash,groundedConPropBindDesHash,nomSchTempl,calcAlgContext);

						if (newLinker) {
							if (!additionalPropBindDesHash) {
								additionalPropBindDesHash = CObjectParameterizingAllocator< CPROCESSINGHASH<CVariable*,CPropagationBindingDescriptor*>,CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(),calcAlgContext->getUsedTaskProcessorContext());;
							}
							for (CPropagationBindingMap::iterator it = propBindMap->begin(), itEnd = propBindMap->end(); it != itEnd; ++it) {
								CPropagationBindingMapData& propBindData = it.value();
								CPropagationBindingDescriptor* propBindDes = propBindData.getPropagationBindingDescriptor();
								CPropagationBinding* propBinding = propBindDes->getPropagationBinding();
								CVariable* variable = propBinding->getBindedVariable();
								additionalPropBindDesHash->insert(variable,propBindDes);
							}
							
						}
					}					

					return newLinker;
				}


				void CConceptNominalSchemaGroundingHandler::collectAllNominalConcepts(CPROCESSINGSET<CConcept*>*& allNominalConceptSet, CCalculationAlgorithmContext* calcAlgContext) {
					if (!allNominalConceptSet) {
						allNominalConceptSet = CObjectParameterizingAllocator< CPROCESSINGSET<CConcept*>,CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(),calcAlgContext->getUsedTaskProcessorContext());
						CIndividualVector* individualVector = calcAlgContext->getProcessingDataBox()->getOntology()->getABox()->getIndividualVector(false);
						cint64 individualCount = 0;
						if (individualVector) {
							individualCount = individualVector->getItemCount();
							// TODO: only active individuals
							for (cint64 i = 0; i < individualCount; ++i) {
								CIndividual* individual = individualVector->getData(i);
								for (CConceptAssertionLinker* assConLinkerIt = individual->getAssertionConceptLinker(); assConLinkerIt; assConLinkerIt = assConLinkerIt->getNext()) {
									CConcept* assCon = assConLinkerIt->getData();
									bool assConNegation = assConLinkerIt->isNegated();
									cint64 assConOpCode = assCon->getOperatorCode();
									if (!assConNegation && assConOpCode == CCNOMINAL) {
										allNominalConceptSet->insert(assCon);
									}
								}
							}
						}
					}
				}


				CConcept* CConceptNominalSchemaGroundingHandler::getNominalConcept(CIndividualProcessNode* processNode, bool forceNotPruned, CCalculationAlgorithmContext* calcAlgContext) {
					CIndividualProcessNodeVector* mIndiVector = calcAlgContext->getUsedProcessingDataBox()->getIndividualProcessNodeVector();
					CIndividualProcessNode* indiNode = mIndiVector->getData(processNode->getIndividualID());
					//while (indiNode->getIndividualID() != indiNode->getMergedIntoIndividualNodeID()) {
					//	indiNode = mIndiVector->getData(indiNode->getMergedIntoIndividualNodeID());
					//}
					CConcept* nomConcept = nullptr;
					if (!forceNotPruned || !indiNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFPURGEDBLOCKED)) {
						CIndividual* nomIndi = indiNode->getNominalIndividual();
						CConceptAssertionLinker* assLinker = nomIndi->getAssertionConceptLinker();
						while (assLinker && !nomConcept) {
							CConcept* assConcept = assLinker->getData();
							bool assNegation = assLinker->isNegated();
							if (!assNegation && assConcept->getOperatorCode() == CCNOMINAL) {
								nomConcept = assConcept;
							}
							assLinker = assLinker->getNext();
						}
					}
					return nomConcept;
				}




				void CConceptNominalSchemaGroundingHandler::addConceptOperand(CConcept* concept, CConcept* opConcept, bool negated) {
					CSortedNegLinker<CConcept*>* opConLinker = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mMemMan);
					opConLinker->init(opConcept,negated);
					concept->addOperandLinker(opConLinker);
					concept->incOperandCount();
				}


				CConcept* CConceptNominalSchemaGroundingHandler::createNominalSchemaConceptCopy(CConcept* concept) {
					CConcept* conceptCopy = CObjectAllocator< CConcept >::allocateAndConstruct(mMemMan);
					conceptCopy->initConceptCopy(concept);
					conceptCopy->setOperandCount(0);
					conceptCopy->setConceptTag(mConceptVec->getItemCount());
					mConceptVec->setData(conceptCopy->getConceptTag(),conceptCopy);
					return conceptCopy;
				}


				CConcept* CConceptNominalSchemaGroundingHandler::createGroundedNominalSchemaConcept(CConcept* concept, CBOXHASH<CConcept*,CConcept*>* templConNomSchConHash, QHash<CConcept*, CPROCESSINGHASH<CConcept*,TConceptPropagationBindingPair>::const_iterator >* nomSchConIndividualItHash, CCalculationAlgorithmContext* calcAlgContext) {

					if (templConNomSchConHash->contains(concept)) {

						cint64 opCode = concept->getOperatorCode();
						if (opCode == CCNOMVAR) {
							CPROCESSINGHASH<CConcept*,TConceptPropagationBindingPair>::const_iterator nomVarIt = nomSchConIndividualItHash->value(concept);
							CConcept* nominalConcept = nomVarIt.value().first;
							return nominalConcept;
						}

						
						if (mConfReuseGroundedNominalSchemaConcepts && mReplaceHash) {
							CConceptNominalSchemaGroundingData mHashedTestData(calcAlgContext->getUsedTaskProcessorContext());
							mHashedTestData.setGroundingConcept(concept);
							CBOXHASH<CConcept*,CConcept*>::const_iterator it = templConNomSchConHash->constFind(concept), itEnd = templConNomSchConHash->constEnd();
							while (it != itEnd && it.key() == concept) {
								CConcept* nomSchemConcept = *it;
								CPROCESSINGHASH<CConcept*,TConceptPropagationBindingPair>::const_iterator nomVarIt = nomSchConIndividualItHash->value(nomSchemConcept);
								CConcept* indiConcept = nomVarIt.value().first;
								mHashedTestData.addBindedNominalSchemaConcept(indiConcept);
								++it;
							}
							CConceptNominalSchemaGroundingData* replaceConData = mReplaceHash->value(CConceptNominalSchemaGroundingHasher(&mHashedTestData),nullptr);
							if (replaceConData) {
								return replaceConData->getGroundedConcept();
							}
						}

						forceExtensionLocalisation(calcAlgContext);
						CConcept* copiedConcept = createNominalSchemaConceptCopy(concept);

						for (CSortedNegLinker<CConcept*>* opConLinkerIt = concept->getOperandList(); opConLinkerIt; opConLinkerIt = opConLinkerIt->getNext()) {
							CConcept* opConcept = opConLinkerIt->getData();
							bool opConNegation = opConLinkerIt->isNegated();

							CConcept* newOpConcept = createGroundedNominalSchemaConcept(opConcept,templConNomSchConHash,nomSchConIndividualItHash,calcAlgContext);
							addConceptOperand(copiedConcept,newOpConcept,opConNegation);
						}

						if (mConfReuseGroundedNominalSchemaConcepts) {
							CConceptNominalSchemaGroundingData* mHashedTestData = CObjectParameterizingAllocator< CConceptNominalSchemaGroundingData,CProcessContext* >::allocateAndConstructAndParameterize(mMemMan,calcAlgContext->getUsedProcessContext());
							mHashedTestData->setGroundingConcept(concept);
							CBOXHASH<CConcept*,CConcept*>::const_iterator it = templConNomSchConHash->constFind(concept), itEnd = templConNomSchConHash->constEnd();
							while (it != itEnd && it.key() == concept) {
								CConcept* nomSchemConcept = *it;
								CPROCESSINGHASH<CConcept*,TConceptPropagationBindingPair>::const_iterator nomVarIt = nomSchConIndividualItHash->value(nomSchemConcept);
								CConcept* indiConcept = nomVarIt.value().first;
								mHashedTestData->addBindedNominalSchemaConcept(indiConcept);
								++it;
							}
							mHashedTestData->setGroundedConcept(copiedConcept);
							mReplaceHash->insert(CConceptNominalSchemaGroundingHasher(mHashedTestData),mHashedTestData);
						}

						return copiedConcept;
					} else {
						return concept;
					}

				}


				bool CConceptNominalSchemaGroundingHandler::forceExtensionLocalisation(CCalculationAlgorithmContext* calcAlgContext) {
					if (!mLocalizedExtensions) {
						mLocalizedExtensions = true;
						mConceptVec = mDataBox->getExtendedConceptVector(true);
						mReplaceHash = mDataBox->getConceptNominalSchemaGroundingHash(true);
						return true;
					}
					return false;
				}


				CSortedNegLinker<CConcept*>* CConceptNominalSchemaGroundingHandler::createNominalSchemaGroundingConcepts(CIndividualProcessNode* indiProcessNode, CConcept* concept, bool negated, CPROCESSINGHASH<CConcept*, TConceptPropagationBindingPair >* nominalSchemaVarBindedNominalHash, CPROCESSINGHASH<CConcept*,CPropagationBindingDescriptor*>*& groundedConPropBindDesHash, CNominalSchemaTemplate* nsTemplate, CCalculationAlgorithmContext* calcAlgContext) {

					CSortedNegLinker<CConcept*>* newGroundedConLinker = nullptr;


					CReapplyConceptLabelSet* conLabelSet = indiProcessNode->getReapplyConceptLabelSet(false);

					CBOXHASH<CConcept*,CConcept*>* templConNomSchConHash = nsTemplate->getTemplateConceptNominalSchemaConceptHash();
					CBOXSET<CConcept*>* nomSchConSet = nsTemplate->getNominalSchemaConceptSet();

					QHash<CConcept*, CPROCESSINGHASH<CConcept*,TConceptPropagationBindingPair>::const_iterator > nomSchConIndividualItHash;
					CPROCESSINGHASH<CConcept*,TConceptPropagationBindingPair>::const_iterator indiItEnd = nominalSchemaVarBindedNominalHash->constEnd();
					for (CBOXSET<CConcept*>::const_iterator it = nomSchConSet->constBegin(), itEnd = nomSchConSet->constEnd(); it != itEnd; ++it) {
						CConcept* nomSchCon(*it);
						CPROCESSINGHASH<CConcept*,TConceptPropagationBindingPair>::const_iterator indiIt = nominalSchemaVarBindedNominalHash->constFind(nomSchCon);
						nomSchConIndividualItHash.insert(nomSchCon,indiIt);
					}



					bool conVarItFinished = false;
					while (!conVarItFinished) {


						CConcept* templateConcept = nsTemplate->getTemplateConcept();
						CSortedNegLinker<CConcept*>* opConLinker = createGroundedNominalSchemaConcept(templateConcept,negated,conLabelSet,templConNomSchConHash,&nomSchConIndividualItHash,calcAlgContext);
						if (opConLinker) {
							if (!groundedConPropBindDesHash) {
								groundedConPropBindDesHash = CObjectParameterizingAllocator< CPROCESSINGHASH<CConcept*,CPropagationBindingDescriptor*>,CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(),calcAlgContext->getUsedTaskProcessorContext());
							}

							QHash<CConcept*, CPROCESSINGHASH<CConcept*,TConceptPropagationBindingPair>::const_iterator >::const_iterator variableIt = nomSchConIndividualItHash.constBegin(), variableItEnd = nomSchConIndividualItHash.constEnd();
							while (variableIt != variableItEnd) {
								CPROCESSINGHASH<CConcept*,TConceptPropagationBindingPair>::const_iterator nomBindIt = variableIt.value();
								CPropagationBindingDescriptor* bindedDes = nomBindIt.value().second;
								if (bindedDes) {
									for (CSortedNegLinker<CConcept*>* opConLinkerIt = opConLinker; opConLinkerIt; opConLinkerIt = opConLinkerIt->getNext()) {
										CConcept* groundedConcept = opConLinkerIt->getData();
										groundedConPropBindDesHash->insertMulti(groundedConcept,bindedDes);
									}
								}
								++variableIt;
							}

							newGroundedConLinker = opConLinker->append(newGroundedConLinker);
						}





						QHash<CConcept*, CPROCESSINGHASH<CConcept*,TConceptPropagationBindingPair>::const_iterator >::const_iterator variableIt = nomSchConIndividualItHash.constBegin();
						QHash<CConcept*, CPROCESSINGHASH<CConcept*,TConceptPropagationBindingPair>::const_iterator >::const_iterator variableItEnd = nomSchConIndividualItHash.constEnd();


						CPROCESSINGHASH<CConcept*,TConceptPropagationBindingPair>::const_iterator varIndiIt = variableIt.value();
						CConcept* varConcept = variableIt.key();

						++varIndiIt;
						bool conVarIteration = true;
						while ((varIndiIt == indiItEnd || varIndiIt.key() != varConcept) && conVarIteration) {
							conVarIteration = false;
							CPROCESSINGHASH<CConcept*,TConceptPropagationBindingPair>::const_iterator indiIt = nominalSchemaVarBindedNominalHash->constFind(varConcept);
							nomSchConIndividualItHash.insert(varConcept,indiIt);

							++variableIt;
							if (variableIt == variableItEnd) {
								conVarItFinished = true;
							} else {
								varIndiIt = variableIt.value();
								varConcept = variableIt.key();
								++varIndiIt;

								if (varIndiIt == indiItEnd) {
									conVarIteration = true;
								}
							}
						} 
						if (!conVarItFinished) {
							nomSchConIndividualItHash.insert(varConcept,varIndiIt);
						}
					}


					return newGroundedConLinker;
				}
















				CSortedNegLinker<CConcept*>* CConceptNominalSchemaGroundingHandler::getGroundingConceptLinker(CIndividualProcessNode* processNode, CVariableBindingPathSet* varBindPathSet, CConcept* concept, bool negated, CPROCESSINGHASH<CConcept*,CVariableBindingPathDescriptor*>*& groundedConVarBindPathDesHash, CCalculationAlgorithmContext* calcAlgContext) {
					CSortedNegLinker<CConcept*>* newLinker = nullptr;

					mDataBox = calcAlgContext->getUsedProcessingDataBox();
					CProcessContext* context = calcAlgContext->getUsedProcessContext();
					mMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
					CMBox* mBox = mDataBox->getOntology()->getDataBoxes()->getMBox();
					CTBox* tBox = mDataBox->getOntology()->getDataBoxes()->getTBox();
					mConceptVec = mDataBox->getExtendedConceptVector(false);
					mReplaceHash = mDataBox->getConceptNominalSchemaGroundingHash(false);
					CNominalSchemaTemplateVector* nomSchVec = mBox->getNominalSchemaTemplateVector(false);
					mLocalizedExtensions = false;


					cint64 nomSchTemplID = concept->getParameter();


					if (nomSchVec) {
						CNominalSchemaTemplate* nomSchTempl = nomSchVec->getData(nomSchTemplID);
						CConcept* templateConcept = nomSchTempl->getTemplateConcept();
						CBOXHASH<CConcept*,CConcept*>* templateConceptNomSchVarHash = nomSchTempl->getTemplateConceptNominalSchemaConceptHash();
						CBOXSET<CConcept*>* templateConceptNomSchVarSet = nomSchTempl->getNominalSchemaConceptSet();

						CVariableBindingPathMap* varBindPathMap = varBindPathSet->getVariableBindingPathMap();
						for (CVariableBindingPathMap::iterator it = varBindPathMap->begin(), itEnd = varBindPathMap->end(); it != itEnd; ++it) {
							CVariableBindingPathMapData& varBindPathData = it.value();
							CVariableBindingPathDescriptor* varBindPathDes = varBindPathData.getVariableBindingPathDescriptor();
							CVariableBindingPath* varBindPath = varBindPathDes->getVariableBindingPath();

							CPROCESSINGHASH<CConcept*,TConceptPropagationBindingPair> nominalSchemaVarBindedNominalHash(calcAlgContext->getUsedTaskProcessorContext());
							CPROCESSINGSET<CConcept*>* allNominalConceptSet = nullptr;

							for (CVariableBindingDescriptor* varBindDesIt = varBindPath->getVariableBindingDescriptorLinker(); varBindDesIt; varBindDesIt = varBindDesIt->getNext()) {
								CVariableBinding* varBind = varBindDesIt->getVariableBinding();
								CVariable* variable = varBind->getBindedVariable();
								CIndividualProcessNode* bindedIndiNode = varBind->getBindedIndividual();
								if (variable->isNominalVariable()) {
									CConcept* nomSchVarConcept = variable->getNominalVariableConcept();
									if (templateConceptNomSchVarSet->contains(nomSchVarConcept)) {
										CConcept* nominalConcept = getNominalConcept(bindedIndiNode,false,calcAlgContext);
										if (nominalConcept) {
											nominalSchemaVarBindedNominalHash.insertMulti(nomSchVarConcept,TConceptPropagationBindingPair(nominalConcept,nullptr));
										}
									}
								}
							}

							for (CBOXSET<CConcept*>::iterator it = templateConceptNomSchVarSet->begin(), itEnd = templateConceptNomSchVarSet->end(); it != itEnd; ++it) {
								CConcept* nomSchVarConcept = *it;
								if (!nominalSchemaVarBindedNominalHash.contains(nomSchVarConcept)) {
									collectAllNominalConcepts(allNominalConceptSet,calcAlgContext);
									for (CPROCESSINGSET<CConcept*>::const_iterator nomIt = allNominalConceptSet->constBegin(), nomItEnd = allNominalConceptSet->constEnd(); nomIt != nomItEnd; ++nomIt) {
										CConcept* nominalConcept(*nomIt);
										nominalSchemaVarBindedNominalHash.insertMulti(nomSchVarConcept,TConceptPropagationBindingPair(nominalConcept,nullptr));
									}
								}
							}

							CSortedNegLinker<CConcept*>* tmpNewLinker = createNominalSchemaGroundingConcepts(processNode,nomSchTempl->getTemplateConcept(),negated,&nominalSchemaVarBindedNominalHash,nomSchTempl,calcAlgContext);
							if (tmpNewLinker) {
								for (CSortedNegLinker<CConcept*>* tmpNewLinkerIt = tmpNewLinker; tmpNewLinkerIt; tmpNewLinkerIt = tmpNewLinkerIt->getNext()) {
									CConcept* concept = tmpNewLinkerIt->getData();
									if (!groundedConVarBindPathDesHash) {
										groundedConVarBindPathDesHash = CObjectParameterizingAllocator< CPROCESSINGHASH<CConcept*,CVariableBindingPathDescriptor*>,CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(),calcAlgContext->getUsedTaskProcessorContext());
									}
									groundedConVarBindPathDesHash->insert(concept,varBindPathDes);
								}
								newLinker = tmpNewLinker->append(newLinker);
							}

						}
					}					

					return newLinker;
				}







				CSortedNegLinker<CConcept*>* CConceptNominalSchemaGroundingHandler::getGroundingConceptLinker(CIndividualProcessNode* processNode, CRepresentativeVariableBindingPathMap* repVarBindPathSetMap, CConcept* concept, bool negated, CPROCESSINGHASH<CConcept*,CVariableBindingPath*>*& groundedConVarBindPathHash, CCalculationAlgorithmContext* calcAlgContext) {
					CSortedNegLinker<CConcept*>* newLinker = nullptr;

					mDataBox = calcAlgContext->getUsedProcessingDataBox();
					CProcessContext* context = calcAlgContext->getUsedProcessContext();
					mMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
					CMBox* mBox = mDataBox->getOntology()->getDataBoxes()->getMBox();
					CTBox* tBox = mDataBox->getOntology()->getDataBoxes()->getTBox();
					mConceptVec = mDataBox->getExtendedConceptVector(false);
					mReplaceHash = mDataBox->getConceptNominalSchemaGroundingHash(false);
					CNominalSchemaTemplateVector* nomSchVec = mBox->getNominalSchemaTemplateVector(false);
					mLocalizedExtensions = false;


					cint64 nomSchTemplID = concept->getParameter();


					if (nomSchVec) {
						CNominalSchemaTemplate* nomSchTempl = nomSchVec->getData(nomSchTemplID);
						CConcept* templateConcept = nomSchTempl->getTemplateConcept();
						CBOXHASH<CConcept*,CConcept*>* templateConceptNomSchVarHash = nomSchTempl->getTemplateConceptNominalSchemaConceptHash();
						CBOXSET<CConcept*>* templateConceptNomSchVarSet = nomSchTempl->getNominalSchemaConceptSet();

						for (CRepresentativeVariableBindingPathMap::const_iterator it = repVarBindPathSetMap->constBegin(), itEnd = repVarBindPathSetMap->constEnd(); it != itEnd; ++it) {
							const CRepresentativeVariableBindingPathMapData& varBindPathData = it.value();
							CVariableBindingPath* varBindPath = varBindPathData.getVariableBindingPath();

							CPROCESSINGHASH<CConcept*,TConceptPropagationBindingPair> nominalSchemaVarBindedNominalHash(calcAlgContext->getUsedTaskProcessorContext());
							CPROCESSINGSET<CConcept*>* allNominalConceptSet = nullptr;

							for (CVariableBindingDescriptor* varBindDesIt = varBindPath->getVariableBindingDescriptorLinker(); varBindDesIt; varBindDesIt = varBindDesIt->getNext()) {
								CVariableBinding* varBind = varBindDesIt->getVariableBinding();
								CVariable* variable = varBind->getBindedVariable();
								CIndividualProcessNode* bindedIndiNode = varBind->getBindedIndividual();
								if (variable->isNominalVariable()) {
									CConcept* nomSchVarConcept = variable->getNominalVariableConcept();
									if (templateConceptNomSchVarSet->contains(nomSchVarConcept)) {
										CConcept* nominalConcept = getNominalConcept(bindedIndiNode,false,calcAlgContext);
										if (nominalConcept) {
											nominalSchemaVarBindedNominalHash.insertMulti(nomSchVarConcept,TConceptPropagationBindingPair(nominalConcept,nullptr));
										}
									}
								}
							}

							for (CBOXSET<CConcept*>::iterator it = templateConceptNomSchVarSet->begin(), itEnd = templateConceptNomSchVarSet->end(); it != itEnd; ++it) {
								CConcept* nomSchVarConcept = *it;
								if (!nominalSchemaVarBindedNominalHash.contains(nomSchVarConcept)) {
									collectAllNominalConcepts(allNominalConceptSet,calcAlgContext);
									for (CPROCESSINGSET<CConcept*>::const_iterator nomIt = allNominalConceptSet->constBegin(), nomItEnd = allNominalConceptSet->constEnd(); nomIt != nomItEnd; ++nomIt) {
										CConcept* nominalConcept(*nomIt);
										nominalSchemaVarBindedNominalHash.insertMulti(nomSchVarConcept,TConceptPropagationBindingPair(nominalConcept,nullptr));
									}
								}
							}

							CSortedNegLinker<CConcept*>* tmpNewLinker = createNominalSchemaGroundingConcepts(processNode,nomSchTempl->getTemplateConcept(),negated,&nominalSchemaVarBindedNominalHash,nomSchTempl,calcAlgContext);
							if (tmpNewLinker) {
								for (CSortedNegLinker<CConcept*>* tmpNewLinkerIt = tmpNewLinker; tmpNewLinkerIt; tmpNewLinkerIt = tmpNewLinkerIt->getNext()) {
									CConcept* concept = tmpNewLinkerIt->getData();
									if (!groundedConVarBindPathHash) {
										groundedConVarBindPathHash = CObjectParameterizingAllocator< CPROCESSINGHASH<CConcept*,CVariableBindingPath*>,CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(),calcAlgContext->getUsedTaskProcessorContext());
									}
									groundedConVarBindPathHash->insert(concept,varBindPath);
								}
								newLinker = tmpNewLinker->append(newLinker);
							}

						}
					}					

					return newLinker;
				}








				CSortedNegLinker<CConcept*>* CConceptNominalSchemaGroundingHandler::createGroundedNominalSchemaConcept(CConcept* concept, bool negated, CReapplyConceptLabelSet* conLabelSet, CBOXHASH<CConcept*,CConcept*>* templConNomSchConHash, QHash<CConcept*, CPROCESSINGHASH<CConcept*,TConceptPropagationBindingPair>::const_iterator >* nomSchConIndividualItHash, CCalculationAlgorithmContext* calcAlgContext) {
					CSortedNegLinker<CConcept*>* conLinker = nullptr;
					cint64 opCode = concept->getOperatorCode();
					if (!negated && (opCode == CCAND) || negated && (opCode == CCOR)) {
						for (CSortedNegLinker<CConcept*>* opConLinkerIt = concept->getOperandList(); opConLinkerIt; opConLinkerIt = opConLinkerIt->getNext()) {
							CConcept* opConcept = opConLinkerIt->getData();
							bool opConNegation = opConLinkerIt->isNegated()^negated;
							CSortedNegLinker<CConcept*>* tmpConLinker = createGroundedNominalSchemaConcept(opConcept,opConNegation,conLabelSet,templConNomSchConHash,nomSchConIndividualItHash,calcAlgContext);
							if (tmpConLinker) {
								conLinker = tmpConLinker->append(conLinker);
							}
						}
					} else {
						CConcept* groundedConcept = createGroundedNominalSchemaConcept(concept,templConNomSchConHash,nomSchConIndividualItHash,calcAlgContext);
						bool alreadyContained = false;
						if (conLabelSet) {
							alreadyContained = conLabelSet->containsConcept(groundedConcept);
						}
						if (!alreadyContained) {
							CSortedNegLinker<CConcept*>* opConLinker = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mMemMan);
							opConLinker->init(groundedConcept,negated);
							conLinker = opConLinker->append(conLinker);
						}

					}
					return conLinker;
				}


				CSortedNegLinker<CConcept*>* CConceptNominalSchemaGroundingHandler::createNominalSchemaGroundingConcepts(CIndividualProcessNode* indiProcessNode, CConcept* concept, bool negated, CPROCESSINGHASH<CConcept*,TConceptPropagationBindingPair>* nominalSchemaVarBindedNominalHash, CNominalSchemaTemplate* nsTemplate, CCalculationAlgorithmContext* calcAlgContext) {

					CSortedNegLinker<CConcept*>* newGroundedConLinker = nullptr;


					CReapplyConceptLabelSet* conLabelSet = indiProcessNode->getReapplyConceptLabelSet(false);

					CBOXHASH<CConcept*,CConcept*>* templConNomSchConHash = nsTemplate->getTemplateConceptNominalSchemaConceptHash();
					CBOXSET<CConcept*>* nomSchConSet = nsTemplate->getNominalSchemaConceptSet();

					QHash<CConcept*, CPROCESSINGHASH<CConcept*,TConceptPropagationBindingPair>::const_iterator > nomSchConIndividualItHash;
					CPROCESSINGHASH<CConcept*,TConceptPropagationBindingPair>::const_iterator indiItEnd = nominalSchemaVarBindedNominalHash->constEnd();
					for (CBOXSET<CConcept*>::const_iterator it = nomSchConSet->constBegin(), itEnd = nomSchConSet->constEnd(); it != itEnd; ++it) {
						CConcept* nomSchCon(*it);
						CPROCESSINGHASH<CConcept*,TConceptPropagationBindingPair>::const_iterator indiIt = nominalSchemaVarBindedNominalHash->constFind(nomSchCon);
						nomSchConIndividualItHash.insert(nomSchCon,indiIt);
					}

					bool conVarItFinished = false;
					while (!conVarItFinished) {

						CConcept* templateConcept = nsTemplate->getTemplateConcept();
						CSortedNegLinker<CConcept*>* opConLinker = createGroundedNominalSchemaConcept(templateConcept,negated,conLabelSet,templConNomSchConHash,&nomSchConIndividualItHash,calcAlgContext);
						if (opConLinker) {
							newGroundedConLinker = opConLinker->append(newGroundedConLinker);
						}



						QHash<CConcept*, CPROCESSINGHASH<CConcept*,TConceptPropagationBindingPair>::const_iterator >::const_iterator variableIt = nomSchConIndividualItHash.constBegin();
						QHash<CConcept*, CPROCESSINGHASH<CConcept*,TConceptPropagationBindingPair>::const_iterator >::const_iterator variableItEnd = nomSchConIndividualItHash.constEnd();

						if (variableIt == variableItEnd) {
							conVarItFinished = true;
						} else {

							CPROCESSINGHASH<CConcept*,TConceptPropagationBindingPair>::const_iterator varIndiIt = variableIt.value();
							CConcept* varConcept = variableIt.key();

							++varIndiIt;
							bool conVarIteration = true;
							while ((varIndiIt == indiItEnd || varIndiIt.key() != varConcept) && conVarIteration) {
								conVarIteration = false;
								CPROCESSINGHASH<CConcept*,TConceptPropagationBindingPair>::const_iterator indiIt = nominalSchemaVarBindedNominalHash->constFind(varConcept);
								nomSchConIndividualItHash.insert(varConcept,indiIt);

								++variableIt;
								if (variableIt == variableItEnd) {
									conVarItFinished = true;
								} else {
									varIndiIt = variableIt.value();
									varConcept = variableIt.key();
									++varIndiIt;

									if (varIndiIt == indiItEnd) {
										conVarIteration = true;
									}
								}
							} 
							if (!conVarItFinished) {
								nomSchConIndividualItHash.insert(varConcept,varIndiIt);
							}
						}
					}


					return newGroundedConLinker;
				}





			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
