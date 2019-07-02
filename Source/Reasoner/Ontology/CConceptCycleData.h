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

#ifndef KONCLUDE_REASONER_ONTOLOGY_CCONCEPTCYCLEDATA_H
#define KONCLUDE_REASONER_ONTOLOGY_CCONCEPTCYCLEDATA_H

// Libraries includes
#include <QSet>

// Namespace includes
#include "OntologySettings.h"
#include "CConcept.h"


// Other includes
#include "Utilities/Container/CNegLinker.h"

#include "Context/CContext.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Utilities::Container;
	using namespace Context;

	namespace Reasoner {

		namespace Ontology {

			typedef QPair<CConcept*,bool> TConceptNegPair;

			/*! 
			 *
			 *		\class		CConceptCycleData
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CConceptCycleData {
				// public methods
				public:
					//! Constructor
					CConceptCycleData(CContext* context = nullptr);

					CConceptCycleData* initConceptCycleData(CConceptCycleData* prevData = nullptr);

					cint64 getCycleID();
					CConceptCycleData* setCycleID(cint64 cycleID);

					cint64 getConceptCount();
					CBOXSET<TConceptNegPair>* getCycleConceptSet();
					CConceptCycleData* addCycleConcept(CConcept* concept, bool negation);
					CConceptCycleData* addCycleConcept(const TConceptNegPair& conNegPair);

					CConceptCycleData* addCycleConcepts(const QSet<TConceptNegPair>& conNegPairSet);

					CBOXSET<CConcept*>* getCycleClassSet();

					CConcept* getRepresentativeCycleClass();

					bool hasNominalConnection();
					CConceptCycleData* setNominalConnection(bool nominalConnection);

				// protected methods
				protected:

				// protected variables
				protected:
					CContext* mContext;
					cint64 mCycleID;
					bool mNominalConnection;
					CBOXSET<TConceptNegPair> mCycleConceptSet;
					CBOXSET<CConcept*> mCycleClassSet;
					CConcept* mRepCycleClass;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ONTOLOGY_CCONCEPTCYCLEDATA_H
