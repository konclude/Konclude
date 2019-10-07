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

#ifndef KONCLUDE_REASONER_ONTOLOGY_CNAME_H
#define KONCLUDE_REASONER_ONTOLOGY_CNAME_H

// Libraries includes
#include <QPair>

// Namespace includes
#include "OntologySettings.h"
#include "CNamePrefix.h"

// Other includes
#include "Utilities/CLinker.hpp"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities;

	namespace Reasoner {

		namespace Ontology {

			/*! 
			 *
			 *		\class		CName
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CName {
				// public methods
				public:
					//! Constructor
					CName();

					//! Destructor
					virtual ~CName();

					virtual qint64 getNameVersionID();
					virtual CName *setNameVersionID(qint64 versionID);

					static qint64 getLastNameVersionID(CLinker<CName *> *nameLinker);

				// protected methods
				protected:

				// protected variables
				protected:
					qint64 nameVersion;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ONTOLOGY_CNAME_H
