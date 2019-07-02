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

#ifndef KONCLUDE_REASONER_REALIZATION_CREALIZATIONINDIVIDUALINSTANCEITEMREFERENCEITERATOR_H
#define KONCLUDE_REASONER_REALIZATION_CREALIZATIONINDIVIDUALINSTANCEITEMREFERENCEITERATOR_H

// Libraries includes

// Namespace includes
#include "RealizationSettings.h"
#include "CRealizationIndividualInstanceItemReference.h"
#include "CRealizationRemainingInstancesEstimation.h"
#include "CRealizationIndividualSorting.h"

// Other includes

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;

		namespace Realization {

			/*! 
			 *
			 *		\class		CRealizationIndividualInstanceItemReferenceIterator
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CRealizationIndividualInstanceItemReferenceIterator {
				// public methods
				public:
					//! Constructor
					CRealizationIndividualInstanceItemReferenceIterator();
					virtual ~CRealizationIndividualInstanceItemReferenceIterator();

					virtual bool requiresInitialization() = 0;
					virtual bool begin() = 0;


					virtual bool atEnd() = 0;
					virtual CRealizationRemainingInstancesEstimation remainingInstancesEstimation() = 0;

					virtual bool moveNext() = 0;
					virtual bool moveTo(const CRealizationIndividualInstanceItemReference& indiInstItemRef, bool moveOver = true) = 0;

					virtual CRealizationIndividualInstanceItemReference currentIndividualInstanceItemReference() = 0;
					virtual bool isCurrentInstancePossible() = 0;
					virtual bool isCurrentInstanceCertain() = 0;

					virtual CRealizationIndividualInstanceItemReferenceIterator* getCopy() = 0;

					virtual CRealizationIndividualSorting getSorting() = 0;


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

#endif // KONCLUDE_REASONER_REALIZATION_CREALIZATIONINDIVIDUALINSTANCEITEMREFERENCEITERATOR_H
