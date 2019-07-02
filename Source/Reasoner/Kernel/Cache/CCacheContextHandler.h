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

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_CCACHEHANDLER_H
#define KONCLUDE_REASONER_KERNEL_CACHE_CCACHEHANDLER_H

// Qt includes



// Namespace includes
#include "CSatisfiableCacheReader.h"
#include "CUnsatisfiableCacheReader.h"
#include "CSatisfiableCacheWriter.h"
#include "CUnsatisfiableCacheWriter.h"
#include "CIncrementalUnsatisfiableCacheReader.h"
#include "CIncrementalSatisfiableCacheReader.h"


// Other includes



// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {

				/*! 
				 *
				 *		\class		CCacheHandler
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CCacheHandler : public CSatisfiableCacheReader, public CUnsatisfiableCacheReader, public CSatisfiableCacheWriter, public CUnsatisfiableCacheWriter {
					// public methods
					public:
						//! Constructor
						CCacheHandler(CSatisfiableCacheReader *satCacheReader, CUnsatisfiableCacheReader *unsatCacheReader, CSatisfiableCacheWriter *satCacheWriter, CUnsatisfiableCacheWriter *unsatCacheWriter);

						//! Deconstructor
						virtual ~CCacheHandler();

						virtual bool isSatisfiable(QVector<CCacheValue> &itemVec, qint64 count);
						virtual bool getSatisfiableOutcome(QVector<CCacheValue> &itemVec, qint64 count, QList<CCacheValue> *outcomeList = 0);

						virtual bool isUnsatisfiable(QVector<CCacheValue> &itemVec, qint64 count);
						virtual QList<CCacheValue> getUnsatisfiableItems(QVector<CCacheValue> &itemVec, qint64 count);

						virtual void setSatisfiable(QVector<CCacheValue> &itemVec, qint64 itemCount, QVector<CCacheValue> &outcomeVec, qint64 outCount);
						virtual void setUnsatisfiable(QVector<CCacheValue> &itemVec, qint64 count, QVector<CCacheValue> &clashVec, qint64 clashCount);

						virtual bool isSatisfiable(CConceptSet *conceptSet);
						virtual bool getSatisfiableOutcome(CConceptSet *conceptSet, QList<CCacheValue> *outcomeList = 0);
						virtual bool isUnsatisfiable(CConceptSet *conceptSet);
						virtual bool isUnsatisfiable(QList<CConceptDescriptorLinker *> &conDesNegLinkerList);
						virtual QList<CCacheValue> getUnsatisfiableItems(CConceptSet *conceptSet);
						virtual void setSatisfiable(CConceptSet *conceptSet);
						virtual void setUnsatisfiable(CConceptSet *conceptSet, CLinker<CConceptDescriptorLinker *> *clashLinker);
						virtual void setUnsatisfiable(CConceptSet *conceptSet, QList<CConceptDescriptorLinker *> &clashList);
						virtual void setUnsatisfiable(CConceptDescriptorLinker *conDesNegLinker, CLinker<CConceptDescriptorLinker *> *clashLinker);
						virtual void setUnsatisfiable(QList<CConceptDescriptorLinker *> &conDesNegLinkerList, CLinker<CConceptDescriptorLinker *> *clashLinker);
						virtual void setUnsatisfiable(QList<CConceptDescriptorLinker *> *conDesNegLinkerList, QList<CConceptDescriptorLinker *> *clashConceptsList);


						virtual bool isSatisfiableIndividualPreferIncrementalTesting(CIndividualProcessBase *indi);
						virtual bool isSatisfiableIndividual(CIndividualProcessBase *indi);


						virtual bool isUnsatisfiableIndividualPreferIncrementalTesting(CIndividualProcessBase *indi, qint64 *maximalIndividualAncestorTestCount = 0);
						virtual QList<CConceptDescriptorLinker *> getIndividualUnsatisfiableConceptDescriptionLinkers(CIndividualProcessBase *indi, qint64 maximalIndividualAncestorTestCount = -1, bool restartTesting = false);

						virtual bool isUnsatisfiableIndividualPreferIncrementalTesting(CIndividualProcessBase *indi, CConcept *additionalTestingConcept, bool additionalConceptNegation, qint64 *maximalIndividualAncestorTestCount = 0);
						virtual QList<CConceptDescriptorLinker *> getIndividualUnsatisfiableConceptDescriptionLinkers(CIndividualProcessBase *indi, CConceptDescriptorLinker *addConDesLinker, qint64 maximalIndividualAncestorTestCount = -1, bool restartTesting = false);

						virtual bool isUnsatisfiableIndividualPreferIncrementalTesting(CIndividualProcessBase *indi, QList<QPair<CConcept *,bool> > *sortedAddConceptsList, qint64 *maximalIndividualAncestorTestCount = 0);
						virtual QList<CConceptDescriptorLinker *> getIndividualUnsatisfiableConceptDescriptionLinkers(CIndividualProcessBase *indi, QList<CConceptDescriptorLinker *> *sortedAddConceptsList, qint64 maximalIndividualAncestorTestCount = -1, bool restartTesting = false);

						virtual bool isIndividualMergingUnsatisfiable(CIndividualProcessBase *indi1, CIndividualProcessBase *indi2);
						virtual QList<CConceptDescriptorLinker *> getIndividualMergingUnsatisfiableItems(CIndividualProcessBase *indi1, CIndividualProcessBase *indi2, bool restartTesting = false);


					// protected methods
					protected:

					// protected variables
					protected:
						CSatisfiableCacheReader *satReader;
						CUnsatisfiableCacheReader *unsatReader;
						CSatisfiableCacheWriter *satWriter;
						CUnsatisfiableCacheWriter *unsatWriter;

						CIncrementalUnsatisfiableCacheReader *incUnsatReader;
						CIncrementalSatisfiableCacheReader *incSatReader;

						QList<CConceptDescriptorLinker *> lastMergingPrimClashList;
						bool lastMergingPrimClash;


					// private methods
					private:

					// private variables
					private:


				};

			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_CACHE_CCACHEHANDLER_H
