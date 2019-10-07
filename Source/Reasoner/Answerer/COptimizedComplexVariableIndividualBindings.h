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

#ifndef KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLEINDIVIDUALBINDINGS_H
#define KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLEINDIVIDUALBINDINGS_H

// Libraries includes


// Namespace includes
#include "AnswererSettings.h"



// Other includes
#include "Reasoner/Realization/CRealizationIndividualInstanceItemReference.h"

#include "Utilities/UtilitiesSettings.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities;

	namespace Reasoner {

		using namespace Realization;

		namespace Answerer {


			class CIndividualInstanceItemDataBinding {
			public:

				enum BindingType {
					NONE, DATA_POINTER, REALIZATION_ITEM
				};

				CIndividualInstanceItemDataBinding() {
					bindingType = NONE;
					reference = CRealizationIndividualInstanceItemReference();
					pointer = nullptr;
				}


				CIndividualInstanceItemDataBinding(const CRealizationIndividualInstanceItemReference& indiItemRef) {
					bindingType = REALIZATION_ITEM;
					reference = indiItemRef;
				}

				CIndividualInstanceItemDataBinding(CDataLiteral* dataLiteral) {
					bindingType = DATA_POINTER;
					pointer = dataLiteral;
				}

				cint64 getHashValue() const {
					if (bindingType == REALIZATION_ITEM) {
						return reference.getIndividualID();
					} else {
						return (cint64)pointer;
					}
				}


				bool operator==(const CIndividualInstanceItemDataBinding& bindings) const {
					if (bindingType == bindings.bindingType) {
						return reference == bindings.reference;
					} else {
						return pointer == bindings.pointer;
					}
				}

				bool operator!=(const CIndividualInstanceItemDataBinding& bindings) const {
					return !operator==(bindings);
				}

				BindingType bindingType;

				union {
					CRealizationIndividualInstanceItemReference reference;
					void* pointer;
				};
			};

			typedef CIndividualInstanceItemDataBinding TIndividualInstanceItemDataBinding;


			/*! 
			 *
			 *		\class		COptimizedComplexVariableIndividualBindings
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedComplexVariableIndividualBindings {
				// public methods
				public:
					//! Constructor
					COptimizedComplexVariableIndividualBindings();
					COptimizedComplexVariableIndividualBindings(cint64 bindingSize, TIndividualInstanceItemDataBinding* bindingArray);
					COptimizedComplexVariableIndividualBindings(cint64 bindingSize);
					COptimizedComplexVariableIndividualBindings(const COptimizedComplexVariableIndividualBindings& varBindings);
					~COptimizedComplexVariableIndividualBindings();

					COptimizedComplexVariableIndividualBindings* initIndividualBindings(cint64 bindingSize);
					COptimizedComplexVariableIndividualBindings* initIndividualBindings(cint64 bindingSize, TIndividualInstanceItemDataBinding* bindingArray);

					
					TIndividualInstanceItemDataBinding* getBindingArray() const;
					TIndividualInstanceItemDataBinding& getBinding(cint64 idx) const;
					COptimizedComplexVariableIndividualBindings* setBinding(cint64 idx, const TIndividualInstanceItemDataBinding& instItem);


					cint64 getBindingSize() const;


					cint64 getHashValue() const;

					bool operator==(const COptimizedComplexVariableIndividualBindings& bindings) const;



				// protected methods
				protected:

				// protected variables
				protected:
					TIndividualInstanceItemDataBinding* mBindingArray;
					cint64 mBindingSize;




				// private methods
				private:

				// private variables
				private:

			};


			inline uint qHash(const COptimizedComplexVariableIndividualBindings& binding) {
				qint64 key = (qint64)binding.getHashValue();
				if (sizeof(quint64) > sizeof(uint)) {
					return uint((key >> (8 * sizeof(uint) - 1)) ^ key);
				} else {
					return uint(key);
				}
			}


		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLEINDIVIDUALBINDINGS_H
