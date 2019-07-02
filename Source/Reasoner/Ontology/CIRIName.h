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

#ifndef KONCLUDE_REASONER_ONTOLOGY_CIRINAME_H
#define KONCLUDE_REASONER_ONTOLOGY_CIRINAME_H

// Libraries includes
#include <QPair>

// Namespace includes
#include "OntologySettings.h"
#include "CConcept.h"
#include "CName.h"

// Other includes
#include "Utilities/CAllocationObject.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities;

	namespace Reasoner {

		namespace Ontology {

			/*! 
			 *
			 *		\class		CIRIName
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CIRIName : public CName, public CAllocationObject {
				// public methods
				public:
					//! Constructor
					CIRIName(const QString &iriNameString = QString(""));

					//! Destructor
					virtual ~CIRIName();

					virtual CIRIName *init(const QString &iriNameString);

					virtual QString getIRIName();
					virtual CIRIName *setIRIName(const QString &iriNameString);

					static QStringList getIRINames(CLinker<CName *> *nameLinker, qint64 nameVersionDifference = 0);
					static QString getRecentIRIName(CLinker<CName *> *nameLinker, qint64 nameVersionDifference = 0);

					static bool hasIRIName(CLinker<CName *> *nameLinker, const QString &name, qint64 nameVersionDifference = 0);


				// protected methods
				protected:

				// protected variables
				protected:
					QString iriString;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ONTOLOGY_CIRINAME_H
