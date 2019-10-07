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

#include "CIndividualProcessNodePriority.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CIndividualProcessNodePriority::CIndividualProcessNodePriority(double conPriority, double indiPriority, bool stirctOrder) {
					mPriorityCon = conPriority;
					mPriorityInd = indiPriority;
					mStrictOrder = stirctOrder;
				}

				bool CIndividualProcessNodePriority::isNullPriority() const {
					return mPriorityCon == 0. && mPriorityInd == 0.;
				}


				double CIndividualProcessNodePriority::getConceptPriority() const {
					return mPriorityCon;
				}

				double CIndividualProcessNodePriority::getIndividualPriority() const {
					return mPriorityInd;
				}

				CIndividualProcessNodePriority* CIndividualProcessNodePriority::setPriority(double conPriority, double indiPriority) {
					mPriorityCon = conPriority;
					mPriorityInd = indiPriority;
					return this;
				}

				CIndividualProcessNodePriority* CIndividualProcessNodePriority::setPriorityToNull() {
					mPriorityCon = 0.;
					mPriorityInd = 0.;
					mStrictOrder = true;
					return this;
				}

				bool CIndividualProcessNodePriority::operator==(const CIndividualProcessNodePriority& indiPriority) const {
					return mPriorityCon == indiPriority.mPriorityCon && mPriorityInd == indiPriority.mPriorityInd;
				}

				bool CIndividualProcessNodePriority::operator!=(const CIndividualProcessNodePriority& indiPriority) const {
					return mPriorityCon != indiPriority.mPriorityCon || mPriorityInd != indiPriority.mPriorityInd;
				}

				bool CIndividualProcessNodePriority::operator<=(const CIndividualProcessNodePriority& indiPriority) const {
					bool strictOrder = mStrictOrder || indiPriority.mStrictOrder;
					if (strictOrder) {
						if (mPriorityInd < indiPriority.mPriorityInd) {
							return true;
						} else if (mPriorityInd > indiPriority.mPriorityInd) {
							return false;
						} else {
							return mPriorityCon <= indiPriority.mPriorityCon;
						}
					} else {
						bool sameConceptPriority = mPriorityCon == indiPriority.mPriorityCon;
						return sameConceptPriority && (mPriorityInd <= indiPriority.mPriorityInd) 
								|| !sameConceptPriority && (mPriorityCon >= indiPriority.mPriorityCon);
					}
				}

				bool CIndividualProcessNodePriority::operator>=(const CIndividualProcessNodePriority& indiPriority) const {
					bool strictOrder = mStrictOrder || indiPriority.mStrictOrder;
					if (strictOrder) {
						if (mPriorityInd > indiPriority.mPriorityInd) {
							return true;
						} else if (mPriorityInd < indiPriority.mPriorityInd) {
							return false;
						} else {
							return mPriorityCon >= indiPriority.mPriorityCon;
						}
					} else {
						bool sameConceptPriority = mPriorityCon == indiPriority.mPriorityCon;
						return sameConceptPriority && (mPriorityInd >= indiPriority.mPriorityInd) 
								|| !sameConceptPriority && (mPriorityCon <= indiPriority.mPriorityCon);
					}
				}

				bool CIndividualProcessNodePriority::operator<(const CIndividualProcessNodePriority& indiPriority) const {
					bool strictOrder = mStrictOrder || indiPriority.mStrictOrder;
					if (strictOrder) {
						if (mPriorityInd < indiPriority.mPriorityInd) {
							return true;
						} else {
							return false;
						}
					} else {
						bool sameConceptPriority = mPriorityCon == indiPriority.mPriorityCon;
						return sameConceptPriority && (mPriorityInd < indiPriority.mPriorityInd) 
								|| !sameConceptPriority && (mPriorityCon > indiPriority.mPriorityCon);
					}
				}

				bool CIndividualProcessNodePriority::operator>(const CIndividualProcessNodePriority& indiPriority) const {
					bool strictOrder = mStrictOrder || indiPriority.mStrictOrder;
					if (strictOrder) {
						if (mPriorityInd > indiPriority.mPriorityInd) {
							return true;
						} else {
							return false;
						}
					} else {
						bool sameConceptPriority = mPriorityCon == indiPriority.mPriorityCon;
						return sameConceptPriority && (mPriorityInd > indiPriority.mPriorityInd) 
								|| !sameConceptPriority && (mPriorityCon < indiPriority.mPriorityCon);
					}
				}

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
