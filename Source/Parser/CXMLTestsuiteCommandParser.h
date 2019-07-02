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

#ifndef KONCLUDE_PARSER_CXMLTESTSUITECOMMANDPARSER_H
#define KONCLUDE_PARSER_CXMLTESTSUITECOMMANDPARSER_H

// Libraries includes
#include <QDomDocument>
#include <QDomElement>
#include <QFileInfo>
#include <QDir>

// Namespace includes
#include "CCommandParser.h"


// Other includes
#include "Control/Command/Instructions/CParseTestsuiteCommandsCommand.h"
#include "Control/Command/Instructions/CTestTestsuiteCommand.h"
#include "Control/Command/Instructions/CSkipUnsupportedCommandCommand.h"
#include "Control/Command/Instructions/CTestTestfileCommand.h"
#include "Control/Command/Instructions/CParseTestsuiteXMLNodeCommand.h"

#include "Control/Command/Records/CFinishProcessCommandRecord.h"
#include "Control/Command/Records/CStartProcessCommandRecord.h"
#include "Control/Command/Records/CStopProcessCommandRecord.h"
#include "Control/Command/Records/CNotSupportedNodeErrorRecord.h"

#include "Control/Command/CCommandDelegater.h"
#include "Control/Command/CCommandProcessedPrecondition.h"

#include "Reasoner/Query/CBooleanQueryResult.h"
#include "Reasoner/Query/CQueryResult.h"


#include "Logger/CLogIdentifier.h"


// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;

using namespace std;


namespace Konclude {

	using namespace Reasoner::Query;
	using namespace Control::Command;
	using namespace Instructions;
	using namespace Records;

	namespace Parser {




		/*! 
		 *
		 *		\class		CXMLTestsuiteCommandParser
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		Parser for OWL/XML Files
		 *
		 */
		class CXMLTestsuiteCommandParser : public CCommandParser, public CLogIdentifier {
			// public methods
			public:
				//! Constructor
				CXMLTestsuiteCommandParser();

				//! Destructor
				virtual ~CXMLTestsuiteCommandParser();

				virtual bool realizeCommand(CParseTestsuiteCommandsCommand *command, CCommandDelegater *commandDelegater);
				virtual bool realizeCommand(CParseTestsuiteXMLNodeCommand *command, CCommandDelegater *commandDelegater);

				virtual bool parse(const QByteArray &content);
				virtual bool parse(QDomElement *node);

			// protected methods
			protected:

				void parseOWLlinkTestsuiteRequestNode(QDomElement *requestNode);


				CTestTestsuiteCommand *parseTestTestsuiteNode(QDomElement *descriptionNode);
				CTestTestfileCommand *parseTestTestfileNode(QDomElement *node, CTestTestsuiteCommand *testTestsuiteComm);


			// protected variables
			protected:
				CParseTestsuiteCommandsCommand *command;
				CParseTestsuiteXMLNodeCommand *nodeCommand;
				CCommandRecorder *recorder;
				CCommandDelegater *delegater;

				QString sourcePath;


			// private methods
			private:


			// private variables
			private:



		};

	}; // end namespace Parser

}; // end namespace Konclude

#endif // KONCLUDE_PARSER_CXMLTESTSUITECOMMANDPARSER_H
