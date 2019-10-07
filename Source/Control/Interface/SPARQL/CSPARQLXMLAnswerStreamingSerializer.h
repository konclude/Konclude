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

#ifndef KONCLUDE_CONTROL_INTERFACE_OWLLINK_CSPARQLXMLANSWERSTREAMINGSERIALIZER_H
#define KONCLUDE_CONTROL_INTERFACE_OWLLINK_CSPARQLXMLANSWERSTREAMINGSERIALIZER_H

// Libraries includes
#include <QString>
#include <QXmlStreamWriter>
#include <QMap>
#include <QVector>
#include <QBuffer>

// Namespace includes

#include "Reasoner/Query/CVariableBindingsAnswersStreamingResult.h"
#include "Reasoner/Query/CVariableBindingsAnswersResult.h"
#include "Reasoner/Query/CBooleanQueryResult.h"
#include "Reasoner/Query/CQueryStatistics.h"

#include "Reasoner/Kernel/Process/CProcessingStatistics.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;


namespace Konclude {

	using namespace Reasoner::Query;
	using namespace Reasoner::Kernel::Process;

	namespace Control {

		namespace Interface {

			namespace SPARQL {

				/*! 
				 *
				 *		\class		CSPARQLXMLAnswerStreamingSerializer
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CSPARQLXMLAnswerStreamingSerializer {
					// public methods
					public:
						//! Constructor
						CSPARQLXMLAnswerStreamingSerializer(QByteArray* buffer = nullptr, bool writeHeaderAndFooter = true);

						//! Destructor
						virtual ~CSPARQLXMLAnswerStreamingSerializer();


						cint64 getBufferWrittenSize();
						QByteArray* updateWriteBuffer(QByteArray* buffer);


						CSPARQLXMLAnswerStreamingSerializer* addResultSerialization(CBooleanQueryResult* booleanResult);
						CSPARQLXMLAnswerStreamingSerializer* addResultSerialization(const QStringList& varList, CVariableBindingsAnswersResult* variableBindingsAnswersResult);
						CSPARQLXMLAnswerStreamingSerializer* addResultStatistics(CQueryStatistics* queryStats);


						CSPARQLXMLAnswerStreamingSerializer* addResultError(const QString& errorString, const QString &nodeString);


						CSPARQLXMLAnswerStreamingSerializer* finish();

						CSPARQLXMLAnswerStreamingSerializer* addResultStreamingStart(const QStringList& varList);
						CSPARQLXMLAnswerStreamingSerializer* addResultStreamingBinding(const QStringList& varList, CVariableBindingsAnswerResult* bindings);


						CSPARQLXMLAnswerStreamingSerializer* forceHeaderWritten();

					// protected methods
					protected:
						virtual CSPARQLXMLAnswerStreamingSerializer* writeResultStreamingStart();
						virtual CSPARQLXMLAnswerStreamingSerializer* writeResultStreamingEnd();



					// protected variables
					protected:
						QBuffer* mCurrentBuffer;
						QByteArray* mCurrentArray;
						QXmlStreamWriter* mStreamWriter;

						bool mWriteHeaderAndFooter;

						bool mSparqlHeaderWritten;
						bool mSparqlFooterWritten;
						CVariableBindingsResultIterator* mVarBindIt = nullptr;

						QString sparqlNS;
						QString owlNS;
						QString xsdNS;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace SPARQL

		}; // end namespace Interface

	}; // end namespace Control

}; // end namespace Konclude

#endif // KONCLUDE_CONTROL_INTERFACE_OWLLINK_CSPARQLXMLAnswerStreamingSerializer_H
