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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CPROPAGATIONVARIABLEBINDINGTRANSITIONEXTENSION_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CPROPAGATIONVARIABLEBINDINGTRANSITIONEXTENSION_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CPropagationBindingDescriptor.h"
#include "CVariableBindingPathDescriptor.h"
#include "CVariableBindingTriggerHash.h"
#include "CVariableBindingPathJoiningHash.h"


// Other includes
#include "Reasoner/Ontology/CVariable.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;

		namespace Kernel {

			namespace Process {

				/*! 
				 *
				 *		\class		CPropagationVariableBindingTransitionExtension
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CPropagationVariableBindingTransitionExtension {
					// public methods
					public:
						//! Constructor
						CPropagationVariableBindingTransitionExtension(CProcessContext* processContext);

						CPropagationVariableBindingTransitionExtension* initPropagationVariableBindingTransitionExtension(CPropagationVariableBindingTransitionExtension* propVarBindTransExtension);


						bool getLastAnalysedPropagateAllFlag();
						CPropagationBindingDescriptor* getLastAnalysedPropagationBindingDescriptor();
						CPropagationVariableBindingTransitionExtension* setLastAnalysedPropagationBindingDescriptor(CPropagationBindingDescriptor* lastAnalPropBindDes);
						CPropagationVariableBindingTransitionExtension* setLastAnalysedPropagateAllFlag(bool propagateAllFlag);

						CVariableBindingTriggerHash* getVariableBindingTriggerHash(bool localize = true);

						bool addAnalysedPropagationBindingDescriptorReturnMatched(CPropagationBindingDescriptor* propBindDes, CVariableBindingTriggerLinker** reapplyTriggerLinker = nullptr);

						CVariableBindingPathJoiningHash* getVariableBindingPathJoiningHash(bool localize = true);

						CVariableBindingPathDescriptor* getLeftLastVariableBindingPathJoiningDescriptor();
						CVariableBindingPathDescriptor* getRightLastVariableBindingPathJoiningDescriptor();
						CPropagationVariableBindingTransitionExtension* setLeftLastVariableBindingPathJoiningDescriptor(CVariableBindingPathDescriptor* varBindPathDes);
						CPropagationVariableBindingTransitionExtension* setRightLastVariableBindingPathJoiningDescriptor(CVariableBindingPathDescriptor* varBindPathDes);

						CPropagationVariableBindingTransitionExtension* setTriggeredVariableIndividualPair(const TVariableIndividualPair& triggeredVarIndPair);
						CPropagationVariableBindingTransitionExtension* setTriggeredVariableIndividualPair(CVariable* variable, CIndividualProcessNode* indiNode);
						TVariableIndividualPair getTriggeredVariableIndividualPair();

						bool isProcessingCompleted();
						CPropagationVariableBindingTransitionExtension* setProcessingCompleted(bool completed);

					// protected methods
					protected:

					// protected variables
					protected:
						CProcessContext* mProcessContext;
						CPropagationBindingDescriptor* mLastAnalysedPropBindDes;
						bool mLastAnalysedPropagateAllFlag;
						bool mProcessingCompleted;
						CVariableBindingTriggerHash* mLocVarBindTriggerHash;
						CVariableBindingTriggerHash* mUseVarBindTriggerHash;

						TVariableIndividualPair mTriggeredVarIndPair;

						CVariableBindingPathJoiningHash* mUseVarBindPathJoiningHash;
						CVariableBindingPathJoiningHash* mLocVarBindPathJoiningHash;
						CVariableBindingPathDescriptor* mLeftLastVarBindPathJoiningDes;
						CVariableBindingPathDescriptor* mRightLastVarBindPathJoiningDes;

					// private methods
					private:

					// private variables
					private:


				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CPROPAGATIONVARIABLEBINDINGTRANSITIONEXTENSION_H
