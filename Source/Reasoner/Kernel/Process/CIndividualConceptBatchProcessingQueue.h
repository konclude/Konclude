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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CINDIVIDUALCONCEPTBATCHPROCESSINGQUEUE_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CINDIVIDUALCONCEPTBATCHPROCESSINGQUEUE_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CIndividualConceptBatchProcessingData.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				/*! 
				 *
				 *		\class		CIndividualConceptBatchProcessingQueue
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CIndividualConceptBatchProcessingQueue {
					// public methods
					public:
						//! Constructor
						CIndividualConceptBatchProcessingQueue(CProcessContext* processContext);

						CIndividualConceptBatchProcessingQueue* initProcessingQueue(CIndividualConceptBatchProcessingQueue* processingQueue);


						bool takeNextConceptProcessIndividual(CConcept*& concept, CIndividualProcessNode*& indiNode, CConceptProcessDescriptor*& conProDes);
						CIndividualConceptBatchProcessingQueue* insertIndiviudalForConcept(CConcept* concept, CIndividualProcessNode* individual, CConceptProcessDescriptor* conProDes);
						CIndividualConceptBatchProcessingQueue* insertIndiviudalForBindingCount(CConcept* concept, cint64 bindCount, CIndividualProcessNode* individual, CConceptProcessDescriptor* conProDes);

						bool isEmpty();


						CIndividualConceptBatchProcessingData* getBatchProcessingData(CConcept* concept, bool createAndLocalize = true);
						CIndividualConceptBatchProcessingData* getBatchProcessingData(CConcept* concept, cint64 bindCount, bool createAndLocalize = true);


					// protected methods
					protected:

					// protected variables
					protected:
						class CIndividualConceptBatchProcessingQueueData {
							public:
								inline CIndividualConceptBatchProcessingQueueData() {
									mUseProcData = nullptr;
									mLocProcData = nullptr;
								}
								inline CIndividualConceptBatchProcessingQueueData(const CIndividualConceptBatchProcessingQueueData& data) {
									mUseProcData = data.mUseProcData;
									mLocProcData = nullptr;
								}
								CIndividualConceptBatchProcessingData* mUseProcData;
								CIndividualConceptBatchProcessingData* mLocProcData;
						};

						CProcessContext* mProcessContext;

						cint64 mCurrentProcessingTag;
						CIndividualConceptBatchProcessingData* mUseCurrentProcessingQueue;
						CIndividualConceptBatchProcessingData* mLocCurrentProcessingQueue;

						CPROCESSMAP<cint64,CIndividualConceptBatchProcessingQueueData> mConceptIndiQueueMap;
						CPROCESSMAP<cint64,CIndividualConceptBatchProcessingQueueData> mBindCountIndiQueueMap;

						class CIndividualConceptQueuedData {
							public:
								inline CIndividualConceptQueuedData(bool queued = false, cint64 prevTag = 0) {
									mPrevTag = prevTag;
									mQueued = queued;
								}
								cint64 mPrevTag;
								bool mQueued;
						};


						cint64 mBindindBasedQueuedCount;


						CPROCESSHASH< QPair<cint64,cint64>,CIndividualConceptQueuedData > mBindCountIndiQueuedHash;



					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CINDIVIDUALCONCEPTBATCHPROCESSINGQUEUE_H
