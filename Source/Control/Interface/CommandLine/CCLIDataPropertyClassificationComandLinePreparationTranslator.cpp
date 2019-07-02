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

#include "CCLIDataPropertyClassificationComandLinePreparationTranslator.h"


namespace Konclude {

	namespace Control {

		namespace Interface {

			namespace CommandLine {

				CCLIDataPropertyClassificationComandLinePreparationTranslator::CCLIDataPropertyClassificationComandLinePreparationTranslator(CLoaderFactory *loaderFactory) : CCLIComandLinePreparationTranslator(loaderFactory) {
					mOperationTypeString = QString("DataPropertyClassification");
					mMinNecessaryArguments = true;
					mFirstArgumentTranslation = false;
					mLoaderTypeString = QString("-CLIDataPropertyClassificationBatchProcessingLoader");

					mTranslationStringSet.insert(QString("classifydat").toUpper());
					mTranslationStringSet.insert(QString("classifydata").toUpper());
					mTranslationStringSet.insert(QString("classifydatproperties").toUpper());
					mTranslationStringSet.insert(QString("classifydataproperties").toUpper());
					mTranslationStringSet.insert(QString("classifyconcreteroles").toUpper());
					mTranslationStringSet.insert(QString("classifydataroles").toUpper());
					mTranslationStringSet.insert(QString("datclassification").toUpper());
					mTranslationStringSet.insert(QString("datapropertyclassification").toUpper());
					mTranslationStringSet.insert(QString("datpropertyclassification").toUpper());
					mTranslationStringSet.insert(QString("dataroleclassification").toUpper());
					mTranslationStringSet.insert(QString("concreteroleclassification").toUpper());
				}


				CCLIDataPropertyClassificationComandLinePreparationTranslator::~CCLIDataPropertyClassificationComandLinePreparationTranslator() {
				}

				bool CCLIDataPropertyClassificationComandLinePreparationTranslator::hasNeccessaryParameters() {
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

				bool CCLIDataPropertyClassificationComandLinePreparationTranslator::translate(const QString& argumentText, QStringList& remainingArgList) {

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

				bool CCLIDataPropertyClassificationComandLinePreparationTranslator::canTranslate(const QString& string) {
					if (mTranslationStringSet.contains(string.toUpper())) {
						return true;
					}
					return false;
				}


			}; // end namespace CommandLine

		}; // end namespace Interface

	}; // end namespace Control

}; // end namespace Konclude
