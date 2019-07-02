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

#ifndef KONCLUDE_Control_COMMAND_CREASONERMANAGERCONFIGTYPE_H
#define KONCLUDE_Control_COMMAND_CREASONERMANAGERCONFIGTYPE_H

// Libraries includes
#include <QString>
#include <QVariant>

// Namespace includes


// Other includes
#include "Reasoner/Kernel/Manager/CReasonerManager.h"

#include "Config/CObjectConfigType.h"

#include "Instructions/CInitializeConfigurationCommand.h"
#include "Instructions/CInitializeReasonerCommand.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Config;
	using namespace Reasoner::Kernel::Manager;

	namespace Control {

		namespace Command {

			using namespace Instructions;

			/*! 
			 *
			 *		\class		CReasonerManagerConfigType
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CReasonerManagerConfigType :  public CObjectConfigType {
				// public methods
				public:
					//! Constructor
					CReasonerManagerConfigType(CReasonerManager *reasonerManager = 0);

					//! Destructor
					virtual ~CReasonerManagerConfigType();

					
					virtual CReasonerManagerConfigType *getTypeCopy();

					virtual CReasonerManager *getReasonerManager();
					virtual CReasonerManagerConfigType *setReasonerManager(CReasonerManager *reasonerManager);


					virtual bool equals(CConfigType *dataType);

				// protected methods
				protected:

				// protected variables
				protected:
					CReasonerManager *rManager;
					
				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude

#endif // KONCLUDE_Control_COMMAND_CREASONERMANAGERCONFIGTYPE_H
