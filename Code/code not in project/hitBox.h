
class hitBox {
	public:

	typedef struct{
		s8 x, y, flipx;
	}pointInfo; 
	
	pointinfo left, right, up, down;
	u8 flipped;

	hitbox(){
		left.x = left.y = 0;
		right.x = right.y = 0;
		up.x = up.y = 0;
		down.x = down.y = 0;
	}

	hitBox setHitBox(string name){
		hitBox hit = new hitBox();
		switch(name){
			case "player":
				hit.left.x=2; 
				hit.left.y=8; 
				hit.right.x=29; 
				hit.right.y=8; 
				hit.down.x=10; 
				hit.down.y=31; 
				hit.up.x=5; 
				hit.up.y=0;
			break;


		}
		return hit;
	}
};