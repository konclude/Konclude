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

#ifndef KONCLUDE_CONTROL_INTERFACE_OWLLINK_CSPARQLXMLANSWERSERIALIZER_H
#define KONCLUDE_CONTROL_INTERFACE_OWLLINK_CSPARQLXMLANSWERSERIALIZER_H

// Libraries includes
#include <QString>
#include <QDomDocument>
#include <QMap>
#include <QVector>

// Namespace includes


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
				 *		\class		CSPARQLXMLAnswerSerializer
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CSPARQLXMLAnswerSerializer {
					// public methods
					public:
						//! Constructor
						CSPARQLXMLAnswerSerializer();

						//! Destructor
						virtual ~CSPARQLXMLAnswerSerializer();


						QByteArray serializeResult(const QStringList& varList, CVariableBindingsAnswersResult* variableBindingsAnswersResult);
						QByteArray serializeResult(CBooleanQueryResult* booleanResult);

						CSPARQLXMLAnswerSerializer* addResultSerialization(CBooleanQueryResult* booleanResult, cint64 pos);
						CSPARQLXMLAnswerSerializer* addResultSerialization(const QStringList& varList, CVariableBindingsAnswersResult* variableBindingsAnswersResult, cint64 pos);
						CSPARQLXMLAnswerSerializer* addResultStatistics(CQueryStatistics* queryStats, cint64 pos);

						QByteArray getCombinedResultSerialization();

						CSPARQLXMLAnswerSerializer* addResultError(const QString& errorString, const QString &nodeString, cint64 pos);


					// protected methods
					protected:
						QList<QDomElement> getResultElements(CBooleanQueryResult* booleanResult, QDomDocument& document);
						QList<QDomElement> getResultElements(const QStringList& varList, CVariableBindingsAnswersResult* variableBindingsAnswersResult, QDomDocument& document);

						CSPARQLXMLAnswerSerializer* initializeCombinedDocument();

					// protected variables
					protected:
						QString mAnonymousExtension;
						QString sparqlNS;
						QString owlNS;

						QMap<cint64, QList<QDomElement> > mPosSerializedResultHash;
						QDomDocument mCombinedDocument;
						bool mCombinedDocumentInitialized = false;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace SPARQL

		}; // end namespace Interface

	}; // end namespace Control

}; // end namespace Konclude

#endif // KONCLUDE_CONTROL_INTERFACE_OWLLINK_CSPARQLXMLANSWERSERIALIZER_H
