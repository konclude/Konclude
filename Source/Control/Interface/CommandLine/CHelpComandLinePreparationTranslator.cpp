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

#include "CHelpComandLinePreparationTranslator.h"


namespace Konclude {

	namespace Control {

		namespace Interface {

			namespace CommandLine {


				CHelpComandLinePreparationTranslator::CHelpComandLinePreparationTranslator(CLoaderFactory *loaderFactory) : CCommandLinePreparationTranslator(loaderFactory) {
					mInterfaceTypeString = QString("CLI");
					mMinNecessaryArguments = true;
					mFirstArgumentTranslation = false;
				}


				CHelpComandLinePreparationTranslator::~CHelpComandLinePreparationTranslator() {
				}


				bool CHelpComandLinePreparationTranslator::translate(const QString& argumentText, QStringList& remainingArgList) {

					bool argumentTranslated = false;


					QString modArgText = argumentText.trimmed();
					if (!mSkipTranslation) {
						if (CCommandLinePreparationTranslator::translate(argumentText,remainingArgList)) {
							argumentTranslated = true;
						} else {
							if (modArgText == "help" || modArgText == "?") {
								mHelpRequested = true;
								argumentTranslated = true;
								mFirstArgumentTranslation = true;
							}
						}
					}
					return argumentTranslated;
				}

				bool CHelpComandLinePreparationTranslator::canTranslate(const QString& string) {
					if (QString::compare(string,"help",Qt::CaseInsensitive) == 0 || QString::compare(string,"?",Qt::CaseInsensitive) == 0) {
						return true;
					}
					return false;
				}


				QStringList CHelpComandLinePreparationTranslator::combineTranslatedArguments() {
					QStringList translatedArgList;

					if (!mFirstArgumentTranslation) {
						mMinNecessaryArguments = false;
					}

					if (!mMinNecessaryArguments) {
						LOG(ERROR,getLogDomain(),logTr("Execution with inadequate command line arguments."),this);
						mHelpRequested = true;
					}
					return translatedArgList;				
				}





			}; // end namespace CommandLine

		}; // end namespace Interface

	}; // end namespace Control

}; // end namespace Konclude
