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

#ifndef KONCLUDE_UTILITIES_CONTAINER_CMODELDATAARRAY_H
#define KONCLUDE_UTILITIES_CONTAINER_CMODELDATAARRAY_H

// Libraries includes


// Namespace includes
#include "ContainerSettings.h"
#include "CModelData.h"
#include "CDataArrayMemoryManager.h"

// Other includes
#include "Utilities/CMath.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Utilities {

		namespace Container {

			/*! 
			 *
			 *		\class		CModelDataArray
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CModelDataArray : public CModelData {
				// public methods
				public:
					//! Constructor
					CModelDataArray();
					CModelDataArray(qint64 tagBegin, qint64 tagBegEndSize);

					//! Destructor
					virtual ~CModelDataArray();

					virtual CModelDataArray *init(qint64 tagBegin = 0, qint64 tagBegEndSize = 64);

					virtual qint64 getBeginTag();
					virtual qint64 getEndTag();
					virtual qint64 getTagSize();
					virtual qint64 getDataArray();

					virtual bool isBaseLevel();
					virtual qint64 getModelLevel();

					virtual CModelDataArray *setBounds(qint64 tagBegin = 0, qint64 tagBegEndSize = 64);

					virtual CModelDataArray *setData(bool value = false);
					virtual CModelDataArray *setLevelData(bool value = false);

					virtual CModelDataArray *getBaseModel(qint64 beginTag);

					virtual bool setFlag(qint64 tag, bool flag, CDataArrayMemoryManager *modelMemMan);
					virtual bool setFlag(qint64 tag, bool flag = true);
					virtual bool getFlag(qint64 tag);

					virtual bool setLevelFlag(qint64 tag, bool flag = true);
					virtual bool getLevelFlag(qint64 tag);
					virtual bool setLevelFlagIndex(qint64 index, bool flag = true);
					virtual bool getLevelFlagIndex(qint64 index);

					virtual qint64 getFirstFlagTag();
					virtual qint64 getLastFlagTag();

					virtual bool isEmpty();

					virtual CModelDataArray *intersectWith(CModelDataArray *modelData, bool *flagModification = 0);
					virtual CModelDataArray *unionWith(CModelDataArray *modelData, bool *flagModification = 0);
					virtual CModelDataArray *intersectWith(CModelDataArray *modelData, CDataArrayMemoryManager *modelMemMan, bool *flagModification = 0);
					virtual CModelDataArray *unionWith(CModelDataArray *modelData, CDataArrayMemoryManager *modelMemMan, bool *flagModification = 0);

					virtual CModelDataArray *copyFrom(CModelDataArray *modelData, bool *flagModification = 0);
					virtual CModelDataArray *copyFrom(CModelDataArray *modelData, CDataArrayMemoryManager *modelMemMan, bool *flagModification = 0);

					virtual qint64 countFlags();

					virtual bool hasIntersectionWith(CModelDataArray *modelData);
					virtual qint64 countIntersectionWith(CModelDataArray *modelData);
					virtual bool isSubsetFrom(CModelDataArray *modelData);
					virtual bool isSuperSetTo(CModelDataArray *modelData);

					virtual bool needsExtension(qint64 minBegTag, qint64 minEndTag);
					virtual bool needsExtension(qint64 tag);
					virtual bool needsExtension(CModelDataArray *otherModelData);

					virtual bool isFullConstructed();

				// protected methods
				protected:

				// protected variables
				protected:
					static const qint64 dataSize = 64;
					static const qint64 log2DataSize = 6;
					qint64 dataArray;

					qint64 begTag;
					qint64 endTag;
					qint64 tagSize;

					qint64 bitIndMask;
					qint64 bitIndShift;


				// private methods
				private:

				// private variables
				private:

			};


		}; // end namespace Container

	}; // end namespace Utilities

}; // end namespace Konclude

#endif // KONCLUDE_UTILITIES_CONTAINER_CMODELDATAARRAY_H
