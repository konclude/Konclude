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

#include "CWriteQueryFileSerializer.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CWriteQueryFileSerializer::CWriteQueryFileSerializer(const QString& fileString) {
				mFileString = fileString;
				mCurrentOutputFile = nullptr;
			}

			const QString CWriteQueryFileSerializer::getOutputName() {
				return mFileString;
			}

			void CWriteQueryFileSerializer::forcedPathCreated(const QString& filePath) {
				QString path = filePath;
				if (path.contains("/") || path.contains("\\")) {
					int lastSlash = path.lastIndexOf("/");
					int lastBackSlash = path.lastIndexOf("\\");
					int lastSeparator = qMax(lastBackSlash,lastSlash);
					path = path.mid(0,lastSeparator);
					if (!path.isEmpty()) {
						QDir dir;
						dir.mkpath(path);
					}
				}
			}


			bool CWriteQueryFileSerializer::startWritingOutput() {
				forcedPathCreated(mFileString);
				QFile* outputFile = new QFile(mFileString);
				mCurrentOutputFile = nullptr;
				if (outputFile->open(QIODevice::WriteOnly)) {
					mCurrentOutputFile = outputFile;
					return true;
				}
				return false;
			}



			bool CWriteQueryFileSerializer::endWritingOutput() {
				if (mCurrentOutputFile) {
					mCurrentOutputFile->close();
					delete mCurrentOutputFile;
					return true;
				}
				return false;
			}


		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
