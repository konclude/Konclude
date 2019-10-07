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

#include "COntologyContextSingleMemoryManagerSingletonProvider.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {

			namespace Memory {

				COntologyContextSingleMemoryManagerSingletonProvider *COntologyContextSingleMemoryManagerSingletonProvider::instance = 0;

				COntologyContextSingleMemoryManagerSingletonProvider::COntologyContextSingleMemoryManagerSingletonProvider() {
				}


				COntologyContextSingleMemoryManagerSingletonProvider::~COntologyContextSingleMemoryManagerSingletonProvider() {
				}


				COntologyContextMemoryManager *COntologyContextSingleMemoryManagerSingletonProvider::getOntologyMemoryManager() {
					return new COntologyContextSingleMemoryManager();
				}


				COntologyContextSingleMemoryManagerSingletonProvider *COntologyContextSingleMemoryManagerSingletonProvider::getInstance() {
					if (instance == 0) {
						// not thread safe, but not really problematic if concurrent threads create more as one instances  
						instance = new COntologyContextSingleMemoryManagerSingletonProvider();
					}
					return instance;
				}


			}; // end namespace Memory

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude
