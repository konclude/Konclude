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

#ifndef KONCLUDE_REASONER_ONTOLOGY_CAbbreviatedIRIName_H
#define KONCLUDE_REASONER_ONTOLOGY_CAbbreviatedIRIName_H

// Libraries includes
#include <QPair>

// Namespace includes
#include "OntologySettings.h"
#include "CConcept.h"
#include "CName.h"
#include "CIRIName.h"

// Other includes

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {

			/*! 
			 *
			 *		\class		CAbbreviatedIRIName
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CAbbreviatedIRIName : public CIRIName {
				// public methods
				public:
					//! Constructor
					CAbbreviatedIRIName(CNamePrefix *namePrefix = 0, const QString &iriNameString = QString(""));

					//! Destructor
					virtual ~CAbbreviatedIRIName();

					virtual CAbbreviatedIRIName *init(CNamePrefix *namePrefix, const QString &iriNameString);

					virtual QString getIRIName();
					virtual QString getAbbreviatedIRIName();
					virtual QString getAbbreviatedPrefixWithAbbreviatedIRIName(const QString &joiningString = QString(":"));

					virtual CNamePrefix *getNamePrefix();

					static QStringList getAbbreviatedIRINames(CLinker<CName *> *nameLinker, qint64 nameVersionDifference = 0);
					static QStringList getAbbreviatedPrefixWithAbbreviatedIRINames(CLinker<CName *> *nameLinker, qint64 nameVersionDifference = 0, const QString &joiningString = QString(":"));
					static QString getRecentAbbreviatedPrefixWithAbbreviatedIRIName(CLinker<CName *> *nameLinker, qint64 nameVersionDifference = 0, const QString &joiningString = QString(":"));
					
					static bool hasAbbreviatedIRIName(CLinker<CName *> *nameLinker, CNamePrefix *namePrefix, const QString &name, qint64 nameVersionDifference = 0);

				// protected methods
				protected:

				// protected variables
				protected:
					CNamePrefix *prefix;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ONTOLOGY_CAbbreviatedIRIName_H
