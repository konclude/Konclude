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

#include "CConnectionSuccessorCorrectionHash.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CConnectionSuccessorCorrectionHash::CConnectionSuccessorCorrectionHash(CProcessContext* context) : CPROCESSHASH<cint64,cint64>(context),mContext(context) {
				}

				CConnectionSuccessorCorrectionHash::~CConnectionSuccessorCorrectionHash() {
				}

				CConnectionSuccessorCorrectionHash* CConnectionSuccessorCorrectionHash::initConnectionSuccessorCorrectionHash(CConnectionSuccessorCorrectionHash* connSuccCorrHash) {
					if (connSuccCorrHash) {
						*this = *connSuccCorrHash;
					} else {
						CPROCESSHASH<cint64,cint64>::clear();
					}
					return this;
				}

				CConnectionSuccessorCorrectionHash* CConnectionSuccessorCorrectionHash::insertConnectionSuccessor(cint64 indiID) {
					CPROCESSHASH<cint64,cint64>::insert(indiID,indiID);
					return this;
				}

				CConnectionSuccessorCorrectionHash* CConnectionSuccessorCorrectionHash::correctSuccessorConnection(cint64 indiID, cint64 correctedID) {
					CPROCESSHASH<cint64,cint64>::insert(indiID,correctedID);
					return this;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
