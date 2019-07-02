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

#ifndef KONCLUDE_CONTROL_INTERFACE_COMMANDLINE_CCLIREALIZATIONCOMANDLINEPREPARATIONTRANSLATOR_H
#define KONCLUDE_CONTROL_INTERFACE_COMMANDLINE_CCLIREALIZATIONCOMANDLINEPREPARATIONTRANSLATOR_H

// Libraries includes
#include <QString>
#include <QDomDocument>
#include <QHash>

// Namespace includes
#include "CCLIComandLinePreparationTranslator.h"

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
				 *		\class		CCLIRealizationComandLinePreparationTranslator
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CCLIRealizationComandLinePreparationTranslator : public CCLIComandLinePreparationTranslator {
					// public methods
					public:
						//! Constructor
						CCLIRealizationComandLinePreparationTranslator(CLoaderFactory *loaderFactory);

						//! Destructor
						virtual ~CCLIRealizationComandLinePreparationTranslator();

						virtual bool canTranslate(const QString& string);

						virtual bool hasNeccessaryParameters();

					// protected methods
					protected:
						virtual bool translate(const QString& argumentText, QStringList& remainingArgList);

					// protected variables
					protected:
						CLoaderFactory *loaderFac;


					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace CommandLine

		}; // end namespace Interface

	}; // end namespace Control

}; // end namespace Konclude

#endif // KONCLUDE_CONTROL_INTERFACE_COMMANDLINE_CCLIREALIZATIONCOMANDLINEPREPARATIONTRANSLATOR_H
