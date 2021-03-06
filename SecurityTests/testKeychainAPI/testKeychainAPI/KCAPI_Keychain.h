// ======================================================================
//	File:		KCAPI_Keychain.h
//
//	Operation classes for core KC APIs:
//		- KCMakeKCRefFromFSRef
//		- KCMakeKCRefFromFSSpec
//		- KCMakeKCRefFromAlias
//		- KCMakeAliasFromKCRef
//		- KCReleaseKeychain
//		- KCUnlockNoUI
//		- KCUnlock
//		- KCLogin
//		- KCChangeLoginPassword
//		- KCLogout
//		- KCUnlockWithInfo
//		- KCLock
//		- KCLockNoUI
//		- KCGetDefaultKeychain
//		- KCSetDefaultKeychain
//		- KCCreateKeychain
//		- KCCreateKeychainNoUI
//		- KCGetStatus
//		- KCChangeSettingsNoUI
//		- KCGetKeychain
//		- KCGetKeychainName
//		- KCChangeSettings
//		- KCCountKeychains
//		- KCGetIndKeychain
//		- KCAddCallback
//		- KCRemoveCallback
//		- KCSetInteractionAllowed
//		- KCIsInteractionAllowed
//
//	Copyright:	Copyright (c) 2000,2003 Apple Computer, Inc. All Rights Reserved.
//
//	Change History (most recent first):
//
//		 <1>	2/25/00	em		Created.
// ======================================================================
#ifndef __KCAPI_KEYCHAIN__
#define __KCAPI_KEYCHAIN__
#include "KCOperation.h"
#include "KCOperationID.h"
#include "KCParamUtility.h"

// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
// 	₯ COp_KCMakeKCRefFromFSRef
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
class COp_KCMakeKCRefFromFSRef : public KCOperation
{
public:
OPERATION_ID(KCMakeKCRefFromFSRef)
								COp_KCMakeKCRefFromFSRef();
	virtual	OSStatus			Operate();

protected:
	CParamFSRef					mFSRef;
private:
	OSStatus					KCMakeKCRefFromFSRef(
									FSRef *inKeychainFSRef,
									KCRef *outKeychain)
								{	
									*outKeychain = (KCRef)NULL;
									return noErr;
								}	
};
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
// 	₯ COp_KCMakeKCRefFromFSSpec
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
class COp_KCMakeKCRefFromFSSpec : public KCOperation
{
public:
OPERATION_ID(KCMakeKCRefFromFSSpec)
								COp_KCMakeKCRefFromFSSpec();						
	virtual	OSStatus			Operate();
protected:
	CParamFSSpec				mKeychainFile;
};
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
// 	₯ COp_KCMakeKCRefFromAlias
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
class COp_KCMakeKCRefFromAlias : public KCOperation
{
public:
OPERATION_ID(KCMakeKCRefFromAlias)
								COp_KCMakeKCRefFromAlias();						
	virtual	OSStatus			Operate();
protected:
};
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
// 	₯ COp_KCMakeAliasFromKCRef
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
class COp_KCMakeAliasFromKCRef : public KCOperation
{
public:
OPERATION_ID(KCMakeAliasFromKCRef)
								
								COp_KCMakeAliasFromKCRef();
	virtual	OSStatus			Operate();
protected:
};
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
// 	₯ COp_KCReleaseKeychain
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
class COp_KCReleaseKeychain : public KCOperation
{
public:
OPERATION_ID(KCReleaseKeychain)
								
								COp_KCReleaseKeychain();
	virtual	OSStatus			Operate();
protected:
};
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
// 	₯ COp_KCUnlockNoUI
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
class COp_KCUnlockNoUI : public KCOperation
{
public:
OPERATION_ID(KCUnlockNoUI)
								
								COp_KCUnlockNoUI();
	virtual	OSStatus			Operate();
    virtual StringPtr			GetPassword(){ return (StringPtr)mPassword; }
protected:
    CParamStringPtr				mPassword;
};
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
// 	₯ COp_KCUnlock
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
class COp_KCUnlock : public KCOperation
{
public:
OPERATION_ID(KCUnlock)
								
								COp_KCUnlock();
	virtual	OSStatus			Operate();
protected:
	CParamStringPtr				mPassword;
};	
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
// 	₯ COp_KCChangeLoginPassword
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
class COp_KCChangeLoginPassword : public KCOperation
{
public:
OPERATION_ID(KCChangeLoginPassword)
								
								COp_KCChangeLoginPassword();
	virtual	OSStatus			Operate();
protected:
	CParamStringPtr				mOldPassword;
	CParamStringPtr				mNewPassword;
};	
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
// 	₯ COp_KCLogin
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
class COp_KCLogin : public KCOperation
{
public:
OPERATION_ID(KCLogin)
								
								COp_KCLogin();
	virtual	OSStatus			Operate();
protected:
	CParamStringPtr				mName;
	CParamStringPtr				mPassword;
};	
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
// 	₯ COp_KCLogout
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
class COp_KCLogout : public KCOperation
{
public:
OPERATION_ID(KCLogout)
								
								COp_KCLogout();
	virtual	OSStatus			Operate();
protected:
};	
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
// 	₯ COp_KCUnlockWithInfo
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
class COp_KCUnlockWithInfo : public KCOperation
{
public:
OPERATION_ID(KCUnlockWithInfo)
								COp_KCUnlockWithInfo();
	virtual	OSStatus			Operate();
protected:
	CParamStringPtr				mPassword;
	CParamStringPtr				mMessage;
};										
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
// 	₯ COp_KCLock
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
class COp_KCLock : public KCOperation
{
public:
OPERATION_ID(KCLock)
								COp_KCLock();
	virtual	OSStatus			Operate();
};
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
// 	₯ COp_KCLockNoUI
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
/*
class COp_KCLockNoUI : public KCOperation
{
public:
OPERATION_ID(KCLockNoUI)
								COp_KCLockNoUI();
	virtual	OSStatus			Operate();
protected:
};
*/										
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
// 	₯ COp_KCGetDefaultKeychain
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
class COp_KCGetDefaultKeychain : public KCOperation
{
public:
OPERATION_ID(KCGetDefaultKeychain)
								COp_KCGetDefaultKeychain();
	virtual	OSStatus			Operate();
};
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
// 	₯ COp_KCSetDefaultKeychain
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
class COp_KCSetDefaultKeychain : public KCOperation
{
public:
OPERATION_ID(KCSetDefaultKeychain)
								COp_KCSetDefaultKeychain();
	virtual	OSStatus			Operate();
};
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
// 	₯ COp_KCCreateKeychain
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
class COp_KCCreateKeychain : public KCOperation
{
public:
OPERATION_ID(KCCreateKeychain)
								COp_KCCreateKeychain();
	virtual	OSStatus			Operate();
protected:
	CParamStringPtr				mPassword;
};
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
// 	₯ COp_KCCreateKeychainNoUI
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
class COp_KCCreateKeychainNoUI : public KCOperation
{
public:
OPERATION_ID(KCCreateKeychainNoUI)
								COp_KCCreateKeychainNoUI();
	virtual	OSStatus			Operate();
    
    virtual StringPtr			GetPassword(){ return (StringPtr)mPassword; }
    virtual	KCRef *				GetKeychainInCallback(){ return &mKeychainInCallback; }
protected:
    CParamStringPtr				mPassword;
	KCRef						mKeychainInCallback;
    static OSStatus				Callback(
                                    KCRef			*outKeychain, 
                                    StringPtr		*outPassword, 
                                    void			*inContext);
};
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
// 	₯ COp_KCGetStatus
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
class COp_KCGetStatus : public KCOperation
{
public:
OPERATION_ID(KCGetStatus)
								COp_KCGetStatus();
	virtual	OSStatus			Operate();
protected:
	CParamUInt32				mKeychainStatus;
};
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
// 	₯ COp_KCChangeSettingsNoUI
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
class COp_KCChangeSettingsNoUI : public KCOperation
{
public:
OPERATION_ID(KCChangeSettingsNoUI)
									COp_KCChangeSettingsNoUI();
	virtual	OSStatus				Operate();
	
#if TARGET_RT_MAC_MACHO
	virtual KCChangeSettingsInfo	
                *GetChangeSettingsInfoPtr(){ return &mChangeSettingsInfo; }
#endif

protected:
#if TARGET_RT_MAC_MACHO
    static OSStatus				Callback(
                                    KCChangeSettingsInfo	*outSettings, 
                                    void					*inContext);
#endif

	CParamBoolean				mLockOnSleep;
	CParamBoolean				mUseKCGetDataSound;
	CParamBoolean				mUseKCGetDataAlert;
	CParamBoolean				mUseLockInterval;
	CParamUInt32				mLockInterval;
	CParamStringPtr				mNewPassword;
	CParamStringPtr				mOldPassword;

#if TARGET_RT_MAC_MACHO
	KCChangeSettingsInfo		mChangeSettingsInfo;
#endif
};
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
// 	₯ COp_KCGetKeychain
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
class COp_KCGetKeychain : public KCItemOperation
{
public:
OPERATION_ID(KCGetKeychain)
								COp_KCGetKeychain();
	virtual	OSStatus			Operate();
protected:
};
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
// 	₯ COp_KCGetKeychainName
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
class COp_KCGetKeychainName : public KCOperation
{
public:
OPERATION_ID(KCGetKeychainName)
								COp_KCGetKeychainName();
	virtual	OSStatus			Operate();
protected:
	CParamStringPtr				mKeychainName;
};
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
// 	₯ COp_KCChangeSettings
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
class COp_KCChangeSettings : public KCOperation
{
public:
OPERATION_ID(KCChangeSettings)
								COp_KCChangeSettings();
	virtual	OSStatus			Operate();
protected:
};
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
// 	₯ COp_KCCountKeychains
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
class COp_KCCountKeychains : public KCOperation
{
public:
OPERATION_ID(KCCountKeychains)
								COp_KCCountKeychains();
	virtual	OSStatus			Operate();

protected:
	CParamUInt16				mCount;
};

// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
// 	₯ COp_KCGetIndKeychain
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
class COp_KCGetIndKeychain : public KCOperation
{
public:
OPERATION_ID(KCGetIndKeychain)
								COp_KCGetIndKeychain();
	virtual	OSStatus			Operate();

protected:
	CParamUInt16				mIndex;
};

// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
// 	₯ COp_KCAddCallback
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
class COp_KCAddCallback : public KCOperation
{
public:
OPERATION_ID(KCAddCallback)
								COp_KCAddCallback();
	virtual	OSStatus			Operate();

protected:
	CParamUInt16				mEvent;
	static UInt32				sCounter[11];
	static KCCallbackUPP		sCallbacks[11];

#define KCADDCALLBACK_DEF(N) \
	static OSStatus				Callback ## N( \
									KCEvent			inKeychainEvent, \
									KCCallbackInfo	*inInfo, \
									void			*inContext)

	KCADDCALLBACK_DEF(0);
	KCADDCALLBACK_DEF(1);
	KCADDCALLBACK_DEF(2);
	KCADDCALLBACK_DEF(3);
	KCADDCALLBACK_DEF(4);
	KCADDCALLBACK_DEF(5);
	KCADDCALLBACK_DEF(6);
	KCADDCALLBACK_DEF(7);
	KCADDCALLBACK_DEF(8);
	KCADDCALLBACK_DEF(9);
	KCADDCALLBACK_DEF(10);
#undef KCADDCALLBACK_DEF


	friend class COp_KCRemoveCallback;
};

// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
// 	₯ COp_KCRemoveCallback
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
class COp_KCRemoveCallback : public KCOperation
{
public:
OPERATION_ID(KCRemoveCallback)
								COp_KCRemoveCallback();
	virtual	OSStatus			Operate();
protected:
	CParamUInt16				mEvent;
	CParamUInt32				mIdleCount;
	CParamUInt32				mLockCount;
	CParamUInt32				mUnlockCount;
	CParamUInt32				mAddCount;
	CParamUInt32				mDeleteCount;
	CParamUInt32				mUpdateCount;
	CParamUInt32				mChangeIdentityCount;
	CParamUInt32				mFindCount;
	CParamUInt32				mSystemCount;
	CParamUInt32				mDefaultChangedCount;
	CParamUInt32				mDataAccessCount;
};

// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
// 	₯ COp_KCSetInteractionAllowed
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
class COp_KCSetInteractionAllowed : public KCOperation
{
public:
OPERATION_ID(KCSetInteractionAllowed)
								COp_KCSetInteractionAllowed();
	virtual	OSStatus			Operate();
protected:
	CParamBoolean				mAllow;
};
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
// 	₯ COp_KCIsInteractionAllowed
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
class COp_KCIsInteractionAllowed : public KCOperation
{
public:
OPERATION_ID(KCIsInteractionAllowed)
								COp_KCIsInteractionAllowed();
	virtual	OSStatus			Operate();
protected:
	CParamBoolean				mAllow;
};
#endif	// __KCAPI_KEYCHAIN__
