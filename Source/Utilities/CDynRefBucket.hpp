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

#ifndef KONCLUDE_UTILITIES_CDYNREFBUCKET_H
#define KONCLUDE_UTILITIES_CDYNREFBUCKET_H

// Libraries includes
#include <QSet>


// Namespace includes


// Other includes


// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;


namespace Konclude {

	namespace Utilities {


		/*! 
		 *
		 *		\class		CDynRefBucket
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		template<class T>
		class CDynRefBucket {
			// public methods
			public:
				//! Constructor
				CDynRefBucket(qint64 setBucketSize = 32, qint64 setItemSize = 1);

				//! Destructor
				virtual ~CDynRefBucket();

				bool isLocal(qint64 index);
				bool isReference(qint64 index);

				void reference(CDynRefBucket<T> *ref);

				void clear();

				T *getData(qint64 index);
				T *getLocalData(qint64 index);
				T *getReferenceData(qint64 index);

				void setLocalData(qint64 index, T *data);
				void setData(qint64 index, T *data);

			// protected methods
			protected:

			// private methods
			private:

			// private variables
			private:
				CDynRefBucket<T> *refBucket;

				QPair<bool, T *> *refDataVec;
				qint64 itemSize;
				qint64 bucketSize;

		};


		template<class T>
		CDynRefBucket<T>::CDynRefBucket(qint64 setBucketSize, qint64 setItemSize) {
			refBucket = 0;
			refDataVec = 0;
			itemSize = setItemSize;
			bucketSize = setBucketSize;
		}


		template<class T>
		CDynRefBucket<T>::~CDynRefBucket() {
			clear();
		}


		template<class T>
		void CDynRefBucket<T>::clear() {
			refBucket = 0;
			if (refDataVec) {
				delete refDataVec;
				refDataVec = 0;
			}
		}


		template<class T>
		void CDynRefBucket<T>::setData(qint64 index, T *data) {
			// writing is allways local
			setLocalData(index,data);
		}


		template<class T>
		void CDynRefBucket<T>::setLocalData(qint64 index, T *data) {
			if (!refDataVec) {
				refDataVec = new QPair<bool, T *>[bucketSize];

				if (refBucket) {
					// init whole vector as referenced
					for (qint64 i = 0; i < bucketSize; ++i) {
						refDataVec[i].first = false;
						refDataVec[i].second = refBucket->refDataVec[i].second;
					}
				} else {
					// init whole vector as not referenced
					for (qint64 i = 0; i < bucketSize; ++i) {
						refDataVec[i].first = true;
						refDataVec[i].second = 0;
					}
				}
			}

			qint64 itemIndex = index / itemSize;
			if (itemIndex < bucketSize) {
				refDataVec[itemIndex].first = true;
				refDataVec[itemIndex].second = data;
			}
		}


		template<class T>
		T *CDynRefBucket<T>::getData(qint64 index) {
			T *val = 0;

			QPair<bool, T *> *lookupRefDataVec = refDataVec;

			if (!lookupRefDataVec) {
				// there is no local data
				if (refBucket) {
					// use the referenced data, if there is one
					lookupRefDataVec = refBucket->refDataVec;
				}				
			} 			
			if (lookupRefDataVec) {
				qint64 itemIndex = index / itemSize;
				if (itemIndex < bucketSize) {
					// index must be in vector size
					QPair<bool, T *> *refDataPair = lookupRefDataVec + itemIndex;
					// data is local available or is referenced, in both case it has to be returned
					val = refDataPair->second;
				}
			}
			return val;
		}


		template<class T>
		T *CDynRefBucket<T>::getLocalData(qint64 index) {
			T *val = 0;

			QPair<bool, T *> *lookupRefDataVec = refDataVec;
		
			if (lookupRefDataVec) {
				qint64 itemIndex = index / itemSize;
				if (itemIndex < bucketSize) {
					// index must be in vector size
					QPair<bool, T *> *refDataPair = lookupRefDataVec + itemIndex;
					if (refDataPair->first == true) {
						// data is local available and has to be returned
						val = refDataPair->second;
					}
				}
			}
			return val;		
		}


		template<class T>
		T *CDynRefBucket<T>::getReferenceData(qint64 index) {
			T *val = 0;

			QPair<bool, T *> *lookupRefDataVec = refDataVec;
			bool referenced = false;

			if (!lookupRefDataVec) {
				// there is no local data
				if (refBucket) {
					// use the referenced data, if there is one
					lookupRefDataVec = refBucket->refDataVec;
					referenced = true;
				}				
			}	
			if (lookupRefDataVec) {
				qint64 itemIndex = index / itemSize;
				if (itemIndex < bucketSize) {
					// index must be in vector size
					QPair<bool, T *> *refDataPair = lookupRefDataVec + itemIndex;
					if (refDataPair->first == false || referenced) {
						// data is referenced available and has to be returned
						val = refDataPair->second;
					}
				}
			}
			return val;	
		}



		template<class T>
		bool CDynRefBucket<T>::isLocal(qint64 index) {

			QPair<bool, T *> *lookupRefDataVec = refDataVec;
		
			if (lookupRefDataVec) {
				qint64 itemIndex = index / itemSize;
				if (itemIndex < bucketSize) {
					// index must be in vector size
					QPair<bool, T *> *refDataPair = lookupRefDataVec + itemIndex;
					if (refDataPair->first == true) {
						// test whether data is local available
						return refDataPair->second != 0;
					}
				}
			}
			return false;		
		}


		template<class T>
		bool CDynRefBucket<T>::isReference(qint64 index) {
			QPair<bool, T *> *lookupRefDataVec = refDataVec;
			bool referenced = false;

			if (!lookupRefDataVec) {
				// there is no local data
				if (refBucket) {
					// use the referenced data, if there is one
					lookupRefDataVec = refBucket->refDataVec;
					referenced = true;
				}				
			}	
			if (lookupRefDataVec) {
				qint64 itemIndex = index / itemSize;
				if (itemIndex < bucketSize) {
					// index must be in vector size
					QPair<bool, T *> *refDataPair = lookupRefDataVec + itemIndex;
					if (refDataPair->first == false || referenced) {
						// test whether data is referenced available
						return refDataPair->second != 0;
					}
				}
			}
			return false;	
		}

		template<class T>
		void CDynRefBucket<T>::reference(CDynRefBucket<T> *ref) {
			clear();
			refBucket = ref;
			while (refBucket != 0 && refBucket->refDataVec == 0) {
				refBucket = refBucket->refBucket;
			}

			if (refBucket) {
				itemSize = refBucket->itemSize;
				bucketSize = refBucket->bucketSize;
			}
		}



	}; // end namespace Utilities

}; // end namespace Konclude

#endif // KONCLUDE_UTILITIES_CDYNREFBUCKET_H
