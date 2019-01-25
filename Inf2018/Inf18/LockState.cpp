
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

#include <wchar.h>
#include "LockState.h"
#include "ConversionWchar.h"

LockState::LockState() : cc(nullptr), obj(nullptr)
{
}

LockState::~LockState()
{
	if (obj)
	{
		Stop(); // call to unregister broadcast receiver
		JNIEnv *env = getJniEnv();
		env->DeleteGlobalRef(obj);
		env->DeleteGlobalRef(cc);
	}
}

void LockState::Initialize(IAddInDefBaseEx* cnn)
{
	if (!obj)
	{
		IAndroidComponentHelper* helper = (IAndroidComponentHelper*)cnn->GetInterface(eIAndroidComponentHelper);
		if (helper)
		{
			WCHAR_T* className = nullptr;
			convToShortWchar(&className, L"ru.infostart.education.LockState");
			jclass ccloc = helper->FindClass(className);
			delete[] className;
			className = nullptr;
			if (ccloc)
			{
				JNIEnv* env = getJniEnv();
				cc = static_cast<jclass>(env->NewGlobalRef(ccloc));
				env->DeleteLocalRef(ccloc);
				jobject activity = helper->GetActivity();
				// call of constructor for java class
				jmethodID methID = env->GetMethodID(cc, "<init>", "(Landroid/app/Activity;J)V");
				jobject objloc = env->NewObject(cc, methID, activity, (jlong)cnn);
				obj = static_cast<jobject>(env->NewGlobalRef(objloc));
				env->DeleteLocalRef(objloc);
				methID = env->GetMethodID(cc, "show", "()V");
				env->CallVoidMethod(obj, methID);
				env->DeleteLocalRef(activity);
			}
		}
	}
}

void LockState::Start() const
{
	if (obj)
	{
		JNIEnv* env = getJniEnv();
		jmethodID methID = env->GetMethodID(cc, "start", "()V");
		env->CallVoidMethod(obj, methID);
	}
}

void LockState::Stop() const
{
	if (obj)
	{
		JNIEnv* env = getJniEnv();
		jmethodID methID = env->GetMethodID(cc, "stop", "()V");
		env->CallVoidMethod(obj, methID);
	}
}

static const wchar_t g_EventSource[] = L"ru.infostart.education";
static const wchar_t g_EventName[] = L"LockChanged";
static WcharWrapper s_EventSource(g_EventSource);
static WcharWrapper s_EventName(g_EventName);

// name of function built according to Java native call
//
extern "C" JNIEXPORT void JNICALL Java_ru_infostart_education_LockState_OnLockChanged(JNIEnv* env, jclass jClass, jlong pObject)
{
	IAddInDefBaseEx* pAddIn = (IAddInDefBaseEx*)pObject;
	pAddIn->ExternalEvent(s_EventSource, s_EventName, nullptr);
}