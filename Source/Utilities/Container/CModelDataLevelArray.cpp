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

#include "CModelDataLevelArray.h"


namespace Konclude {

	namespace Utilities {

		namespace Container {

			CModelDataLevelArray::CModelDataLevelArray() : CModelDataArray() {
				for (qint64 i = 0; i < dataSize; ++i) {
					modelArray[i] = 0;
				}
				level = 0;
			}


			CModelDataLevelArray::CModelDataLevelArray(qint64 tagBegin, qint64 tagBegEndSize) : CModelDataArray(tagBegin,tagBegEndSize) {
				for (qint64 i = 0; i < dataSize; ++i) {
					modelArray[i] = 0;
				}
			}


			CModelDataLevelArray::~CModelDataLevelArray() {
			}


			CModelDataArray *CModelDataLevelArray::init(qint64 tagBegin, qint64 tagBegEndSize) {
				CModelDataArray::init(tagBegin,tagBegEndSize);
				for (qint64 i = 0; i < dataSize; ++i) {
					modelArray[i] = 0;
				}
				return this;
			}

			bool CModelDataLevelArray::isBaseLevel() {
				return tagSize == dataSize;
			}
			
			qint64 CModelDataLevelArray::getModelLevel() {
				return level;
			}

			CModelDataLevelArray *CModelDataLevelArray::setBounds(qint64 tagBegin, qint64 tagBegEndSize) {
				CModelDataArray::setBounds(tagBegin,tagBegEndSize);
				level = bitIndShift / log2DataSize;
				return this;
			}

			CModelDataArray **CModelDataLevelArray::getModelArray() {
				return modelArray;
			}


			CModelDataArray *CModelDataLevelArray::getNextModelArray(qint64 index) {
				return modelArray[index];
			}

			CModelDataArray *CModelDataLevelArray::getBaseModel(qint64 beginTag) {	
				CModelDataArray *modelData = getNextModelArray(getArrayIndex(beginTag));
				if (modelData) {
					return modelData->getBaseModel(beginTag);
				} else {
					return 0;
				}
			}

			qint64 CModelDataLevelArray::getFirstFlagTag() {
				for (qint64 mask = 0x1, idx = 0; idx < dataSize; mask<<=1,++idx) {
					if ((mask & dataArray) != 0) {
						CModelDataArray *model = modelArray[idx];
						return model->getFirstFlagTag();
					}
				}
				return endTag;
			}

			qint64 CModelDataLevelArray::getLastFlagTag() {
				for (qint64 mask = Q_INT64_C(0x8000000000000000), idx = dataSize-1; idx >= 0; mask>>=1,--idx) {
					if ((mask & dataArray) != 0) {
						CModelDataArray *model = modelArray[idx];
						return model->getLastFlagTag();
					}
				}
				return begTag;
			}


			CModelDataArray *CModelDataLevelArray::setNextModelArray(qint64 index, CModelDataArray *nextModelDataArray) {
				modelArray[index] = nextModelDataArray;
				if (nextModelDataArray && nextModelDataArray->getDataArray() != 0) {
					setLevelFlagIndex(index,true);
				} else {
					setLevelFlagIndex(index,false);
				}
				return this;
			}


			CModelDataLevelArray *CModelDataLevelArray::setData(bool value) {
				CModelDataArray::setData(value);
				for (qint64 i = 0; i < dataSize; ++i) {
					CModelDataArray *modelData = modelArray[i];
					if (modelData) {
						modelData->setData(value);
					}
				}
				return this;
			}


			CModelDataArray *CModelDataLevelArray::createNextModelArray(qint64 index, CDataArrayMemoryManager *modelMemMan) {
				CModelDataArray *modelData = modelArray[index];
				if (!modelData && modelMemMan) {
					if (level <= 1) {
						modelData = modelMemMan->allocateModelDataArray();
					} else {
						modelData = modelMemMan->allocateModelDataLevelArray();
					}
					qint64 nextTagSize = tagSize/dataSize;
					qint64 nextBegTag = begTag + index*nextTagSize;
					modelData->init(nextBegTag,nextTagSize);
					modelArray[index] = modelData;
				}
				return modelData;
			}


			bool CModelDataLevelArray::setFlag(qint64 tag, bool flag, CDataArrayMemoryManager *modelMemMan) {
				if (flag) {
					CModelDataArray::setFlag(tag,flag);
				}
				if (begTag >= tag && tag < endTag) {
					qint64 index = (tag-begTag) >> bitIndShift;
					qint64 arrayIndex = index & bitIndMask;
					CModelDataArray *modelData = createNextModelArray(arrayIndex,modelMemMan);
					if (modelData->setFlag(tag,flag)) {
						if (modelData->getDataArray() == 0) {
							CModelDataArray::setLevelFlagIndex(arrayIndex,false);
						}
						return true;
					}
				}					
				return false;
			}


			bool CModelDataLevelArray::setFlag(qint64 tag, bool flag) {
				if (begTag >= tag && tag < endTag) {
					if (flag) {
						CModelDataArray::setFlag(tag,flag);
					}
					qint64 index = (tag-begTag) >> bitIndShift;
					qint64 arrayIndex = index & bitIndMask;
					CModelDataArray *modelData = modelArray[arrayIndex];
					if (modelData) {
						if (modelData->setFlag(tag,flag)) {
							if (modelData->getDataArray() == 0) {
								CModelDataArray::setFlag(tag,false);
							}
							return true;
						}
					}
				}
				return false;
			}

			bool CModelDataLevelArray::getFlag(qint64 tag) {
				if (begTag >= tag && tag < endTag) {
					qint64 index = (tag-begTag) >> bitIndShift;
					qint64 arrayIndex = index & bitIndMask;
					CModelDataArray *modelData = modelArray[arrayIndex];
					if (modelData) {
						return modelData->getFlag(tag);
					}
				}
				return 0;
			}


			qint64 CModelDataLevelArray::getArrayIndex(qint64 tag) {
				qint64 index = (tag-begTag) >> bitIndShift;
				qint64 arrayIndex = index & bitIndMask;
				return arrayIndex;
			}


			CModelDataArray *CModelDataLevelArray::intersectWith(CModelDataArray *modelData, bool *flagModification) {
				return intersectWith(modelData,0,flagModification);
			}


			CModelDataArray *CModelDataLevelArray::unionWith(CModelDataArray *modelData, bool *flagModification) {
				return unionWith(modelData,0,flagModification);
			}



			bool CModelDataLevelArray::testHasIntersectBetween(CModelDataArray *modelData1, CModelDataArray *modelData2) {
				if (!modelData1 || !modelData2) {
					return false;
				} else {
					CModelDataLevelArray *modelDataLevel = dynamic_cast<CModelDataLevelArray *>(modelData1);
					if (modelDataLevel) {
						modelData1->hasIntersectionWith(modelData2);
					}
					return modelData2->hasIntersectionWith(modelData1);
				}
			}


			bool CModelDataLevelArray::testIsSubsetBetween(CModelDataArray *modelData1, CModelDataArray *modelData2) {
				if (!modelData1) {
					return true;
				} else if (!modelData2) {
					return false;
				} else {
					CModelDataLevelArray *modelDataLevel = dynamic_cast<CModelDataLevelArray *>(modelData1);
					if (modelDataLevel) {
						return modelData1->isSubsetFrom(modelData2);
					} else {
						return modelData2->isSuperSetTo(modelData1);
					}
				}
			}


			bool CModelDataLevelArray::isSubsetFrom(CModelDataArray *modelData) {
				if (dataArray == 0) {
					return true;
				}
				if (modelData) {
					qint64 modelDataArray = modelData->getDataArray();
					if (modelDataArray == 0 && dataArray != 0) {
						return false;
					}
					qint64 modelDataBegTag = modelData->getBeginTag();
					qint64 modelDataEndTag = modelData->getEndTag();
					if (begTag <= modelDataBegTag && modelDataEndTag <= endTag) {
						qint64 modelDataTagSize = modelData->getTagSize();
						if (tagSize == modelDataTagSize) {
							// modelData has the same interval as this
							qint64 dataArrayIntersection = dataArray & modelDataArray;
							if (dataArrayIntersection != dataArray) {
								return false;
							} else {
								// test possible subset
								if (isBaseLevel()) {
									return true;
								} else {
									CModelDataLevelArray *modelDataLevelArray = (CModelDataLevelArray *)modelData;
									for (qint64 i = 0, mask = 0x1; i < 64 && mask <= dataArrayIntersection; ++i,mask <<= 1) {
										if ((dataArrayIntersection & mask) != 0) {
											CModelDataArray *nextLevelModel = modelArray[i];
											if (!nextLevelModel->isSubsetFrom(modelDataLevelArray->getNextModelArray(i))) {
												return false;
											}
										}
									}
									return true;
								}
							}
						} else {
							// modelData has a smaller interval than this
							if (modelDataArray != 0 && getLevelFlag(modelDataBegTag)) {
								qint64 arrayIndex = getArrayIndex(modelDataBegTag);
								if ((1<<arrayIndex) != dataArray) {
									return false;
								} else {
									// test modelData subset with next level
									CModelDataArray *nextLevelModel = modelArray[arrayIndex];
									return nextLevelModel->isSubsetFrom(modelData);
								}
							}
						}
					} else if (begTag >= modelDataBegTag && modelDataEndTag >= endTag) {
						// modelData has a larger interval than this
						CModelDataLevelArray *modelDataLevelArray = (CModelDataLevelArray *)modelData;
						qint64 arrayIndex = modelDataLevelArray->getArrayIndex(begTag);
						return isSubsetFrom(modelDataLevelArray->getNextModelArray(arrayIndex));
					}						
				} 
				return false;
			}


			bool CModelDataLevelArray::isSuperSetTo(CModelDataArray *modelData) {
				if (modelData == 0) {
					return true;
				}
				if (modelData) {
					qint64 modelDataArray = modelData->getDataArray();
					if (modelDataArray != 0 && dataArray == 0) {
						return false;
					}
					qint64 modelDataBegTag = modelData->getBeginTag();
					qint64 modelDataEndTag = modelData->getEndTag();
					if (begTag <= modelDataBegTag && modelDataEndTag <= endTag) {
						qint64 modelDataTagSize = modelData->getTagSize();
						if (tagSize == modelDataTagSize) {
							// modelData has the same interval as this
							qint64 dataArrayIntersection = dataArray & modelDataArray;
							if (dataArrayIntersection != modelDataArray) {
								return false;
							} else {
								// test possible subset
								if (isBaseLevel()) {
									return true;
								} else {
									CModelDataLevelArray *modelDataLevelArray = (CModelDataLevelArray *)modelData;
									for (qint64 i = 0, mask = 0x1; i < 64 && mask <= dataArrayIntersection; ++i,mask <<= 1) {
										if ((dataArrayIntersection & mask) != 0) {
											CModelDataArray *nextLevelModel = modelArray[i];
											if (!nextLevelModel->isSuperSetTo(modelDataLevelArray->getNextModelArray(i))) {
												return false;
											}
										}
									}
									return true;
								}
							}
						} else {
							// modelData has a smaller interval than this
							if (getLevelFlag(modelDataBegTag)) {
								qint64 arrayIndex = getArrayIndex(modelDataBegTag);
								// test modelData subset with next level
								CModelDataArray *nextLevelModel = modelArray[arrayIndex];
								return nextLevelModel->isSuperSetTo(modelData);
							}
						}
					} else if (begTag >= modelDataBegTag && modelDataEndTag >= endTag) {
						// modelData has a larger interval than this
						CModelDataLevelArray *modelDataLevelArray = (CModelDataLevelArray *)modelData;
						qint64 arrayIndex = modelDataLevelArray->getArrayIndex(begTag);
						if ((1<<arrayIndex) != modelDataArray) {
							return false;
						} else {
							return isSuperSetTo(modelDataLevelArray->getNextModelArray(arrayIndex));
						}
					}						
				} 
				return false;
			}

			bool CModelDataLevelArray::hasIntersectionWith(CModelDataArray *modelData) {
				if (modelData) {
					qint64 modelDataBegTag = modelData->getBeginTag();
					qint64 modelDataEndTag = modelData->getEndTag();
					if (begTag <= modelDataBegTag && modelDataEndTag <= endTag) {
						qint64 modelDataTagSize = modelData->getTagSize();
						if (tagSize == modelDataTagSize) {
							// modelData has the same interval as this
							qint64 modelDataArray = modelData->getDataArray();
							qint64 dataArrayIntersection = dataArray & modelDataArray;
							if (dataArrayIntersection != 0) {
								// test possible intersections
								if (isBaseLevel()) {
									return true;
								} else {
									CModelDataLevelArray *modelDataLevelArray = (CModelDataLevelArray *)modelData;
									for (qint64 i = 0, mask = 0x1; i < 64 && mask <= dataArrayIntersection; ++i,mask <<= 1) {
										if ((dataArrayIntersection & mask) != 0) {
											CModelDataArray *nextLevelModel = modelArray[i];
											if (nextLevelModel && nextLevelModel->hasIntersectionWith(modelDataLevelArray->getNextModelArray(i))) {
												return true;
											}
										}
									}
								}
							}
						} else {
							// modelData has a smaller interval than this
							qint64 modelDataArray = modelData->getDataArray();
							if (modelDataArray != 0 && getLevelFlag(modelDataBegTag)) {
								qint64 arrayIndex = getArrayIndex(modelDataBegTag);
								// test modelData intersection with next level
								CModelDataArray *nextLevelModel = modelArray[arrayIndex];
								if (nextLevelModel && nextLevelModel->hasIntersectionWith(modelData)) {
									return true;
								}
							}
						}
					} else if (begTag >= modelDataBegTag && modelDataEndTag >= endTag) {
						// modelData has a larger interval than this
						if (dataArray != 0) {
							CModelDataLevelArray *modelDataLevelArray = (CModelDataLevelArray *)modelData;
							qint64 arrayIndex = modelDataLevelArray->getArrayIndex(begTag);
							if (hasIntersectionWith(modelDataLevelArray->getNextModelArray(arrayIndex))) {
								return true;
							}
						}
					}
				}
				return false;
			}




			qint64 CModelDataLevelArray::countIntersectionWith(CModelDataArray *modelData) {
				qint64 inCount = 0;
				if (modelData) {
					qint64 modelDataBegTag = modelData->getBeginTag();
					qint64 modelDataEndTag = modelData->getEndTag();
					if (begTag <= modelDataBegTag && modelDataEndTag <= endTag) {
						qint64 modelDataTagSize = modelData->getTagSize();
						if (tagSize == modelDataTagSize) {
							// modelData has the same interval as this
							qint64 modelDataArray = modelData->getDataArray();
							qint64 dataArrayIntersection = dataArray & modelDataArray;
							if (dataArrayIntersection != 0) {
								// test possible intersections
								if (isBaseLevel()) {
									inCount = CModelDataArray::countIntersectionWith(modelData);
								} else {
									CModelDataLevelArray *modelDataLevelArray = (CModelDataLevelArray *)modelData;
									for (qint64 i = 0, mask = 0x1; i < 64 && mask <= dataArrayIntersection; ++i,mask <<= 1) {
										if ((dataArrayIntersection & mask) != 0) {
											CModelDataArray *nextLevelModel = modelArray[i];
											if (nextLevelModel) {
												inCount += nextLevelModel->countIntersectionWith(modelDataLevelArray->getNextModelArray(i));
											}
										}
									}
								}
							}
						} else {
							// modelData has a smaller interval than this
							qint64 modelDataArray = modelData->getDataArray();
							if (modelDataArray != 0 && getLevelFlag(modelDataBegTag)) {
								qint64 arrayIndex = getArrayIndex(modelDataBegTag);
								// test modelData intersection with next level
								CModelDataArray *nextLevelModel = modelArray[arrayIndex];
								if (nextLevelModel) {
									inCount += nextLevelModel->countIntersectionWith(modelData);
								}
							}
						}
					} else if (begTag >= modelDataBegTag && modelDataEndTag >= endTag) {
						// modelData has a larger interval than this
						if (dataArray != 0) {
							CModelDataLevelArray *modelDataLevelArray = (CModelDataLevelArray *)modelData;
							qint64 arrayIndex = modelDataLevelArray->getArrayIndex(begTag);
							inCount += countIntersectionWith(modelDataLevelArray->getNextModelArray(arrayIndex));
						}
					}
				}
				return inCount;
			}


			qint64 CModelDataLevelArray::countFlags() {
				qint64 count = 0;
				if (!isBaseLevel()) {
					for (qint64 i = 0, mask = 0x1; i < 64; ++i,mask <<= 1) {
						if ((dataArray & mask) != 0) {
							CModelDataArray *modelArray = getNextModelArray(i);
							if (modelArray) {
								count += modelArray->countFlags();
							}
						}
					}
				} else {
					count = CModelDataArray::countFlags();
				}
				return count;
			}



			CModelDataArray *CModelDataLevelArray::intersectWith(CModelDataArray *modelData, CDataArrayMemoryManager *modelMemMan, bool *flagModification) {
				if (modelData) {
					qint64 modelDataBegTag = modelData->getBeginTag();
					qint64 modelDataEndTag = modelData->getEndTag();
					if (begTag <= modelDataBegTag && modelDataEndTag <= endTag) {
						qint64 modelDataTagSize = modelData->getTagSize();
						if (tagSize == modelDataTagSize) {
							// modelData has the same interval as this
							CModelDataArray::intersectWith(modelData,flagModification);
							if (!isBaseLevel()) {
								CModelDataLevelArray *modelDataLevelArray = (CModelDataLevelArray *)modelData;
								for (qint64 i = 0, mask = 0x1; i < 64; ++i,mask <<= 1) {
									CModelDataArray *nextLevelModel = modelArray[i];
									if (nextLevelModel) {
										if ((dataArray & mask) != 0) {
											nextLevelModel->intersectWith(modelDataLevelArray->getNextModelArray(i),modelMemMan,flagModification);
										} else {
											// alternative is to remove next level data 
											nextLevelModel->setData(false);
										}
										if (nextLevelModel->getDataArray() == 0) {
											setLevelFlagIndex(i,false);
										}
									} 
								}
							}
						} else {
							// modelData has a smaller interval than this
							qint64 modelDataArray = modelData->getDataArray();
							if (modelDataArray != 0) {
								if (getLevelFlag(modelDataBegTag)) {
									qint64 arrayIndex = getArrayIndex(modelDataBegTag);
									// intersect modelData with next level
									for (qint64 i = 0; i < 64; ++i) {
										if (i != arrayIndex) {
											CModelDataArray *nextLevelModel = modelArray[i];
											// alternative is to remove next level data 
											if (nextLevelModel) {
												nextLevelModel->setData(false);
											}
										}
									}
									CModelDataArray *nextLevelModel = modelArray[arrayIndex];
									if (nextLevelModel) {
										nextLevelModel->intersectWith(modelData,modelMemMan,flagModification);
										setLevelData(false);
										setLevelFlagIndex(arrayIndex,nextLevelModel->getDataArray() != 0);
									} else {
										setLevelData(false);
									}
								} else {
									setData(false);
								}
							}
						}
					} else if (begTag >= modelDataBegTag && modelDataEndTag >= endTag) {
						// modelData has a larger interval than this
						CModelDataLevelArray *modelDataLevelArray = (CModelDataLevelArray *)modelData;
						qint64 arrayIndex = modelDataLevelArray->getArrayIndex(begTag);
						intersectWith(modelDataLevelArray->getNextModelArray(arrayIndex),modelMemMan,flagModification);
					} else {
						// modelData has another range than this
						// resulting in no intersection
						setData(false);
					}
				} else {
					setData(false);
				}
				return this;
			}


			CModelDataArray *CModelDataLevelArray::unionWith(CModelDataArray *modelData, CDataArrayMemoryManager *modelMemMan, bool *flagModification) {
				if (modelData) {
					qint64 modelDataBegTag = modelData->getBeginTag();
					qint64 modelDataEndTag = modelData->getEndTag();
					if (begTag <= modelDataBegTag && modelDataEndTag <= endTag) {
						qint64 modelDataTagSize = modelData->getTagSize();
						if (tagSize == modelDataTagSize) {
							// modelData has the same interval as this
							CModelDataArray::unionWith(modelData,flagModification);
							if (!isBaseLevel()) {
								CModelDataLevelArray *modelDataLevelArray = (CModelDataLevelArray *)modelData;
								for (qint64 i = 0, mask = 0x1; i < 64 && mask <= dataArray; ++i,mask <<= 1) {
									qint64 modelDataArray = modelDataLevelArray->getDataArray();
									if ((modelDataArray & mask) != 0) {
										CModelDataArray *nextLevelModel = createNextModelArray(i,modelMemMan);
										if (nextLevelModel) {
											nextLevelModel->unionWith(modelDataLevelArray->getNextModelArray(i),modelMemMan,flagModification);
										}
									}
								}
							}
						} else {
							// modelData has a smaller interval than this
							qint64 modelDataArray = modelData->getDataArray();
							if (modelDataArray != 0) {
								// intersect modelData with next level
								qint64 arrayIndex = getArrayIndex(modelDataBegTag);
								setLevelFlagIndex(arrayIndex,true);
								CModelDataArray *nextLevelModel = createNextModelArray(arrayIndex,modelMemMan);
								if (nextLevelModel) {
									nextLevelModel->unionWith(modelData,modelMemMan,flagModification);
								}
							}
						}
					} else if (begTag >= modelDataBegTag && modelDataEndTag >= endTag) {
						// modelData has a larger interval than this
						// executing this block produces mistakes, call extendModel before
						CModelDataLevelArray *modelDataLevelArray = (CModelDataLevelArray *)modelData;
						qint64 arrayIndex = modelDataLevelArray->getArrayIndex(begTag);
						this->unionWith(modelDataLevelArray->getNextModelArray(arrayIndex),modelMemMan,flagModification);
					}
				}
				return this;
			}


			CModelDataArray *CModelDataLevelArray::copyFrom(CModelDataArray *modelData, bool *flagModification) {
				return copyFrom(modelData,0,flagModification);
			}

			CModelDataArray *CModelDataLevelArray::copyFrom(CModelDataArray *modelData, CDataArrayMemoryManager *modelMemMan, bool *flagModification) {
				if (modelData) {
					qint64 modelDataBegTag = modelData->getBeginTag();
					qint64 modelDataEndTag = modelData->getEndTag();
					if (begTag <= modelDataBegTag && modelDataEndTag <= endTag) {
						qint64 modelDataTagSize = modelData->getTagSize();
						if (tagSize == modelDataTagSize) {
							// modelData has the same interval as this
							CModelDataArray::copyFrom(modelData,flagModification);
							if (!isBaseLevel()) {
								CModelDataLevelArray *modelDataLevelArray = (CModelDataLevelArray *)modelData;
								for (qint64 i = 0, mask = 0x1; i < 64 && mask <= dataArray; ++i,mask <<= 1) {
									CModelDataArray *nextLevelModel = modelArray[i];
									if ((dataArray & mask) != 0) {
										nextLevelModel = createNextModelArray(i,modelMemMan);
									}
									if (nextLevelModel) {
										if ((dataArray & mask) != 0) {
											nextLevelModel->copyFrom(modelDataLevelArray->getNextModelArray(i),modelMemMan,flagModification);
										} else {
											// alternative is to remove next level data 
											nextLevelModel->setData(false);
										}
									}
								}
							}
						} else {
							// modelData has a smaller interval than this
							qint64 modelDataArray = modelData->getDataArray();
							setLevelData(false);
							if (modelDataArray != 0) {
								qint64 arrayIndex = getArrayIndex(modelDataBegTag);
								// intersect modelData with next level
								for (qint64 i = 0; i < 64; ++i) {
									if (i != arrayIndex) {
										CModelDataArray *nextLevelModel = modelArray[i];
										// alternative is to remove next level data
										if (nextLevelModel) {
											nextLevelModel->setData(false);
										}
									}
								}
								CModelDataArray *nextLevelModel = createNextModelArray(arrayIndex,modelMemMan);
								nextLevelModel->copyFrom(modelData,modelMemMan,flagModification);
								if (nextLevelModel->getDataArray() != 0) {
									setLevelFlagIndex(arrayIndex,true);
								}
							}
						}
					} else if (begTag >= modelDataBegTag && modelDataEndTag >= endTag) {
						// modelData has a larger interval than this
						// executing this block produces mistakes, call extendModel before
						CModelDataLevelArray *modelDataLevelArray = (CModelDataLevelArray *)modelData;
						qint64 arrayIndex = modelDataLevelArray->getArrayIndex(begTag);
						copyFrom(modelDataLevelArray->getNextModelArray(arrayIndex),modelMemMan,flagModification);
					} else {
						// modelData has another range than this
						// executing this block produces mistakes, call extendModel before
						setData(false);
					}
				} else {
					setData(false);
				}
				return this;				
			}


			bool CModelDataLevelArray::isFullConstructed() {
				for (qint64 i = 0; i < dataSize; ++i) {
					if (modelArray[i] == 0) {
						return false;
					}
				}
				for (qint64 i = 0; i < dataSize; ++i) {
					if (!modelArray[i]->isFullConstructed()) {
						return false;
					}
				}
				return true;
			}



			CModelDataArray *CModelDataLevelArray::extendModel(CModelDataArray *modelData, qint64 minTag, CDataArrayMemoryManager *modelMemMan) {
				if (!modelData) {
					modelData = modelMemMan->allocateModelDataArray();
					qint64 extTagSize = dataSize;
					qint64 extBegTagDiff = minTag % extTagSize;
					qint64 extBegTag = minTag - extBegTagDiff;
					modelData->setBounds(extBegTag,extTagSize);
				}
				return extendModel(modelData,minTag,minTag,modelMemMan);
			}


			CModelDataArray *CModelDataLevelArray::extendModel(CModelDataArray *modelData, CModelDataArray *minModelDataExt, CDataArrayMemoryManager *modelMemMan) {
				if (minModelDataExt) {
					if (!modelData) {
						if (minModelDataExt->isBaseLevel()) {
							modelData = modelMemMan->allocateModelDataArray();
						} else {
							modelData = modelMemMan->allocateModelDataLevelArray();
						}
						modelData->init(minModelDataExt->getBeginTag(),minModelDataExt->getTagSize());
					} 
					return extendModel(modelData,minModelDataExt->getBeginTag(),minModelDataExt->getEndTag(),modelMemMan);
				} else {
					return modelData;
				}
			}



			CModelDataArray *CModelDataLevelArray::extendModel(CModelDataArray *modelData, qint64 minBegTag, qint64 minEndTag, CDataArrayMemoryManager *modelMemMan) {
				CModelDataArray *extendedModel = modelData;

				//for (qint64 i = 0; i < 64; ++i) {
				//	if (!extendedModel->isBaseLevel()) {
				//		if (((CModelDataLevelArray *)extendedModel)->getNextModelArray(i) == extendedModel) {
				//			bool bug = true;
				//		}
				//	}
				//}


				qint64 nBegTag = extendedModel->getBeginTag();
				while (minBegTag < nBegTag) {						
					CModelDataLevelArray *nextModelExtension = modelMemMan->allocateModelDataLevelArray();
					//if (nextModelExtension == extendedModel) {
					//	bool bug = true;
					//}
					qint64 extTagSize = extendedModel->getTagSize() * dataSize;
					qint64 extBegTagDiff = nBegTag % extTagSize;
					qint64 extBegTag = nBegTag - extBegTagDiff;
					nextModelExtension->init(extBegTag,extTagSize);
					qint64 arrayIndex = nextModelExtension->getArrayIndex(nBegTag);
					nextModelExtension->setNextModelArray(arrayIndex,extendedModel);
					extendedModel = nextModelExtension;
					nBegTag = extendedModel->getBeginTag();
				}

				//for (qint64 i = 0; i < 64; ++i) {
				//	if (!extendedModel->isBaseLevel()) {
				//		if (((CModelDataLevelArray *)extendedModel)->getNextModelArray(i) == extendedModel) {
				//			bool bug = true;
				//		}
				//	}
				//}


				nBegTag = extendedModel->getBeginTag();
				qint64 nEndTag = extendedModel->getEndTag();
				while (minEndTag > nEndTag) {
					CModelDataLevelArray *nextModelExtension = modelMemMan->allocateModelDataLevelArray();
					//if (nextModelExtension == extendedModel) {
					//	bool bug = true;
					//}

					qint64 extTagSize = extendedModel->getTagSize() * dataSize;
					qint64 extBegTagDiff = nBegTag % extTagSize;
					qint64 extBegTag = nBegTag - extBegTagDiff;
					nextModelExtension->init(extBegTag,extTagSize);
					qint64 arrayIndex = nextModelExtension->getArrayIndex(nBegTag);
					nextModelExtension->setNextModelArray(arrayIndex,extendedModel);
					extendedModel = nextModelExtension;
					nBegTag = extendedModel->getBeginTag();
					nEndTag = extendedModel->getEndTag();
				}

				//for (qint64 i = 0; i < 64; ++i) {
				//	if (!extendedModel->isBaseLevel()) {
				//		if (((CModelDataLevelArray *)extendedModel)->getNextModelArray(i) == extendedModel) {
				//			bool bug = true;
				//		}
				//	}
				//}

				return extendedModel;
			}


			CModelDataArray *CModelDataLevelArray::reduceModel(CModelDataArray *modelData) {
				if (modelData) {
					qint64 modelDataArray = modelData->getDataArray();
					if (modelDataArray == 0) {
						return 0;
					} else {
						if (modelData->isBaseLevel()) {
							return modelData;
						}
						qint64 onlyInd = -1;
						for (qint64 i = 0, mask = 0x1; i < 64; ++i,mask <<= 1) {
							if ((modelDataArray & mask) != 0) {
								if (onlyInd != -1) {
									return modelData;
								} else {
									onlyInd = i;
								}
							}
						}
						CModelDataArray *reducedModelData = ((CModelDataLevelArray *)modelData)->getNextModelArray(onlyInd);
						reducedModelData = reduceModel(reducedModelData);
						return reducedModelData;
					}
				}
				return 0;
			}



			CModelDataArray *CModelDataLevelArray::extendAndUnionModelWith(CModelDataArray *modelData, CModelDataArray *minModelDataExt, CDataArrayMemoryManager *modelMemMan, bool *flagModification) {
				modelData = extendModel(modelData,minModelDataExt,modelMemMan);
				if (modelData) {
					modelData->unionWith(minModelDataExt,modelMemMan,flagModification);
				}
				return modelData;
			}

			CModelDataArray *CModelDataLevelArray::extendAndCopyModelFrom(CModelDataArray *modelData, CModelDataArray *minModelDataExt, CDataArrayMemoryManager *modelMemMan, bool *flagModification) {
				modelData = extendModel(modelData,minModelDataExt,modelMemMan);
				if (modelData) {
					modelData->copyFrom(minModelDataExt,modelMemMan,flagModification);
					modelData = reduceModel(modelData);
				}
				return modelData;
			}

			CModelDataArray *CModelDataLevelArray::reduceAndIntersecModelWith(CModelDataArray *modelData, CModelDataArray *minModelDataExt, CDataArrayMemoryManager *modelMemMan, bool *flagModification) {
				if (modelData) {
					modelData->intersectWith(minModelDataExt,modelMemMan,flagModification);
					modelData = reduceModel(modelData);
				}
				return modelData;
			}

			CModelDataArray *CModelDataLevelArray::extendAndSetFlag(CModelDataArray *modelData, qint64 tag, CDataArrayMemoryManager *modelMemMan, bool *flagModification) {
				modelData = extendModel(modelData,tag,modelMemMan);
				if (!modelData->getFlag(tag)) {
					modelData->setFlag(tag,true,modelMemMan);
					if (flagModification) {
						*flagModification = true;
					}
				}
				return modelData;
			}


		}; // end namespace Container

	}; // end namespace Utilities

}; // end namespace Konclude
