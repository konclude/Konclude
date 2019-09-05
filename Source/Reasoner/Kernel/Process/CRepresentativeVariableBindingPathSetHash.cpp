/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU General Public License
 *		(LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU General Public License for more details.
 *
 *		You should have received a copy of the GNU General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "CRepresentativeVariableBindingPathSetHash.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CRepresentativeVariableBindingPathSetHash::CRepresentativeVariableBindingPathSetHash(CProcessContext* processContext) : mProcessContext(processContext),CPROCESSHASH<cint64,CRepresentativeVariableBindingPathSetHashData>(processContext) {
				}

				CRepresentativeVariableBindingPathSetHash* CRepresentativeVariableBindingPathSetHash::initRepresentativeVariableBindingPathSetHash(CRepresentativeVariableBindingPathSetHash* prevMap) {
					if (prevMap) {
						CPROCESSHASH<cint64, CRepresentativeVariableBindingPathSetHashData>::operator =(*prevMap);
					} else {
						CPROCESSHASH<cint64,CRepresentativeVariableBindingPathSetHashData>::clear();
					}
					return this;
				}


				CRepresentativeVariableBindingPathSetHash* CRepresentativeVariableBindingPathSetHash::insertRepresentativeVariableBindingPathSetData(CRepresentativeVariableBindingPathSetData* repSetData) {
					cint64 key = repSetData->getRepresentativeKey();
					CRepresentativeVariableBindingPathSetHashData& data = CPROCESSHASH<cint64,CRepresentativeVariableBindingPathSetHashData>::operator[](key);
					data.mLocDataLinker = repSetData->append(data.mUseDataLinker);
					data.mUseDataLinker = data.mLocDataLinker;
					return this;
				}

				CRepresentativeVariableBindingPathSetData* CRepresentativeVariableBindingPathSetHash::getRepresentativeVariableBindingPathSetData(CRepresentativeVariableBindingPathSetData* repSetData, bool createOrLocalize) {
					CRepresentativeVariableBindingPathSetData* repData = nullptr;
					cint64 key = repSetData->getRepresentativeKey();
					if (createOrLocalize) {
						CRepresentativeVariableBindingPathSetHashData& data = CPROCESSHASH<cint64,CRepresentativeVariableBindingPathSetHashData>::operator[](key);

						CRepresentativeVariableBindingPathSetData* foundDataLinker = nullptr;

						for (CRepresentativeVariableBindingPathSetData* dataLinkerIt = data.mUseDataLinker; dataLinkerIt && !foundDataLinker; dataLinkerIt = dataLinkerIt->getNext()) {
							if (dataLinkerIt->getRepresentativeID() == repSetData->getRepresentativeID()) {
								foundDataLinker = dataLinkerIt;
							}
						}


						CRepresentativeVariableBindingPathSetData* incLocDataLinker = nullptr;
						if (foundDataLinker) {
							incLocDataLinker = foundDataLinker;
						}


						if (incLocDataLinker) {
							CRepresentativeVariableBindingPathSetData* returnDataLinker = nullptr;
							CRepresentativeVariableBindingPathSetData* nextIncLocDataLinker = incLocDataLinker->getNext();
							CRepresentativeVariableBindingPathSetData* newLocDataLinker = nullptr;
							CRepresentativeVariableBindingPathSetData* lastNewLocDataLinker = nullptr;
							for (CRepresentativeVariableBindingPathSetData* dataLinkerIt = data.mUseDataLinker; dataLinkerIt != nextIncLocDataLinker; dataLinkerIt = dataLinkerIt->getNext()) {
								CRepresentativeVariableBindingPathSetData* locLinker = dataLinkerIt;
								if (!dataLinkerIt->isLocalizationTagUpToDate(mProcessContext->getProcessTagger())) {
									locLinker = CObjectParameterizingAllocator<CRepresentativeVariableBindingPathSetData,CProcessContext*>::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
									locLinker->initRepresentativeVariableBindingPathData(dataLinkerIt);
								}
								if (dataLinkerIt == incLocDataLinker) {
									returnDataLinker = locLinker;
								}
								if (lastNewLocDataLinker) {
									lastNewLocDataLinker->setNext(locLinker);
									lastNewLocDataLinker = locLinker;
								} else {
									lastNewLocDataLinker = locLinker;
									newLocDataLinker = locLinker;
								}
							}

							data.mLocDataLinker = newLocDataLinker;
							data.mUseDataLinker = newLocDataLinker;
							return returnDataLinker;
						} else {
							CRepresentativeVariableBindingPathSetData* locLinker = CObjectParameterizingAllocator<CRepresentativeVariableBindingPathSetData,CProcessContext*>::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
							locLinker->initRepresentativeVariableBindingPathData(nullptr);
							locLinker->setNext(data.mUseDataLinker);
							data.mLocDataLinker = locLinker;
							data.mUseDataLinker = locLinker;

							return data.mLocDataLinker;
						}

					} else {
						const CRepresentativeVariableBindingPathSetHashData& data = CPROCESSHASH<cint64,CRepresentativeVariableBindingPathSetHashData>::value(key);

						for (CRepresentativeVariableBindingPathSetData* dataLinkerIt = data.mUseDataLinker; dataLinkerIt; dataLinkerIt = dataLinkerIt->getNext()) {
							if (dataLinkerIt->getRepresentativeID() == repSetData->getRepresentativeID()) {
								return dataLinkerIt;
							}
						}
					}
					return nullptr;
				}



				CRepresentativeVariableBindingPathSetData* CRepresentativeVariableBindingPathSetHash::getRepresentativeVariableBindingPathSetData(CRepresentativePropagationSet* repPropSet, bool createOrLocalize) {
					CRepresentativeVariableBindingPathSetData* repData = nullptr;
					cint64 key = repPropSet->getIncomingRepresentativePropagationSignatureKey();
					if (createOrLocalize) {
						CRepresentativeVariableBindingPathSetHashData& data = CPROCESSHASH<cint64,CRepresentativeVariableBindingPathSetHashData>::operator[](key);

						CRepresentativeVariableBindingPathSetData* selectedDataLinker = nullptr;
						CRepresentativeVariableBindingPathSetData* foundDataLinker = nullptr;

						for (CRepresentativeVariableBindingPathSetData* dataLinkerIt = data.mUseDataLinker; dataLinkerIt && !foundDataLinker; dataLinkerIt = dataLinkerIt->getNext()) {
							if (!dataLinkerIt->hasMigrateData()) {
								if (!selectedDataLinker) {
									selectedDataLinker = dataLinkerIt;
								}
							} else {
								// test identical
								CRepresentativeVariableBindingPathSetMigrateData* migrateData = dataLinkerIt->getMigrateData(false);

								CRepresentativeContainingMap* repContMap = migrateData->getRepresentativeContainingMap();
								CRepresentativePropagationMap* repPropMap = repPropSet->getRepresentativePropagationMap();

								if (isRepresentativePropagationMapIdenticalToRepresentativeContainingMap(repPropMap,repContMap)) {
									foundDataLinker = dataLinkerIt;
								}
							}
						}


						CRepresentativeVariableBindingPathSetData* incLocDataLinker = nullptr;
						if (foundDataLinker) {
							incLocDataLinker = foundDataLinker;
						} else if (selectedDataLinker) {
							incLocDataLinker = selectedDataLinker;
						}


						if (incLocDataLinker) {
							CRepresentativeVariableBindingPathSetData* returnDataLinker = nullptr;
							CRepresentativeVariableBindingPathSetData* nextIncLocDataLinker = incLocDataLinker->getNext();
							CRepresentativeVariableBindingPathSetData* newLocDataLinker = nullptr;
							CRepresentativeVariableBindingPathSetData* lastNewLocDataLinker = nullptr;
							for (CRepresentativeVariableBindingPathSetData* dataLinkerIt = data.mUseDataLinker; dataLinkerIt != nextIncLocDataLinker; dataLinkerIt = dataLinkerIt->getNext()) {
								CRepresentativeVariableBindingPathSetData* locLinker = dataLinkerIt;
								if (!dataLinkerIt->isLocalizationTagUpToDate(mProcessContext->getProcessTagger())) {
									locLinker = CObjectParameterizingAllocator<CRepresentativeVariableBindingPathSetData,CProcessContext*>::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
									locLinker->initRepresentativeVariableBindingPathData(dataLinkerIt);
								}
								if (dataLinkerIt == incLocDataLinker) {
									returnDataLinker = locLinker;
								}
								if (lastNewLocDataLinker) {
									lastNewLocDataLinker->setNext(locLinker);
									lastNewLocDataLinker = locLinker;
								} else {
									lastNewLocDataLinker = locLinker;
									newLocDataLinker = locLinker;
								}
							}

							data.mLocDataLinker = newLocDataLinker;
							data.mUseDataLinker = newLocDataLinker;
							return returnDataLinker;
						} else {
							CRepresentativeVariableBindingPathSetData* locLinker = CObjectParameterizingAllocator<CRepresentativeVariableBindingPathSetData,CProcessContext*>::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
							locLinker->initRepresentativeVariableBindingPathData(nullptr);
							locLinker->setNext(data.mUseDataLinker);
							data.mLocDataLinker = locLinker;
							data.mUseDataLinker = locLinker;

							return data.mLocDataLinker;
						}

					} else {
						const CRepresentativeVariableBindingPathSetHashData& data = CPROCESSHASH<cint64,CRepresentativeVariableBindingPathSetHashData>::value(key);

						for (CRepresentativeVariableBindingPathSetData* dataLinkerIt = data.mUseDataLinker; dataLinkerIt; dataLinkerIt = dataLinkerIt->getNext()) {
							if (dataLinkerIt->hasMigrateData()) {

								CRepresentativeVariableBindingPathSetMigrateData* migrateData = dataLinkerIt->getMigrateData(false);
								CRepresentativeContainingMap* repContMap = migrateData->getRepresentativeContainingMap();
								CRepresentativePropagationMap* repPropMap = repPropSet->getRepresentativePropagationMap();

								if (isRepresentativePropagationMapIdenticalToRepresentativeContainingMap(repPropMap,repContMap)) {
									return dataLinkerIt;
								}
							}
						}
					}
					return nullptr;
				}


				bool CRepresentativeVariableBindingPathSetHash::isRepresentativePropagationMapIdenticalToRepresentativeContainingMap(CRepresentativePropagationMap* repPropMap, CRepresentativeContainingMap* repContMap) {
					if (repPropMap->count() != repContMap->count()) {
						return false;
					}

					CRepresentativePropagationMap::const_iterator repPropIt = repPropMap->constBegin(), repPropItEnd = repPropMap->constEnd();
					CRepresentativeContainingMap::const_iterator repContIt = repContMap->constBegin(), repContItEnd = repContMap->constEnd();
					while (repPropIt != repPropItEnd) {
						if (repPropIt.key() != repContIt.key()) {
							return false;
						}
						++repPropIt;
						++repContIt;
					}
					return true;
				}

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
