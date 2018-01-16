#pragma once
//#include "Note.h"
class CTag
{
	unsigned short registedNoteNum;
public:
	static const char MAX_TAGNAMELENGHTH = 30;
	static const char MAX_TAGNSHORTAMELENGHTH = 20;

	WCHAR tagName[MAX_TAGNAMELENGHTH + 1];
	WCHAR tagShortName[MAX_TAGNSHORTAMELENGHTH + 1];

	CTag(LPWSTR _tagName);
	virtual ~CTag();

	void AddRegistration();
	void ResetRegistration();
	bool RemoveRegistration();
};

