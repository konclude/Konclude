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

#ifndef KONCLUDE_UTILITIES_CVARIANTTRIBLEBASE_H
#define KONCLUDE_UTILITIES_CVARIANTTRIBLEBASE_H

// Libraries includes
#include <QPair>

// Namespace includes


// Other includes


// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;


namespace Konclude {

	namespace Utilities {

		/*!
		 *
		 *		\class		CVariantTribleBase
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		template<class BASE, class T1, class T2, class T3>
		class CVariantTribleBase : public QPair<QPair<T1,T2>,T3> {

			// public Functions
			public:
				//! Constructor
				CVariantTribleBase(const T1& first, const T2& second, const T3& third);

				const T1& getFirst() const;
				const T2& getSecond() const;
				const T3& getThird() const;

				BASE* setFirst(const T1& first);
				BASE* setSecond(const T2& second);
				BASE* setThird(const T3& third);

				BASE* set(const T1& first, const T2& second, const T3& third);

			// private Functions
			private:

			// private Variables
			private:
		
		};


		template<class BASE, class T1, class T2, class T3>
		CVariantTribleBase<BASE,T1,T2,T3>::CVariantTribleBase(const T1 &first, const T2 &second, const T3 &third) {
			this->first.first = first;
			this->first.second = second;
			this->second = third;
		}

		template<class BASE, class T1, class T2, class T3>
		BASE* CVariantTribleBase<BASE,T1,T2,T3>::setFirst(const T1 &first) {
			this->first.first = first;
			return (BASE*)this;
		}

		template<class BASE, class T1, class T2, class T3>
		BASE* CVariantTribleBase<BASE,T1,T2,T3>::setSecond(const T2 &second) {
			this->first.second = second;
			return (BASE*)this;
		}

		template<class BASE, class T1, class T2, class T3>
		BASE* CVariantTribleBase<BASE,T1,T2,T3>::setThird(const T3 &third) {
			this->second = third;
			return (BASE*)this;
		}

		template<class BASE, class T1, class T2, class T3>
		BASE* CVariantTribleBase<BASE,T1,T2,T3>::set(const T1 &first, const T2 &second, const T3 &third) {
			this->first.first = first;
			this->first.second = second;
			this->second = third;
			return (BASE*)this;
		}


		template<class BASE, class T1, class T2, class T3>
		const T1& CVariantTribleBase<BASE,T1,T2,T3>::getFirst() const {
			return this->first.first;
		}

		template<class BASE, class T1, class T2, class T3>
		const T2& CVariantTribleBase<BASE,T1,T2,T3>::getSecond() const {
			return this->first.second;
		}

		template<class BASE, class T1, class T2, class T3>
		const T3& CVariantTribleBase<BASE,T1,T2,T3>::getThird() const {
			return this->second;
		}



		template<class T1, class T2, class T3>
		class CVariantTrible : public CVariantTribleBase<CVariantTrible<T1,T2,T3>,T1,T2,T3> {
			public:
                CVariantTrible(const T1 &first, const T2 &second, const T3 &third) : CVariantTribleBase<CVariantTrible<T1,T2,T3>,T1,T2,T3>(first,second,third) {
				}
		};


	}; // end namespace Utilities

}; // end namespace Konclude

#endif // KONCLUDE_UTILITIES_CVARIANTTRIBLEBASE_H
