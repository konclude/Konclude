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

#include "CQtIODeviceStreamReader.h"


namespace Konclude {

	namespace Parser {

		namespace FunctionalJAVACC {

			CQtIODeviceStreamReader::CQtIODeviceStreamReader(QIODevice* device) : mDevice(device) {
			}

			size_t CQtIODeviceStreamReader::read(JAVACC_CHAR_TYPE *bufptr, int offset, size_t len) {
				int remainingLength = len;
				size_t readSize = 0;
				if (!mConvBuffer.isEmpty()) {
					int length = mConvBuffer.length();
					int copyLength = qMin(remainingLength,length);
					for (int i = 0; i < copyLength; ++i) {
						bufptr[offset+i] = mConvBuffer[i].unicode();
					}
					remainingLength = remainingLength - copyLength;
					offset += copyLength;
					if (copyLength == length) {
						mConvBuffer.clear();
					} else {
						mConvBuffer = mConvBuffer.mid(copyLength);
					}
					readSize += copyLength;
				}
				if (!mDevice->atEnd() && remainingLength > 0 && mConvBuffer.isEmpty()) {
					QByteArray lineByteData(mDevice->readLine());
					mConvBuffer = QString::fromUtf8(lineByteData.constData(),lineByteData.length());

					if (!mConvBuffer.isEmpty()) {
						int length = mConvBuffer.length();
						int copyLength = qMin(remainingLength,length);
						for (int i = 0; i < copyLength; ++i) {
							bufptr[offset+i] = mConvBuffer[i].unicode();
						}
						remainingLength = remainingLength - copyLength;
						offset += copyLength;
						if (copyLength == length) {
							mConvBuffer.clear();
						} else {
							mConvBuffer = mConvBuffer.mid(copyLength);
						}
						readSize += copyLength;
					}

				}
				return readSize;
			}

			bool CQtIODeviceStreamReader::endOfInput() {
				return mConvBuffer.isEmpty() && mDevice->atEnd();
			}


		}; // end namespace FunctionalJAVACC

	}; // end namespace Parser

}; // end namespace Konclude
