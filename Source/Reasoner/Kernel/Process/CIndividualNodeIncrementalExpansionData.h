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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CINDIVIDUALNODEINCREMENTALEXPANSIONDATA_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CINDIVIDUALNODEINCREMENTALEXPANSIONDATA_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CProcessContext.h"
#include "CConceptDescriptor.h"
#include "CLinkEdge.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				/*! 
				 *
				 *		\class		CIndividualNodeIncrementalExpansionData
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CIndividualNodeIncrementalExpansionData {
					// public methods
					public:
						//! Constructor
						CIndividualNodeIncrementalExpansionData(CProcessContext* processContext = nullptr);
						
						CIndividualNodeIncrementalExpansionData* initIncrementalExpansionData(CIndividualNodeIncrementalExpansionData* prevData);

						CIndividualProcessNode* getDirectlyChangedNeighbourConnectionNode();
						CIndividualNodeIncrementalExpansionData* setDirectlyChangedNeighbourConnectionNode(CIndividualProcessNode* node);

						bool hasDirectlyChangedNeighbourConnection();

						bool isCompatibilityChanged();
						bool isDirectlyChanged();

						CIndividualNodeIncrementalExpansionData* setDirectlyChanged(bool directlyChanged);


						bool isPreviousCompletionGraphCompatible();
						CIndividualNodeIncrementalExpansionData* setPreviousCompletionGraphCompatible(bool compatible);

						CConceptDescriptor* getLastCompatibleCheckedConceptDescriptor();
						CLinkEdge* getLastCompatibleCheckedLink();

						CIndividualNodeIncrementalExpansionData* setLastCompatibleCheckedConceptDescriptor(CConceptDescriptor* conDes);
						CIndividualNodeIncrementalExpansionData* setLastCompatibleCheckedLink(CLinkEdge* link);


						CIndividualProcessNode* getPreviousCompletionGraphCorrespondenceIndividualNode();
						CIndividualNodeIncrementalExpansionData* setPreviousCompletionGraphCorrespondenceIndividualNode(CIndividualProcessNode* node);
					
						bool isPreviousCompletionGraphCorrespondenceIndividualNodeLoaded();
						CIndividualNodeIncrementalExpansionData* setPreviousCompletionGraphCorrespondenceIndividualNodeLoaded(bool loaded);

						CPROCESSLIST<CIndividualProcessNode*>* getNeighbourPropagatedDirectlyChangedList(bool create = true);
						CIndividualNodeIncrementalExpansionData* addNeighbourPropagatedDirectlyChanged(CIndividualProcessNode* indiNode);
						CIndividualNodeIncrementalExpansionData* clearNeighbourPropagatedDirectlyChangedList();
						bool hasNeighbourPropagatedDirectlyChanged();



						CPROCESSLIST<CIndividual*>* getIncrementalExpansionList(bool create = true);
						bool isIncremetnalExpansionListInitialized();
						CIndividualNodeIncrementalExpansionData* setIncremetnalExpansionListInitialized(bool initialized);
						bool requiresFurtherIncrementalExpansion();
						CIndividual* takeNextIncrementalExpansionIndividual();
						CIndividual* getNextIncrementalExpansionIndividual();

						double getExpansionPriority();
						CIndividualNodeIncrementalExpansionData* setExpansionPriority(double priority);
						CIndividualNodeIncrementalExpansionData* setExpansionID(cint64 id);

						double getNextIncrementalExpansionPriority();


					// protected methods
					protected:

					// protected variables
					protected:
						CProcessContext* mProcessContext;

						double mExpansionPriority;

						CPROCESSLIST<CIndividual*>* mUseIncrementalExpansionList;
						CPROCESSLIST<CIndividual*>* mLocIncrementalExpansionList;
						bool mIncremetnalExpansionListInitialized;

						CPROCESSLIST<CIndividualProcessNode*>* mUseNeighbourPropagatedDirectlyChangedList;
						CPROCESSLIST<CIndividualProcessNode*>* mLocNeighbourPropagatedDirectlyChangedList;

						bool mDirectlyChanged;
						CIndividualProcessNode* mDirectlyChangedNeighbourConnNode;

						CIndividualProcessNode* mPrevCompGraphCorrIndiNode;
						bool mPrevCompGraphCorrIndiNodeLoaded;

						bool mCompletionGraphCompatible;
						CConceptDescriptor* mLastCompatibleCheckedConDes;
						CLinkEdge* mLastCompatibleCheckedLink;

						

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CINDIVIDUALNODEINCREMENTALEXPANSIONDATA_H
