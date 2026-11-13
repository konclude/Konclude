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
 * Receives a set of sets of entities, which is how the hierarchy queries report their
 * results, one object set per set of equivalent entities.
 *
 * The native side looks up the methods startObjectSet, visitObject and endObjectSet as well
 * as the field mNativeDataPointer and the parameterless constructor, see
 * CJNIHandlingClassData, so none of them must be renamed.
 *
 * Note that a reported set can be empty, for instance for the bottom node if no object has
 * been associated with owl:Nothing while the ontology was built.
 */
public class SetOfObjectSetCallbackListener {

	/** looked up by the native side, currently not used by it */
	private long mNativeDataPointer = 0;

	private final List<List<Object>> mObjectSets = new ArrayList<List<Object>>();
	private List<Object> mCurrentObjectSet = null;

	/** the native side instantiates this class, so the parameterless constructor is required */
	public SetOfObjectSetCallbackListener() {
	}

	public void startObjectSet() {
		mCurrentObjectSet = new ArrayList<Object>();
	}

	public void visitObject(Object object) {
		if (mCurrentObjectSet == null) {
			// be robust in case the native side reports an object without starting a set
			startObjectSet();
		}
		mCurrentObjectSet.add(object);
	}

	public void endObjectSet() {
		if (mCurrentObjectSet != null) {
			mObjectSets.add(mCurrentObjectSet);
			mCurrentObjectSet = null;
		}
	}

	/** the reported sets, in the order in which they were reported */
	public List<List<Object>> getObjectSets() {
		return Collections.unmodifiableList(mObjectSets);
	}

	/** all reported objects, without the grouping into sets of equivalent entities */
	public List<Object> getObjects() {
		List<Object> objects = new ArrayList<Object>();
		for (List<Object> objectSet : mObjectSets) {
			objects.addAll(objectSet);
		}
		return objects;
	}
}
