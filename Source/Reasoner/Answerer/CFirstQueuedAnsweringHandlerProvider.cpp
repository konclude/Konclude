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

#include "CFirstQueuedAnsweringHandlerProvider.h"

namespace Konclude {

	namespace Reasoner {

		namespace Answerer {

			
			CFirstQueuedAnsweringHandlerProvider::CFirstQueuedAnsweringHandlerProvider(CAnsweringHandlerFactory* answeringHandlerFactory) {
				mAnsweringHandlerFactory = answeringHandlerFactory;
			}


			CFirstQueuedAnsweringHandlerProvider::~CFirstQueuedAnsweringHandlerProvider() {
			}
			

			CAnsweringHandler* CFirstQueuedAnsweringHandlerProvider::getAnsweringHandler(CComplexAnsweringQuery* complexAnsweringQuery) {
				CConcreteOntology* ontology = complexAnsweringQuery->getBaseOntology();
				return getAnsweringHandler(ontology, dynamic_cast<CComplexAnsweringCompositionQuery*>(complexAnsweringQuery) != nullptr);
			}

			CAnsweringHandler* CFirstQueuedAnsweringHandlerProvider::getAnsweringHandler(CConcreteOntology* ontology, bool composedQuery) {
				CAnsweringHandler* answeringHandler = nullptr;
				CAnsweringHandlerData*& handlerData = mOntoloyHandlerDataHash[ontology];
				if (!handlerData) {
					handlerData = new CAnsweringHandlerData();
				}
				if (!handlerData->mQueuedHandlerList.isEmpty()) {
					for (QList<CAnsweringHandler*>::iterator it = handlerData->mQueuedHandlerList.begin(), itEnd = handlerData->mQueuedHandlerList.end(); it != itEnd && !answeringHandler; ++it) {
						CAnsweringHandler* testAnsweringHandler = *it;
						if (testAnsweringHandler->canAnsweringComplexQuery(composedQuery)) {
							answeringHandler = testAnsweringHandler;
							it = handlerData->mQueuedHandlerList.erase(it);
						}
					}
				} 
				if (!answeringHandler) {
					answeringHandler = mAnsweringHandlerFactory->createAnsweringHandler(ontology, composedQuery);
					mHandlerHandlerDataHash.insert(answeringHandler, handlerData);
				}
				if (handlerData->mStatisticsCollection) {
					answeringHandler->storeAnsweringStatistics();
				}
				handlerData->mProcessingHandlerSet.insert(answeringHandler);
				return answeringHandler;
			}

			CAnsweringHandlerProvider* CFirstQueuedAnsweringHandlerProvider::releaseAnsweringHandler(CAnsweringHandler* answeringHandler) {
				CAnsweringHandlerData* handlerData = mHandlerHandlerDataHash[answeringHandler];
				handlerData->mProcessingHandlerSet.remove(answeringHandler);
				handlerData->mQueuedHandlerList.append(answeringHandler);
				if (handlerData->mStatisticsCollection) {
					answeringHandler->addAnsweringStatistics(handlerData->mCollectedStats, true);
				}
				return this;
			}



			CAnsweringHandlerProvider* CFirstQueuedAnsweringHandlerProvider::addAnsweringStatistics(CConcreteOntology* ontology, CAnsweringStatistics* stats) {
				CAnsweringHandlerData*& handlerData = mOntoloyHandlerDataHash[ontology];
				if (!handlerData) {
					handlerData = new CAnsweringHandlerData();
				}
				if (!handlerData->mStatisticsCollection) {
					handlerData->mStatisticsCollection = true;
					handlerData->mCollectedStats = new CAnsweringStatisticsCollectionStrings();					
					for (CAnsweringHandler* handler : handlerData->mQueuedHandlerList) {
						handler->addAnsweringStatistics(handlerData->mCollectedStats, false);
					}
				}
				for (const QString& statName : handlerData->mCollectedStats->getStatisticsNameStringList()) {
					cint64 value = handlerData->mCollectedStats->getStatisticIntegerValue(statName);
					stats->addProcessingStatistics(statName, value);
				}
				return this;
			}


		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
