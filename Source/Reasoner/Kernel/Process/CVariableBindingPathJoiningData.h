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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CVARIABLEBINDINGPATHJOININGDATA_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CVARIABLEBINDINGPATHJOININGDATA_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CVariableBindingDescriptor.h"
#include "CVariableBindingPathDescriptor.h"


// Other includes
#include "Utilities/Container/CLinker.h"
#include "Utilities/CSortedLinker.hpp"

#include "Reasoner/Ontology/CConcept.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;

		namespace Kernel {

			namespace Process {

				/*! 
				 *
				 *		\class		CVariableBindingPathJoiningData
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CVariableBindingPathJoiningData {
					// public methods
					public:
						//! Constructor
						CVariableBindingPathJoiningData();

						CVariableBindingPathJoiningData* initVariableBindingPathJoiningData(CVariableBindingPathJoiningData* prevJoinData);
						CVariableBindingPathJoiningData* initVariableBindingPathJoiningData(CVariableBindingDescriptor* keyVarBindDesLinker, CVariableBindingPathDescriptor* leftVarBindPathDesLinker, CVariableBindingPathDescriptor* rightVarBindPathDesLinker);

						CVariableBindingDescriptor* getKeyVariableBindingDescriptorLinker() const;
						CVariableBindingPathDescriptor* getLeftVariableBindingPathDescriptorLinker() const;
						CVariableBindingPathDescriptor* getRightVariableBindingPathDescriptorLinker() const;
						CVariableBindingDescriptor* getNextKeyTriggerLinker(bool moveNext = false);

						CVariableBindingPathJoiningData* setKeyVariableBindingDescriptorLinker(CVariableBindingDescriptor* keyVarBindDesLinker);
						CVariableBindingPathJoiningData* setLeftVariableBindingPathDescriptorLinker(CVariableBindingPathDescriptor* leftVarBindPathDesLinker);
						CVariableBindingPathJoiningData* setRightVariableBindingPathDescriptorLinker(CVariableBindingPathDescriptor* rightVarBindPathDesLinker);
						CVariableBindingPathJoiningData* setNextKeyTriggerLinker(CVariableBindingDescriptor* nextKeyTrigger);

						CVariableBindingPathJoiningData* addKeyVariableBindingDescriptorLinker(CVariableBindingDescriptor* keyVarBindDesLinker);
						CVariableBindingPathJoiningData* addLeftVariableBindingPathDescriptorLinker(CVariableBindingPathDescriptor* leftVarBindPathDesLinker);
						CVariableBindingPathJoiningData* addRightVariableBindingPathDescriptorLinker(CVariableBindingPathDescriptor* rightVarBindPathDesLinker);

						bool allKeyTriggersAvailable();
						CVariableBindingPathJoiningData* setAllKeyTriggersAvailable(bool allAvailable);


						cint64 getCalculatedHashValue() const;

						bool isKeyEquivalentTo(const CVariableBindingPathJoiningData& data) const;
						bool isKeyEquivalentTo(CVariableBindingPath* varBindPath) const;

					// protected methods
					protected:

					// protected variables
					protected:
						mutable cint64 mCalculatedHashValue;
						mutable bool mHashValueCalculated;
						CVariableBindingDescriptor* mKeyVarBindDesLinker;
						CVariableBindingPathDescriptor* mLeftVarBindPathDesLinker;
						CVariableBindingPathDescriptor* mRightVarBindPathDesLinker;

						bool mAllKeyTriggersAvailable;
						CVariableBindingDescriptor* mNextKeyTriggerLinker;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CVARIABLEBINDINGPATHJOININGDATA_H
