class Collider 
{
	public:
		int posX;
		int posY;
		int dimX;
		int dimY;
		int typeCollision;
		bool active;

		Collider(int, int, int, int, int, bool);

		void DisplayFigureCollision();
		bool SquaretoSquareCollision( Collider*);
		bool Collision( Collider*);


};

list<Collider*> CollisionBlocks;


Collider::Collider(int posX, int posY, int dimX, int dimY, int typeCollision, bool active) {
	this->posX = posX;
	this->posY = posY;
	this->dimX = dimX-1;
	this->dimY = dimY-1;
	this->typeCollision = typeCollision;
	this->active = active;
}

void Collider::DisplayFigureCollision() {
	if(typeCollision == 0) al_draw_rectangle(posX, posY, posX+dimX, posY+dimY, al_map_rgb(255, 0, 0),2);
}

bool Collider::Collision( Collider* obj2) {
	if (typeCollision == 0)  return SquaretoSquareCollision(obj2);
}

bool Collider::SquaretoSquareCollision(Collider* obj2) {
	return ((posX <= obj2->posX + obj2->dimX) && (posX + dimX >= obj2->posX) &&
		(posY <= obj2->posY + obj2->dimY) && (posY + dimY >= obj2->posY));
}


bool Collision2OBJ(Collider *obj1, Collider *obj2) {
	if ((obj1->posX <= obj2->posX + obj2->dimX) && (obj1->posX + obj1->dimX >= obj2->posX) &&
		(obj1->posY <= obj2->posY + obj2->dimY) && (obj1->posY + obj1->dimY >= obj2->posY)) {
		return true;
	}
	else return false;
}