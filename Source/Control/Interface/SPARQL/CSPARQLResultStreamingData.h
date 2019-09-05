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

// Other includes
#include "Reasoner/Query/CVariableBindingsAnswersResult.h"
#include "Reasoner/Query/CBooleanQueryResult.h"
#include "Reasoner/Query/CComplexAssertionsIndividualVariablesAnsweringQuery.h"
#include "Reasoner/Query/CVariableBindingsAnswersStreamingHandler.h"
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

				/*! 
				 *
				 *		\class		CSPARQLResultStreamingData
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CSPARQLResultStreamingData : public CQueryCommandProvider, public CVariableBindingsAnswersStreamingHandler {
					// public methods
					public:
						//! Constructor
						CSPARQLResultStreamingData(CQueryCommandProvider* queryProvider, cint64 sequenceNumber, cint64 bufferSize, CSPARQLResultStreamingController* streamingController);
						CSPARQLResultStreamingData(CQuery* query, cint64 sequenceNumber, cint64 bufferSize, CSPARQLResultStreamingController* streamingController);

						//! Destructor
						virtual ~CSPARQLResultStreamingData();


						virtual CQuery *getQuery();
						virtual CCommand *getCommand();


						virtual CVariableBindingsAnswersStreamingHandler* initResultStreaming(const QStringList& varNames);

						virtual CVariableBindingsAnswersStreamingHandler* streamResultVariableBindings(CVariableBindingsAnswerResult* varBindings, cint64 cardinality = 1);

						virtual bool streamingFlush();

						virtual CVariableBindingsAnswersStreamingHandler* finishResultStreaming();

						virtual CSPARQLResultStreamingData* handleQueryResult(CQuery* query, CQueryResult* queryResult);
						virtual CSPARQLResultStreamingData* handleQueryError(CQuery* query, const QString& error);



						CSPARQLResultStreamingData* finalize();

						QList<QByteArray*> takeWriteableBuffers();
						cint64 getWriteableBufferSize();
						cint64 getWriteableBufferCount();
						cint64 getSequenceNumber();

					// protected methods
					protected:
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
						bool mStreamingFinished;
						bool mStreamingInitialized;
						bool mFinalized;
						bool mContinueSerialization;

						cint64 mFlushId;

						QList<QByteArray*> mUsedBufferList;
						cint64 mUsedBufferSize;

						QMutex mBufferMutex;

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
