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

#include "CVariableBindingPathMergingHash.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CVariableBindingPathMergingHash::CVariableBindingPathMergingHash(CProcessContext* processContext) : mProcessContext(processContext) {
				}


				CVariableBindingPathMergingHash* CVariableBindingPathMergingHash::initVariableBindingPathMergingHash(CVariableBindingPathMergingHash* prevHash) {
					if (prevHash) {
						CPROCESSHASH<TPathIDPair, CVariableBindingPathMergingHashData>::operator=(*prevHash);
					} else {
						CPROCESSHASH<TPathIDPair,CVariableBindingPathMergingHashData>::clear();
					}
					return this;
				}


				CVariableBindingPathMergingHashData* CVariableBindingPathMergingHash::getMergedVariableBindingPathData(CVariableBindingPath* varBindPath1, CVariableBindingPath* varBindPath2) {
					TPathIDPair pathIDPair(qMin(varBindPath1->getPropagationID(),varBindPath2->getPropagationID()),qMax(varBindPath1->getPropagationID(),varBindPath2->getPropagationID()));
					CVariableBindingPathMergingHashData& data = CPROCESSHASH<TPathIDPair,CVariableBindingPathMergingHashData>::operator[](pathIDPair);
					return &data;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
