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

#ifndef KONCLUDE_CONTROL_INTERFACE_COMMANDLINE_CCOMMANDLINEPREPARATIONTRANSLATOR_H
#define KONCLUDE_CONTROL_INTERFACE_COMMANDLINE_CCOMMANDLINEPREPARATIONTRANSLATOR_H

// Libraries includes
#include <QString>
#include <QDomDocument>
#include <QHash>

// Namespace includes


// Other includes
#include "Control/Loader/CLoaderFactory.h"

// Logger includes
#include "Logger/CLogger.h"
#include "Logger/CLogIdentifier.h"

using namespace Konclude::Logger;


namespace Konclude {

	namespace Control {

		using namespace Loader;

		namespace Interface {

			namespace CommandLine {

				/*! 
				 *
				 *		\class		CCommandLinePreparationTranslator
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CCommandLinePreparationTranslator : public CLogIdentifier {
					// public methods
					public:
						//! Constructor
						CCommandLinePreparationTranslator(CLoaderFactory *loaderFactory);

						//! Destructor
						virtual ~CCommandLinePreparationTranslator();


						virtual QStringList translateArguments(int argc, char *argv[]);
						virtual QStringList translateArguments(const QStringList& argList);

						virtual QStringList combineTranslatedArguments();

						virtual bool canTranslate(const QString& string) = 0;

					// protected methods
					protected:
						virtual bool translate(const QString& argumentText, QStringList& remainingArgList);

					// protected variables
					protected:
						QString mInterfaceTypeString;
						QString mLoaderTypeString;

						CLoaderFactory* mLoaderFac;

 						bool mSkipTranslation;
						bool mHelpRequested;

						QStringList mLogConfList;
						QStringList mConfigConfList;
						QStringList mRequestConfList;
						QStringList mMiscellaneousList;

						QString mRequestFileName;
						bool mGenerateAutoResponseFile;


						bool mRequestFileTranslated;
						bool mResponseFileTranslated;
						bool mConfigFileTranslated;
						bool mIRITranslated;

						bool mExtendendLogging;
						QStringList mGeneralConfList;

						QStringList mReasonerConfList;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace CommandLine

		}; // end namespace Interface

	}; // end namespace Control

}; // end namespace Konclude

#endif // KONCLUDE_CONTROL_INTERFACE_COMMANDLINE_CCOMMANDLINEPREPARATIONTRANSLATOR_H
