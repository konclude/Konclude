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

#ifndef KONCLUDE_UTILITIES_CONTAINER_CMODELDATALEVELARRAY_H
#define KONCLUDE_UTILITIES_CONTAINER_CMODELDATALEVELARRAY_H

// Libraries includes


// Namespace includes
#include "ContainerSettings.h"
#include "CModelData.h"
#include "CModelDataArray.h"
#include "CDataArrayMemoryManager.h"

// Other includes
#include "Utilities/CMath.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities;

	namespace Utilities {

		namespace Container {

			/*! 
			 *
			 *		\class		CModelDataLevelArray
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CModelDataLevelArray : public CModelDataArray {
				// public methods
				public:
					//! Constructor
					CModelDataLevelArray();
					CModelDataLevelArray(qint64 tagBegin, qint64 tagBegEndSize);

					//! Destructor
					virtual ~CModelDataLevelArray();

					virtual CModelDataArray *init(qint64 tagBegin = 0, qint64 tagBegEndSize = 64);

					virtual bool isBaseLevel();
					virtual qint64 getModelLevel();

					virtual CModelDataArray **getModelArray();
					virtual CModelDataArray *getNextModelArray(qint64 index);
					virtual CModelDataArray *setNextModelArray(qint64 index, CModelDataArray *nextModelDataArray);

					virtual CModelDataArray *createNextModelArray(qint64 index, CDataArrayMemoryManager *modelMemMan);

					virtual CModelDataLevelArray *setBounds(qint64 tagBegin = 0, qint64 tagBegEndSize = 64);

					virtual CModelDataLevelArray *setData(bool value = false);

					virtual bool setFlag(qint64 tag, bool flag, CDataArrayMemoryManager *modelMemMan);
					virtual bool setFlag(qint64 tag, bool flag = true);
					virtual bool getFlag(qint64 tag);

					virtual CModelDataArray *getBaseModel(qint64 beginTag);

					virtual qint64 countFlags();

					virtual qint64 getFirstFlagTag();
					virtual qint64 getLastFlagTag();

					virtual CModelDataArray *intersectWith(CModelDataArray *modelData, bool *flagModification = 0);
					virtual CModelDataArray *unionWith(CModelDataArray *modelData, bool *flagModification = 0);
					virtual CModelDataArray *intersectWith(CModelDataArray *modelData, CDataArrayMemoryManager *modelMemMan, bool *flagModification = 0);
					virtual CModelDataArray *unionWith(CModelDataArray *modelData, CDataArrayMemoryManager *modelMemMan, bool *flagModification = 0);

					virtual CModelDataArray *copyFrom(CModelDataArray *modelData, bool *flagModification = 0);
					virtual CModelDataArray *copyFrom(CModelDataArray *modelData, CDataArrayMemoryManager *modelMemMan, bool *flagModification = 0);

					virtual qint64 getArrayIndex(qint64 tag);

					virtual bool isFullConstructed();

					static CModelDataArray *extendModel(CModelDataArray *modelData, qint64 minBegTag, qint64 minEndTag, CDataArrayMemoryManager *modelMemMan);
					static CModelDataArray *extendModel(CModelDataArray *modelData, qint64 minTag, CDataArrayMemoryManager *modelMemMan);
					static CModelDataArray *extendModel(CModelDataArray *modelData, CModelDataArray *minModelDataExt, CDataArrayMemoryManager *modelMemMan);


					static CModelDataArray *extendAndUnionModelWith(CModelDataArray *modelData, CModelDataArray *minModelDataExt, CDataArrayMemoryManager *modelMemMan, bool *flagModification = 0);
					static CModelDataArray *extendAndCopyModelFrom(CModelDataArray *modelData, CModelDataArray *minModelDataExt, CDataArrayMemoryManager *modelMemMan, bool *flagModification = 0);
					static CModelDataArray *reduceAndIntersecModelWith(CModelDataArray *modelData, CModelDataArray *minModelDataExt, CDataArrayMemoryManager *modelMemMan, bool *flagModification = 0);
					static CModelDataArray *extendAndSetFlag(CModelDataArray *modelData, qint64 tag, CDataArrayMemoryManager *modelMemMan, bool *flagModification = 0);

					static CModelDataArray *reduceModel(CModelDataArray *modelData);

					static bool testHasIntersectBetween(CModelDataArray *modelData1, CModelDataArray *modelData2);
					static bool testIsSubsetBetween(CModelDataArray *modelData1, CModelDataArray *modelData2);

					virtual bool hasIntersectionWith(CModelDataArray *modelData);
					virtual qint64 countIntersectionWith(CModelDataArray *modelData);
					virtual bool isSubsetFrom(CModelDataArray *modelData);
					virtual bool isSuperSetTo(CModelDataArray *modelData);

				// protected methods
				protected:

				// protected variables
				protected:
					CModelDataArray *modelArray[dataSize];

					qint64 level;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Container

	}; // end namespace Utilities

}; // end namespace Konclude

#endif // KONCLUDE_UTILITIES_CONTAINER_CMODELDATALEVELARRAY_H
