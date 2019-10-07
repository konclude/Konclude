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

#include "CSPARQLResultStreamingData.h"


namespace Konclude {

	namespace Control {

		namespace Interface {

			namespace SPARQL {



				CSPARQLResultStreamingData::CSPARQLResultStreamingData(CQueryCommandProvider* queryProvider, cint64 sequenceNumber, cint64 bufferSize, CSPARQLResultStreamingController* streamingController) {
					init(sequenceNumber, bufferSize, streamingController);
					mQueryProvider = queryProvider;
					mQuery = nullptr;
				}

				CSPARQLResultStreamingData::CSPARQLResultStreamingData(CQuery* query, cint64 sequenceNumber, cint64 bufferSize, CSPARQLResultStreamingController* streamingController) {
					init(sequenceNumber, bufferSize, streamingController);
					mQuery = query;
					mQueryProvider = nullptr;
				}



				CSPARQLResultStreamingData::~CSPARQLResultStreamingData() {
					delete mSerializer;
					delete mBufferMutex;
				}


				CSPARQLResultStreamingData* CSPARQLResultStreamingData::init(cint64 sequenceNumber, cint64 bufferSize, CSPARQLResultStreamingController* streamingController) {
					mBufferMutex = new QMutex(QMutex::Recursive);
					mStreamingController = streamingController;
					mInitialBufferSize = 1000000;
					mBufferSize = mInitialBufferSize;
					mMaxBufferSize = bufferSize;
					mSequenceNumber = sequenceNumber;
					mStreamingResult = nullptr;

					QByteArray* bufferArray = new QByteArray();
					bufferArray->reserve(mBufferSize + 10000);
					mSerializer = new CSPARQLXMLAnswerStreamingPrecompiledByteArraySerializer(bufferArray, false);
					mStreamingFinished = false;
					mStreamingInitialized = false;
					mFinalized = false;
					mUsedBufferSize = 0;
					mFlushId = 0;
					mContinueSerialization = true;
					mWritingScheduled = false;
					mConcurrentFinishCount = 0;
					return this;
				}


				CQuery* CSPARQLResultStreamingData::getQuery() {
					if (!mQuery && mQueryProvider) {
						mQuery = mQueryProvider->getQuery();
					}
					if (mQuery) {
						CComplexAssertionsIndividualVariablesAnsweringQuery* compAssIndVarQuery = dynamic_cast<CComplexAssertionsIndividualVariablesAnsweringQuery*>(mQuery);
						if (compAssIndVarQuery) {
							if (!compAssIndVarQuery->getQueryResult()) {
								if (!compAssIndVarQuery->getResultOrderingLinker() && !compAssIndVarQuery->isBooleanEntailmentResultRequired()) {

									LOG(NOTICE, "::Konclude::Control::Interface::SPARQL::SPARQLResultStreamer", logTr("Enabling results streaming for query '%1'.").arg(mQuery->getQueryName()), this);

									mStreamingResult = new CVariableBindingsAnswersStreamingResult(this);
									compAssIndVarQuery->setQueryResult(mStreamingResult);
								}
							}
						}
					}
					return mQuery;
				}

				CCommand* CSPARQLResultStreamingData::getCommand() {
					return mQueryProvider->getCommand();
				}




				bool  CSPARQLResultStreamingData::initResultStreaming(const QStringList& varNames) {
					mVarNames = varNames;
					if (!mStreamingInitialized) {
						mStreamingInitialized = true;
						mSerializer->addResultStreamingStart(mVarNames);
					}
					return mStreamingController->canWrite();
				}

				CVariableBindingsAnswersStreamingHandler* CSPARQLResultStreamingData::streamResultVariableBindings(CVariableBindingsAnswerResult* varBindings, cint64 cardinality) {
					if (mStreamingInitialized && !mStreamingFinished && mContinueSerialization) {
						mSerializer->addResultStreamingBindingToTemporaryBuffer(mVarNames, varBindings);
						for (cint64 i = 0; i < cardinality; ++i) {
							mSerializer->addTemporaryBuffer();
							if (mSerializer->getBufferWrittenSize() > mBufferSize) {
								mWritingScheduled = true;
								mBufferSize *= 2;								
								if (mBufferSize > mMaxBufferSize) {
									mBufferSize = mMaxBufferSize;
								}
								mContinueSerialization &= streamingFlush();
							}
						}
						mSerializer->clearTemporaryBuffer();
					}
					return this;
				}


				bool CSPARQLResultStreamingData::streamingFlush() {
					if (mSerializer->getBufferWrittenSize() > 0 || !mUsedBufferList.isEmpty()) {
						mWritingScheduled = true;
						if (mSerializer->getBufferWrittenSize() > 0) {
							mSerializer->writeFlush(++mFlushId);
							QByteArray* bufferArray = new QByteArray();
							bufferArray->reserve(mBufferSize + 10000);
							QByteArray* usedBuffer = mSerializer->updateWriteBuffer(bufferArray);
							mBufferMutex->lock();
							mUsedBufferSize += usedBuffer->size();
							mUsedBufferList.append(CSPARQLResultBufferWriteData(1, usedBuffer));
							mBufferMutex->unlock();
						}
						mContinueSerialization &= mStreamingController->notifyWriteRequest(mSequenceNumber);
					}
					return mContinueSerialization;
				}


				CVariableBindingsAnswersStreamingHandler* CSPARQLResultStreamingData::finishResultStreaming() {
					if (mStreamingInitialized && !mStreamingFinished) {
						streamingFlush();
						mStreamingFinished = true;
						mSerializer->addResultSerialization(mVarNames, mStreamingResult);
					}
					return this;
				}


				CSPARQLResultStreamingData* CSPARQLResultStreamingData::finalize() {
					if (!mFinalized) {
						QByteArray* usedBuffer = mSerializer->updateWriteBuffer(nullptr);
						mBufferMutex->lock();
						mUsedBufferSize += usedBuffer->size();
						mUsedBufferList.append(CSPARQLResultBufferWriteData(1, usedBuffer));
						mBufferMutex->unlock();
						mFinalized = true;
					}
					return this;
				}


				CSPARQLResultStreamingData* CSPARQLResultStreamingData::handleQueryResult(CQuery* query, CQueryResult* queryResult) {
					finishResultStreaming();
					if (mStreamingResult != queryResult) {

						CComplexAssertionsIndividualVariablesAnsweringQuery* compAssIndVarQuery = dynamic_cast<CComplexAssertionsIndividualVariablesAnsweringQuery*>(query);
						if (compAssIndVarQuery) {
							if (dynamic_cast<CBooleanQueryResult*>(queryResult)) {
								mSerializer->addResultSerialization((CBooleanQueryResult*)queryResult);
							} else if (dynamic_cast<CVariableBindingsAnswersResult*>(queryResult)) {
								CVariableBindingsAnswersResult* varBindAnsRes = (CVariableBindingsAnswersResult*)queryResult;
								QStringList varList;
								for (auto varExp : *compAssIndVarQuery->getDistinguishedVariableExpressions()) {
									varList.append(varExp->getName());
								}
								mSerializer->addResultSerialization(varList, varBindAnsRes);
							}
						}


					}
					CQueryStatistics* stats = query->getQueryStatistics();
					if (stats) {
						mSerializer->addResultStatistics(stats);
					}
					return this;
				}

				CSPARQLResultStreamingData* CSPARQLResultStreamingData::handleQueryError(CQuery* query, const QString& error) {
					if (mStreamingInitialized && !mStreamingFinished) {
						mSerializer->addResultSerialization(mVarNames, mStreamingResult);
					}
					mSerializer->addResultError(error, "error");
					return this;
				}


				CSPARQLResultConcurrentStreamingDataBufferReciever* CSPARQLResultStreamingData::transferConcurrentBuffers(QList<CSPARQLResultBufferWriteData>& bufferList, cint64& bufferSize) {
					mBufferMutex->lock();
					mUsedBufferList.append(bufferList);
					mUsedBufferSize += bufferSize;
					bool flush = false;
					if (mUsedBufferSize > mBufferSize && !mWritingScheduled) {
						mBufferSize *= 2;
						if (mBufferSize > mMaxBufferSize) {
							mBufferSize = mMaxBufferSize;
						}
						flush = true;
						mWritingScheduled = true;
					}
					if (flush) {
						mContinueSerialization &= streamingFlush();
					}
					mBufferMutex->unlock();
					bufferSize = 0;
					bufferList.clear();
					return this;
				}

				QList<CSPARQLResultBufferWriteData> CSPARQLResultStreamingData::takeWriteableBuffers() {
					mBufferMutex->lock();
					mWritingScheduled = false;
					QList<CSPARQLResultBufferWriteData> tmpList = mUsedBufferList;
					mUsedBufferList.clear();
					mUsedBufferSize = 0;
					mBufferMutex->unlock();
					return tmpList;
				}

				cint64 CSPARQLResultStreamingData::getWriteableBufferSize() {
					return mUsedBufferSize;
				}
				
				cint64 CSPARQLResultStreamingData::getWriteableBufferCount() {
					return mUsedBufferList.size();
				}


				cint64 CSPARQLResultStreamingData::getSequenceNumber() {
					return mSequenceNumber;
				}




				CVariableBindingsAnswersConcurrentStreamingHandler* CSPARQLResultStreamingData::getConcurrentStreamingHandler() {
					CSPARQLResultConcurrentStreamingData* conStreamData = new CSPARQLResultConcurrentStreamingData(mInitialBufferSize, mMaxBufferSize, &mVarNames, &mContinueSerialization, this);
					return conStreamData;
				}


				CVariableBindingsAnswersStreamingHandler* CSPARQLResultStreamingData::releaseConcurrentStreamingHandler(CVariableBindingsAnswersConcurrentStreamingHandler* handler) {
					CSPARQLResultConcurrentStreamingData* conStreamData = (CSPARQLResultConcurrentStreamingData*)handler;
					if (conStreamData->hasData()) {
						mBufferMutex->lock();
						++mConcurrentFinishCount;
						conStreamData->addBuffers(mUsedBufferList, mUsedBufferSize);
						bool flush = false;
						if (mUsedBufferSize > mBufferSize && !mWritingScheduled) {
							mBufferSize *= 2;
							if (mBufferSize > mMaxBufferSize) {
								mBufferSize = mMaxBufferSize;
							}
							flush = true;
							mWritingScheduled = true;
						}
						mBufferMutex->unlock();
						if (flush) {
							mContinueSerialization &= streamingFlush();
						}
					}
					delete conStreamData;
					return this;
				}




			}; // end namespace SPARQL

		}; // end namespace Interface

	}; // end namespace Control

}; // end namespace Konclude
