#ifndef CGAMECONTEXT_H_
#define CGAMECONTEXT_H_

class CGameContext {

	CGameObject player;
	CGameObject menu;
	RArray<CGameObject> otherPlayers;
	RArray<CGameObject> scenary_parts;
	RArray<CGameObject> enemies;	
	
	public:
		
		void animate(TUint ms;)
};

#endif /*CGAMECONTEXT_H_*/
