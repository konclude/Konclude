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

#include "COWL2FunctionalStyleParserTokenManager.h"


namespace Konclude {

	namespace Parser {

		namespace FunctionalJAVACC {

			COWL2FunctionalStyleParserTokenManager::COWL2FunctionalStyleParserTokenManager(JAVACC_CHARSTREAM *stream) : OWL2FunctionalStyleParserTokenManager(stream) {
				mLexicalError = false;
			}


			void COWL2FunctionalStyleParserTokenManager::lexicalError() {
				mLexicalError = true;
			}

			bool COWL2FunctionalStyleParserTokenManager::hasLexicalError() {
				return mLexicalError;
			}

		}; // end namespace FunctionalJAVACC

	}; // end namespace Parser

}; // end namespace Konclude
