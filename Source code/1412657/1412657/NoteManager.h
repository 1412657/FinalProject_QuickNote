#pragma once
#include "Note.h"
#include <vector>

using namespace std;
class CNoteManager
{
	vector<CNote*> noteList;
	static CNoteManager* instance;
	CNoteManager();

public:
	virtual ~CNoteManager();

	void AddNote(CNote * _note);
};

