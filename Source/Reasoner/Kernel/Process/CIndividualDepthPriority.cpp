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

#include "CIndividualDepthPriority.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CIndividualDepthPriority::CIndividualDepthPriority() {
					mIndiDepth = CINT64_MAX;
					mIndiID = CINT64_MIN;
				}

				CIndividualDepthPriority::CIndividualDepthPriority(cint64 indiDepth, cint64 indiID) {
					mIndiDepth = indiDepth;
					mIndiID = indiID;
				}

				cint64 CIndividualDepthPriority::getIndividualDepth() const {
					return mIndiDepth;
				}

				cint64 CIndividualDepthPriority::getIndividualID() const {
					return mIndiID;
				}

				CIndividualDepthPriority* CIndividualDepthPriority::setPriority(cint64 indiDepth, cint64 indiID) {
					mIndiDepth = indiDepth;
					mIndiID = indiID;
					return this;
				}

				bool CIndividualDepthPriority::operator==(const CIndividualDepthPriority& indiPriority) const {
					return mIndiID == indiPriority.mIndiID && mIndiDepth == indiPriority.mIndiDepth;
				}

				bool CIndividualDepthPriority::operator<=(const CIndividualDepthPriority& indiPriority) const {
					if (mIndiDepth < indiPriority.mIndiDepth) {
						return true;
					} else if (mIndiDepth == indiPriority.mIndiDepth) {
						return mIndiID <= indiPriority.mIndiID;
					}
					return false;
				}

				bool CIndividualDepthPriority::operator<(const CIndividualDepthPriority& indiPriority) const {
					if (mIndiDepth < indiPriority.mIndiDepth) {
						return true;
					} else if (mIndiDepth == indiPriority.mIndiDepth) {
						return mIndiID < indiPriority.mIndiID;
					}
					return false;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
