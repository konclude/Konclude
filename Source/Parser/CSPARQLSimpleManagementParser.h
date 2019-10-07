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

#ifndef KONCLUDE_PARSER_CSPARQLSIMPLEMANAGEMENTPARSER_H
#define KONCLUDE_PARSER_CSPARQLSIMPLEMANAGEMENTPARSER_H

// Libraries includes


// Namespace includes
#include "CQueryParser.h"
#include "COntologyParser.h"
#include "CSPARQLSimpleParser.h"

// Other includes

#include "Control/Command/CCommandDelegater.h"
#include "Control/Command/CCommandProcessedPrecondition.h"

#include "Control/Command/Instructions/CCreateKnowledgeBaseRevisionUpdateCommand.h"
#include "Control/Command/Instructions/CLoadKnowledgeBaseOWLAutoOntologyCommand.h"
#include "Control/Command/Instructions/CCreateKnowledgeBaseCommand.h"
#include "Control/Command/Instructions/CReleaseKnowledgeBaseCommand.h"
#include "Control/Command/Instructions/CRealizeQueryCommand.h"
#include "Control/Command/Instructions/CClassifyQueryCommand.h"
#include "Control/Command/Instructions/CPrepareKnowledgeBaseCommand.h"
#include "Control/Command/Instructions/CGetCurrentKnowledgeBaseRevisionCommand.h"

// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;


namespace Konclude {

	namespace Parser {

		using namespace Control::Command;
		using namespace Control::Command::Instructions;


		/*! 
		 *
		 *		\class		CSPARQLSimpleManagementParser
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CSPARQLSimpleManagementParser : public CSPARQLSimpleParser {
			// public methods
			public:
				//! Constructor
				CSPARQLSimpleManagementParser(CCommandDelegater *commandDelegater, const QString& kbName, CCommand* command);

				//! Destructor
				virtual ~CSPARQLSimpleManagementParser();

				virtual bool parseUpdateFile(const QString& filename);
				virtual bool parseUpdateText(const QString& filetext);
				virtual bool parseUpdateTextList(const QStringList& queryTextList);

			// protected methods
			protected:
				bool processParts(QStringList& parsedParts);

				QString getGraphName(const QString& currentPartString, QStringList& parsedParts, bool directGraphReference);

			// protected variables
			protected:
				CCommandDelegater *mCommandDelegater;
				CCommand* mCommand;
				QString mKBName;


			// private methods
			private:


			// private variables
			private:



		};

	}; // end namespace Parser

}; // end namespace Konclude

#endif // KONCLUDE_PARSER_CSPARQLSIMPLEMANAGEMENTPARSER_H
