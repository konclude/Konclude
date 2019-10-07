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

#ifndef KONCLUDE_REASONER_ONTOLOGY_CONTOLOGYCORECONCEPTCYCLESDATA_H
#define KONCLUDE_REASONER_ONTOLOGY_CONTOLOGYCORECONCEPTCYCLESDATA_H

// Libraries includes


// Namespace includes
#include "OntologySettings.h"
#include "CConceptCycleData.h"
#include "COntologyContext.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		namespace Ontology {

			/*! 
			 *
			 *		\class		COntologyCoreConceptCyclesData
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COntologyCoreConceptCyclesData {
				// public methods
				public:
					//! Constructor
					COntologyCoreConceptCyclesData(COntologyContext* ontoContext = nullptr);
					~COntologyCoreConceptCyclesData();

					CBOXHASH<TConceptNegPair,CConceptCycleData*>* getConceptCycleHash();
					CBOXLIST<CConceptCycleData*>* getConceptCycleList();
					cint64 getConceptCycleCount();

					COntologyCoreConceptCyclesData* installCycle(CConceptCycleData* cycleData);


				// protected methods
				protected:

				// protected variables
				protected:
					COntologyContext* mContext;
					CBOXHASH<TConceptNegPair,CConceptCycleData*> mConceptCycleHash;
					CBOXLIST<CConceptCycleData*> mCycleList;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ONTOLOGY_CONTOLOGYCORECONCEPTCYCLESDATA_H
