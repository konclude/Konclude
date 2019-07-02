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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CINDIVIDUALNODEEXPRESSIVITYVIOLATIONFLAGS_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CINDIVIDUALNODEEXPRESSIVITYVIOLATIONFLAGS_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CProcessContext.h"


// Other includes
#include "Reasoner/Ontology/CRole.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities;

	namespace Reasoner {

		using namespace Ontology;

		namespace Kernel {

			namespace Process {

				/*! 
				 *
				 *		\class		CIndividualNodeExpressivityViolationFlags
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CIndividualNodeExpressivityViolationFlags {
					// public methods
					public:
						//! Constructor
						CIndividualNodeExpressivityViolationFlags(CProcessContext* context = nullptr);

						CIndividualNodeExpressivityViolationFlags* initViolationFlags(CIndividualNodeExpressivityViolationFlags* violationFlags = nullptr);

						bool setViolationFlags(cint64 flags);
						bool setViolationFlags(CIndividualNodeExpressivityViolationFlags* violationFlags);

						bool hasPartialViolationFlags(cint64 flags);
						bool hasViolationFlags(cint64 flags);

						bool hasViolations();

						const static cint64 VFEXPRESSIVITYVIOLATED							= 0x1;
						const static cint64 VFFUNCTIONALEXPRESSIVITYVIOLATED				= 0x2;
						const static cint64 VFFUNCTIONALANCESTOREXPRESSIVITYVIOLATED		= 0x3;
						const static cint64 VFINVERSEEXPRESSIVITYVIOLATED					= 0x4;

					// protected methods
					protected:

					// protected variables
					protected:
						CProcessContext* mContext;
						cint64 mViolFlags;

					// private methods
					private:

					// private variables
					private:


				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CINDIVIDUALNODEEXPRESSIVITYVIOLATIONFLAGS_H
