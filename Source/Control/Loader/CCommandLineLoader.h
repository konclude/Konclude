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

#ifndef KONCLUDE_Control_LOADER_CCOMMANDLINELOADER_H
#define KONCLUDE_Control_LOADER_CCOMMANDLINELOADER_H

// Libraries includes
#include <QString>
#include <QRegExp>

// Namespace includes
#include "CLoader.h"

// Other includes
#include "Concurrent/CThread.h"

#include "Utilities/CSingletonProvider.hpp"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Logger;
	using namespace Concurrent;
	using namespace Utilities;

	namespace Control {

		namespace Loader {

			/*! 
			 *
			 *		\class		CCommandLineLoader
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CCommandLineLoader : public CLoader, public CThread {
				// public methods
				public:
					//! Constructor
					CCommandLineLoader(const QStringList &argList, bool qtGUIApplication);
					CCommandLineLoader(int argc, char *argv[], bool qtGUIApplication);
					CCommandLineLoader();

					//! Destructor
					virtual ~CCommandLineLoader();


					static QStringList getEncodedArguments(int argc, char *argv[], bool skipFirst = true, bool encodeSpaces = true);
					static QString getDecodedArgument(const QString& string);



					virtual CLoader *init(CLoaderFactory *loaderFactory = 0, CConfiguration *config = 0);
					virtual CLoader *load();
					virtual CLoader *exit();


				// protected methods
				protected:
					virtual CCommandLineLoader *setConfiguration(const QString &configString, CConfiguration *configuration, bool equalsConfSet = false);

					virtual void threadStarted();
					virtual void threadStopped();

					virtual QStringList getMatchesFromPattern(const QString &pattern, const QString &text);
					virtual QStringList getMatchesFromPattern(const QString &pattern, const QString &text, qint64 maxPosition);
					virtual qint64 getFirstTextOccur(const QString &text, const QString &textOccur, qint64 beginSearchPos = 0);

				// protected variables
				protected:
					QStringList commandLineParameters;

					CConfiguration *configuration;
					bool qtGuiUsable;

					QString loadingText;

					CLoaderFactory *lFactory;
					QLinkedList<CLoader *> loaderContainer;
					QLinkedList<CConfiguration *> configContainer;

					QMutex containerSync;



				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Loader

	}; // end namespace Control

}; // end namespace Konclude

#endif // KONCLUDE_Control_LOADER_CCOMMANDLINELOADER_H
