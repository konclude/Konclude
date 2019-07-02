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

#include "CCLIClassificationComandLinePreparationTranslator.h"


namespace Konclude {

	namespace Control {

		namespace Interface {

			namespace CommandLine {

				CCLIClassificationComandLinePreparationTranslator::CCLIClassificationComandLinePreparationTranslator(CLoaderFactory *loaderFactory) : CCLIComandLinePreparationTranslator(loaderFactory) {
					mOperationTypeString = QString("Classification");
					mMinNecessaryArguments = true;
					mFirstArgumentTranslation = false;
					mLoaderTypeString = QString("-CLIClassificationBatchProcessingLoader");
				}


				CCLIClassificationComandLinePreparationTranslator::~CCLIClassificationComandLinePreparationTranslator() {
				}

				bool CCLIClassificationComandLinePreparationTranslator::hasNeccessaryParameters() {
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

				bool CCLIClassificationComandLinePreparationTranslator::translate(const QString& argumentText, QStringList& remainingArgList) {

					bool argumentTranslated = false;


					QString modArgText = argumentText.trimmed();
					if (!mSkipTranslation) {
						if (CCommandLinePreparationTranslator::translate(argumentText,remainingArgList)) {
							argumentTranslated = true;
						} else {
							if (modArgText.indexOf("-") == 0) {
							} else if (modArgText == "classification" || modArgText == "classify") {
								argumentTranslated = true;
								mFirstArgumentTranslation = true;
							}
						}
					}
					return argumentTranslated;
				}

				bool CCLIClassificationComandLinePreparationTranslator::canTranslate(const QString& string) {
					if (QString::compare(string,"classification",Qt::CaseInsensitive) == 0 || QString::compare(string,"classify",Qt::CaseInsensitive) == 0) {
						return true;
					}
					return false;
				}


			}; // end namespace CommandLine

		}; // end namespace Interface

	}; // end namespace Control

}; // end namespace Konclude
