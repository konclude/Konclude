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

package com.konclude.owlapi;

import org.semanticweb.owlapi.model.OWLObject;

/**
 * Thrown by KoncludeOWLAPITranslator for a construct that the JNI bridge of Konclude has no
 * entry point for.
 *
 * The exception carries the construct that could not be translated, so that a caller can
 * report it. KoncludeOWLAPITranslator catches it per axiom, which is why an unsupported
 * expression makes the whole axiom unsupported.
 */
public class KoncludeUnsupportedConstructException extends RuntimeException {

	private static final long serialVersionUID = 1L;

	private final OWLObject mConstruct;

	public KoncludeUnsupportedConstructException(OWLObject construct, String reason) {
		super(construct.getClass().getSimpleName() + " is not supported by the JNI bridge of "
				+ "Konclude, " + reason + ": " + construct);
		mConstruct = construct;
	}

	/** the construct that could not be translated */
	public OWLObject getConstruct() {
		return mConstruct;
	}
}
