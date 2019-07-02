/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU General Public License
 *		(LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU General Public License for more details.
 *
 *		You should have received a copy of the GNU General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef KONCLUDE_UTILITIES_CDYNREFVECTOR_H
#define KONCLUDE_UTILITIES_CDYNREFVECTOR_H

// Libraries includes
#include <QVector>
#include <QPair>

// Namespace includes
#include "CDynRefBucket.hpp"
#include "CAllocationObject.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;


namespace Konclude {

	namespace Utilities {


		/*! 
		 *
		 *		\class		CDynRefVector
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		template<class T>
		class CDynRefVector : public CAllocationObject {
			// public methods
			public:
				//! Constructor
				CDynRefVector();

				//! Destructor
				virtual ~CDynRefVector();

				void reference(CDynRefVector<T> *vec);
				void clear();

				bool isExtendingReference();

				bool isLocal(qint64 index);
				bool isReference(qint64 index);


				T *getData(qint64 index);
				T *getLocalData(qint64 index);
				T *getReferenceData(qint64 index);

				void setLocalData(qint64 index, T *data);
				void setData(qint64 index, T *data);

				qint64 getItemCount();


			// protected methods
			protected:

			// private methods
			private:

			// private variables
			private:
				QVector<CDynRefBucket<T> *> *refDataVec;
				CDynRefVector<T> *refVec;
				
				qint64 bucketSize;
				qint64 bucketCount;
				qint64 itemCount;

				qint64 extendSize;
		};


		template<class T>
		CDynRefVector<T>::CDynRefVector() {
			refVec = 0;
			refDataVec = 0;
			bucketSize = 32;
			bucketCount = 0;
			itemCount = 0;
			extendSize = 8;
		}

		template<class T>
		CDynRefVector<T>::~CDynRefVector() {
			clear();
		}


		template<class T>
		qint64 CDynRefVector<T>::getItemCount() {
			return itemCount;
		}


		template<class T>
		void CDynRefVector<T>::setData(qint64 index, T *data) {
			// writing is always local
			setLocalData(index,data);
		}

		template<class T>
		void CDynRefVector<T>::setLocalData(qint64 index, T *data) {
			itemCount = qMax(index+1,itemCount);
			if (!refDataVec) {
				qint64 initialSize = bucketCount;
				if (initialSize < extendSize) {
					initialSize = extendSize;
				}
				refDataVec = new QVector<CDynRefBucket<T> *>(bucketCount);
				for (qint64 i = 0; i < bucketCount; ++i) {
					(*refDataVec)[i] = new CDynRefBucket<T>(bucketSize);
					if (refVec) {
						(*refDataVec)[i]->reference((*refVec->refDataVec)[i]);
					}
				}
			}

			qint64 bucketIndex = index / bucketSize;
			if (bucketIndex >= bucketCount) {
				if (bucketIndex >= refDataVec->size()) {
					qint64 newVecSize = (bucketIndex / extendSize + 1) * extendSize;
					refDataVec->resize(newVecSize);
				}
				bucketCount = bucketIndex+1;
			}

			if (refDataVec) {
				if (!(*refDataVec)[bucketIndex]) {
					(*refDataVec)[bucketIndex] = new CDynRefBucket<T>;
				}

				qint64 bucketItemIndex = index % bucketSize;
				(*refDataVec)[bucketIndex]->setLocalData(bucketItemIndex,data);
			}

		}


		template<class T>
		T *CDynRefVector<T>::getData(qint64 index) {

			T *val = 0;
			CDynRefBucket<T> *bucket = 0;

			qint64 bucketIndex = index / bucketSize;

			if (!refDataVec) {
				if (refVec) {
					if (bucketIndex < bucketCount) {
						bucket = (*refVec->refDataVec)[bucketIndex];
					}
				}
			} else {
				if (bucketIndex < bucketCount) {
					bucket = (*refDataVec)[bucketIndex];
				}
			}

			if (bucket) {
				qint64 bucketItemIndex = index % bucketSize;
				val = bucket->getData(bucketItemIndex);
			}
			return val;

		}


		template<class T>
		T *CDynRefVector<T>::getLocalData(qint64 index){
			T *val = 0;
			CDynRefBucket<T> *bucket = 0;

			qint64 bucketIndex = index / bucketSize;

			if (refDataVec) {
				if (bucketIndex < bucketCount) {
					bucket = (*refDataVec)[bucketIndex];
				}
			}

			if (bucket) {
				qint64 bucketItemIndex = index % bucketSize;
				val = bucket->getLocalData(bucketItemIndex);
			}
			return val;
		}



		template<class T>
		bool CDynRefVector<T>::isLocal(qint64 index) {
			CDynRefBucket<T> *bucket = 0;

			qint64 bucketIndex = index / bucketSize;

			if (refDataVec) {
				if (bucketIndex < bucketCount) {
					bucket = (*refDataVec)[bucketIndex];
				}
			}

			if (bucket) {
				qint64 bucketItemIndex = index % bucketSize;
				return bucket->isLocal(bucketItemIndex);
			}
			return false;
		}



		template<class T>
		bool CDynRefVector<T>::isReference(qint64 index) {
			CDynRefBucket<T> *bucket = 0;

			qint64 bucketIndex = index / bucketSize;
			bool referenced = false;

			if (!refDataVec) {
				if (refVec) {
					if (bucketIndex < bucketCount) {
						bucket = (*refVec->refDataVec)[bucketIndex];
						referenced = true;
					}
				}
			} else {
				if (bucketIndex < bucketCount) {
					bucket = (*refDataVec)[bucketIndex];
				}
			}

			if (bucket) {
				qint64 bucketItemIndex = index % bucketSize;
				if (referenced) {
					return bucket->isLocal(bucketItemIndex);
				} 
				return bucket->isReference(bucketItemIndex);				
			}
			return false;
		}



		template<class T>
		T *CDynRefVector<T>::getReferenceData(qint64 index){
			T *val = 0;
			CDynRefBucket<T> *bucket = 0;

			qint64 bucketIndex = index / bucketSize;
			bool referenced = false;

			if (!refDataVec) {
				if (refVec) {
					if (bucketIndex < bucketCount) {
						bucket = (*refVec->refDataVec)[bucketIndex];
						referenced = true;
					}
				}
			} else {
				if (bucketIndex < bucketCount) {
					bucket = (*refDataVec)[bucketIndex];
				}
			}

			if (bucket) {
				qint64 bucketItemIndex = index % bucketSize;
				if (referenced) {
					val = bucket->getData(bucketItemIndex);
				} else {
					val = bucket->getReferenceData(bucketItemIndex);
				}
			}
			return val;
		}



		template<class T>
		void CDynRefVector<T>::reference(CDynRefVector<T> *vec) {
			clear();


			CDynRefVector<T> *refVector = vec;
			// search the first vector which has in fact data
			while (refVector != 0 && refVector->refDataVec == 0) {
				refVector = refVector->refVec;
			}
			refVec = refVector;

			if (refVec) {
				itemCount = refVec->itemCount;
				bucketCount = refVec->bucketCount;
				bucketSize = refVec->bucketSize;
			}
		}


		template<class T>
		void CDynRefVector<T>::clear() {
			if (refDataVec) {
				for (qint64 i = 0; i < bucketCount; ++i) {
					if ((*refDataVec)[i] != 0) {
						delete (*refDataVec)[i];
					}
				}
				delete refDataVec;
				refDataVec = 0;
			}
			bucketCount = 0;
			itemCount = 0;
		}


		template<class T>
		bool CDynRefVector<T>::isExtendingReference() {
			if (refDataVec) {
				return true;
			}
			return false;
		}




	}; // end namespace Utilities

}; // end namespace Konclude

#endif // KONCLUDE_UTILITIES_CDYNREFVECTOR_H
