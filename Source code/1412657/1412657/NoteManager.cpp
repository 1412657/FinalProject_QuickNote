#include "stdafx.h"
#include "NoteManager.h"


CNoteManager::CNoteManager()
{
}


CNoteManager::~CNoteManager()
{
}

void CNoteManager::AddNote(CNote * _note)
{
	noteList.push_back(_note);

}
