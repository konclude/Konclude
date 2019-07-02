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

#include "CIndividualNodeExpressivityViolationFlags.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CIndividualNodeExpressivityViolationFlags::CIndividualNodeExpressivityViolationFlags(CProcessContext* context) : mContext(context) {
				}


				CIndividualNodeExpressivityViolationFlags* CIndividualNodeExpressivityViolationFlags::initViolationFlags(CIndividualNodeExpressivityViolationFlags* violationFlags) {
					mViolFlags = 0;
					if (violationFlags) {
						mViolFlags = violationFlags->mViolFlags;
					}
					return this;
				}


				bool CIndividualNodeExpressivityViolationFlags::setViolationFlags(cint64 flags) {
					if (flags != 0) {
						flags |= VFEXPRESSIVITYVIOLATED;
					}
					bool diff = (mViolFlags & flags) != flags;
					mViolFlags |= flags;
					return diff;
				}

				bool CIndividualNodeExpressivityViolationFlags::setViolationFlags(CIndividualNodeExpressivityViolationFlags* violationFlags) {
					return setViolationFlags(violationFlags->mViolFlags);
				}

				bool CIndividualNodeExpressivityViolationFlags::hasPartialViolationFlags(cint64 flags) {
					return (mViolFlags & flags) != 0;
				}

				bool CIndividualNodeExpressivityViolationFlags::hasViolationFlags(cint64 flags) {
					return (mViolFlags & flags) == flags;
				}

				bool CIndividualNodeExpressivityViolationFlags::hasViolations() {
					return mViolFlags != 0;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
