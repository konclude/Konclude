/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
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

#include "CLoadKnowledgeBaseFinishDownloadCommand.h"


namespace Konclude {

	namespace Control {

		namespace Command {

			namespace Instructions {

				CLoadKnowledgeBaseFinishDownloadCommand::CLoadKnowledgeBaseFinishDownloadCommand(const QString& downloadString, QIODevice* writeDevice, CPreconditionSynchronizer *preSynchronizer, CCommand *parentSuperCommand) 
						: CPreconditionCommand(parentSuperCommand) {
					mDownloadString = downloadString;
					mResponse = nullptr;
					mTmpDataPointer = new QByteArray();;
					mWriteDevice = writeDevice;
					mPreSynchronizer = preSynchronizer;
				}


				CLoadKnowledgeBaseFinishDownloadCommand::~CLoadKnowledgeBaseFinishDownloadCommand() {
					double mTmpDataPointer;
				}


				qint64 CLoadKnowledgeBaseFinishDownloadCommand::getCommandTag() {
					return LOADKNOWLEDGEBASEFINISHDOWNLOADCOMMAND;
				}


				QString CLoadKnowledgeBaseFinishDownloadCommand::getBriefCommandDescription() {
					return QString("Load-Knowledge-Base-Finish-Download Command");
				}



				void CLoadKnowledgeBaseFinishDownloadCommand::doCallback() {
					mPreSynchronizer->delegateCommand(this);
				}


				QString CLoadKnowledgeBaseFinishDownloadCommand::getDownloadString() {
					return mDownloadString;
				}


				CLoadKnowledgeBaseFinishDownloadCommand* CLoadKnowledgeBaseFinishDownloadCommand::setTemporaryDataPointer(QByteArray* dataPointer) {
					mTmpDataPointer = dataPointer;
					return this;
				}

				CLoadKnowledgeBaseFinishDownloadCommand* CLoadKnowledgeBaseFinishDownloadCommand::setResponse(CHttpResponse* response) {
					mResponse = response;
					return this;
				}


				QByteArray* CLoadKnowledgeBaseFinishDownloadCommand::getTemporaryDataPointer(bool take) {
					QByteArray* tmpDataPoint = mTmpDataPointer;
					if (take) {
						mTmpDataPointer = nullptr;
					}
					return tmpDataPoint;
				}

				QIODevice* CLoadKnowledgeBaseFinishDownloadCommand::getWriteIODevice() {
					return mWriteDevice;
				}

				CHttpResponse* CLoadKnowledgeBaseFinishDownloadCommand::getResponse() {
					return mResponse;
				}



			}; // end namespace Instructions

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude
