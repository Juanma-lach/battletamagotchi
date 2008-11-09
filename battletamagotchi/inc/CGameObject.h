#ifndef CGAMEOBJECT_H_
#define CGAMEOBJECT_H_

#include "CGameSprite.h"
#include "base/CGameRect.h"

class CGameObject {

	RArray<CGameSprite> anim;
		
	TInt layer;
	
	public:
		
		CGameRect rect;
				
		virtual void animate(TInt ms);
		
		TInt verifyCollision(CGameRect& r);
		
		CGameSpriteFrame* getCurrentSpriteFrame(void);
		
		TInt getLayer(void);
		void setLayer(TInt l);
		
		bool collides;
		
	enum COL_RETURN {
		COL_DONT_COLLIDES_FLAG,
		COL_DONT_COLLIDES,
		COL_COLLIDES
	};
};

#endif /*CGAMEOBJECT_H_*/
