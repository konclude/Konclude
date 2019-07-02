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

#ifndef KONCLUDE_Control_COMMAND_CREASONERCONFIGURATIONGROUP_H
#define KONCLUDE_Control_COMMAND_CREASONERCONFIGURATIONGROUP_H

// Libraries includes
#include <QVector>


// Namespace includes
#include "CCommanderManagerConfigType.h"
#include "CReasonerManagerConfigType.h"
#include "CClassificationManagerConfigType.h"
#include "COntologyRevisionManagerConfigType.h"

// Other includes
#include "Config/CConfigurationGroup.h"
#include "Config/CIntegerConfigType.h"
#include "Config/CStringConfigType.h"
#include "Config/CBooleanConfigType.h"
#include "Config/CConfigDescription.h"
#include "KoncludeSettings.h"
#include "CKoncludeInfo.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Config;

	namespace Control {

		namespace Command {

			/*! 
			 *
			 *		\class		CReasonerConfigurationGroup
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CReasonerConfigurationGroup : public CConfigurationGroup {
				// public methods
				public:
					//! Constructor
					CReasonerConfigurationGroup();

					//! Destructor
					virtual ~CReasonerConfigurationGroup();

				// protected methods
				protected:				

				// protected variables
				protected:
				
				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude

#endif // KONCLUDE_Control_COMMAND_CREASONERCONFIGURATIONGROUP_H
