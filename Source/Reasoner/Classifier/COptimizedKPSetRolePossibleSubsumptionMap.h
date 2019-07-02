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

#ifndef KONCLUDE_REASONER_CLASSIFIER_COPTIMIZEDKPSETROLEPOSSIBLESUBSUMPTIONMAP_H
#define KONCLUDE_REASONER_CLASSIFIER_COPTIMIZEDKPSETROLEPOSSIBLESUBSUMPTIONMAP_H

// Libraries includes
#include <QMap>


// Namespace includes
#include "ClassifierSettings.h"
#include "CPropertyPossibleSubsumptionMap.h"
#include "COptimizedKPSetRolePossibleSubsumptionData.h"
#include "COptimizedKPSetRolePossibleSubsumptionMapIterator.h"


// Other includes
#include "Reasoner/Ontology/CRoleTagComparer.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;

		namespace Classifier {


			/*! 
			 *
			 *		\class		COptimizedKPSetRolePossibleSubsumptionMap
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedKPSetRolePossibleSubsumptionMap : public QMap<CRoleTagComparer,COptimizedKPSetRolePossibleSubsumptionData*>, public CPropertyPossibleSubsumptionMap {
				// public methods
				public:
					//! Constructor
					COptimizedKPSetRolePossibleSubsumptionMap();

					bool isPossibleSubsumptionUpdateRequired();
					cint64 getRemainingPossibleSubsumptionCount();

					COptimizedKPSetRolePossibleSubsumptionMap* setRemainingPossibleSubsumptionCount(cint64 possSubsumCount);
					COptimizedKPSetRolePossibleSubsumptionMap* decRemainingPossibleSubsumptionCount(cint64 decCount = 1);
					COptimizedKPSetRolePossibleSubsumptionMap* incRemainingPossibleSubsumptionCount(cint64 incCount = 1);
					COptimizedKPSetRolePossibleSubsumptionMap* setPossibleSubsumptionUpdateRequired(bool requiredUpdate = true);

					bool hasRemainingPossibleSubsumptions();
					CPropertyPossibleSubsumptionMapIterator* getIterator(CPropertyPossibleSubsumptionMapIterator* reuseIterator, CContext* context);


				// protected methods
				protected:

				// protected variables
				protected:
					bool mRequirePossibleSubsumptionUpdate;
					cint64 mRemainingPossibleSubsumptionCount;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CLASSIFIER_COPTIMIZEDKPSETROLEPOSSIBLESUBSUMPTIONMAP_H
