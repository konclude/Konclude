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

#ifndef KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLEJOININGITEM_H
#define KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLEJOININGITEM_H

// Libraries includes
#include <QVector>

// Namespace includes
#include "AnswererSettings.h"
#include "COptimizedComplexVariableCompositionItem.h"
#include "COptimizedComplexConceptItem.h"
#include "COptimizedComplexVariableJoiningBindingPositionMapping.h"
#include "COptimizedComplexVariableJoiningHash.h"
#include "COptimizedComplexVariableJoiningHashMemoryManaged.h"
#include "COptimizedComplexVariableCompositionItemDependence.h"



// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {


	namespace Reasoner {

		namespace Answerer {


			/*! 
			 *
			 *		\class		COptimizedComplexVariableJoiningItem
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedComplexVariableJoiningItem : public COptimizedComplexVariableCompositionItem {
				// public methods
				public:
					//! Constructor
					COptimizedComplexVariableJoiningItem(COptimizedComplexVariableCompositionItem* itemLeft, COptimizedComplexVariableCompositionItem* itemRight, const COptimizedComplexVariableJoiningBindingPositionMapping& positionMapping);

					virtual ~COptimizedComplexVariableJoiningItem();

					virtual COMPOSITION_TYPE getCompositionType();

					
					COptimizedComplexVariableCompositionItem* getLeftItem();
					COptimizedComplexVariableCompositionItem* getRightItem();

					COptimizedComplexVariableCompositionItemDependence* getLeftItemDependence();
					COptimizedComplexVariableCompositionItemDependence* getRightItemDependence();

					COptimizedComplexVariableJoiningBindingPositionMapping* getPositionMapping();


					COptimizedComplexVariableJoiningHash*& getJoiningHash();
					QVector<COptimizedComplexVariableJoiningHashMemoryManaged*>& getJoiningHashVector();


					cint64& getLeftSampleKeyCount();
					cint64& getRightSampleKeyCount();

					cint64& getLeftSampleInsertionCount();
					cint64& getRightSampleInsertionCount();


					bool isSamplingCompleted();
					COptimizedComplexVariableJoiningItem* setSamplingCompleted(bool completed);

					bool isInsertionSideDecided();
					bool isInsertionSideLeft();

					COptimizedComplexVariableJoiningItem* setInsertionSideDecided(bool decided);
					COptimizedComplexVariableJoiningItem* setInsertionSideLeft(bool left);

					//QHash<QString, cint64> debugVarBindStringCardHash;

				// protected methods
				protected:

				// protected variables
				protected:
					COptimizedComplexVariableCompositionItemDependence mLeftItemDep;
					COptimizedComplexVariableCompositionItemDependence mRightItemDep;

					COptimizedComplexVariableCompositionItem* mItemLeft;
					COptimizedComplexVariableCompositionItem* mItemRight;
					COptimizedComplexVariableJoiningBindingPositionMapping mPositionMapping;


					COptimizedComplexVariableJoiningHash* mJoiningHash;
					QVector<COptimizedComplexVariableJoiningHashMemoryManaged*> mJoiningHashVector;


					cint64 mLeftSampleKeyCount;
					cint64 mRightSampleKeyCount;

					cint64 mLeftSampleInsertionCount;
					cint64 mRightSampleInsertionCount;

					bool mSamplingCompleted;

					bool mInsertionSideDecided;
					bool mInsertionSideLeft;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLEJOININGITEM_H
