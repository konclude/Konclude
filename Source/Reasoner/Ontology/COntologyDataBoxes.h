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

#ifndef KONCLUDE_REASONER_ONTOLOGY_CONTOLOGYDATABOXES_H
#define KONCLUDE_REASONER_ONTOLOGY_CONTOLOGYDATABOXES_H

// Libraries includes


// Namespace includes
#include "OntologySettings.h"
#include "CExpressionDataBoxMapping.h"
#include "COntologyContext.h"
#include "CTBox.h"
#include "CABox.h"
#include "CRBox.h"
#include "CMBox.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {

			/*! 
			 *
			 *		\class		COntologyDataBoxes
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COntologyDataBoxes {
				// public methods
				public:
					//! Constructor
					COntologyDataBoxes(CBoxContext* boxContext = nullptr);

					//! Destructor
					virtual ~COntologyDataBoxes();

					COntologyDataBoxes* referenceDataBoxes(COntologyDataBoxes* dataBoxes);

					CConcept *getTopConcept();
					CConcept *getBottomConcept();

					CExpressionDataBoxMapping* getExpressionDataBoxMapping();
					COntologyDataBoxes* setExpressionDataBoxMapping(CExpressionDataBoxMapping* expDatBoxMapping);

					CTBox* getTBox();
					CRBox* getRBox();
					CABox* getABox();
					CMBox* getMBox();

					CBoxContext* getBoxContext();

					bool isBasicBuild();
					bool isInitialBuild();
					bool isIterationBuild();

					COntologyDataBoxes* setBasicBuild(bool basicBuild);
					COntologyDataBoxes* setInitialBuild(bool initialBuild);
					COntologyDataBoxes* setIterationBuild(bool iterationBuild);

					COntologyDataBoxes* setDataBoxesContinuation(COntologyDataBoxes* dataBoxes);


				// protected methods
				protected:

				// protected variables
				protected:
					CBoxContext* mBoxContext;

					bool mBasicBuild;
					bool mInitialBuild;
					bool mIterationBuild;

					CTBox* mTBox;
					CABox* mABox;
					CRBox* mRBox;
					CMBox* mMBox;
					CExpressionDataBoxMapping* mExpDataBoxMapping;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ONTOLOGY_CONTOLOGYDATABOXES_H
