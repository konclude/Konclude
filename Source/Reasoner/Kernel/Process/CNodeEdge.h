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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CNODEEDGE_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CNODEEDGE_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CLocalizationTag.h"
#include "CDependencyTracker.h"
#include "CProcessContext.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				/*! 
				 *
				 *		\class		CNodeEdge
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CNodeEdge : public CLocalizationTag, public CDependencyTracker {
					// public methods
					public:
						//! Constructor
						CNodeEdge(CProcessContext* processContext = nullptr);


						CNodeEdge* initNodeEdge(CIndividualProcessNode* sourceIndividual, CIndividualProcessNode* destinationIndividual, CDependencyTrackPoint* depTrackPoint);
						CNodeEdge* initNodeEdge(CNodeEdge* nodeEdge);

						CIndividualProcessNode* getSourceIndividual();
						CIndividualProcessNode* getDestinationIndividual();
						cint64 getSourceIndividualID();
						cint64 getDestinationIndividualID();

						CNodeEdge* setSourceIndividual(CIndividualProcessNode* sourceIndi);
						CNodeEdge* setDestinationIndividual(CIndividualProcessNode* destinationIndi);


						CIndividualProcessNode* getOppositeIndividual(CIndividualProcessNode* indi);
						CIndividualProcessNode* getOppositeIndividual(cint64 indiID);
						cint64 getOppositeIndividualID(CIndividualProcessNode* indi);
						cint64 getOppositeIndividualID(cint64 indiID);


						bool isDestinationIndividual(CIndividualProcessNode* indi);
						bool isDestinationIndividualID(CIndividualProcessNode* indi);
						bool isDestinationIndividualID(cint64 indiID);
						bool isSourceIndividual(CIndividualProcessNode* indi);
						bool isSourceIndividualID(CIndividualProcessNode* indi);
						bool isSourceIndividualID(cint64 indiID);

						cint64 getCoupledIndividualID();


					// protected methods
					protected:

					// protected variables
					protected:
						CIndividualProcessNode* mSourceIndividual;
						CIndividualProcessNode* mDestinationIndividual;

					// private methods
					private:

					// private variables
					private:


				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CNODEEDGE_H
