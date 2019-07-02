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

#ifndef KONCLUDE_LOGGER_COREFILEPRINTLOGOBSERVER_H
#define KONCLUDE_LOGGER_COREFILEPRINTLOGOBSERVER_H


// Libraries includes
#include <QSet>
#include <QStringList>
#include <QFile>
#include <QDir>

// Namespace includes
#include "CAbstractLogObserver.h"
#include "CLogger.h"


// Other includes
#include <iostream>


// Logger includes


namespace Konclude {

	namespace Logger {


		/*! 
		*
		*		\class		COREFilePrintLogObserver
		*		\author		Andreas Steigmiller
		*		\version	0.1
		*		\brief		TODO
		*
		*/
		class COREFilePrintLogObserver : public CAbstractLogObserver {

			public:
				COREFilePrintLogObserver(const QString& outputFileString);
				virtual ~COREFilePrintLogObserver();

				virtual void postLogMessage(CLogMessage *message);

				void flushLogging();
				void closeOutputFile();
				void forcedPathCreated(const QString& filePath);


			private:
				QString mOutputFileString;
				QFile mOutputFile;
				bool mOutputFileOpened;
				double mMinLogLevel;
				double mMaxLogLevel;
		
		};

	}; // end namespace Logger

}; // end namespace Konclude

#endif // KONCLUDE_LOGGER_COREFILEPRINTLOGOBSERVER_H
