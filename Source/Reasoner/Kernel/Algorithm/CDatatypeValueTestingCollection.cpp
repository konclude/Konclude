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

#include "CDatatypeValueTestingCollection.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Algorithm {


				CDatatypeValueTestingCollection::CDatatypeValueTestingCollection(CCalculationAlgorithmContext* collectionCalcAlgContext) {
					mCollectionCalcAlgContext = collectionCalcAlgContext;

					mFreeLinker = nullptr;
					for (cint64 i = 0; i < CDatatypeValueSpaceType::VALUESPACETYPECOUNT; ++i) {
						mFreeDataLiteralValueVec[i] = nullptr;
						mUseDataLiteralValueVec[i] = nullptr;
					}

					CTaskProcessorContext* taskProcessorContext = mCollectionCalcAlgContext->getUsedTaskProcessorContext();

				}


				CDatatypeValueTestingCollection* CDatatypeValueTestingCollection::releaseUsedDataLiteralValues() {
					for (cint64 i = 0; i < CDatatypeValueSpaceType::VALUESPACETYPECOUNT; ++i) {
						CXLinker<CDataLiteralValue*>* dataLiteralValueLinker = mUseDataLiteralValueVec[i];
						while (dataLiteralValueLinker) {
							CXLinker<CDataLiteralValue*>* tmpDataLiteralValueLinker = dataLiteralValueLinker;
							dataLiteralValueLinker = dataLiteralValueLinker->getNext();
							tmpDataLiteralValueLinker->clearNext();
							releaseFreeLinker(tmpDataLiteralValueLinker);
						}
						mUseDataLiteralValueVec[i] = nullptr;
					}
					return this;
				}


				CDatatypeValueTestingCollection* CDatatypeValueTestingCollection::addDataLiteralValueUse(CDataLiteralValue* dataLiteralValue, CDatatypeValueSpaceType* valueSpaceType) {
					cint64 valueSpaceIndex = valueSpaceType->getValueSpaceTypeIndex();
					CXLinker<CDataLiteralValue*>* dataLiteralValueLinker = createFreeLinker();
					dataLiteralValueLinker->initLinker(dataLiteralValue);
					mUseDataLiteralValueVec[valueSpaceIndex] = dataLiteralValueLinker->append(mUseDataLiteralValueVec[valueSpaceIndex]);
					return this;
				}


				bool CDatatypeValueTestingCollection::isDataLiteralValueUsed(CDataLiteralValue* dataLiteralValue, CDatatypeValueSpaceType* valueSpaceType) {
					cint64 valueSpaceIndex = valueSpaceType->getValueSpaceTypeIndex();
					CXLinker<CDataLiteralValue*>* usedDataLiteralValueLinker = mUseDataLiteralValueVec[valueSpaceIndex];
					for (CXLinker<CDataLiteralValue*>* usedDataLiteralValueLinkerIt = usedDataLiteralValueLinker; usedDataLiteralValueLinkerIt; usedDataLiteralValueLinkerIt = usedDataLiteralValueLinkerIt->getNext()) {
						CDataLiteralValue* usedDataLiteralValue = usedDataLiteralValueLinkerIt->getData();
						if (usedDataLiteralValue->isEqualTo(dataLiteralValue)) {
							return true;
						}
					}
					return false;
				}



				CDataLiteralValue* CDatatypeValueTestingCollection::createDataLiteralValue(CDatatypeValueSpaceType* valueSpaceType) {
					CDataLiteralValue* dataLiteralValue = nullptr;
					cint64 valueSpaceIndex = valueSpaceType->getValueSpaceTypeIndex();
					CXLinker<CDataLiteralValue*>* dataLiteralValueLinker = mFreeDataLiteralValueVec[valueSpaceIndex];
					if (dataLiteralValueLinker) {
						dataLiteralValue = dataLiteralValueLinker->getData();
						CXLinker<CDataLiteralValue*>* nextDataLiteralValueLinker = dataLiteralValueLinker->getNext();
						mFreeDataLiteralValueVec[valueSpaceIndex] = nextDataLiteralValueLinker;
						dataLiteralValueLinker->clearNext();
						releaseFreeLinker(dataLiteralValueLinker);
					} else {
						if (valueSpaceType->getValueSpaceType() == CDatatypeValueSpaceType::VALUESPACEREALTYPE) {
							dataLiteralValue = CObjectAllocator< CDataLiteralRealValue >::allocateAndConstruct(mCollectionCalcAlgContext->getUsedProcessTaskMemoryAllocationManager());
						} else if (valueSpaceType->getValueSpaceType() == CDatatypeValueSpaceType::VALUESPACESTRINGTYPE) {
							dataLiteralValue = CObjectParameterizingAllocator< CDataLiteralStringValue, CContext* >::allocateAndConstructAndParameterize(mCollectionCalcAlgContext->getUsedProcessTaskMemoryAllocationManager(), mCollectionCalcAlgContext);
						} else if (valueSpaceType->getValueSpaceType() == CDatatypeValueSpaceType::VALUESPACEBOOLEANTYPE) {
							dataLiteralValue = CObjectAllocator< CDataLiteralBooleanValue>::allocateAndConstruct(mCollectionCalcAlgContext->getUsedProcessTaskMemoryAllocationManager());
						} else if (valueSpaceType->getValueSpaceType() == CDatatypeValueSpaceType::VALUESPACEDOUBLETYPE) {
							dataLiteralValue = CObjectAllocator< CDataLiteralDoubleValue>::allocateAndConstruct(mCollectionCalcAlgContext->getUsedProcessTaskMemoryAllocationManager());
						} else if (valueSpaceType->getValueSpaceType() == CDatatypeValueSpaceType::VALUESPACEFLOATTYPE) {
							dataLiteralValue = CObjectAllocator< CDataLiteralFloatValue>::allocateAndConstruct(mCollectionCalcAlgContext->getUsedProcessTaskMemoryAllocationManager());
						} else if (valueSpaceType->getValueSpaceType() == CDatatypeValueSpaceType::VALUESPACEIRITYPE) {
							dataLiteralValue = CObjectAllocator< CDataLiteralIRIValue>::allocateAndConstruct(mCollectionCalcAlgContext->getUsedProcessTaskMemoryAllocationManager());
						} else if (valueSpaceType->getValueSpaceType() == CDatatypeValueSpaceType::VALUESPACEXMLTYPE) {
							dataLiteralValue = CObjectAllocator< CDataLiteralXMLValue>::allocateAndConstruct(mCollectionCalcAlgContext->getUsedProcessTaskMemoryAllocationManager());
						} else if (valueSpaceType->getValueSpaceType() == CDatatypeValueSpaceType::VALUESPACEHEXDATATYPE) {
							dataLiteralValue = CObjectParameterizingAllocator< CDataLiteralBinaryHexDataValue,CContext* >::allocateAndConstructAndParameterize(mCollectionCalcAlgContext->getUsedProcessTaskMemoryAllocationManager(),mCollectionCalcAlgContext);
						} else if (valueSpaceType->getValueSpaceType() == CDatatypeValueSpaceType::VALUESPACEBASE64DATATYPE) {
							dataLiteralValue = CObjectParameterizingAllocator< CDataLiteralBinaryBase64DataValue,CContext* >::allocateAndConstructAndParameterize(mCollectionCalcAlgContext->getUsedProcessTaskMemoryAllocationManager(),mCollectionCalcAlgContext);
						} else if (valueSpaceType->getValueSpaceType() == CDatatypeValueSpaceType::VALUESPACEDATETIMETYPE) {
							dataLiteralValue = CObjectAllocator< CDataLiteralDateTimeValue >::allocateAndConstruct(mCollectionCalcAlgContext->getUsedProcessTaskMemoryAllocationManager());
						}
					}
					return dataLiteralValue;
				}

				CDatatypeValueTestingCollection* CDatatypeValueTestingCollection::releaseDataLiteralValue(CDataLiteralValue* dataLiteralValue, CDatatypeValueSpaceType* valueSpaceType) {
					if (dataLiteralValue) {
						cint64 valueSpaceIndex = valueSpaceType->getValueSpaceTypeIndex();
						CXLinker<CDataLiteralValue*>* dataLiteralValueLinker = createFreeLinker();
						dataLiteralValueLinker->initLinker(dataLiteralValue);
						mFreeDataLiteralValueVec[valueSpaceIndex] = dataLiteralValueLinker->append(mFreeDataLiteralValueVec[valueSpaceIndex]);
					}
					return this;
				}


				CXLinker<CDataLiteralValue*>* CDatatypeValueTestingCollection::createFreeLinker() {
					CXLinker<CDataLiteralValue*>* linker = mFreeLinker;
					if (linker) {
						mFreeLinker = mFreeLinker->getNext();
						linker->clearNext();
					}
					if (!linker) {
						linker = CObjectAllocator< CXLinker<CDataLiteralValue*> >::allocateAndConstruct(mCollectionCalcAlgContext->getUsedProcessTaskMemoryAllocationManager());
					}
					return linker;
				}

				CDatatypeValueTestingCollection*  CDatatypeValueTestingCollection::releaseFreeLinker(CXLinker<CDataLiteralValue*>* linker) {
					mFreeLinker = linker->append(mFreeLinker);
					return this;
				}



			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
