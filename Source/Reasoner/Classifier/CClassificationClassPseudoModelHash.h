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

#ifndef KONCLUDE_REASONER_CLASSIFIER_CCLASSIFICATIONCLASSPSEUDOMODELHASH_H
#define KONCLUDE_REASONER_CLASSIFIER_CCLASSIFICATIONCLASSPSEUDOMODELHASH_H

// Libraries includes


// Namespace includes
#include "ClassifierSettings.h"
#include "CClassificationClassPseudoModelData.h"

// Other includes
#include "Utilities/Memory/CMemoryPool.h"
#include "Utilities/Memory/CObjectParameterizingAllocator.h"
#include "Utilities/Container/CLinker.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities;
	using namespace Utilities::Memory;
	using namespace Utilities::Container;

	namespace Reasoner {

		namespace Classifier {


			/*! 
			 *
			 *		\class		CClassificationClassPseudoModelHash
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CClassificationClassPseudoModelHash {
				// public methods
				public:
					//! Constructor
					CClassificationClassPseudoModelHash(CContext* context);

					CClassificationClassPseudoModelData* getPseudoModelData(cint64 node, bool create);
					cint64 getCount();

					CClassificationClassPseudoModelHash* initPseudoModelHash(CClassificationClassPseudoModelHash* hash);

				// protected methods
				protected:

				// protected variables
				protected:
					class CClassificationClassPseudoModelHashData {
						public:
							CClassificationClassPseudoModelHashData() {
								mUseData =  nullptr;
								mLocData =  nullptr;
							}
							CClassificationClassPseudoModelHashData(const CClassificationClassPseudoModelHashData& data) {
								mUseData =  data.mUseData;
								mLocData =  nullptr;
							}
							CClassificationClassPseudoModelData* mUseData;
							CClassificationClassPseudoModelData* mLocData;
					};

					CContext* mContext;
					CCLASSPSEUDOMODELHASH<cint64,CClassificationClassPseudoModelHashData> mPMDataHash;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CLASSIFIER_CCLASSIFICATIONCLASSPSEUDOMODELHASH_H
