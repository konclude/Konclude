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

#include "CDatatypeValueSpaceTriggeringIterator.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			CDatatypeValueSpaceTriggeringIterator::CDatatypeValueSpaceTriggeringIterator(const CBOXMAP<CDatatypeValueSpaceTriggeringMapArranger,CDatatypeValueSpaceTriggeringMapData>::const_iterator beginIt, const CBOXMAP<CDatatypeValueSpaceTriggeringMapArranger,CDatatypeValueSpaceTriggeringMapData>::const_iterator endIt, bool upIteration) {
				mBegIt = beginIt;
				mCurIt = mBegIt;
				mEndIt = endIt;
				mLastIt = endIt;
				mUpIteration = upIteration;
			}



			CDatatypeValueSpaceTriggeringIterator::CDatatypeValueSpaceTriggeringIterator(const CBOXMAP<CDatatypeValueSpaceTriggeringMapArranger,CDatatypeValueSpaceTriggeringMapData>::const_iterator beginIt, const CBOXMAP<CDatatypeValueSpaceTriggeringMapArranger,CDatatypeValueSpaceTriggeringMapData>::const_iterator lastIt, const CBOXMAP<CDatatypeValueSpaceTriggeringMapArranger,CDatatypeValueSpaceTriggeringMapData>::const_iterator endIt, bool upIteration) {
				mBegIt = beginIt;
				mCurIt = mBegIt;
				mEndIt = endIt;
				mLastIt = lastIt;
				mUpIteration = upIteration;
			}


			bool CDatatypeValueSpaceTriggeringIterator::hasNext() {
				if (mCurIt != mEndIt) {
					return true;
				}
				return false;
			}

			CDatatypeValueSpaceTriggeringData* CDatatypeValueSpaceTriggeringIterator::next(bool moveNext) {
				CDatatypeValueSpaceTriggeringData* triggerData = nullptr;
				if (mCurIt != mEndIt) {
					const CDatatypeValueSpaceTriggeringMapData& mapData = mCurIt.value();
					triggerData = mapData.mUseValue;
					if (moveNext) {
						if (mCurIt == mLastIt) {
							mCurIt = mEndIt;
						} else {
							if (mUpIteration) {
								++mCurIt;
							} else {
								--mCurIt;
							}
						}
					}
				}
				return triggerData;
			}



		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude
