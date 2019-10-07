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

#include "CModelDataArray.h"


namespace Konclude {

	namespace Utilities {

		namespace Container {

			CModelDataArray::CModelDataArray() {
				bitIndMask = dataSize-1;
				begTag = 0;					
				tagSize = endTag = dataSize;
				bitIndShift = 0;
				dataArray = 0;
			}


			CModelDataArray::CModelDataArray(qint64 tagBegin, qint64 tagBegEndSize) {
				setBounds(tagBegin,tagBegEndSize);
				dataArray = 0;
			}


			CModelDataArray::~CModelDataArray() {
			}


			CModelDataArray *CModelDataArray::init(qint64 tagBegin, qint64 tagBegEndSize) {
				setBounds(tagBegin,tagBegEndSize);
				dataArray = 0;
				return this;
			}



			CModelDataArray *CModelDataArray::setBounds(qint64 tagBegin, qint64 tagBegEndSize) {
				begTag = tagBegin;
				tagSize = tagBegEndSize;
				endTag = begTag + tagSize;

				bitIndShift = CMath::log2(tagSize>>log2DataSize);
				bitIndMask = dataSize-1;
				return this;
			}


			bool CModelDataArray::isBaseLevel() {
				return true;
			}

			qint64 CModelDataArray::getModelLevel() {
				return 0;
			}

			qint64 CModelDataArray::getBeginTag() {
				return begTag;
			}

			qint64 CModelDataArray::getEndTag() {
				return endTag;
			}

			qint64 CModelDataArray::getTagSize() {
				return tagSize;
			}

			qint64 CModelDataArray::getDataArray() {
				return dataArray;
			}


			CModelDataArray *CModelDataArray::setData(bool value) {
				if (value == false) {
					dataArray = 0;
				} else {
					dataArray = Q_INT64_C(0xffffffffffffffff);
				}
				return this;
			}

			CModelDataArray *CModelDataArray::setLevelData(bool value) {
				if (value == false) {
					dataArray = 0;
				} else {
					dataArray = Q_INT64_C(0xffffffffffffffff);
				}
				return this;
			}


			bool CModelDataArray::setFlag(qint64 tag, bool flag, CDataArrayMemoryManager *modelMemMan) {
				return setFlag(tag,flag);;
			}

			
			CModelDataArray *CModelDataArray::getBaseModel(qint64 beginTag) {
				if (begTag != beginTag) {
					return 0;
				} else {
					return this;
				}
			}


			bool CModelDataArray::setFlag(qint64 tag, bool flag) {
				if (begTag <= tag && tag < endTag) {
					qint64 index = (tag-begTag) >> bitIndShift;
					qint64 bitIndex = index & bitIndMask;
					if (flag) {
						dataArray |= qint64(1)<<bitIndex;
					} else {
						dataArray &= ~(qint64(1)<<bitIndex);
					}
					return true;
				}
				return false;
			}

			bool CModelDataArray::getFlag(qint64 tag) {
				if (begTag <= tag && tag < endTag) {
					qint64 index = (tag-begTag) >> bitIndShift;
					qint64 bitIndex = index & bitIndMask;
					return (dataArray & qint64(1)<<bitIndex) != 0;
				}
				return false;
			}


			bool CModelDataArray::setLevelFlag(qint64 tag, bool flag) {
				return CModelDataArray::setFlag(tag,flag);
			}

			bool CModelDataArray::getLevelFlag(qint64 tag) {
				return CModelDataArray::getFlag(tag);
			}


			bool CModelDataArray::setLevelFlagIndex(qint64 index, bool flag) {
				if (index >= 0 && index < dataSize) {
					qint64 bitIndex = index;
					if (flag) {
						dataArray |= qint64(1)<<bitIndex;
					} else {
						dataArray &= ~(qint64(1)<<bitIndex);
					}
					return true;
				}
				return false;
			}

			bool CModelDataArray::getLevelFlagIndex(qint64 index) {
				if (index >= 0 && index < dataSize) {
					qint64 bitIndex = index;
					return (dataArray & qint64(1)<<bitIndex) != 0;
				}
				return false;
			}


			qint64 CModelDataArray::countFlags() {
				qint64 count = 0;
				for (qint64 mask = 0x1, cn = 0; cn < dataSize; mask<<=1,++cn) {
					count += (bool)(mask & dataArray);
				}
				return count;
			}

			bool CModelDataArray::isEmpty() {
				return dataArray == 0;
			}


			qint64 CModelDataArray::getFirstFlagTag() {
				qint64 flagTag = begTag;
				for (qint64 mask = 0x1, cn = 0; cn < dataSize; mask<<=1,++cn,++flagTag) {
					if ((mask & dataArray) != 0) {
						break;
					}
				}
				return flagTag;
			}

			qint64 CModelDataArray::getLastFlagTag() {
				qint64 flagTag = endTag-1;
				for (qint64 mask = Q_INT64_C(0x8000000000000000), cn = 0; cn < dataSize; mask>>=1,++cn,--flagTag) {
					if ((mask & dataArray) != 0) {
						break;
					}
				}
				return flagTag;
			}


			CModelDataArray *CModelDataArray::intersectWith(CModelDataArray *modelData, bool *flagModification) {
				if (modelData) {
					CModelDataArray *testModelData = 0;
					if (isBaseLevel()) {
						testModelData = modelData->getBaseModel(begTag);
					} else {
						testModelData = modelData;
					}
					if (testModelData && begTag == testModelData->getBeginTag() && tagSize == testModelData->getTagSize()) {
						qint64 dataArrayBackup = dataArray;
						dataArray &= testModelData->dataArray;
						if (dataArrayBackup != dataArray && flagModification) {
							*flagModification = true;
						}
					} else {
						if (0 != dataArray && flagModification) {
							*flagModification = true;
						}
						dataArray = 0;
					}
				} else {						
					if (0 != dataArray && flagModification) {
						*flagModification = true;
					}
					dataArray = 0;
				}
				return this;
			}


			CModelDataArray *CModelDataArray::unionWith(CModelDataArray *modelData, bool *flagModification) {
				if (modelData) {
					if (begTag == modelData->getBeginTag() && tagSize == modelData->getTagSize()) {
						qint64 dataArrayBackup = dataArray;
						dataArray |= modelData->dataArray;
						if (dataArrayBackup != dataArray && flagModification) {
							*flagModification = true;
						}
					}
				}
				return this;
			}


			CModelDataArray *CModelDataArray::intersectWith(CModelDataArray *modelData, CDataArrayMemoryManager *modelMemMan, bool *flagModification) {
				if (modelData) {
					CModelDataArray *testModelData = 0;
					if (isBaseLevel()) {
						testModelData = modelData->getBaseModel(begTag);
					} else {
						testModelData = modelData;
					}
					if (testModelData && begTag == testModelData->getBeginTag() && tagSize == testModelData->getTagSize()) {
						qint64 dataArrayBackup = dataArray;
						dataArray &= testModelData->dataArray;
						if (dataArrayBackup != dataArray && flagModification) {
							*flagModification = true;
						}
					} else {
						if (0 != dataArray && flagModification) {
							*flagModification = true;
						}
						dataArray = 0;
					}
				} else {						
					if (0 != dataArray && flagModification) {
						*flagModification = true;
					}
					dataArray = 0;
				}
				return this;
			}


			CModelDataArray *CModelDataArray::unionWith(CModelDataArray *modelData, CDataArrayMemoryManager *modelMemMan, bool *flagModification) {
				if (modelData) {
					if (begTag == modelData->getBeginTag() && tagSize == modelData->getTagSize()) {
						qint64 dataArrayBackup = dataArray;
						dataArray |= modelData->dataArray;
						if (dataArrayBackup != dataArray && flagModification) {
							*flagModification = true;
						}
					}
				}
				return this;
			}


			bool CModelDataArray::needsExtension(qint64 minBegTag, qint64 minEndTag) {
				if (minBegTag < begTag || endTag > minEndTag) {
					return true;
				} else {
					return false;
				}
			}


			bool CModelDataArray::needsExtension(qint64 tag) {
				if (tag < begTag || endTag > tag) {
					return true;
				} else {
					return false;
				}
			}


			CModelDataArray *CModelDataArray::copyFrom(CModelDataArray *modelData, bool *flagModification) {
				if (modelData) {
					if (begTag == modelData->getBeginTag() && tagSize == modelData->getTagSize()) {
						qint64 dataArrayBackup = dataArray;
						dataArray = modelData->getDataArray();
						if (dataArrayBackup != dataArray && flagModification) {
							*flagModification = true;
						}
					}
				} else {
					setData(false);
				}
				return this;
			}

			CModelDataArray *CModelDataArray::copyFrom(CModelDataArray *modelData, CDataArrayMemoryManager *modelMemMan, bool *flagModification) {
				if (modelData) {
					if (begTag == modelData->getBeginTag() && tagSize == modelData->getTagSize()) {
						qint64 dataArrayBackup = dataArray;
						dataArray = modelData->getDataArray();
						if (dataArrayBackup != dataArray && flagModification) {
							*flagModification = true;
						}
					}
				} else {
					setData(false);
				}
				return this;
			}


			bool CModelDataArray::hasIntersectionWith(CModelDataArray *modelData) {
				if (modelData) {
					CModelDataArray *baseModelTag = modelData->getBaseModel(begTag);
					if (baseModelTag && begTag == baseModelTag->getBeginTag() && tagSize == baseModelTag->getTagSize()) {
						return (dataArray & baseModelTag->getDataArray()) != 0;
					} 
				}
				return false;
			}

			qint64 CModelDataArray::countIntersectionWith(CModelDataArray *modelData) {
				qint64 inCount = 0;
				if (modelData) {
					CModelDataArray *baseModelTag = modelData->getBaseModel(begTag);
					if (baseModelTag && begTag == baseModelTag->getBeginTag() && tagSize == baseModelTag->getTagSize()) {
						qint64 otherBaseDataArray = baseModelTag->getDataArray();
						qint64 intersectDataArray = dataArray & otherBaseDataArray;
						if ((intersectDataArray) != 0) {
							for (qint64 mask = 0x1, cn = 0; cn < dataSize; mask<<=1,++cn) {
								if ((mask & intersectDataArray) != 0) {
									inCount++;
								}
							}

						}
					}
				}
				return inCount;
			}

			bool CModelDataArray::isSuperSetTo(CModelDataArray *modelData) {
				if (!modelData || modelData->isEmpty()) {
					return true;
				}
				if (modelData->getFirstFlagTag() < begTag) {
					return false;
				}
				if (modelData->getLastFlagTag() < endTag) {
					return false;
				}
				CModelDataArray *testModelData = modelData;
				if (isBaseLevel()) {
					testModelData = modelData->getBaseModel(begTag);
				}

				qint64 modelDataArray = testModelData->getDataArray();
				return (dataArray & modelDataArray) == modelDataArray;
			}


			bool CModelDataArray::isSubsetFrom(CModelDataArray *modelData) {
				if (isEmpty()) {
					return true;
				}
				if (modelData) {
					CModelDataArray *testModelData = modelData;
					if (isBaseLevel()) {
						testModelData = modelData->getBaseModel(begTag);
					}
					if (!testModelData) {
						return false;
					}
					return (dataArray & testModelData->getDataArray()) == dataArray;
				}
				return false;
			}


			bool CModelDataArray::needsExtension(CModelDataArray *otherModelData) {
				if (otherModelData) {
					return needsExtension(otherModelData->getBeginTag(),otherModelData->getEndTag());
				}
				return false;
			}

			bool CModelDataArray::isFullConstructed() {
				return true;
			}


		}; // end namespace Container

	}; // end namespace Utilities

}; // end namespace Konclude
