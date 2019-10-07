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

#ifndef KONCLUDE_REASONER_REALIZATION_CSAMEREALIZATION_H
#define KONCLUDE_REASONER_REALIZATION_CSAMEREALIZATION_H

// Libraries includes


// Namespace includes
#include "RealizationSettings.h"
#include "CSameRealizationIndividualVisitor.h"
#include "CRealizationIndividualInstanceItemReference.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Realization {

			/*! 
			 *
			 *		\class		CSameRealization
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CSameRealization {
				// public methods
				public:
					//! Constructor
					CSameRealization();

					virtual bool visitSameIndividuals(CIndividual* individual, CSameRealizationIndividualVisitor* visitor);
					virtual bool visitSameIndividuals(const CIndividualReference& indiRef, CSameRealizationIndividualVisitor* visitor);

					virtual bool visitSameIndividuals(const CRealizationIndividualInstanceItemReference& itemRef, CSameRealizationIndividualVisitor* visitor) = 0;

					virtual CRealizationIndividualInstanceItemReference getSameInstanceItemReference(const CIndividualReference& indiRef) = 0;


					virtual bool hasPotentiallySameIndividuals() = 0;

					// deprecated
					virtual bool visitSameIndividuals(CSameInstanceItem* item, CSameRealizationIndividualVisitor* visitor) = 0;
					virtual CSameInstanceItem* getSameInstanceItem(CIndividual* individual);

				// protected methods
				protected:

				// protected variables
				protected:

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Realization

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_REALIZATION_CSAMEREALIZATION_H
