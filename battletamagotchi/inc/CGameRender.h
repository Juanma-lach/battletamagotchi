#ifndef CGAMERENDER_H_
#define CGAMERENDER_H_

#include <e32cmn.h>
#include "CGameObject.h"
#include "CGameSprite.h"

class CGameRender {

	RArray<CGameObject*> objs;

	public:
	
		void addGameObject(CGameObject* o);
		CGameObject* removeGameObject(CGameObject* o);
	
		void draw();
};

#endif /*CGAMERENDER_H_*/
