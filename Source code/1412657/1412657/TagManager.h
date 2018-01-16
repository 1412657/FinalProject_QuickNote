#pragma once
#include <vector>
#include "Tag.h"
using namespace std; 
class CTagManager
{
	vector<CTag*> tagList;
	static CTagManager * instance;
	CTagManager();
	
public:
	static CTagManager * GetInstance();
	virtual ~CTagManager();
	int AddTag(LPWSTR _tagName, CTag * &tagInfo);
	int AddTag(CTag * tagInfo);
	CTag * GetTag(LPWSTR _tagName);
	CTag * GetTag(unsigned int index);
	int GetTagIndex(CTag * tagInfo);
	int GetTagNum();
	bool IsExist(LPWSTR _tagName);
	bool IsExist(CTag* _pTag);
};

