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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CINDIVIDUALSATURATIONREFERENCELINKINGDATA_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CINDIVIDUALSATURATIONREFERENCELINKINGDATA_H

// Libraries includes


// Namespace includes
#include "OntologySettings.h"
#include "CProcessReference.h"
#include "CConceptReferenceLinking.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		namespace Ontology {

			/*! 
			 *
			 *		\class		CIndividualSaturationReferenceLinkingData
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CIndividualSaturationReferenceLinkingData : public CConceptReferenceLinking {
				// public methods
				public:
					//! Constructor
					CIndividualSaturationReferenceLinkingData();


					CProcessReference* getIndividualProcessNodeForConcept();
					CIndividualSaturationReferenceLinkingData* setIndividualProcessNodeForIndividual(CProcessReference* indiNode);

					
					CIndividualSaturationReferenceLinkingData* setSaturationID(cint64 saturationID);
					cint64 getSaturationID();

				// protected methods
				protected:

				// protected variables
				protected:
					CProcessReference* mIndiProcessNodeForIndividual;
					cint64 mSaturationID;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CINDIVIDUALSATURATIONREFERENCELINKINGDATA_H
