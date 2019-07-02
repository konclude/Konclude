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

#include "COntologyStringMapping.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			COntologyStringMapping::COntologyStringMapping(COntologyContext* ontologyContext) {
				mOntoContext = ontologyContext;
				mClassNameConceptMapHash = nullptr;
				mPropertyNameRoleMapHash = nullptr;
				mIndividualNameIndividualMapHash = nullptr;
				mAbbreviatedNamePrefixMapHash = nullptr;
				mNamePrefixMapHash = nullptr;
				mPrefixList = nullptr;
			}


			COntologyStringMapping::~COntologyStringMapping() {
				CMemoryAllocationManager* memMan = CContext::getMemoryAllocationManager(mOntoContext);
				COADestroyAndRelease(mClassNameConceptMapHash,memMan);
				COADestroyAndRelease(mPropertyNameRoleMapHash,memMan);
				COADestroyAndRelease(mIndividualNameIndividualMapHash,memMan);
				COADestroyAndRelease(mAbbreviatedNamePrefixMapHash,memMan); 
				COADestroyAndRelease(mNamePrefixMapHash,memMan);
				COADestroyAndRelease(mPrefixList,memMan);
			}


			COntologyStringMapping* COntologyStringMapping::referenceStringMapping(COntologyStringMapping* stringMapping) {
				if (stringMapping->mClassNameConceptMapHash) {
					(*getClassNameConceptMappingHash(true)) = *stringMapping->mClassNameConceptMapHash;
				} else {
					if (mClassNameConceptMapHash) {
						mClassNameConceptMapHash->clear();
					}
				}
				if (stringMapping->mPropertyNameRoleMapHash) {
					(*getPropertyNameRoleMappingHash(true)) = *stringMapping->mPropertyNameRoleMapHash;
				} else {
					if (mPropertyNameRoleMapHash) {
						mPropertyNameRoleMapHash->clear();
					}
				}
				if (stringMapping->mIndividualNameIndividualMapHash) {
					(*getIndividualNameIndividualMappingHash(true)) = *stringMapping->mIndividualNameIndividualMapHash;
				} else {
					if (mIndividualNameIndividualMapHash) {
						mIndividualNameIndividualMapHash->clear();
					}
				}
				if (stringMapping->mAbbreviatedNamePrefixMapHash) {
					(*getAbbreviatedNamePrefixHash(true)) = *stringMapping->mAbbreviatedNamePrefixMapHash;
				} else {
					if (mAbbreviatedNamePrefixMapHash) {
						mAbbreviatedNamePrefixMapHash->clear();
					}
				}
				if (stringMapping->mNamePrefixMapHash) {
					(*getNamePrefixHash(true)) = *stringMapping->mNamePrefixMapHash;
				} else {
					if (mNamePrefixMapHash) {
						mNamePrefixMapHash->clear();
					}
				}
				if (stringMapping->mPrefixList) {
					(*getPrefixList(true)) = *stringMapping->mPrefixList;
				} else {
					if (mPrefixList) {
						mPrefixList->clear();
					}
				}
				return this;
			}



			CMAPPINGHASH<CStringRefStringHasher,CConcept*>* COntologyStringMapping::getClassNameConceptMappingHash(bool create) {
				if (!mClassNameConceptMapHash && create) {
					mClassNameConceptMapHash = CObjectParameterizingAllocator< CMAPPINGHASH<CStringRefStringHasher,CConcept*>,CContext* >::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mOntoContext),mOntoContext);
				}
				return mClassNameConceptMapHash;
			}

			CMAPPINGHASH<CStringRefStringHasher,CRole*>* COntologyStringMapping::getPropertyNameRoleMappingHash(bool create) {
				if (!mPropertyNameRoleMapHash && create) {
					mPropertyNameRoleMapHash = CObjectParameterizingAllocator< CMAPPINGHASH<CStringRefStringHasher,CRole*>,CContext* >::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mOntoContext),mOntoContext);
				}
				return mPropertyNameRoleMapHash;
			}

			CMAPPINGHASH<CStringRefStringHasher,CIndividual*>* COntologyStringMapping::getIndividualNameIndividualMappingHash(bool create) {
				if (!mIndividualNameIndividualMapHash && create) {
					mIndividualNameIndividualMapHash = CObjectParameterizingAllocator< CMAPPINGHASH<CStringRefStringHasher,CIndividual*>,CContext* >::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mOntoContext),mOntoContext);
				}
				return mIndividualNameIndividualMapHash;
			}


			CMAPPINGHASH<QString,CNamePrefix*>* COntologyStringMapping::getAbbreviatedNamePrefixHash(bool create) {
				if (!mAbbreviatedNamePrefixMapHash && create) {
					mAbbreviatedNamePrefixMapHash = CObjectParameterizingAllocator< CMAPPINGHASH<QString,CNamePrefix*>,CContext* >::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mOntoContext),mOntoContext);
				}
				return mAbbreviatedNamePrefixMapHash;
			}

			CMAPPINGHASH<QString,CNamePrefix*>* COntologyStringMapping::getNamePrefixHash(bool create) {
				if (!mNamePrefixMapHash && create) {
					mNamePrefixMapHash = CObjectParameterizingAllocator< CMAPPINGHASH<QString,CNamePrefix*>,CContext* >::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mOntoContext),mOntoContext);
				}
				return mNamePrefixMapHash;
			}

			CMAPPINGLIST<CNamePrefix*>* COntologyStringMapping::getPrefixList(bool create) {
				if (!mPrefixList && create) {
					mPrefixList = CObjectParameterizingAllocator< CMAPPINGLIST<CNamePrefix*>,CContext* >::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mOntoContext),mOntoContext);
				}
				return mPrefixList;
			}


			CConcept* COntologyStringMapping::getConceptFromName(const QString& fullClassName) {
				CConcept* concept = nullptr;
				if (mClassNameConceptMapHash) {
					concept = mClassNameConceptMapHash->value(fullClassName,nullptr);
				}
				return concept;
			}

			CRole* COntologyStringMapping::getRoleFromName(const QString& fullPropertyName) {
				CRole* role = nullptr;
				if (mPropertyNameRoleMapHash) {
					role = mPropertyNameRoleMapHash->value(fullPropertyName,nullptr);
				}
				return role;
			}

			CIndividual* COntologyStringMapping::getIndividualFromName(const QString& fullIndividualName) {
				CIndividual* individual = nullptr;
				if (mIndividualNameIndividualMapHash) {
					individual = mIndividualNameIndividualMapHash->value(fullIndividualName,nullptr);
				}
				return individual;
			}



		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude
