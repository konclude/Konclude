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

#ifndef KONCLUDE_CONTROL_COMMAND_CLOADKNOWLEDGEBASEFINISHDOWNLOADCOMMAND_H
#define KONCLUDE_CONTROL_COMMAND_CLOADKNOWLEDGEBASEFINISHDOWNLOADCOMMAND_H

// Libraries includes
#include <QByteArray>
#include <QIODevice>

// Namespace includes
#include "CKnowledgeBaseRevisionCommandProvider.h"
#include "CLoadKnowledgeBaseAllDownloadedContinuer.h"


// Other includes
#include "Control/Command/CommandSettings.h"
#include "Control/Command/CCommand.h"
#include "Control/Command/CPreconditionCommand.h"
#include "Control/Command/CCommandProcessedPrecondition.h"
#include "Control/Command/CPreconditionSynchronizer.h"


#include "Concurrent/Callback/CCallbackData.h"

#include "Network/HTTP/CHttpResponse.h"



// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;


namespace Konclude {

	using namespace Concurrent::Callback;
	using namespace Network::HTTP;

	namespace Control {

		namespace Command {

			namespace Instructions {

				/*! 
				 *
				 *		\class		CLoadKnowledgeBaseFinishDownloadCommand
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CLoadKnowledgeBaseFinishDownloadCommand : public CPreconditionCommand, public CCallbackData {
					// public methods
					public:
						//! Constructor
						CLoadKnowledgeBaseFinishDownloadCommand(const QString& downloadString, QIODevice* writeDevice, CPreconditionSynchronizer *preSynchronizer, CCommand *parentSuperCommand = 0);

						//! Destructor
						virtual ~CLoadKnowledgeBaseFinishDownloadCommand();

						virtual qint64 getCommandTag();
						virtual QString getBriefCommandDescription();


						virtual void doCallback();

						CLoadKnowledgeBaseFinishDownloadCommand* setTemporaryDataPointer(QByteArray* dataPointer);
						CLoadKnowledgeBaseFinishDownloadCommand* setResponse(CHttpResponse* response);

						QByteArray* getTemporaryDataPointer(bool take = false);
						QIODevice* getWriteIODevice();
						CHttpResponse* getResponse();

						QString getDownloadString();



					// protected methods
					protected:

					// protected variables
					protected:
						QString mDownloadString;
						CHttpResponse* mResponse;
						QByteArray* mTmpDataPointer;
						QIODevice* mWriteDevice;
						CPreconditionSynchronizer* mPreSynchronizer;


					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Instructions

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude

#endif // KONCLUDE_CONTROL_COMMAND_CLOADKNOWLEDGEBASEFINISHDOWNLOADCOMMAND_H
