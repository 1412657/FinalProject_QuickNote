#include "stdafx.h"
#include "Tag.h"


CTag::CTag(LPWSTR _tagName)
{
	registedNoteNum = 0;
	wcscpy_s(tagName, MAX_TAGNAMELENGHTH, _tagName);
	for (char i = 0; i < MAX_TAGNSHORTAMELENGHTH; ++i)
	{
		tagShortName[i] = _tagName[i];
	}
	tagShortName[MAX_TAGNSHORTAMELENGHTH] = 0;
	if (wcslen(_tagName) > MAX_TAGNSHORTAMELENGHTH)
	{
		tagShortName[MAX_TAGNSHORTAMELENGHTH - 3] = tagShortName[MAX_TAGNSHORTAMELENGHTH - 2] = tagShortName[MAX_TAGNSHORTAMELENGHTH - 1] = L'.';
		tagShortName[MAX_TAGNSHORTAMELENGHTH] = 0;
	}
}


CTag::~CTag()
{
}

void CTag::AddRegistration()
{
	++registedNoteNum;
}

void CTag::ResetRegistration()
{
	registedNoteNum = 0;
}

bool CTag::RemoveRegistration()
{
	if (registedNoteNum == 0)
		return false;
	--registedNoteNum;
	return true;
}
