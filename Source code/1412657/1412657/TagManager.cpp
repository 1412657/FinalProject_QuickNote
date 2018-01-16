#include "stdafx.h"
#include "TagManager.h"
using namespace std;

CTagManager * CTagManager::instance = nullptr;

CTagManager::CTagManager()
{
}
CTagManager * CTagManager::GetInstance()
{
	if (instance == nullptr)
		instance = new CTagManager();
	return instance;
}

CTagManager::~CTagManager()
{
	delete instance;
	for (unsigned char i = 0; i < tagList.size(); ++i)
	{
		delete tagList[i];
	}
}

int CTagManager::AddTag(LPWSTR _tagName, CTag * &tagInfo)
{
	for (unsigned int i = 0; i < tagList.size(); ++i)
	{
		if (wcscmp(tagList[i]->tagName, _tagName) == 0)
		{
			tagInfo = tagList[i];
			return 1;
		}
	}
	CTag * pTag = new CTag(_tagName);
	tagList.push_back(pTag);
	tagInfo = pTag;
	return 0;
}

int CTagManager::AddTag(CTag * tagInfo)
{
	if (!IsExist(tagInfo))
	{
		tagList.push_back(tagInfo);
		return 0;
	}
	return 1;
}

CTag * CTagManager::GetTag(LPWSTR _tagName)
{
	CTag * pTag;
	AddTag(_tagName, pTag);
	return pTag;
}

CTag * CTagManager::GetTag(unsigned int index)
{
	if (index < tagList.size())
		return tagList[index];
	return nullptr;
}

int CTagManager::GetTagIndex(CTag * tagInfo)
{
	for (unsigned int i = 0; i < tagList.size(); ++i)
	{
		if (tagInfo == tagList[i])
			return i;
	}
	return -1;
}

int CTagManager::GetTagNum()
{
	return tagList.size();
}

bool CTagManager::IsExist(LPWSTR _tagName)
{
	for (unsigned int i = 0; i < tagList.size(); ++i)
	{
		if (wcscmp(tagList[i]->tagName, _tagName) == 0)
		{
			return true;
		}
	}
	return false;
}

bool CTagManager::IsExist(CTag * _pTag)
{
	for (unsigned int i = 0; i < tagList.size(); ++i)
	{
		if (tagList[i] == _pTag)
			return true;
	}
	return false;
}
