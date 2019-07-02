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

#include "CPropagationRepresentativeTransitionExtension.h"
#include "CIndividualProcessNode.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CPropagationRepresentativeTransitionExtension::CPropagationRepresentativeTransitionExtension(CProcessContext* processContext) : mLeftRepPropMap(processContext),mRightRepPropMap(processContext) {
					mProcessContext = processContext;
				}


				CPropagationRepresentativeTransitionExtension* CPropagationRepresentativeTransitionExtension::initPropagationRepresentativeTransitionExtension(CPropagationRepresentativeTransitionExtension* propVarBindTransExtension) {
					if (propVarBindTransExtension) {
						mLastAnalysedPropBindDes = propVarBindTransExtension->mLastAnalysedPropBindDes;
						mLastAnalysedPropagateAllFlag = propVarBindTransExtension->mLastAnalysedPropagateAllFlag;
						mLeftLastRepPropDes = propVarBindTransExtension->mLeftLastRepPropDes;
						mRightLastRepPropDes = propVarBindTransExtension->mRightLastRepPropDes;
						mLeftRepPropMap = propVarBindTransExtension->mLeftRepPropMap;
						mRightRepPropMap = propVarBindTransExtension->mRightRepPropMap;
					} else {
						mLastAnalysedPropBindDes = nullptr;
						mLastAnalysedPropagateAllFlag = false;
						mRightLastRepPropDes = nullptr;
						mLeftLastRepPropDes = nullptr;
						mLeftRepPropMap.clear();
						mRightRepPropMap.clear();
					}
					return this;
				}

				bool CPropagationRepresentativeTransitionExtension::getLastAnalysedPropagateAllFlag() {
					return mLastAnalysedPropagateAllFlag;
				}

				CPropagationRepresentativeTransitionExtension* CPropagationRepresentativeTransitionExtension::setLastAnalysedPropagateAllFlag(bool propagateAllFlag) {
					mLastAnalysedPropagateAllFlag = propagateAllFlag;
					return this;
				}


				CPropagationBindingDescriptor* CPropagationRepresentativeTransitionExtension::getLastAnalysedPropagationBindingDescriptor() {
					return mLastAnalysedPropBindDes;
				}

				CPropagationRepresentativeTransitionExtension* CPropagationRepresentativeTransitionExtension::setLastAnalysedPropagationBindingDescriptor(CPropagationBindingDescriptor* lastAnalPropBindDes) {
					mLastAnalysedPropBindDes = lastAnalPropBindDes;
					return this;
				}

				CRepresentativePropagationDescriptor* CPropagationRepresentativeTransitionExtension::getLeftLastRepresentativeJoiningDescriptor() {
					return mLeftLastRepPropDes;
				}

				CRepresentativePropagationDescriptor* CPropagationRepresentativeTransitionExtension::getRightLastRepresentativeJoiningDescriptor() {
					return mRightLastRepPropDes;
				}


				CPropagationRepresentativeTransitionExtension* CPropagationRepresentativeTransitionExtension::setLeftLastRepresentativeJoiningDescriptor(CRepresentativePropagationDescriptor* varBindPathDes) {
					mLeftLastRepPropDes = varBindPathDes;
					return this;
				}

				CPropagationRepresentativeTransitionExtension* CPropagationRepresentativeTransitionExtension::setRightLastRepresentativeJoiningDescriptor(CRepresentativePropagationDescriptor* varBindPathDes) {
					mRightLastRepPropDes = varBindPathDes;
					return this;
				}

				CRepresentativePropagationMap* CPropagationRepresentativeTransitionExtension::getLeftRepresentativePropagationMap() {
					return &mLeftRepPropMap;
				}

				CRepresentativePropagationMap* CPropagationRepresentativeTransitionExtension::getRightRepresentativePropagationMap() {
					return &mRightRepPropMap;
				}



			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
