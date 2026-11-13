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

package com.konclude.jnibridge;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

/**
 * Receives a flat set of entities, the counterpart of SetOfObjectSetCallbackListener for the
 * queries that do not group their results into sets of equivalent entities.
 *
 * The native side looks up the methods startObjectSet, visitObject and endObjectSet as well
 * as the field mNativeDataPointer and the parameterless constructor, see
 * CJNIHandlingClassData, so none of them must be renamed. The class is required by
 * CJNIHandler even if the smoke test does not use it.
 */
public class ObjectSetCallbackListener {

	/** looked up by the native side, currently not used by it */
	private long mNativeDataPointer = 0;

	private final List<Object> mObjects = new ArrayList<Object>();

	/** the native side instantiates this class, so the parameterless constructor is required */
	public ObjectSetCallbackListener() {
	}

	public void startObjectSet() {
	}

	public void visitObject(Object object) {
		mObjects.add(object);
	}

	public void endObjectSet() {
	}

	public List<Object> getObjects() {
		return Collections.unmodifiableList(mObjects);
	}
}
