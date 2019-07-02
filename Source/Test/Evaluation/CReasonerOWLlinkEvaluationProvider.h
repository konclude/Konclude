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

#ifndef KONCLUDE_TEST_EVALUATION_CREASONEROWLLINKEVALUATIONPROVIDER_H
#define KONCLUDE_TEST_EVALUATION_CREASONEROWLLINKEVALUATIONPROVIDER_H

// Libraries includes
#include <QProcess>
#include <QString>
#include <QStringList>
#include <QDir>
#include <QObject>

// Namespace includes
#include "CReasonerEvaluationTerminationResult.h"
#include "CReasonerEvaluationProvider.h"


// Other includes
#include "Config/CConfigDataReader.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Config;

	namespace Test {

		namespace Evaluation {


			/*! 
			 *
			 *		\class		CReasonerOWLlinkEvaluationProvider
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CReasonerOWLlinkEvaluationProvider : public QObject, public CReasonerEvaluationProvider {
				Q_OBJECT

				// public methods
				public:
					//! Constructor
					CReasonerOWLlinkEvaluationProvider();

					//! Destructor
					virtual ~CReasonerOWLlinkEvaluationProvider();


					virtual bool createReasoner(CConfiguration *config);
					virtual CReasonerEvaluationTerminationResult* destroyReasoner();

					virtual QString getReasonerAddressString();

				// protected methods
				protected:
					void addLibrariesRecursive(const QString& dirString, QStringList& jarStringList);

				public slots:
					void processError(QProcess::ProcessError error);
					void processFinished(int exitCode, QProcess::ExitStatus exitStatus);

				// protected variables
				protected:
					QString mReasonerName;
					QString mReasonerClassCallName;
					cint64 mReasonerPort;
					cint64 mMemoryLimit;
					QString mReasonerLibDirectory;
					QString mReasonerLibSeparator;
					QString mReasonerAdditionalParameters;

					QString mSupportProcessString;


					QProcess* mSupportProcess;
					QProcess* mAdapterProcess;
					bool mProcessError;
					bool mProcessFinished;

					cint64 mKillTimeout;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_EVALUATION_CREASONEROWLLINKEVALUATIONPROVIDER_H
