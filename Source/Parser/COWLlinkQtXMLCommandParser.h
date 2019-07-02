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

#ifndef KONCLUDE_PARSER_COWLLINKQTXMLCOMMANDPARSER_H
#define KONCLUDE_PARSER_COWLLINKQTXMLCOMMANDPARSER_H

// Libraries includes
#include <QDomDocument>
#include <QDomElement>

// Namespace includes
#include "CCommandParser.h"
#include "CStringPrimitiveConverter.h"
#include "COWLlinkQtXMLResultParser.h"


// Other includes
#include "Control/Command/Instructions/CGetDescriptionCommand.h"
#include "Control/Command/Instructions/CParseOWLlinkCommandsCommand.h"
#include "Control/Command/Instructions/CTellKnowledgeBaseOWL2XMLNodeCommand.h"
#include "Control/Command/Instructions/CCreateKnowledgeBaseCommand.h"
#include "Control/Command/Instructions/CIsSatisfiableQueryOWL2XMLNodeCommand.h"
#include "Control/Command/Instructions/CAreClassesEquivalentQueryOWL2XMLNodeCommand.h"
#include "Control/Command/Instructions/CSkipUnsupportedCommandCommand.h"
#include "Control/Command/Instructions/CReleaseKnowledgeBaseCommand.h"
#include "Control/Command/Instructions/CAreClassesDisjointQueryOWL2XMLNodeCommand.h"
#include "Control/Command/Instructions/CIsClassSubsumedByQueryOWL2XMLNodeCommand.h"
#include "Control/Command/Instructions/CIsInstanceOfQueryOWL2XMLNodeCommand.h"
#include "Control/Command/Instructions/CGetFlattenedTypesOWL2XMLNodeCommand.h"
#include "Control/Command/Instructions/CTestExtendedCalculateQueryCommand.h"
#include "Control/Command/Instructions/CParseTestsuiteXMLNodeCommand.h"
#include "Control/Command/Instructions/CInsertResponseCommentCommand.h"
#include "Control/Command/Instructions/CSetKnowledgeBaseConfigCommand.h"
#include "Control/Command/Instructions/CGetKnowledgeBaseConfigCommand.h"
#include "Control/Command/Instructions/CGetKnowledgeBaseSettingsCommand.h"
#include "Control/Command/Instructions/CGetSubClassHierarchyQueryOWL2XMLNodeCommand.h"
#include "Control/Command/Instructions/CLoadKnowledgeBaseKRSSTestConceptCommand.h"
#include "Control/Command/Instructions/CSaveOWL2XMLExtractedKnowledgeBaseCommand.h"
#include "Control/Command/Instructions/CSaveOWLlinkOWL2XMLSatisfiableTestExtractedKnowledgeBaseCommand.h"
#include "Control/Command/Instructions/CLoadKnowledgeBaseOWLXMLOntologyCommand.h"
#include "Control/Command/Instructions/CProcessQueryOWL2XMLNodeCommand.h"
#include "Control/Command/Instructions/CKnowledgeBaseClassifyCommand.h"
#include "Control/Command/Instructions/CKnowledgeBaseRealizeCommand.h"
#include "Control/Command/Instructions/CEvaluateTestsuiteCommand.h"
#include "Control/Command/Instructions/CIsConsistentQueryOWL2XMLNodeCommand.h"
#include "Control/Command/Instructions/CClassifyQueryCommand.h"
#include "Control/Command/Instructions/CRealizeQueryCommand.h"
#include "Control/Command/Instructions/CGetKnowledgeBaseClassEntitiesCommand.h"
#include "Control/Command/Instructions/CGetKnowledgeBaseObjectPropertyEntitiesCommand.h"
#include "Control/Command/Instructions/CGetKnowledgeBaseNamedIndividualEntitiesCommand.h"
#include "Control/Command/Instructions/CGetKnowledgeBaseLanguageCommand.h"

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
		 *		\class		COWLlinkQtXMLCommandParser
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		Parser for OWL/XML Files
		 *
		 */
		class COWLlinkQtXMLCommandParser : public CCommandParser, public CLogIdentifier {
			// public methods
			public:
				//! Constructor
				COWLlinkQtXMLCommandParser();

				//! Destructor
				virtual ~COWLlinkQtXMLCommandParser();

				virtual bool realizeCommand(CParseOWLlinkCommandsCommand *command, CCommandDelegater *commandDelegater);

				virtual bool parse(const QByteArray &content);
				virtual bool parse(QDomElement *node);

			// protected methods
			protected:

				void parseOWLlinkRequestNode(QDomElement *requestNode);


				CGetDescriptionCommand *parseGetDescriptionNode(QDomElement *descriptionNode);
				CCreateKnowledgeBaseCommand *parseCreateKnowledgeBaseNode(QDomElement *createKBNode);
				CTellKnowledgeBaseOWL2XMLNodeCommand *parseTellKnowledgeBaseNode(QDomElement *node);

				CLoadKnowledgeBaseKRSSTestConceptCommand* parseLoadKnowledgeBaseKRSSTestConceptNode(QDomElement *node);
				CLoadKnowledgeBaseOWLXMLOntologyCommand* parseLoadKnowledgeBaseOWLXMLOntologyNode(QDomElement *node);
				CSaveOWL2XMLExtractedKnowledgeBaseCommand* parseSaveExtractedKnowledgeBaseNode(QDomElement *node);
				CSaveOWLlinkOWL2XMLSatisfiableTestExtractedKnowledgeBaseCommand* parseSaveExtractedOWLlinkTestCaseKnowledgeBaseNode(QDomElement *node);


				CProcessQueryOWL2XMLNodeCommand *parseProcessQueryNode(QDomElement *node);

				CGetSubClassHierarchyQueryOWL2XMLNodeCommand *parseGetSubClassHierarchyNode(QDomElement *node);
				CIsConsistentQueryOWL2XMLNodeCommand *parseIsConsistentNode(QDomElement *node);

				CClassifyQueryCommand* parseKnowledgeBaseClassifyNode(QDomElement *node);
				CRealizeQueryCommand* parseKnowledgeBaseRealizeNode(QDomElement *node);
				CReleaseKnowledgeBaseCommand *parseReleaseKnowledgeBaseNode(QDomElement *node);
				CParseTestsuiteXMLNodeCommand *parseTestTestsuiteNode(QDomElement *node);
				CEvaluateTestsuiteCommand* parseEvaluateTestsuiteNode(QDomElement* node);
				CInsertResponseCommentCommand *parseInsertResponseCommentNode(QDomElement *node);

				CGetKnowledgeBaseLanguageCommand *parseGetKnowledgeBaseLanguageNode(QDomElement *node);
				CGetKnowledgeBaseClassEntitiesCommand *parseGetKnowledgeBaseClassEntitiesNode(QDomElement *node);
				CGetKnowledgeBaseObjectPropertyEntitiesCommand *parseGetKnowledgeBaseObjectPropertyEntitiesNode(QDomElement *node);
				CGetKnowledgeBaseNamedIndividualEntitiesCommand *parseGetKnowledgeBaseIndividualEntitiesNode(QDomElement *node);

				CGetConfigCommand *parseGetSettingNode(QDomElement *node);
				CSetConfigCommand *parseSetSettingNode(QDomElement *node);

				CGetSettingsCommand *parseGetSettingsNode(QDomElement *node);

				void parseQueryExtensions(QDomElement *node, CKnowledgeBaseQueryCommand *queryCommand);
				void parseQueryExpectedResult(QDomElement *node, CKnowledgeBaseQueryCommand *queryCommand);

				CQueryResult *parseQueryResult(QDomElement *node);


				QString getOWLLinkMappedConfigString(const QString &keyName);


				bool requiresComplexQuery(QDomElement *node);

			// protected variables
			protected:
				CParseOWLlinkCommandsCommand *command;
				CCommandRecorder *recorder;
				CCommandDelegater *delegater;


			// private methods
			private:


			// private variables
			private:



		};

	}; // end namespace Parser

}; // end namespace Konclude

#endif // KONCLUDE_PARSER_COWLLINKQTXMLCOMMANDPARSER_H
