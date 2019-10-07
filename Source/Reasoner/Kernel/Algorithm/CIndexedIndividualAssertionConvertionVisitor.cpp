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

#include "CIndexedIndividualAssertionConvertionVisitor.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Algorithm {


				CIndexedIndividualAssertionConvertionVisitor::CIndexedIndividualAssertionConvertionVisitor(cint64 retrievingIndiId, CCalculationAlgorithmContext* calcAlgContext) : mPreprocessContext(calcAlgContext->getProcessingDataBox()->getOntology(), calcAlgContext->getSatisfiableCalculationTask()->getCalculationConfiguration(), calcAlgContext->getUsedProcessTaskMemoryAllocationManager()) {
					mCalcAlgContext = calcAlgContext;
					mMemoryAllocationManager = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
					mRetrievingIndiId = retrievingIndiId;
					mRetrievingIndi = getExistingOrCreateNewTemporaryIndividual(mRetrievingIndiId);
				}


				CIndexedIndividualAssertionConvertionVisitor::CIndexedIndividualAssertionConvertionVisitor(CIndividual* retrievingIndi, CCalculationAlgorithmContext* calcAlgContext) : mPreprocessContext(calcAlgContext->getProcessingDataBox()->getOntology(), calcAlgContext->getSatisfiableCalculationTask()->getCalculationConfiguration(), calcAlgContext->getUsedProcessTaskMemoryAllocationManager()) {
					mCalcAlgContext = calcAlgContext;
					mMemoryAllocationManager = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
					mRetrievingIndiId = retrievingIndi->getIndividualID();
					mRetrievingIndi = getNewTemporaryIndividual(retrievingIndi);
					mConceptAssertionLinker = retrievingIndi->getAssertionConceptLinker();
					mRoleAssertionLinker = retrievingIndi->getAssertionRoleLinker();
					mReverseRoleAssertionLinker = retrievingIndi->getReverseAssertionRoleLinker();
					mDataAssertionLinker = retrievingIndi->getAssertionDataLinker();
				}


				bool CIndexedIndividualAssertionConvertionVisitor::visitIndividualName(const QString& indiName, COntologyTriplesAssertionsAccessor* accessor) {
					if (!mRetrievingIndi->getIndividualNameLinker()) {
						CIRIName* name = CObjectAllocator< CIRIName >::allocateAndConstruct(mMemoryAllocationManager);
						name->setIRIName(indiName);
						CLinker<CName*>* nameLinker = CObjectAllocator< CLinker<CName*> >::allocateAndConstruct(mMemoryAllocationManager);
						nameLinker->init(name);
						mRetrievingIndi->setNameLinker(nameLinker);
					}
					return true;
				}


				bool CIndexedIndividualAssertionConvertionVisitor::visitRoleAssertion(CRole* role, cint64 otherIndividualId, COntologyTriplesAssertionsAccessor* accessor) {
					CRoleAssertionLinker* roleAssertionLinker = CObjectAllocator<CRoleAssertionLinker>::allocateAndConstruct(mMemoryAllocationManager);
					CIndividual* existingOrNewTemporaryIndi = getExistingOrCreateNewTemporaryIndividual(otherIndividualId);
					roleAssertionLinker->initRoleAssertionLinker(role, existingOrNewTemporaryIndi);
					mRoleAssertionLinker = roleAssertionLinker->append(mRoleAssertionLinker);
					return true;
				}

				bool CIndexedIndividualAssertionConvertionVisitor::visitReverseRoleAssertion(CRole* role, cint64 otherIndividualId, COntologyTriplesAssertionsAccessor* accessor) {
					CReverseRoleAssertionLinker* reversRoleAssertionLinker = CObjectAllocator<CReverseRoleAssertionLinker>::allocateAndConstruct(mMemoryAllocationManager);
					CIndividual* existingOrNewTemporaryIndi = getExistingOrCreateNewTemporaryIndividual(otherIndividualId);

					CRoleAssertionLinker* roleAssertionLinker = CObjectAllocator<CRoleAssertionLinker>::allocateAndConstruct(mMemoryAllocationManager);
					roleAssertionLinker->initRoleAssertionLinker(role, mRetrievingIndi);

					reversRoleAssertionLinker->initReverseRoleAssertionLinker(roleAssertionLinker, existingOrNewTemporaryIndi);
					mReverseRoleAssertionLinker = reversRoleAssertionLinker->append(mReverseRoleAssertionLinker);
					return true;
				}

				bool CIndexedIndividualAssertionConvertionVisitor::visitDataAssertion(CRole* role, CDataLiteral* dataLiteral, COntologyTriplesAssertionsAccessor* accessor) {
					CDataLiteral* copiedDataLiteral = CObjectParameterizingAllocator<CDataLiteral,CContext*>::allocateAndConstructAndParameterize(mMemoryAllocationManager, mCalcAlgContext);
					copiedDataLiteral->initDataLiteral(dataLiteral->getLexicalDataLiteralValueString(), dataLiteral->getDatatype());
					createDataLiteralValue(copiedDataLiteral);
					// TODO: clean up of internal data of data literal and data literal value

					CDataAssertionLinker* dataAssertionLinker = CObjectAllocator<CDataAssertionLinker>::allocateAndConstruct(mMemoryAllocationManager);
					dataAssertionLinker->initDataAssertionLinker(role, copiedDataLiteral);
					mDataAssertionLinker = dataAssertionLinker->append(mDataAssertionLinker);
					return true;
				}

				bool CIndexedIndividualAssertionConvertionVisitor::visitConceptAssertion(CConcept* concept, COntologyTriplesAssertionsAccessor* accessor) {
					CConceptAssertionLinker* conceptAssertionLinker = CObjectAllocator<CConceptAssertionLinker>::allocateAndConstruct(mMemoryAllocationManager);
					conceptAssertionLinker->initNegLinker(concept, false);
					mConceptAssertionLinker = conceptAssertionLinker->append(mConceptAssertionLinker);
					return true;
				}


				CIndividual* CIndexedIndividualAssertionConvertionVisitor::getExistingOrCreateNewTemporaryIndividual(cint64 indiId) {
					CIndividual* indi = nullptr;
					CIndividualVector* indiVector = mCalcAlgContext->getProcessingDataBox()->getIndividualVector(true);
					indi = indiVector->getLocalData(indiId);
					if (!indi) {
						indi = CObjectAllocator<CIndividual>::allocateAndConstruct(mMemoryAllocationManager);
						indi->initIndividual(indiId);
						indi->setTemporaryIndividual(true);
						indiVector->setLocalData(indiId, indi);
					}
					return indi;
				}


				CIndividual* CIndexedIndividualAssertionConvertionVisitor::getNewTemporaryIndividual(CIndividual* existingIndi) {
					CIndividualVector* indiVector = mCalcAlgContext->getProcessingDataBox()->getIndividualVector(true);
					CIndividual* newIndi = CObjectAllocator<CIndividual>::allocateAndConstruct(mMemoryAllocationManager);
					newIndi->initIndividual(existingIndi->getIndividualID());
					newIndi->setTemporaryIndividual(true);
					newIndi->setIndividualData(existingIndi->getIndividualData());
					indiVector->setLocalData(existingIndi->getIndividualID(), newIndi);
					return newIndi;
				}

				CDataLiteralValue* CIndexedIndividualAssertionConvertionVisitor::createDataLiteralValue(CDataLiteral* dataLiteral) {
					CDataLiteralValue* dataLiteralValue = mDataLiteralNormalizer.createPreprocessedDataLiteralValue(dataLiteral, &mPreprocessContext, mCalcAlgContext);
					return dataLiteralValue;
				}


				CConceptAssertionLinker* CIndexedIndividualAssertionConvertionVisitor::getConceptAssertionLinker() {
					return mConceptAssertionLinker;
				}

				CRoleAssertionLinker* CIndexedIndividualAssertionConvertionVisitor::getRoleAssertionLinker() {
					return mRoleAssertionLinker;
				}

				CReverseRoleAssertionLinker* CIndexedIndividualAssertionConvertionVisitor::getReverseRoleAssertionLinker() {
					return mReverseRoleAssertionLinker;
				}

				CDataAssertionLinker* CIndexedIndividualAssertionConvertionVisitor::getDataAssertionLinker() {
					return mDataAssertionLinker;
				}

				CIndividual* CIndexedIndividualAssertionConvertionVisitor::getRetrievalIndividual() {
					mRetrievingIndi->setAssertionDataLinker(mDataAssertionLinker);
					mRetrievingIndi->setAssertionConceptLinker(mConceptAssertionLinker);
					mRetrievingIndi->setAssertionRoleLinker(mRoleAssertionLinker);
					mRetrievingIndi->setReverseAssertionRoleLinker(mReverseRoleAssertionLinker);
					return mRetrievingIndi;
				}
				



			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
