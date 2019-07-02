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

#ifndef KONCLUDE_REASONER_REVISION_CLOCALONTOLOGYCONFIGURATIONEXTENSION_H
#define KONCLUDE_REASONER_REVISION_CLOCALONTOLOGYCONFIGURATIONEXTENSION_H

// Libraries includes


// Namespace includes


// Other includes
#include "Config/CConfigurationBase.h"
#include "Config/CLocalConfigurationFixedExtension.h"
#include "Config/CGlobalConfigurationBase.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Config;

	namespace Reasoner {

		namespace Revision {

			/*! 
			 *
			 *		\class		COntologyConfigurationExtension
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COntologyConfigurationExtension : public CLocalConfigurationFixedExtension {
				// public methods
				public:
					//! Constructor
					COntologyConfigurationExtension(CConfigurationBase *fixedConfigurationExt, qint64 configID);
					COntologyConfigurationExtension(COntologyConfigurationExtension *previousLocalOntologyExtension, CConfigurationBase *fixedConfigurationExt, qint64 configID);
					COntologyConfigurationExtension(COntologyConfigurationExtension *previousLocalOntologyExtension, const QLinkedList<CConfigurationBase *> &fixedConfigExtList, qint64 configID);

					//! Destructor
					virtual ~COntologyConfigurationExtension();


				// protected methods
				protected:

				// protected variables
				protected:
					
				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Revision

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_REVISION_CLOCALONTOLOGYCONFIGURATIONEXTENSION_H
