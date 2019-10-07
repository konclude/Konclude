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

#include "CCLIClassClassificationComandLinePreparationTranslator.h"


namespace Konclude {

	namespace Control {

		namespace Interface {

			namespace CommandLine {

				CCLIClassClassificationComandLinePreparationTranslator::CCLIClassClassificationComandLinePreparationTranslator(CLoaderFactory *loaderFactory) : CCLIComandLinePreparationTranslator(loaderFactory) {
					mOperationTypeString = QString("Classification");
					mMinNecessaryArguments = true;
					mFirstArgumentTranslation = false;
					mLoaderTypeString = QString("-CLIClassClassificationBatchProcessingLoader");

					mTranslationStringSet.insert(QString("classify").toUpper());
					mTranslationStringSet.insert(QString("classifyclasses").toUpper());
					mTranslationStringSet.insert(QString("classifyconcepts").toUpper());
					mTranslationStringSet.insert(QString("classification").toUpper());
					mTranslationStringSet.insert(QString("classclassification").toUpper());
					mTranslationStringSet.insert(QString("conceptclassification").toUpper());
				}


				CCLIClassClassificationComandLinePreparationTranslator::~CCLIClassClassificationComandLinePreparationTranslator() {
				}

				bool CCLIClassClassificationComandLinePreparationTranslator::hasNeccessaryParameters() {
					bool minNecessaryArguments = true;
					if (!mFirstArgumentTranslation) {
						minNecessaryArguments = false;
					}
					if (!mRequestFileTranslated) {
						LOG(ERROR,getLogDomain(),logTr("%1 request file not specified.").arg(mOperationTypeString),this);
						minNecessaryArguments = false;
					}
					return minNecessaryArguments;
				}

				bool CCLIClassClassificationComandLinePreparationTranslator::translate(const QString& argumentText, QStringList& remainingArgList) {

					bool argumentTranslated = false;


					QString modArgText = argumentText.trimmed();
					if (!mSkipTranslation) {
						if (CCommandLinePreparationTranslator::translate(argumentText,remainingArgList)) {
							argumentTranslated = true;
						} else {
							if (modArgText.indexOf("-") == 0) {
							} else if (mTranslationStringSet.contains(modArgText.toUpper())) {
								argumentTranslated = true;
								mFirstArgumentTranslation = true;
							}
						}
					}
					return argumentTranslated;
				}

				bool CCLIClassClassificationComandLinePreparationTranslator::canTranslate(const QString& string) {
					if (mTranslationStringSet.contains(string.toUpper())) {
						return true;
					}
					return false;
				}


			}; // end namespace CommandLine

		}; // end namespace Interface

	}; // end namespace Control

}; // end namespace Konclude
