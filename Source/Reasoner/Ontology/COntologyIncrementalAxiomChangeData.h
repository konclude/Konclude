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

#ifndef KONCLUDE_REASONER_ONTOLOGY_CONTOLOGYINCREMENTALAXIOMCHANGEDATA_H
#define KONCLUDE_REASONER_ONTOLOGY_CONTOLOGYINCREMENTALAXIOMCHANGEDATA_H

// Libraries includes


// Namespace includes
#include "OntologySettings.h"
#include "COntologyContext.h"


// Other includes

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {

			/*! 
			 *
			 *		\class		COntologyIncrementalAxiomChangeData
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COntologyIncrementalAxiomChangeData {
				// public methods
				public:
					//! Constructor
					COntologyIncrementalAxiomChangeData(COntologyContext* ontoContext = nullptr);

					cint64 getAddedABoxAxiomCount();
					cint64 getAddedTBoxAxiomCount();
					cint64 getAddedRBoxAxiomCount();

					cint64 getDeletedABoxAxiomCount();
					cint64 getDeletedTBoxAxiomCount();
					cint64 getDeletedRBoxAxiomCount();


					COntologyIncrementalAxiomChangeData* incAddedABoxAxiomCount(cint64 incCount = 1);
					COntologyIncrementalAxiomChangeData* incAddedTBoxAxiomCount(cint64 incCount = 1);
					COntologyIncrementalAxiomChangeData* incAddedRBoxAxiomCount(cint64 incCount = 1);


					COntologyIncrementalAxiomChangeData* incDeletedABoxAxiomCount(cint64 incCount = 1);
					COntologyIncrementalAxiomChangeData* incDeletedTBoxAxiomCount(cint64 incCount = 1);
					COntologyIncrementalAxiomChangeData* incDeletedRBoxAxiomCount(cint64 incCount = 1);

					bool hasAddedABoxAxioms();
					bool hasAddedTBoxAxioms();
					bool hasAddedRBoxAxioms();

					bool hasDeletedABoxAxioms();
					bool hasDeletedTBoxAxioms();
					bool hasDeletedRBoxAxioms();




					bool hasChangedABoxAxioms();
					bool hasChangedTBoxAxioms();
					bool hasChangedRBoxAxioms();



					bool hasAxiomAdditions();
					bool hasAxiomDeletions();

					bool hasChangedAxioms();
					bool hasOnlyChangedABoxAxioms();
					bool hasOnlyChangedTBoxAxioms();
					bool hasOnlyChangedRBoxAxioms();


				// protected methods
				protected:

				// protected variables
				protected:
					COntologyContext* mOntoContext;

					cint64 mAddedABoxAxiomCount;
					cint64 mAddedTBoxAxiomCount;
					cint64 mAddedRBoxAxiomCount;


					cint64 mDeletedABoxAxiomCount;
					cint64 mDeletedTBoxAxiomCount;
					cint64 mDeletedRBoxAxiomCount;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ONTOLOGY_CONTOLOGYINCREMENTALAXIOMCHANGEDATA_H
