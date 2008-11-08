#ifndef __CLASS_GAME_RECT_H_
#define __CLASS_GAME_RECT_H_

struct CGameRect {
	TInt x;
	TInt y;
	TInt w;
	TInt h;
	CGameRect(TInt x=0, TInt y=0, TInt w=0, TInt h=0){this->x = x; this->y = y; this->w = w; this->h = h;}
};

static bool collide(const CGameRect& a, const CGameRect& b){
	
	TInt xM,yM, w, h;
	const CGameRect *l, *bo;
		
	if(a.x > b.x){
		xM = a.x - b.x;
		l = &b;
	} else {
		xM = b.x - a.x;
		l = &a;
	}
	
	w = (a.w > b.w)? a.w : b.w;
	
	if(xM>w) return false;
	
	if(a.y > b.y){
		yM = a.y - b.y;
		bo = &b;
	} else {
		yM = b.y - a.y;
		bo = &a;
	}
	
	h = (a.h > b.h)? a.h : b.h;
	
	if(yM>h) return false;
	
	if(l->w < w) return false;
	if(bo->h < h) return false;
	
	return true;
}

#endif
