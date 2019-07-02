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

#include "CCommandLinePreparationTranslatorSelector.h"


namespace Konclude {

	namespace Control {

		namespace Interface {

			namespace CommandLine {

				CCommandLinePreparationTranslatorSelector::CCommandLinePreparationTranslatorSelector(CLoaderFactory *loaderFactory) : CLogIdentifier("::Konclude::Loader::CommandLineTranslatorSelector",this) {
					mLoaderFactory = loaderFactory;
					mTranslatorList.append(new COWLLinkServerComandLinePreparationTranslator(loaderFactory));
					mTranslatorList.append(new COWLLinkFileComandLinePreparationTranslator(loaderFactory));
					mTranslatorList.append(new CSPARQLFileComandLinePreparationTranslator(loaderFactory));
					mTranslatorList.append(new CCLIClassClassificationComandLinePreparationTranslator(loaderFactory));
					mTranslatorList.append(new CCLIObjectPropertyClassificationComandLinePreparationTranslator(loaderFactory));
					mTranslatorList.append(new CCLIDataPropertyClassificationComandLinePreparationTranslator(loaderFactory));
					mTranslatorList.append(new CCLIConsistencyComandLinePreparationTranslator(loaderFactory));
					mTranslatorList.append(new CCLISatisfiabilityComandLinePreparationTranslator(loaderFactory));
					mTranslatorList.append(new CCLIRealizationComandLinePreparationTranslator(loaderFactory));
					mTranslatorList.append(new CSPARQLServerComandLinePreparationTranslator(loaderFactory));
				}


				CCommandLinePreparationTranslatorSelector::~CCommandLinePreparationTranslatorSelector() {
				}





				QStringList CCommandLinePreparationTranslatorSelector::translateArguments(const QStringList& argList) {
					QStringList processList(argList);
					QStringList translatedArgList;

					if (!processList.isEmpty()) {
						QString commandString = processList.first();
						CCommandLinePreparationTranslator* translator = getCommandLinePreparationTranslator(commandString);

						if (translator) {
							translatedArgList = translator->translateArguments(processList);
						} else {
							QString loaderString = commandString.replace("-","");
							if (mLoaderFactory->canCreateLoaderFromName(loaderString)) {
								translatedArgList = argList;
							} else {
								translatedArgList += QString("-ConfigurableCoutLogObserverLoader");
								translatedArgList += QString("-CommandLineTranlatorHelpLoader");
							}
						}
					}
					if (translatedArgList.isEmpty()) {
						translatedArgList += QString("-ConfigurableCoutLogObserverLoader");
						translatedArgList += QString("-CommandLineTranlatorHelpLoader");
					}
					return translatedArgList;
				}


				
				CCommandLinePreparationTranslator* CCommandLinePreparationTranslatorSelector::getCommandLinePreparationTranslator(const QString& commandString) {
					CCommandLinePreparationTranslator* selectedTranslator = nullptr;

					for (QList<CCommandLinePreparationTranslator*>::const_iterator it = mTranslatorList.constBegin(), itEnd = mTranslatorList.constEnd(); !selectedTranslator && it != itEnd; ++it) {
						CCommandLinePreparationTranslator* translator = (*it);
						if (translator && translator->canTranslate(commandString)) {
							selectedTranslator = translator;
						}
					}

					return selectedTranslator;
				}



			}; // end namespace CommandLine

		}; // end namespace Interface

	}; // end namespace Control

}; // end namespace Konclude
