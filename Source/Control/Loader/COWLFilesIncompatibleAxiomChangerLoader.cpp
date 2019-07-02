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

#include "COWLFilesIncompatibleAxiomChangerLoader.h"


namespace Konclude {

	namespace Control {

		namespace Loader {


			COWLFilesIncompatibleAxiomChangerLoader::COWLFilesIncompatibleAxiomChangerLoader() {
				loaderConfig = nullptr;
			}



			COWLFilesIncompatibleAxiomChangerLoader::~COWLFilesIncompatibleAxiomChangerLoader() {
			}


			CLoader *COWLFilesIncompatibleAxiomChangerLoader::init(CLoaderFactory *loaderFactory, CConfiguration *config) {
				loaderConfig = config;
				CConfigData *data = config->getLastConfigChange("Konclude.Test.Converter.InputDirectory");
				if (data) {
					mInputDir = data->getString();
				}

				data = config->getLastConfigChange("Konclude.Test.Converter.OutputDirectory");
				if (data) {
					mOutputDir = data->getString();
				}

				return this;
			}



			CLoader *COWLFilesIncompatibleAxiomChangerLoader::load() {
				COWLFileAxiomRemover* changer = new COWLFileAxiomRemover(loaderConfig);

				QDir inputDir(mInputDir);
				QDir outputDir(mOutputDir);
				outputDir.mkpath(".");
				foreach (QString fileString, inputDir.entryList(QDir::Files)) {
					QString absoluteFileString = inputDir.absoluteFilePath(fileString);
					QString outFileString = fileString;
					cint64 fiIn = outFileString.lastIndexOf(".owl.xml");
					if (fiIn == -1) {
						fiIn = outFileString.lastIndexOf(".xml");
					}
					if (fiIn == -1) {
						fiIn = outFileString.lastIndexOf(".owl");
					}
					if (fiIn == -1) {
						fiIn = outFileString.lastIndexOf(".");
					}
					if (fiIn != -1) {
						outFileString.replace(fiIn,1,"-A.");
					}
					if (mOutputDir.endsWith("/")) {
						outFileString = mOutputDir+outFileString;
					} else {
						outFileString = mOutputDir+"/"+outFileString;
					}
					outFileString.replace(".obo","");
					if (!outFileString.contains(".owl.xml")) {
						outFileString.replace(".owl",".owl.xml");
					}
					changer->loadRemoveAxiomsAndSave(absoluteFileString,outFileString);
				}

				LOG(INFO,"::Konclude::Converter::FileAxiomChanger",logTr("Converted all files in %1 to %2.").arg(mInputDir).arg(mOutputDir),this);

				delete changer;
				return this;
			}




			CLoader *COWLFilesIncompatibleAxiomChangerLoader::exit() {
				return this;
			}



		}; // end namespace Loader

	}; // end namespace Control

}; // end namespace Konclude
