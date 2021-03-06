/*
 * Copyright (c) 2000-2006,2011-2012,2014 Apple Inc. All Rights Reserved.
 * 
 * @APPLE_LICENSE_HEADER_START@
 * 
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this
 * file.
 * 
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@
 */

#include <security_cdsa_utilities/AuthorizationData.h>
#include <security_cdsa_utilities/AuthorizationWalkers.h>
#include <security_cdsa_utilities/walkers.h>
#include <Security/checkpw.h>
#include <grp.h>
#include <pwd.h>


// checkpw() that uses provided struct passwd
extern "C"
{
int checkpw_internal( const struct passwd *pw, const char* password );
}


namespace Authorization {


AuthValueRef::AuthValueRef(const AuthValue &value) : 
	RefPointer<AuthValue>(new AuthValue(value)) {}

AuthValueRef::AuthValueRef(const AuthorizationValue &value) : 
	RefPointer<AuthValue>(new AuthValue(value)) {}

AuthValue::AuthValue(const AuthorizationValue &value) :
    mOwnsValue(false)
{
    mValue.length = value.length;
    mValue.data = value.data;
}

AuthValueRef::AuthValueRef(UInt32 length, void *data) : 
	RefPointer<AuthValue>(new AuthValue(length, data)) {}

AuthValue::AuthValue(UInt32 length, void *data) :
    mOwnsValue(true)
{
    mValue.length = length;
    mValue.data = new uint8_t[length];
    if (length)
        memcpy(mValue.data, data, length);
}

AuthValue::~AuthValue()
{
    if (mOwnsValue)
	{
		memset(mValue.data, 0, mValue.length);
        delete[] reinterpret_cast<uint8_t*>(mValue.data);
	}
}

AuthValue &
AuthValue::operator = (const AuthValue &other)
{
    if (mOwnsValue)
	{
		memset(mValue.data, 0 , mValue.length);
        delete[] reinterpret_cast<uint8_t*>(mValue.data);
	}
	
    mValue = other.mValue;
    mOwnsValue = other.mOwnsValue;
    other.mOwnsValue = false;
    return *this;
}

void
AuthValue::fillInAuthorizationValue(AuthorizationValue &value)
{
    value.length = mValue.length;
    value.data = mValue.data;
}

AuthValueVector &
AuthValueVector::operator = (const AuthorizationValueVector& valueVector)
{
    clear();
    for (unsigned int i=0; i < valueVector.count; i++)
        push_back(AuthValueRef(valueVector.values[i]));
    return *this;
}

void
AuthValueVector::copy(AuthorizationValueVector **data, size_t *length) const
{
    AuthorizationValueVector valueVector;
    valueVector.count = (UInt32)size();
    valueVector.values = new AuthorizationValue[valueVector.count];
    int i = 0;
	for (const_iterator it = begin(); it != end(); ++it, ++i)
    {
		(*it)->fillInAuthorizationValue(valueVector.values[i]);
	}

	DataWalkers::Copier<AuthorizationValueVector> flatValueVector(&valueVector);
	*length = flatValueVector.length();
	*data = flatValueVector.keep();

	delete[] valueVector.values;
}

AuthItem::AuthItem(const AuthorizationItem &item) :
    mFlags(item.flags),
    mOwnsName(true),
    mOwnsValue(true)
{
	if (!item.name)
		MacOSError::throwMe(errAuthorizationInternal);
	size_t nameLen = strlen(item.name) + 1;
	mName = new char[nameLen];
	memcpy(const_cast<char *>(mName), item.name, nameLen);

	mValue.length = item.valueLength;
	mValue.data = new uint8_t[item.valueLength];
	if (mValue.length)
		memcpy(mValue.data, item.value, item.valueLength);
}


AuthItem::AuthItem(AuthorizationString name) :
    mName(name),
    mFlags(0),
    mOwnsName(false),
    mOwnsValue(false)
{
    mValue.length = 0;
    mValue.data = NULL;
}

AuthItem::AuthItem(AuthorizationString name, AuthorizationValue value, AuthorizationFlags flags) :
    mFlags(flags),
    mOwnsName(true),
    mOwnsValue(true)
{
	if (!name)
		MacOSError::throwMe(errAuthorizationInternal);
	size_t nameLen = strlen(name) + 1;
	mName = new char[nameLen];
	memcpy(const_cast<char *>(mName), name, nameLen);

	mValue.length = value.length;
	mValue.data = new uint8_t[value.length];
	if (mValue.length)
		memcpy(mValue.data, value.data, value.length);
}

AuthItem::~AuthItem()
{
    if (mOwnsName)
        delete[] mName;
    if (mOwnsValue)
	{
		memset(mValue.data, 0, mValue.length);
        delete[] reinterpret_cast<uint8_t*>(mValue.data);
	}
}

bool
AuthItem::operator < (const AuthItem &other) const
{
    return strcmp(mName, other.mName) < 0;
}

AuthItem &
AuthItem::operator = (const AuthItem &other)
{
    if (mOwnsName)
        delete[] mName;
    if (mOwnsValue)
	{
		memset(mValue.data, 0, mValue.length);
        delete[] reinterpret_cast<uint8_t*>(mValue.data);
	}

    mName = other.mName;
    mValue = other.mValue;
    mFlags = other.mFlags;
    mOwnsName = other.mOwnsName;
    other.mOwnsName = false;
    mOwnsValue = other.mOwnsValue;
    other.mOwnsValue = false;
    return *this;
}

void
AuthItem::fillInAuthorizationItem(AuthorizationItem &item)
{
    item.name = mName;
    item.valueLength = mValue.length;
    item.value = mValue.data;
    item.flags = mFlags;
}

bool 
AuthItem::getBool(bool &value)
{
	if (mValue.length == sizeof(bool))
	{
		bool *tmpValue = (bool *)mValue.data;
		
		if (tmpValue)
		{
			value = *tmpValue;
			return true;
		}
	}
	
	return false;
}

bool
AuthItem::getString(string &value)
{
    	value = string(static_cast<char*>(mValue.data), mValue.length);
	return true;
}

bool
AuthItem::getCssmData(CssmAutoData &value)
{
	value = CssmData(static_cast<uint8_t*>(mValue.data), mValue.length);
	return true;
}	


AuthItemRef::AuthItemRef(const AuthorizationItem &item) : RefPointer<AuthItem>(new AuthItem(item)) {}

AuthItemRef::AuthItemRef(AuthorizationString name) : RefPointer<AuthItem>(new AuthItem(name)) {}

AuthItemRef::AuthItemRef(AuthorizationString name, AuthorizationValue value, AuthorizationFlags flags) : RefPointer<AuthItem>(new AuthItem(name, value, flags)) {}


//
// AuthItemSet
//
AuthItemSet::AuthItemSet()
: firstItemName(NULL)
{
}

AuthItemSet::~AuthItemSet()
{
	if (NULL != firstItemName)
		free(firstItemName);
}

AuthItemSet &
AuthItemSet::operator = (const AuthorizationItemSet& itemSet)
{
    clear();

    for (unsigned int i=0; i < itemSet.count; i++)
        insert(AuthItemRef(itemSet.items[i]));

    return *this;
}

AuthItemSet&
AuthItemSet::operator=(const AuthItemSet& itemSet)
{
	std::set<AuthItemRef>::operator=(itemSet);	
	
	if (this != &itemSet) {
		duplicate(itemSet);
	}
	
	return *this;
}

AuthItemSet::AuthItemSet(const AuthorizationItemSet *itemSet)
: firstItemName(NULL)
{
	if (NULL != itemSet && NULL != itemSet->items)
	{
		if (0 < itemSet->count && NULL != itemSet->items[0].name)
			firstItemName = strdup(itemSet->items[0].name);

		for (unsigned int i=0; i < itemSet->count; i++)
			insert(AuthItemRef(itemSet->items[i]));
	}
}

AuthItemSet::AuthItemSet(const AuthItemSet& itemSet)
: std::set<AuthItemRef>(itemSet)
{
	duplicate(itemSet);
}

void
AuthItemSet::duplicate(const AuthItemSet& itemSet)
{
	if (itemSet.firstItemName != NULL)
		firstItemName = strdup(itemSet.firstItemName);
	else
		firstItemName = NULL;	
}
	
void
AuthItemSet::copy(AuthorizationItemSet *&data, size_t &length, Allocator &alloc) const
{
    AuthorizationItemSet itemSet;
    itemSet.count = (UInt32)size();
    itemSet.items = new AuthorizationItem[itemSet.count];
    int i = 0;
    for (const_iterator it = begin(); it != end(); ++it, ++i)
    {
        (*it)->fillInAuthorizationItem(itemSet.items[i]);
    }

	DataWalkers::Copier<AuthorizationItemSet> flatItemSet(&itemSet, alloc);
	length = flatItemSet.length();
		
	data = flatItemSet.keep();
	// else flatItemSet disappears again

    delete[] itemSet.items;
}

AuthorizationItemSet *
AuthItemSet::copy() const
{
	AuthorizationItemSet *aCopy;
	size_t aLength;
	copy(aCopy, aLength);
	return aCopy;
}

AuthItem *
AuthItemSet::find(const char *name)
{
	AuthItemSet::const_iterator found = find_if(this->begin(), this->end(), FindAuthItemByRightName(name) );
	if (found != this->end())
		return *found;
	
	return NULL;
}

}	// end namespace Authorization
