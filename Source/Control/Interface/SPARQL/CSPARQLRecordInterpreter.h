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

#ifndef KONCLUDE_CONTROL_INTERFACE_OWLLINK_CSPARQLRECORDINTERPRETER_H
#define KONCLUDE_CONTROL_INTERFACE_OWLLINK_CSPARQLRECORDINTERPRETER_H

// Libraries includes
#include <QString>
#include <QDomDocument>
#include <QHash>

// Namespace includes
#include "CSPARQLXMLAnswerSerializer.h"

// Other includes
#include "Control/Command/CCommandRecorder.h"
#include "Control/Command/CCommandDelegater.h"

#include "Control/Command/Records/CClosureProcessCommandRecord.h"

#include "Control/Command/Instructions/CKnowledgeBaseQueryCommand.h"

#include "Reasoner/Query/CVariableBindingsAnswersResult.h"
#include "Reasoner/Query/CQueryInconsitentOntologyError.h"

#include "Reasoner/Kernel/Process/CProcessingStatistics.h"

#include "Config/CConfiguration.h"
#include "Config/CConfigDataReader.h"


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

			namespace SPARQL {

				/*! 
				 *
				 *		\class		CSPARQLRecordInterpreter
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CSPARQLRecordInterpreter : public CCommandDelegater, public CCommandRecorder {
					// public methods
					public:
						//! Constructor
						CSPARQLRecordInterpreter(CCommandDelegater *commandDelegater, CConfiguration* config);

						//! Destructor
						virtual ~CSPARQLRecordInterpreter();

						virtual CCommandDelegater *delegateCommand(CCommand *command);
						virtual CCommandRecorder *recordData(CCommandRecordData *recData);


						QByteArray getByteArray();


					// protected methods
					protected:
						QString getErrorString(CCommandRecordData *recData, const QString &nodeString);
						QString getErrorLevelString(double level);

					// protected variables
					protected:
						QHash<CCommand *,qint64> commandSequenceHash;
						CCommandDelegater *delegater;
						qint64 nextSeqNumber;
						qint64 recNextSeqNumber;

						QMutex seqSyncMutex;


						QString owllinkNS;
						QString owlNS;

						cint64 mAnonymousExtension;


						CConfiguration* mConfig;
						bool mConfExtendedErrorReporting;


						CSPARQLXMLAnswerSerializer mSerializer;


					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace SPARQL

		}; // end namespace Interface

	}; // end namespace Control

}; // end namespace Konclude

#endif // KONCLUDE_CONTROL_INTERFACE_OWLLINK_CSPARQLRECORDINTERPRETER_H
