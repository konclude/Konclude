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

#include "CActiveEntitySubExpressionVisitorUpdater.h"


namespace Konclude {

	namespace Reasoner {

		namespace Generator {

			
			CActiveEntitySubExpressionVisitorUpdater::CActiveEntitySubExpressionVisitorUpdater(CActiveEntityCountVector* countVector) {
				mCountVector = countVector;
			}





			bool CActiveEntitySubExpressionVisitorUpdater::updateEntitiesForAxioms(CBuildExpression* expression, bool added) {
				mVisitingAxiomAdded = added;
				return expression->visitSubExpressions(this);
			}




			bool CActiveEntitySubExpressionVisitorUpdater::visitSubExpression(CBuildExpression* expression, CBuildExpression* subExpression, CSubExpressionVisitor* visitor) {
				if (mCountVector) {
					subExpression->visitSubExpressions(this);
				}
				return true;
			}



			bool CActiveEntitySubExpressionVisitorUpdater::visitEntity(CBuildExpression* expression, CExpressionEntity* entity, CSubExpressionVisitor* visitor) {
				if (mCountVector) {
					CExpressionEntity* entityExp = entity;
					if (entityExp) {
						cint64 itemID = entityExp->getEntityID();
						if (mVisitingAxiomAdded) {
							// increment entity occurrence
							mCountVector->incActiveEntityCount(entityExp);
						} else {
							// decrement entity occurrence
							mCountVector->decActiveEntityCount(entityExp);
						}
					}
				}
				return true;
			}



		}; // end namespace Generator

	}; // end namespace Reasoner

}; // end namespace Konclude