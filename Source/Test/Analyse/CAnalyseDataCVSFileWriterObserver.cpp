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

#include "CAnalyseDataCVSFileWriterObserver.h"


namespace Konclude {

	namespace Test {

		namespace Analyse {


			CAnalyseDataCVSFileWriterObserver::CAnalyseDataCVSFileWriterObserver(const QString &outputFilename) {
				filename = outputFilename;
				file = 0;
			}


			CAnalyseDataCVSFileWriterObserver::~CAnalyseDataCVSFileWriterObserver() {
				if (file) {
					file->close();
					delete file;
					file = 0;
				}
			}


			CAnalyseDataObserver *CAnalyseDataCVSFileWriterObserver::notifyDataUpdate(CAnalyseDataSeries *data) {
				if (!file) {
					file = new QFile(filename);
					if (file->open(QIODevice::Append)) {
						QString headerString;
						qint64 count = data->getAnalyseDataDescription()->getAnalyseValueCount();
						for (qint64 i = 0; i < count; ++i) {
							QString valueName = data->getAnalyseDataDescription()->getValueDescription(i)->getValueName();
							headerString.append(valueName+",\t");
						}
						headerString.append("\n");
                        file->write(headerString.toLocal8Bit());
					} else {
						delete file;
						file = 0;
					}
				}
				if (file) {
					QString valueString;
					qint64 count = data->getAnalyseDataDescription()->getAnalyseValueCount();
					CAnalyseDataLinker *dataLinker = data->getAnalyseDataLinker();
					CAnalyseData *analyseData = dataLinker->getData();
					for (qint64 i = 0; i < count; ++i) {
						CAnalyseValue *analyseValue = analyseData->getValue(i);
						CConvertStringAnalyseValue *stringAnalyseValue = 0;
						if (analyseValue) {
							dynamic_cast<CConvertStringAnalyseValue *>(analyseValue);
						}
						if (stringAnalyseValue) {
							QString value = stringAnalyseValue->getString();
							valueString.append(value+",\t");
						} else {
							valueString.append(",\t");
						}
					}
					valueString.append("\n");
                    file->write(valueString.toLocal8Bit());
				}
				return this;
			}



		}; // end namespace Analyse

	}; // end namespace Test

}; // end namespace Konclude
