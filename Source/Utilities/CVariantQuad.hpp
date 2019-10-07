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

#ifndef KONCLUDE_UTILITIES_CVARIANTQUADBASE_H
#define KONCLUDE_UTILITIES_CVARIANTQUADBASE_H

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
		 *		\class		CVariantQuadBase
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		template<class BASE, class T1, class T2, class T3, class T4>
		class CVariantQuadBase : public QPair<QPair<T1,T2>, QPair<T3, T4>> {

			// public Functions
			public:
				//! Constructor
				CVariantQuadBase(const T1& first, const T2& second, const T3& third, const T4& fourth);

				const T1& getFirst() const;
				const T2& getSecond() const;
				const T3& getThird() const;
				const T4& getFourth() const;

				BASE* setFirst(const T1& first);
				BASE* setSecond(const T2& second);
				BASE* setThird(const T3& third);
				BASE* setFourth(const T4& fourth);

				BASE* set(const T1& first, const T2& second, const T3& third, const T4& fourth);

			// private Functions
			private:

			// private Variables
			private:
		
		};


		template<class BASE, class T1, class T2, class T3, class T4>
		CVariantQuadBase<BASE,T1,T2,T3,T4>::CVariantQuadBase(const T1 &first, const T2 &second, const T3 &third, const T4& fourth) {
			this->first.first = first;
			this->first.second = second;
			this->second.first = third;
			this->second.second = fourth;
		}

		template<class BASE, class T1, class T2, class T3, class T4>
		BASE* CVariantQuadBase<BASE,T1,T2,T3,T4>::setFirst(const T1 &first) {
			this->first.first = first;
			return (BASE*)this;
		}

		template<class BASE, class T1, class T2, class T3, class T4>
		BASE* CVariantQuadBase<BASE,T1,T2,T3,T4>::setSecond(const T2 &second) {
			this->first.second = second;
			return (BASE*)this;
		}

		template<class BASE, class T1, class T2, class T3, class T4>
		BASE* CVariantQuadBase<BASE,T1,T2,T3,T4>::setThird(const T3 &third) {
			this->second.first = third;
			return (BASE*)this;
		}

		template<class BASE, class T1, class T2, class T3, class T4>
		BASE* CVariantQuadBase<BASE,T1,T2,T3,T4>::setFourth(const T4 &fourth) {
			this->second.second = fourth;
			return (BASE*)this;
		}

		template<class BASE, class T1, class T2, class T3, class T4>
		BASE* CVariantQuadBase<BASE,T1,T2,T3,T4>::set(const T1 &first, const T2 &second, const T3 &third, const T4& fourth) {
			this->first.first = first;
			this->first.second = second;
			this->second.first = third;
			this->second.second = fourth;
			return (BASE*)this;
		}


		template<class BASE, class T1, class T2, class T3, class T4>
		const T1& CVariantQuadBase<BASE,T1,T2,T3,T4>::getFirst() const {
			return this->first.first;
		}

		template<class BASE, class T1, class T2, class T3, class T4>
		const T2& CVariantQuadBase<BASE,T1,T2,T3,T4>::getSecond() const {
			return this->first.second;
		}

		template<class BASE, class T1, class T2, class T3, class T4>
		const T3& CVariantQuadBase<BASE,T1,T2,T3,T4>::getThird() const {
			return this->second.first;
		}

		template<class BASE, class T1, class T2, class T3, class T4>
		const T4& CVariantQuadBase<BASE,T1,T2,T3,T4>::getFourth() const {
			return this->second.second;
		}


		template<class T1, class T2, class T3, class T4>
		class CVariantQuad : public CVariantQuadBase<CVariantQuad<T1,T2,T3,T4>,T1,T2,T3,T4> {
			public:
                CVariantQuad(const T1 &first, const T2 &second, const T3 &third, const T4& fourth) : CVariantQuadBase<CVariantQuad<T1,T2,T3,T4>,T1,T2,T3,T4>(first,second,third, fourth) {
				}
		};


	}; // end namespace Utilities

}; // end namespace Konclude

#endif // KONCLUDE_UTILITIES_CVARIANTQUADBASE_H
