/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#include "COntologyExpressivity.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			COntologyExpressivity::COntologyExpressivity() {
			}

			COntologyExpressivity::COntologyExpressivity(const QString& string) {
				initExpressivity(string);
			}

			COntologyExpressivity* COntologyExpressivity::initExpressivity(const QString& string) {
				mExpressivityString = string;
				mExpressivityFlags = 0;
				if (mExpressivityString.indexOf("AL") != -1) {
					mExpressivityFlags |= EXPRESSIVITY_AL;

					if (mExpressivityString.indexOf("E") != -1) {
						mExpressivityFlags |= EXPRESSIVITY_E | EXPRESSIVITY_EL;
					}
					if (mExpressivityString.indexOf("C") != -1) {
						mExpressivityFlags |= EXPRESSIVITY_C | EXPRESSIVITY_EL | EXPRESSIVITY_E;
						if (mExpressivityString.indexOf("+") != -1) {
							mExpressivityFlags |= EXPRESSIVITY_S;
							if (mExpressivityString.indexOf("R") != -1) {
								mExpressivityFlags |= EXPRESSIVITY_EPLUSPLUS;
							}
						} 
					}
				}
				if (mExpressivityString.indexOf("S") != -1) {
					mExpressivityFlags |= EXPRESSIVITY_AL | EXPRESSIVITY_EL | EXPRESSIVITY_E | EXPRESSIVITY_C | EXPRESSIVITY_S | EXPRESSIVITY_TRANS;
					if (mExpressivityString.indexOf("R") != -1) {
						mExpressivityFlags |= EXPRESSIVITY_EPLUSPLUS;
					} 
				} 
				if (mExpressivityString.indexOf("EL") != -1) {
					mExpressivityFlags |= EXPRESSIVITY_EL;
					if (mExpressivityString.indexOf("++") != -1) {
						mExpressivityFlags |= EXPRESSIVITY_EPLUSPLUS;
					} 
				} 
				if (mExpressivityString.indexOf("+") != -1) {
					mExpressivityFlags |= EXPRESSIVITY_TRANS;
				} 
				if (mExpressivityString.indexOf("D") != -1) {
					mExpressivityFlags |= EXPRESSIVITY_D;
				} 

				if (mExpressivityString.indexOf("H") != -1) {
					mExpressivityFlags |= EXPRESSIVITY_H;
				} 
				if (mExpressivityString.indexOf("R") != -1) {
					mExpressivityFlags |= EXPRESSIVITY_R | EXPRESSIVITY_H | EXPRESSIVITY_TRANS;
				} 

				if (mExpressivityString.indexOf("F") != -1) {
					mExpressivityFlags |= EXPRESSIVITY_F;
				} 
				if (mExpressivityString.indexOf("N") != -1) {
					mExpressivityFlags |= EXPRESSIVITY_F | EXPRESSIVITY_N;
				} 
				if (mExpressivityString.indexOf("Q") != -1) {
					mExpressivityFlags |= EXPRESSIVITY_F | EXPRESSIVITY_N | EXPRESSIVITY_Q;
				}

				if (mExpressivityString.indexOf("I") != -1) {
					mExpressivityFlags |= EXPRESSIVITY_I;
				}

				if (mExpressivityString.indexOf("O") != -1) {
					mExpressivityFlags |= EXPRESSIVITY_O;
				}
	
				return this;
			}


			bool COntologyExpressivity::isExpressivityEntailed(COntologyExpressivity* expressivity) {
				return (expressivity->mExpressivityFlags & mExpressivityFlags) == expressivity->mExpressivityFlags;
			}

			bool COntologyExpressivity::isExpressivityEqual(COntologyExpressivity* expressivity) {
				return expressivity->mExpressivityFlags == mExpressivityFlags;
			}

			QString COntologyExpressivity::getExpressivityString() {
				return mExpressivityString;
			}


		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude
