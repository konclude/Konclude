/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#include "CVariableBindingTriggerLinker.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				CVariableBindingTriggerLinker::CVariableBindingTriggerLinker(CVariableBindingPathDescriptor* varBindPathDes) : CLinkerBase<CVariableBindingPathDescriptor*,CVariableBindingTriggerLinker>(varBindPathDes) {
				}


				CVariableBindingTriggerLinker* CVariableBindingTriggerLinker::initTriggerLinker(CVariableBindingPathDescriptor* varBindPathDes, CVariableBindingDescriptor* varBindDes, bool leftTriggered) {
					initLinker(varBindPathDes);
					mNextTriggerVariableBinding = varBindDes;
					mLeftTriggered = leftTriggered;
					return this;
				}


				CVariableBindingPathDescriptor* CVariableBindingTriggerLinker::getVariableBindingPathDescriptor() {
					return getData();
				}

				CVariableBindingTriggerLinker* CVariableBindingTriggerLinker::setVariableBindingPathDescriptor(CVariableBindingPathDescriptor* varBindPathDes) {
					setData(varBindPathDes);
					return this;
				}

				CVariableBindingDescriptor* CVariableBindingTriggerLinker::getNextTriggerVariableBindingDescriptor() {
					return mNextTriggerVariableBinding;
				}

				CVariableBindingTriggerLinker* CVariableBindingTriggerLinker::setNextTriggerVariableBindingDescriptor(CVariableBindingDescriptor* varBindDes) {
					mNextTriggerVariableBinding = varBindDes;
					return this;
				}

				bool CVariableBindingTriggerLinker::isLeftTriggered() {
					return mLeftTriggered;
				}

				CVariableBindingTriggerLinker* CVariableBindingTriggerLinker::setLeftTriggered(bool leftTriggered) {
					mLeftTriggered = leftTriggered;
					return this;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
