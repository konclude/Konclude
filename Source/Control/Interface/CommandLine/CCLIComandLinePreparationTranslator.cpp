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

#include "CCLIComandLinePreparationTranslator.h"


namespace Konclude {

	namespace Control {

		namespace Interface {

			namespace CommandLine {

				CCLIComandLinePreparationTranslator::CCLIComandLinePreparationTranslator(CLoaderFactory *loaderFactory) : CCommandLinePreparationTranslator(loaderFactory) {
					mInterfaceTypeString = QString("CLI");
					mMinNecessaryArguments = false;
					mFirstArgumentTranslation = false;
				}


				CCLIComandLinePreparationTranslator::~CCLIComandLinePreparationTranslator() {
				}

				QStringList CCLIComandLinePreparationTranslator::combineTranslatedArguments() {
					QStringList translatedArgList;

					if (!hasNeccessaryParameters()) {
						mMinNecessaryArguments = false;
					}

					if (!mMinNecessaryArguments) {
						LOG(ERROR,getLogDomain(),logTr("Execution with inadequate command line arguments."),this);
						mHelpRequested = true;
					}

					if (!mHelpRequested) {
						if (mLogConfList.isEmpty()) {
							mLogConfList.append(QString("-ConfigurableCoutLogObserverLoader -LoggerConfigurationLoader"));
						}
						translatedArgList += mLogConfList;
						translatedArgList += mGeneralConfList;

						translatedArgList += QString("-DefaultReasonerLoader ");
						translatedArgList += mReasonerConfList;
						translatedArgList += mGeneralConfList;

						if (!mConfigConfList.isEmpty()) {
							translatedArgList += QString("-OWLlinkBatchFileLoader +=Konclude.OWLlink.CloseAfterProcessedRequest=false +=Konclude.OWLlink.BlockUntilProcessedRequest=true");
							translatedArgList += mConfigConfList;
						}

						translatedArgList += mLoaderTypeString;
						if (mExtendendLogging) {
							translatedArgList += QString("+=Konclude.OWLlink.LoggingExtendedProcessingTimes=true");
						}
						translatedArgList += mRequestConfList;

						translatedArgList += mMiscellaneousList;
					} else {
						translatedArgList += QString("-ConfigurableCoutLogObserverLoader -CommandLineTranlatorHelpLoader");
					}
					return translatedArgList;
				}

				bool CCLIComandLinePreparationTranslator::translate(const QString& argumentText, QStringList& remainingArgList) {

					bool argumentTranslated = false;


					QString modArgText = argumentText.trimmed();
					if (!mSkipTranslation) {
						if (CCommandLinePreparationTranslator::translate(argumentText,remainingArgList)) {
							argumentTranslated = true;
						}
					}
					return argumentTranslated;
				}

				bool CCLIComandLinePreparationTranslator::canTranslate(const QString& string) {
					return false;
				}


			}; // end namespace CommandLine

		}; // end namespace Interface

	}; // end namespace Control

}; // end namespace Konclude
