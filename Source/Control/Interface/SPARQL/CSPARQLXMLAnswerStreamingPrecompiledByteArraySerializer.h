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

#ifndef KONCLUDE_CONTROL_INTERFACE_OWLLINK_CSPARQLXMLANSWERSTREAMINGPRECOMPILEDBYTEARRAYSERIALIZER_H
#define KONCLUDE_CONTROL_INTERFACE_OWLLINK_CSPARQLXMLANSWERSTREAMINGPRECOMPILEDBYTEARRAYSERIALIZER_H

// Libraries includes
#include <QString>
#include <QXmlStreamWriter>
#include <QMap>
#include <QVector>
#include <QBuffer>

// Namespace includes
#include "CSPARQLXMLAnswerStreamingSerializer.h"

#include "Reasoner/Query/CVariableBindingsAnswersStreamingResult.h"
#include "Reasoner/Query/CVariableBindingsAnswersResult.h"
#include "Reasoner/Query/CBooleanQueryResult.h"
#include "Reasoner/Query/CVariableBindingStringResult.h"
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
				 *		\class		CSPARQLXMLAnswerStreamingPrecompiledByteArraySerializer
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CSPARQLXMLAnswerStreamingPrecompiledByteArraySerializer : public CSPARQLXMLAnswerStreamingSerializer {
					// public methods
					public:
						//! Constructor
						CSPARQLXMLAnswerStreamingPrecompiledByteArraySerializer(QByteArray* buffer = nullptr, bool writeHeaderAndFooter = true);

						//! Destructor
						virtual ~CSPARQLXMLAnswerStreamingPrecompiledByteArraySerializer();


						CSPARQLXMLAnswerStreamingPrecompiledByteArraySerializer* addResultStreamingBindingToTemporaryBuffer(const QStringList& varList, CVariableBindingsAnswerResult* bindings);

						CSPARQLXMLAnswerStreamingPrecompiledByteArraySerializer* addTemporaryBuffer();
						CSPARQLXMLAnswerStreamingPrecompiledByteArraySerializer* clearTemporaryBuffer();



						CSPARQLXMLAnswerStreamingPrecompiledByteArraySerializer* writeFlush(cint64 flushNumber);


					// protected methods
					protected:
						virtual CSPARQLXMLAnswerStreamingSerializer* writeResultStreamingStart();
						virtual CSPARQLXMLAnswerStreamingSerializer* writeResultStreamingEnd();

						CSPARQLXMLAnswerStreamingPrecompiledByteArraySerializer* writeUTF8String(const QString& string);
						CSPARQLXMLAnswerStreamingPrecompiledByteArraySerializer* writeUTF8StringEscaped(const QString& string);


					// protected variables
					protected:
						QByteArray mTemporaryBuffer;


						QByteArray mResultsBegin;
						QByteArray mResultsEnd;


						QByteArray mResultBegin;
						QByteArray mResultEnd;

						QByteArray mBindingNameBegin;
						QByteArray mBindingNameEnd;
						QByteArray mBindingEnd;

						QByteArray mUriBegin;
						QByteArray mUriEnd;

						QByteArray mBnodeBegin;
						QByteArray mBnodeEnd;

						QByteArray mLiteralDatatypeBegin;
						QByteArray mLiteralDatatypeEnd;
						QByteArray mLiteralEnd;

						bool mConfAvoidStringCopying;


						QString mDoubleQuoteEscapeString;
						QString mSingleQuoteEscapeString;
						QString mSmallerEscapeString;
						QString mLargerEscapeString;
						QString mAndEscapeString;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace SPARQL

		}; // end namespace Interface

	}; // end namespace Control

}; // end namespace Konclude

#endif // KONCLUDE_CONTROL_INTERFACE_OWLLINK_CSPARQLXMLANSWERSTREAMINGPRECOMPILEDBYTEARRAYSERIALIZER_H
