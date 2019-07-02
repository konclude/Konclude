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

#ifndef KONCLUDE_CONTROL_INTERFACE_COMMANDLINE_CCLICOMANDLINEPREPARATIONTRANSLATOR_H
#define KONCLUDE_CONTROL_INTERFACE_COMMANDLINE_CCLICOMANDLINEPREPARATIONTRANSLATOR_H

// Libraries includes
#include <QString>
#include <QDomDocument>
#include <QHash>

// Namespace includes
#include "CCommandLinePreparationTranslator.h"

// Other includes
#include "Control/Loader/CLoaderFactory.h"

// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;


namespace Konclude {

	namespace Control {

		using namespace Loader;

		namespace Interface {

			namespace CommandLine {

				/*! 
				 *
				 *		\class		CCLIComandLinePreparationTranslator
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CCLIComandLinePreparationTranslator : public CCommandLinePreparationTranslator {
					// public methods
					public:
						//! Constructor
						CCLIComandLinePreparationTranslator(CLoaderFactory *loaderFactory);

						//! Destructor
						virtual ~CCLIComandLinePreparationTranslator();

						virtual QStringList combineTranslatedArguments();

						virtual bool canTranslate(const QString& string);

						virtual bool hasNeccessaryParameters() = 0;


					// protected methods
					protected:
						virtual bool translate(const QString& argumentText, QStringList& remainingArgList);

					// protected variables
					protected:
						CLoaderFactory *loaderFac;

						bool mMinNecessaryArguments;
						bool mFirstArgumentTranslation;

						QString mOperationTypeString;
						QString mLoaderTypeString;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace CommandLine

		}; // end namespace Interface

	}; // end namespace Control

}; // end namespace Konclude

#endif // KONCLUDE_CONTROL_INTERFACE_COMMANDLINE_CCLICOMANDLINEPREPARATIONTRANSLATOR_H
