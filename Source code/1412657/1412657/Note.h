#pragma once
#include <string>
#include <vector>
#include <ctime>
#include "TagManager.h"
using namespace std;
class CNote
{
	wstring content;
	vector<CTag*> tagList;
	bool isModified;

public:
	static const int BRIEFLENGTH = 50;
	static const int MAXLENGTH = 500;

	CNote();
	CNote(wstring _content, vector<CTag*> tagList);
	virtual ~CNote();

	wstring Replace(wstring _newContent);

	int AddTagWithCtreateTag(LPWSTR _tagName);
	int AddTagWithoutCtreateTag(LPWSTR _tagName);

	int RemoveTag(LPWSTR _tagName);
	void RemoveAllTag();
	bool IsExist(LPWSTR _tagName);

	bool IsModified();

	wstring GetContent();
	wstring GetBrieftContent();

	int GetTagNum();
	CTag * GetTag(unsigned int index);
};
