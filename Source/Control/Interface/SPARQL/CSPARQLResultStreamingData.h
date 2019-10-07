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

#ifndef KONCLUDE_CONTROL_INTERFACE_OWLLINK_CSPARQLRESULTSTREAMINGDATA_H
#define KONCLUDE_CONTROL_INTERFACE_OWLLINK_CSPARQLRESULTSTREAMINGDATA_H

// Libraries includes
#include <QString>
#include <QDomDocument>
#include <QMap>
#include <QVector>
#include <QMutex>

// Namespace includes
#include "CSPARQLXMLAnswerStreamingPrecompiledByteArraySerializer.h"
#include "CSPARQLResultStreamingController.h"
#include "CSPARQLResultBufferWriteData.h"

// Other includes
#include "Reasoner/Query/CVariableBindingsAnswersResult.h"
#include "Reasoner/Query/CBooleanQueryResult.h"
#include "Reasoner/Query/CComplexAssertionsIndividualVariablesAnsweringQuery.h"
#include "Reasoner/Query/CVariableBindingsAnswersStreamingHandler.h"
#include "Reasoner/Query/CVariableBindingsAnswersConcurrentStreamingHandler.h"
#include "Reasoner/Query/CVariableBindingsAnswersStreamingResult.h"


#include "Control/Command/Instructions/CQueryCommandProvider.h"
#include "Control/Command/CCommand.h"


// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;


namespace Konclude {

	using namespace Reasoner::Query;

	namespace Control {

		using namespace Command;
		using namespace Command::Instructions;

		namespace Interface {

			namespace SPARQL {



				class CSPARQLResultConcurrentStreamingDataBufferReciever {
				public:

					virtual CSPARQLResultConcurrentStreamingDataBufferReciever* transferConcurrentBuffers(QList<CSPARQLResultBufferWriteData>& bufferList, cint64& bufferSize) = 0;

				};

				/*! 
				 *
				 *		\class		CSPARQLResultStreamingData
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CSPARQLResultStreamingData : public CQueryCommandProvider, public CVariableBindingsAnswersStreamingHandler, public CSPARQLResultConcurrentStreamingDataBufferReciever {
					// public methods
					public:
						//! Constructor
						CSPARQLResultStreamingData(CQueryCommandProvider* queryProvider, cint64 sequenceNumber, cint64 bufferSize, CSPARQLResultStreamingController* streamingController);
						CSPARQLResultStreamingData(CQuery* query, cint64 sequenceNumber, cint64 bufferSize, CSPARQLResultStreamingController* streamingController);

						//! Destructor
						virtual ~CSPARQLResultStreamingData();


						virtual CQuery *getQuery();
						virtual CCommand *getCommand();


						virtual bool initResultStreaming(const QStringList& varNames);

						virtual CVariableBindingsAnswersStreamingHandler* streamResultVariableBindings(CVariableBindingsAnswerResult* varBindings, cint64 cardinality = 1);

						virtual bool streamingFlush();

						virtual CVariableBindingsAnswersStreamingHandler* finishResultStreaming();

						virtual CSPARQLResultStreamingData* handleQueryResult(CQuery* query, CQueryResult* queryResult);
						virtual CSPARQLResultStreamingData* handleQueryError(CQuery* query, const QString& error);



						CSPARQLResultStreamingData* finalize();

						QList<CSPARQLResultBufferWriteData> takeWriteableBuffers();
						cint64 getWriteableBufferSize();
						cint64 getWriteableBufferCount();
						cint64 getSequenceNumber();


						virtual CVariableBindingsAnswersConcurrentStreamingHandler* getConcurrentStreamingHandler();
						virtual CVariableBindingsAnswersStreamingHandler* releaseConcurrentStreamingHandler(CVariableBindingsAnswersConcurrentStreamingHandler* handler);

						virtual CSPARQLResultConcurrentStreamingDataBufferReciever* transferConcurrentBuffers(QList<CSPARQLResultBufferWriteData>& bufferList, cint64& bufferSize);


					// protected methods
					protected:
						class CSPARQLResultConcurrentStreamingData : public CVariableBindingsAnswersConcurrentStreamingHandler {
						public:
							CSPARQLResultConcurrentStreamingData(cint64 bufferSize, cint64 maxBufferSize, QStringList* varNames, bool* serialize, CSPARQLResultConcurrentStreamingDataBufferReciever* reciever) {
								mBufferSize = bufferSize;
								mMaxBufferSize = maxBufferSize;
								mVarNames = varNames;
								mSerializer = nullptr;

								mUsedBufferSize = 0;
								mSerialize = serialize;
								mReciever = reciever;
								mMaxBufferCollectingCount = 10;
							}

							~CSPARQLResultConcurrentStreamingData() {
								if (mSerializer) {
									delete mSerializer;
								}
							}


							CVariableBindingsAnswersConcurrentStreamingHandler* concurrentlyStreamResultVariableBindings(CVariableBindingsAnswerResult* varBindings, cint64 cardinality) {

								if (*mSerialize) {
									if (!mSerializer) {
										QByteArray* bufferArray = new QByteArray();
										bufferArray->reserve(mBufferSize + 10000);
										mSerializer = new CSPARQLXMLAnswerStreamingPrecompiledByteArraySerializer(bufferArray, this);
									}
									mSerializer->addResultStreamingBindingToTemporaryBuffer(*mVarNames, varBindings);
									bool onlyVarBindWritten = false;
									cint64 onlyVarBindCount = 0;
									if (mSerializer->getBufferWrittenSize() <= 0) {
										onlyVarBindWritten = true;
									}
									while (cardinality > 0) {
										mSerializer->addTemporaryBuffer();
										++onlyVarBindCount;
										--cardinality;
										if (mSerializer->getBufferWrittenSize() > mBufferSize) {
											mBufferSize *= 2;
											if (mBufferSize > mMaxBufferSize) {
												mBufferSize = mMaxBufferSize;
											}
											cint64 bufferWriteCount = 1;
											if (onlyVarBindWritten) {
												while (cardinality > onlyVarBindCount) {
													cardinality -= onlyVarBindCount;
													++bufferWriteCount;
												}
											}
											QByteArray* bufferArray = new QByteArray();
											bufferArray->reserve(mBufferSize + 10000);
											QByteArray* usedBuffer = mSerializer->updateWriteBuffer(bufferArray);
											mUsedBufferSize += usedBuffer->size();
											mUsedBufferList.append(CSPARQLResultBufferWriteData(bufferWriteCount, usedBuffer));
											onlyVarBindWritten = true;
											onlyVarBindCount = 0;
											if (mUsedBufferList.size() >= mMaxBufferCollectingCount) {
												mReciever->transferConcurrentBuffers(mUsedBufferList, mUsedBufferSize);
											}
										}
									}
									mSerializer->clearTemporaryBuffer();
								}

								return this;
							}


							bool hasData() {
								return mSerializer && (mSerializer->getBufferWrittenSize() > 0 || mUsedBufferSize > 0);
							}

							CSPARQLResultConcurrentStreamingData* addBuffers(QList<CSPARQLResultBufferWriteData>& bufferList, cint64& bufferSize) {
								if (mSerializer && mSerializer->getBufferWrittenSize() > 0) {
									QByteArray* usedBuffer = mSerializer->updateWriteBuffer(nullptr);
									CSPARQLResultBufferWriteData usedBufferData(1, usedBuffer);
									mUsedBufferSize += usedBuffer->size();
									mUsedBufferList.append(usedBufferData);
								}
								bufferList.append(mUsedBufferList);
								bufferSize += mUsedBufferSize;
								return this;
							}


							CSPARQLXMLAnswerStreamingPrecompiledByteArraySerializer* mSerializer;

							QList<CSPARQLResultBufferWriteData> mUsedBufferList;
							cint64 mUsedBufferSize;
							cint64 mBufferSize;
							cint64 mMaxBufferSize;
							QStringList* mVarNames;
							bool* mSerialize = nullptr;

							CSPARQLResultConcurrentStreamingDataBufferReciever* mReciever;
							cint64 mMaxBufferCollectingCount;

						};


						CSPARQLResultStreamingData* init(cint64 sequenceNumber, cint64 bufferSize, CSPARQLResultStreamingController* streamingController);

					// protected variables
					protected:
						CSPARQLResultStreamingController* mStreamingController;
						CSPARQLXMLAnswerStreamingPrecompiledByteArraySerializer* mSerializer;
						CQueryCommandProvider* mQueryProvider;
						CQuery* mQuery;
						cint64 mSequenceNumber;
						CVariableBindingsAnswersStreamingResult* mStreamingResult;
						QStringList mVarNames;
						cint64 mMaxBufferSize;
						cint64 mBufferSize;
						cint64 mInitialBufferSize;
						bool mStreamingFinished;
						bool mStreamingInitialized;
						bool mFinalized;
						bool mContinueSerialization;

						cint64 mFlushId;

						QList<CSPARQLResultBufferWriteData> mUsedBufferList;
						cint64 mUsedBufferSize;
						bool mWritingScheduled;
						cint64 mConcurrentFinishCount;

						QMutex* mBufferMutex;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace SPARQL

		}; // end namespace Interface

	}; // end namespace Control

}; // end namespace Konclude

#endif // KONCLUDE_CONTROL_INTERFACE_OWLLINK_CSPARQLRESULTSTREAMINGDATA_H
