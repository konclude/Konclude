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

#include "CConcreteOntology.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			CConcreteOntology::CConcreteOntology(CConfigurationBase* configuration) {
				mStructSumm = nullptr;
				mStringMapping = nullptr;
				mDataBoxes = nullptr;
				mBuildData = nullptr;
				mTax = nullptr;
				mConsistence = nullptr;
				mAxiomSet = nullptr;
				mConceptCyclesData = nullptr;
				mPrecomputation = nullptr;
				mProcessingSteps = nullptr;

				mConfiguration = configuration;

				mClassification = new CClassification();
				mPrecomputation = new CPrecomputation();
				mPreprocessing = new CPreprocessing();
				mRealization = new CRealization();
				mOntConctext = new CConcreteOntologyContextBase();
				mDataBoxes = CObjectParameterizingAllocator< COntologyDataBoxes,CBoxContext* >::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mOntConctext),mOntConctext);
				mStringMapping = CObjectParameterizingAllocator< COntologyStringMapping,CBoxContext* >::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mOntConctext),mOntConctext);
				mStructSumm = CObjectParameterizingAllocator< COntologyStructureSummary,CBoxContext* >::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mOntConctext),mOntConctext);
				mBuildData = CObjectParameterizingAllocator< COntologyBuildData,CBoxContext* >::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mOntConctext),mOntConctext);
				mConceptCyclesData = CObjectParameterizingAllocator< COntologyCoreConceptCyclesData,CBoxContext* >::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mOntConctext),mOntConctext);
				mProcessingSteps = CObjectParameterizingAllocator< COntologyProcessingSteps,CBoxContext* >::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mOntConctext),mOntConctext);
			}


			CConcreteOntology::CConcreteOntology(CConcreteOntology* refOntology, CConfigurationBase* configuration) {
				mStructSumm = nullptr;
				mStringMapping = nullptr;
				mDataBoxes = nullptr;
				mBuildData = nullptr;
				mTax = nullptr;
				mConsistence = nullptr;
				mAxiomSet = nullptr;
				mConceptCyclesData = nullptr;
				mPrecomputation = nullptr;
				mProcessingSteps = nullptr;

				mConfiguration = configuration;

				mClassification = new CClassification();
				mPrecomputation = new CPrecomputation();
				mPreprocessing = new CPreprocessing();
				mRealization = new CRealization();
				mOntConctext = new CConcreteOntologyContextBase();
				mDataBoxes = CObjectParameterizingAllocator< COntologyDataBoxes,CBoxContext* >::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mOntConctext),mOntConctext);
				mStringMapping = CObjectParameterizingAllocator< COntologyStringMapping,CBoxContext* >::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mOntConctext),mOntConctext);
				mStructSumm = CObjectParameterizingAllocator< COntologyStructureSummary,CBoxContext* >::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mOntConctext),mOntConctext);
				mBuildData = CObjectParameterizingAllocator< COntologyBuildData,CBoxContext* >::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mOntConctext),mOntConctext);
				mConceptCyclesData = CObjectParameterizingAllocator< COntologyCoreConceptCyclesData,CBoxContext* >::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mOntConctext),mOntConctext);
				mProcessingSteps = CObjectParameterizingAllocator< COntologyProcessingSteps,CBoxContext* >::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mOntConctext),mOntConctext);

				if (refOntology) {
					referenceOntology(refOntology);
				}
			}


			CConcreteOntology::~CConcreteOntology() {
				COPADestroyAndRelease(mDataBoxes,CContext::getMemoryAllocationManager(mOntConctext));
				COPADestroyAndRelease(mStructSumm,CContext::getMemoryAllocationManager(mOntConctext));
				COPADestroyAndRelease(mStringMapping,CContext::getMemoryAllocationManager(mOntConctext));
				COPADestroyAndRelease(mBuildData,CContext::getMemoryAllocationManager(mOntConctext));
				COPADestroyAndRelease(mConceptCyclesData,CContext::getMemoryAllocationManager(mOntConctext));
				COPADestroyAndRelease(mProcessingSteps,CContext::getMemoryAllocationManager(mOntConctext));

				delete mOntConctext;
				delete mConsistence;
				delete mClassification;
				delete mPrecomputation;
				delete mPreprocessing;
				delete mRealization;
			}

			CConfigurationBase* CConcreteOntology::getConfiguration() {
				return mConfiguration;
			}


			CConcreteOntology* CConcreteOntology::referenceOntology(CConcreteOntology* ontology) {
				mDataBoxes->referenceDataBoxes(ontology->mDataBoxes);
				mStringMapping->referenceStringMapping(ontology->mStringMapping);
				mBuildData->referenceBuildData(ontology->mBuildData);
				mStructSumm->referenceStructureSummary(ontology->mStructSumm);
				return this;
			}


			CABox* CConcreteOntology::getABox() {
				return mDataBoxes->getABox();
			}

			CTBox* CConcreteOntology::getTBox() {
				return mDataBoxes->getTBox();
			}

			CRBox* CConcreteOntology::getRBox() {
				return mDataBoxes->getRBox();
			}


			CConcept* CConcreteOntology::getConcept(const QString& className) {
				CConcept* concept = nullptr;
				CMAPPINGHASH<CStringRefStringHasher,CConcept*>* classNameConceptMappingHash = mStringMapping->getClassNameConceptMappingHash(false);
				if (classNameConceptMappingHash) {
					concept = classNameConceptMappingHash->value(className,nullptr);
				}
				return concept;
			}



			CConcreteOntology* CConcreteOntology::setConceptTaxonomy(CTaxonomy *taxonomy) {
				mTax = taxonomy;
				return this;
			}

			CTaxonomy *CConcreteOntology::getConceptTaxonomy() {
				return mTax;
			}

			bool CConcreteOntology::hasConceptTaxonomy() {
				return mTax != nullptr;
			}


			CConsistence* CConcreteOntology::getConsistence() {
				return mConsistence;
			}

			bool CConcreteOntology::hasConsistence() {
				return mConsistence != nullptr;
			}

			CConcreteOntology *CConcreteOntology::setConsistence(CConsistence *consistence) {
				mConsistence = consistence;
				return this;
			}



			CClassification* CConcreteOntology::getClassification() {
				return mClassification;
			}

			bool CConcreteOntology::hasClassification() {
				return mClassification != nullptr;
			}

			CConcreteOntology *CConcreteOntology::setClassification(CClassification *classification) {
				mClassification = classification;
				return this;
			}


			CPrecomputation* CConcreteOntology::getPrecomputation() {
				return mPrecomputation;
			}

			bool CConcreteOntology::hasPrecomputation() {
				return mPrecomputation != nullptr;
			}

			CConcreteOntology* CConcreteOntology::setPrecomputation(CPrecomputation* precomputation) {
				mPrecomputation = precomputation;
				return this;
			}


			CPreprocessing* CConcreteOntology::getPreprocessing() {
				return mPreprocessing;
			}

			bool CConcreteOntology::hasPreprocessing() {
				return mPreprocessing != nullptr;
			}

			CConcreteOntology* CConcreteOntology::setPreprocessing(CPreprocessing* preprocessing) {
				mPreprocessing = preprocessing;
				return this;
			}



			CRealization* CConcreteOntology::getRealization() {
				return mRealization;
			}

			bool CConcreteOntology::hasRealization() {
				return mRealization != nullptr;
			}

			CConcreteOntology* CConcreteOntology::setRealization(CRealization* realization) {
				mRealization = realization;
				return this;
			}


			COntologyStructureSummary *CConcreteOntology::getStructureSummary() {
				return mStructSumm;
			}

			CConcreteOntology *CConcreteOntology::setStructureSummary(COntologyStructureSummary *structureSummary) {
				mStructSumm = structureSummary;
				return this;
			}

			COntologyDataBoxes *CConcreteOntology::getDataBoxes() {
				return mDataBoxes;
			}

			CConcreteOntology *CConcreteOntology::setDataBoxes(COntologyDataBoxes *dataBoxes) {
				mDataBoxes = dataBoxes;
				return this;
			}


			COntologyStringMapping *CConcreteOntology::getStringMapping() {
				return mStringMapping;
			}

			CConcreteOntology *CConcreteOntology::setStringMapping(COntologyStringMapping *stringMapping) {
				mStringMapping = stringMapping;
				return this;
			}


			COntologyContext* CConcreteOntology::getOntologyContext() {
				return mOntConctext;
			}

			COntologyBuildData *CConcreteOntology::getBuildData() {
				return mBuildData;
			}

			CConcreteOntology *CConcreteOntology::setBuildData(COntologyBuildData *buildData) {
				mBuildData = buildData;
				return this;
			}

			COntologyCoreConceptCyclesData* CConcreteOntology::getCoreConceptCyclesData() {
				return mConceptCyclesData;
			}

			CConcreteOntology* CConcreteOntology::setCoreConceptCyclesData(COntologyCoreConceptCyclesData* cyclesData) {
				mConceptCyclesData = cyclesData;
				return this;
			}

			COntologyProcessingSteps* CConcreteOntology::getProcessingSteps() {
				return mProcessingSteps;
			}

			CConcreteOntology* CConcreteOntology::setProcessingSteps(COntologyProcessingSteps* stepData) {
				mProcessingSteps = stepData;
				return this;
			}

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude
