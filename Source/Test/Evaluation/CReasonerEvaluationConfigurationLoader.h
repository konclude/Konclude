/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
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

#ifndef KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONCONFIGURATIONLOADER_H
#define KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONCONFIGURATIONLOADER_H

// Libraries includes
#include <QString>
#include <QRegExp>
#include <QTime>
//#include <QTest>

// Namespace includes
#include "EvaluationSettings.h"

// Other includes
#include "Control/Interface/OWLlink/COWLlinkProcessor.h"

#include "Control/Command/CConfigManagerReader.h"

// Logger includes
#include "Logger/CLogger.h"
#include "Logger/CLogIdentifier.h"


#include <stdio.h>
#include <iostream>


namespace Konclude {

	using namespace Logger;
	using namespace Control::Interface::OWLlink;
	using namespace Control::Command;

	namespace Test {

		namespace Evaluation {


			/*! 
			 *
			 *		\class		CReasonerEvaluationConfigurationLoader
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CReasonerEvaluationConfigurationLoader : public COWLlinkProcessor {
				// public methods
				public:
					//! Constructor
					CReasonerEvaluationConfigurationLoader();

					//! Destructor
					virtual ~CReasonerEvaluationConfigurationLoader();


					virtual bool loadConfiguration(const QString& owllinkConfigFile, CConfiguration *config = 0);



				// protected methods
				protected:
					virtual COWLlinkProcessor* initializeOWLlinkContent();
					virtual COWLlinkProcessor* concludeOWLlinkContent();

					virtual CConfiguration* getConfiguration();

				// protected variables
				protected:
					QString mReqFileString;

					bool mBlockUntilProcessed;
					QTime mResponseTime;
					QSemaphore mBlockingSem;

					CConfiguration* mLoaderConfig;
					bool mConfigSucessfullyLoaded;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONCONFIGURATIONLOADER_H
