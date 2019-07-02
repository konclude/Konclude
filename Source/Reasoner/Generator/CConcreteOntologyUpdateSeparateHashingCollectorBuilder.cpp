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

#include "CConcreteOntologyUpdateSeparateHashingCollectorBuilder.h"


namespace Konclude {

	namespace Reasoner {

		namespace Generator {

			CConcreteOntologyUpdateSeparateHashingCollectorBuilder::CConcreteOntologyUpdateSeparateHashingCollectorBuilder(CConcreteOntology* updateConcreteOntology) : CConcreteOntologyUpdateCollectorBuilder(updateConcreteOntology) {
				LOG(NOTICE,"::Konclude::Reasoner::Generator::ConcretOntologySeparateHashingUpdateBuilder",logTr("ConcretOntologySeparateHashingUpdateBuilder initialized."),this);
			}

			CConcreteOntologyUpdateSeparateHashingCollectorBuilder::~CConcreteOntologyUpdateSeparateHashingCollectorBuilder() {
			}




			CConcreteOntologyBuildDataUpdater* CConcreteOntologyUpdateSeparateHashingCollectorBuilder::addHashedExpression(CBuildExpression* expression) {
				mSeparateExpressionBuildHash.insert(CExpressionHasher(expression), expression);
				return this;
			}


			CBuildExpression* CConcreteOntologyUpdateSeparateHashingCollectorBuilder::getHashedExpression(CBuildExpression* expression) {
				CBuildExpression* hashExpression = nullptr;
				CExpressionHasher expHasher(expression);
				hashExpression = mExpressionBuildHash->value(expHasher, nullptr);
				if (!hashExpression) {
					hashExpression = mSeparateExpressionBuildHash.value(expHasher, nullptr);
				}
				return hashExpression;
			}


		}; // end namespace Generator

	}; // end namespace Reasoner

}; // end namespace Konclude