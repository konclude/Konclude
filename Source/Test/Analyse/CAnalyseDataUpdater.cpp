/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#include "CAnalyseDataUpdater.h"


namespace Konclude {

	namespace Test {

		namespace Analyse {


			CAnalyseDataUpdater::CAnalyseDataUpdater() 
					: CIntervalThread("Analyse-Data-Updater") {
				nextTimerID = 1;
				startThread();
				waitSynchronization();
			}


			CAnalyseDataUpdater::~CAnalyseDataUpdater() {
				stopThread();
			}


			CAnalyseDataUpdater *CAnalyseDataUpdater::installAnalyseDataUpdate(CAnalyseDataSeries *dataSeries, qint64 updateInterval) {
				timerMutex.lock();
				qint64 timerID = nextTimerID++;
				CAnalyseDataUpdateItem *analyseDataUpdateItem = new CAnalyseDataUpdateItem(dataSeries,updateInterval,timerID);
				analyseUpdateHash.insert(dataSeries,analyseDataUpdateItem);
				timerIDUpdateHash.insert(timerID,analyseDataUpdateItem);
				timerMutex.unlock();
				startTimerWithInterval(timerID,updateInterval);
				return this;
			}



			CAnalyseDataUpdater *CAnalyseDataUpdater::installAnalyseDataUpdateObserver(CAnalyseDataSeries *dataSeries, CAnalyseDataObserver *observer) {
				timerMutex.lock();
				CAnalyseDataUpdateItem *updateItem = analyseUpdateHash.value(dataSeries,0);
				if (updateItem) {
					updateItem->addUpdateObserver(observer);
				}
				timerMutex.unlock();
				return this;
			}


			bool CAnalyseDataUpdater::processTimer(qint64 timerID) {
				bool timerProcessed = false;
				timerMutex.lock();
				if (timerIDUpdateHash.contains(timerID)) {
					CAnalyseDataUpdateItem *analyseDataUpdateItem = timerIDUpdateHash.value(timerID);
					if (analyseDataUpdateItem) {
						CAnalyseDataSeries *dataSeries = analyseDataUpdateItem->getAnalyseDataSeries();
						if (dataSeries) {
							CAnalyseDataDescription *dataDes = dataSeries->getAnalyseDataDescription();
							if (dataDes) {
								CAnalyseData *newAnalyseData = dataDes->createData();
								qint64 valueCount = dataDes->getAnalyseValueCount();
								for (int i = 0; i < valueCount; ++i) {
									CAnalyseValueDescription *valueDes = dataDes->getValueDescription(i);
									CAnalyseUpdateDescription *updateDes = dataDes->getUpdateDescription(i);
									if (valueDes && updateDes) {
										CAnalyseValueUpdateReader *updateReader = updateDes->getUpdateReader();
										if (updateReader) {
											CAnalyseValue *analyseValue = updateReader->getAnalyseValueUpdate(valueDes);
											if (analyseValue) {
												newAnalyseData->setValue(i,analyseValue);
											}
										}
									}
								}

								CAnalyseDataLinker *dataLinker = new CAnalyseDataLinker();
								dataSeries->addAnalyseData((CAnalyseDataLinker *)(dataLinker->init(newAnalyseData)));
								analyseDataUpdateItem->notifyObserversDataUpdate();
							}
						}
					}
					timerProcessed = true;
				}
				timerMutex.unlock();
				return timerProcessed;
			}


		}; // end namespace Analyse

	}; // end namespace Test

}; // end namespace Konclude
