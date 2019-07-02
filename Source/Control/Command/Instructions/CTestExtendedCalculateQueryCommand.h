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

#ifndef KONCLUDE_Control_COMMAND_CTESTEXTENDEDCALCULATEQUERYCOMMAND_H
#define KONCLUDE_Control_COMMAND_CTESTEXTENDEDCALCULATEQUERYCOMMAND_H

// Libraries includes
#include <QString>
#include <QDomElement>

// Namespace includes
#include "CCalculateQueryCommand.h"

// Other includes
#include "Control/Command/CommandSettings.h"
#include "Control/Command/CCommand.h"
#include "Control/Command/CPreconditionCommand.h"
#include "Control/Command/CCommandProcessedPrecondition.h"


#include "Reasoner/Query/CQueryResult.h"


// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;


namespace Konclude {

	using namespace Reasoner::Query;

	namespace Control {

		namespace Command {

			namespace Instructions {

				/*! 
				 *
				 *		\class		CTestExtendedCalculateQueryCommand
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CTestExtendedCalculateQueryCommand : public CCalculateQueryCommand {
					// public methods
					public:
						//! Constructor
						CTestExtendedCalculateQueryCommand(CQueryCommandProvider *queryCommandProvider, CCommand *parentSuperCommand = 0);

						//! Destructor
						virtual ~CTestExtendedCalculateQueryCommand();

						virtual qint64 getCommandTag();

						virtual QString getBriefCommandDescription();

						virtual qint64 getTimingReportLevel();
						virtual CTestExtendedCalculateQueryCommand *setTimingReportLevel(qint64 timingReportLevel);

						virtual qint64 getCalculationTime();
						virtual bool hasCalculationTime();
						virtual CTestExtendedCalculateQueryCommand *setCalculationTime(qint64 calculationTime);

						virtual qint64 getCreationElapsedMilliSeconds();

						virtual CTestExtendedCalculateQueryCommand *setExpectedQueryResult(CQueryResult *expectedQueryResult);
						virtual CQueryResult *getExpectedQueryResult();

					// protected methods
					protected:

					// protected variables
					protected:
						qint64 timingLevel;
						qint64 calcTime;
						bool hasCalcTime;

						QTime creationTime;

						CQueryResult *expectedResult;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Instructions

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude

#endif // KONCLUDE_Control_COMMAND_CTESTEXTENDEDCALCULATEQUERYCOMMAND_H
