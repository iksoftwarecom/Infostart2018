
/////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Examples for the report "Making external components for 1C mobile platform for Android""
// at the conference INFOSTART 2018 EVENT EDUCATION https://event.infostart.ru/2018/
//
// Sample 1: Delay in code
// Sample 2: Getting device information
// Sample 3: Device blocking: receiving external event about changing of sceen
//
// Copyright: Igor Kisil 2018
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "../include/AddInDefBase.h"
#include "../include/IAndroidComponentHelper.h"
#include "../jni/jnienv.h"
#include "../include/IMemoryManager.h"

/* Wrapper calling class LockState from java build ru.infostart.education */

class LockState
{
private:

	jclass cc;
	jobject obj;

public:

	LockState();
	~LockState();

	void Initialize(IAddInDefBaseEx*);

	void Start() const; // Start monitoring lock state
	void Stop() const; // End of monitoring
};