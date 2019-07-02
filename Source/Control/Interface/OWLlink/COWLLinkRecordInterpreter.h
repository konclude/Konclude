/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
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

#ifndef KONCLUDE_Control_Interface_OWLLINK_COWLLINKRECORDINTERPRETER_H
#define KONCLUDE_Control_Interface_OWLLINK_COWLLINKRECORDINTERPRETER_H

// Libraries includes
#include <QString>
#include <QDomDocument>
#include <QHash>

// Namespace includes
#include "Control/Command/CCommandRecorder.h"
#include "Control/Command/CCommandDelegater.h"

#include "Control/Command/Records/CClosureProcessCommandRecord.h"

#include "Control/Command/Instructions/CGetDescriptionCommand.h"
#include "Control/Command/Instructions/CTellKnowledgeBaseOWL2XMLNodeCommand.h"
#include "Control/Command/Instructions/CCreateKnowledgeBaseCommand.h"
#include "Control/Command/Instructions/CIsSatisfiableQueryOWL2XMLNodeCommand.h"
#include "Control/Command/Instructions/CSkipUnsupportedCommandCommand.h"
#include "Control/Command/Instructions/CReleaseKnowledgeBaseCommand.h"
#include "Control/Command/Instructions/CTestExtendedCalculateQueryCommand.h"
#include "Control/Command/Instructions/CParseTestsuiteXMLNodeCommand.h"
#include "Control/Command/Instructions/CTestTestsuiteCommand.h"
#include "Control/Command/Instructions/CInsertResponseCommentCommand.h"
#include "Control/Command/Instructions/CSetConfigCommand.h"
#include "Control/Command/Instructions/CGetConfigCommand.h"
#include "Control/Command/Instructions/CGetSettingsCommand.h"
#include "Control/Command/Instructions/CLoadKnowledgeBaseKRSSTestConceptCommand.h"
#include "Control/Command/Instructions/CKnowledgeBaseProcessCommand.h"
#include "Control/Command/Instructions/CGetKnowledgeBaseClassEntitiesCommand.h"
#include "Control/Command/Instructions/CGetKnowledgeBaseObjectPropertyEntitiesCommand.h"
#include "Control/Command/Instructions/CGetKnowledgeBaseNamedIndividualEntitiesCommand.h"
#include "Control/Command/Instructions/CGetKnowledgeBaseLanguageCommand.h"

#include "Reasoner/Query/CClassHierarchyResult.h"
#include "Reasoner/Query/CClassesResult.h"
#include "Reasoner/Query/CIndividualsResult.h"
#include "Reasoner/Query/CClassSynsetsResult.h"
#include "Reasoner/Query/CIndividualSynsetsResult.h"
#include "Reasoner/Query/CQueryInconsitentOntologyError.h"
#include "Reasoner/Query/CSucceedQueryResult.h"
#include "Reasoner/Query/CClassSetResult.h"
#include "Reasoner/Query/CIndividualClassAssertionsResult.h"
#include "Reasoner/Query/CIndividualSynonymsResult.h"

#include "Reasoner/Kernel/Process/CProcessingStatistics.h"

#include "Config/CConfiguration.h"
#include "Config/CConfigDataReader.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;


namespace Konclude {

	using namespace Config;
	using namespace Reasoner::Query;
	using namespace Reasoner::Kernel::Process;

	namespace Control {

		using namespace Command;
		using namespace Instructions;
		using namespace Records;

		namespace Interface {

			namespace OWLlink {

				/*! 
				 *
				 *		\class		COWLLinkRecordInterpreter
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class COWLLinkRecordInterpreter : public CCommandDelegater, public CCommandRecorder {
					// public methods
					public:
						//! Constructor
						COWLLinkRecordInterpreter(CCommandDelegater *commandDelegater, CConfiguration* config);

						//! Destructor
						virtual ~COWLLinkRecordInterpreter();

						virtual CCommandDelegater *delegateCommand(CCommand *command);
						virtual CCommandRecorder *recordData(CCommandRecordData *recData);

						virtual QByteArray getByteArray();

						virtual QDomElement getErrorNode(CCommandRecordData *recData, const QString &nodeString = "Error");
						virtual QDomElement getErrorNode(const QString &errorText, const QString &nodeString = "Error");
						virtual QString getErrorString(CCommandRecordData *recData, const QString &nodeString);
						QString getErrorLevelString(double level);

						virtual QDomElement getOneOfStringConfigNode(const QString &keyString, const QString &oneOfOptions, const QString &defaultOption, const QString &nodeString = "Property");
						virtual QDomElement getBooleanConfigNode(const QString &keyString, const QString &defaultValue, const QString &nodeString = "Property");
						virtual QDomElement getStringConfigNode(const QString &keyString, const QString &defaultValue, const QString &nodeString = "Property");
						virtual QDomElement getIntegerConfigNode(const QString &keyString, qint64 defaultValue, const QString &nodeString = "Property");

						virtual QString getOWLLinkMappedConfigString(const QString &keyName);
						
						virtual bool hasReasonerCalculatedWrongResult();


						virtual QDomElement getEntitiesSetNode(const QString &nodeString, const QString &entityString, CExtractedKnowledgeBaseEntitiesContainer* entityContainer);

					// protected methods
					protected:

					// protected variables
					protected:
						QHash<CCommand *,qint64> commandSequenceHash;
						CCommandDelegater *delegater;
						qint64 nextSeqNumber;
						qint64 recNextSeqNumber;

						QHash<qint64,QDomNode> seqNodeHash;

						QMutex seqSyncMutex;

						QDomDocument document;
						QDomElement rootNode;

						QString owllinkNS;
						QString owlNS;

						bool mConfFileAppendSimpleStats;
						QString mStatFileString;
						QString mStatKBString;
						QString mStatLoadString;
						QString mStatQueryString;
						QStringList mStatStringList;
						QVector<cint64> mStatVector;

						bool mCalculatedWrongResult;
						bool mRespondQueryStatistics;

						CConfiguration* mConfig;
						bool mConfExtendedErrorReporting;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace OWLlink

		}; // end namespace Interface

	}; // end namespace Control

}; // end namespace Konclude

#endif // KONCLUDE_Control_Interface_OWLLINK_COWLLINKRECORDINTERPRETER_H
