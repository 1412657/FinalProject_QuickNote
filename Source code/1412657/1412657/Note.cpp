#include "stdafx.h"
#include "Note.h"

CNote::CNote()
{
	isModified = false;
}

CNote::CNote(wstring _content, vector<CTag*> tagList) :CNote()
{
	content = _content;
}


CNote::~CNote()
{
}

wstring CNote::Replace(wstring _newContent)
{
	wstring oldContent = content;
	content = _newContent;
	isModified = true;
	return oldContent;
}

int CNote::AddTagWithCtreateTag(LPWSTR _tagName)
{
	CTagManager* tagMngmnt = CTagManager::GetInstance();
	CTag* tag;
	tagMngmnt->AddTag(_tagName, tag);
	if (!IsExist(_tagName))
	{
		tagList.push_back(tag);
		isModified = true;
		tag->AddRegistration();
		return 0;
	}
	return 1;
}

int CNote::AddTagWithoutCtreateTag(LPWSTR _tagName)
{
	CTagManager* tagMngmnt = CTagManager::GetInstance();
	if (tagMngmnt->IsExist(_tagName))
	{
		CTag * tag = tagMngmnt->GetTag(_tagName);
		if (!IsExist(_tagName))
		{
			tagList.push_back(tagMngmnt->GetTag(_tagName));
			tag->AddRegistration();
			isModified = true;
			return 0;
		}
		return 1;
	}
	return -1;
}

int CNote::RemoveTag(LPWSTR _tagName)
{
	for (unsigned int i = 0; i < tagList.size(); ++i)
	{
		if (wcscmp(_tagName, tagList[i]->tagName) == 0)
		{
			tagList[i]->RemoveRegistration();
			tagList.erase(tagList.begin() + i, tagList.begin() + i);
			isModified = true;
			return 0;
		}
	}
	return 1;
}

void CNote::RemoveAllTag()
{
	for (unsigned int i = 0; i < tagList.size(); ++i)
	{
		tagList[i]->RemoveRegistration();
	}
	tagList.clear();
}

bool CNote::IsExist(LPWSTR _tagName)
{
	for (unsigned int i = 0; i < tagList.size(); ++i)
	{
		if (wcscmp(_tagName, tagList[i]->tagName) == 0)
			return true;
	}
	return false;
}

bool CNote::IsModified()
{
	return isModified;
}

wstring CNote::GetContent()
{
	return content;
}

wstring CNote::GetBrieftContent()
{
	wstring brieftStr = content;
	brieftStr.resize(BRIEFLENGTH, 0);
	int length = brieftStr.length();
	brieftStr[length - 1] = L'.';
	brieftStr[length - 2] = L'.';
	brieftStr[length - 3] = L'.';

	return brieftStr;
}

int CNote::GetTagNum()
{
	return tagList.size();
}

CTag * CNote::GetTag(unsigned int index)
{
	return tagList[index];
}

