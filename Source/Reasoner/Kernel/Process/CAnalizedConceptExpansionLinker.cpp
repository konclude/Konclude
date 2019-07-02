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

#include "CAnalizedConceptExpansionLinker.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CAnalizedConceptExpansionLinker::CAnalizedConceptExpansionLinker() : CLinkerBase<CAnalizedConceptExpansionLinker*,CAnalizedConceptExpansionLinker>(this) {
					mDependendConDesLinker = nullptr;
					mConDes = nullptr;
				}


				CAnalizedConceptExpansionLinker* CAnalizedConceptExpansionLinker::initAnalizedConceptExpansion(CXLinker<CConceptDescriptor*>* dependendConDesLinker, CConceptDescriptor* conDes) {
					mDependendConDesLinker = dependendConDesLinker;
					mConDes = conDes;
					return this;
				}


				CAnalizedConceptExpansionLinker* CAnalizedConceptExpansionLinker::addDependendConceptDescriptorLinker(CXLinker<CConceptDescriptor*>* dependendConDesLinker) {
					if (mDependendConDesLinker) {
						mDependendConDesLinker = dependendConDesLinker->append(mDependendConDesLinker);
					} else {
						mDependendConDesLinker = dependendConDesLinker;
					}
					return this;
				}


				CAnalizedConceptExpansionLinker* CAnalizedConceptExpansionLinker::setConceptDescriptor(CConceptDescriptor* conDes) {
					mConDes = conDes;
					return this;
				}

				CConceptDescriptor* CAnalizedConceptExpansionLinker::getConceptDescriptor() {
					return mConDes;
				}

				CXLinker<CConceptDescriptor*>* CAnalizedConceptExpansionLinker::getDependendConceptDescriptorLinker() {
					return mDependendConDesLinker;
				}

				bool CAnalizedConceptExpansionLinker::hasMultipleDependencies() {
					return mDependendConDesLinker && mDependendConDesLinker->hasNext();
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
