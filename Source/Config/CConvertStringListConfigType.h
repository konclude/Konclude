/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public
 *		License (LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU (Lesser) General Public License for more details.
 *
 *		You should have received a copy of the GNU (Lesser) General Public
 *		License along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef KONCLUDE_CONFIG_CCONVERTSTRINGLISTCONFIGTYPE_H
#define KONCLUDE_CONFIG_CCONVERTSTRINGLISTCONFIGTYPE_H

// Libraries includes
#include <QString>
#include <QVariant>

// Namespace includes
#include "CConfigType.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Config {

		/*! 
		 *
		 *		\class		CConvertStringListConfigType
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CConvertStringListConfigType : virtual public CConfigType {
			// public methods
			public:
				//! Constructor
				CConvertStringListConfigType();

				//! Destructor
				virtual ~CConvertStringListConfigType();

				
				virtual bool supportsStringListConvertion();

				virtual bool readFromStringList(const QStringList &stringList) = 0;
				virtual QStringList getStringList() = 0;

			// protected methods
			protected:

			// protected variables
			protected:
				
			// private methods
			private:

			// private variables
			private:

		};

	}; // end namespace Config

}; // end namespace Konclude

#endif // KONCLUDE_CONFIG_CCONVERTSTRINGLISTCONFIGTYPE_H
