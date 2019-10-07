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

#ifndef KONCLUDE_UTILITIES_CONTAINER_CONTAINERSETTINGS
#define KONCLUDE_UTILITIES_CONTAINER_CONTAINERSETTINGS

// Library includes


// Namespace includes


// Other includes
#include "Utilities/UtilitiesSettings.h"

// Logger includes


namespace Konclude {

	namespace Utilities {

		namespace Container {


			/*! 
			 *
			 *		\file		ContainerSettings
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */


			// forward declarations

			class CModelDataArray;
			class CModelDataLevelArray;



			// Konclude code configs

#define KONCLUDE_CONTAINER_VECTOR_USE_QT_VECTOR_FOR_DEFAULT
#define KONCLUDE_CONTAINER_LIST_USE_QT_LIST_FOR_DEFAULT
#define KONCLUDE_CONTAINER_HASH_USE_QT_HASH_FOR_DEFAULT
#define KONCLUDE_CONTAINER_SET_USE_QT_SET_FOR_DEFAULT


#ifdef KONCLUDE_CONTAINER_VECTOR_USE_QT_VECTOR_FOR_DEFAULT
#define KONCLUDE_EMPLOY_QT_VECTOR_CONTAINER
#endif // KONCLUDE_CONTAINER_VECTOR_USE_QT_VECTOR_FOR_DEFAULT

#ifdef KONCLUDE_CONTAINER_LIST_USE_QT_LIST_FOR_DEFAULT
#define KONCLUDE_EMPLOY_QT_LIST_CONTAINER
#endif // KONCLUDE_CONTAINER_LIST_USE_QT_LIST_FOR_DEFAULT

#ifdef KONCLUDE_CONTAINER_SET_USE_QT_SET_FOR_DEFAULT
#define KONCLUDE_EMPLOY_QT_SET_CONTAINER
#endif // KONCLUDE_CONTAINER_SET_USE_QT_SET_FOR_DEFAULT

#ifdef KONCLUDE_CONTAINER_HASH_USE_QT_HASH_FOR_DEFAULT
#define KONCLUDE_EMPLOY_QT_HASH_CONTAINER
#endif // KONCLUDE_CONTAINER_HASH_USE_QT_HASH_FOR_DEFAULT


#define KONCLUDE_EMPLOY_QT_MANAGED_RESTRICTED_MODIFICATION_HASH_CONTAINER
#define KONCLUDE_EMPLOY_QT_MANAGED_RESTRICTED_MODIFICATION_SET_CONTAINER
#define KONCLUDE_EMPLOY_QT_MANAGED_RESTRICTED_MODIFICATION_LIST_CONTAINER
#define KONCLUDE_EMPLOY_QT_MANAGED_RESTRICTED_MODIFICATION_MAP_CONTAINER

// foreach macro definition analog to the foreach macro of qt
class CForeachContainerIt {};

template<class T>
class CForeachContainerItTemplate : public CForeachContainerIt {
	public:
		CForeachContainerItTemplate(const T& container) 
				: it(container.begin()),itEnd(container.end()),breakCnt(0) {
		}
		mutable class T::const_iterator it, itEnd;
		mutable int breakCnt;
		inline bool condition() const {
			return !breakCnt++ && it != itEnd;
		}
};

template<class T>
inline CForeachContainerItTemplate<T> createContainerItTemplate(const T& container) {
	return CForeachContainerItTemplate<T>(container);
}

template<class T>
inline const CForeachContainerItTemplate<T>* getContainerItTemplate(const CForeachContainerIt* conItTemplate, const T& convertT) {
	return static_cast<const CForeachContainerItTemplate<T>*>(conItTemplate);
}

#define FOREACHIT(variable, container) foreach(variable, container)

/*
#define FOREACHIT(variable, container) \
	for (const CForeachContainerIt& conItTemplate(createContainerItTemplate(container)); \
			getContainerItTemplate(&conItTemplate,container)->condition(); \
			++(getContainerItTemplate(&conItTemplate,container)->it)) \
		for (variable = *((getContainerItTemplate(&conItTemplate,container)->it)); \
			getContainerItTemplate(&conItTemplate,container)->breakCnt; \
			--getContainerItTemplate(&conItTemplate,container)->breakCnt) 
*/

#define FOREACHHASHIT(keyVariable, valueVariable, container) \
	for (const CForeachContainerIt& conItTemplate(createContainerItTemplate(container)); \
			getContainerItTemplate(&conItTemplate,container)->condition(); \
			++(getContainerItTemplate(&conItTemplate,container)->it)) \
		for (keyVariable = ((getContainerItTemplate(&conItTemplate,container)->it)).key(); \
				getContainerItTemplate(&conItTemplate,container)->breakCnt == 1; \
				++getContainerItTemplate(&conItTemplate,container)->breakCnt) \
			for (valueVariable = ((getContainerItTemplate(&conItTemplate,container)->it)).value(); \
					getContainerItTemplate(&conItTemplate,container)->breakCnt == 1; \
					getContainerItTemplate(&conItTemplate,container)->breakCnt-=2) 


#define cForeach FOREACHIT
#define cDeleteAll qDeleteAll

		}; // end namespace Container

	}; // end namespace Utilities

}; // end namespace Konclude


#endif // end KONCLUDE_UTILITIES_CONTAINER_CONTAINERSETTINGS
