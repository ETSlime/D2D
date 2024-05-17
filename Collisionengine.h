//#pragma once
//
//#include <Utilities/Animator.h>
//
//struct boxdata
//{
//
//	BoundingBox* box;
//	Vector3* position;
//	Vector3 velocity;
//	class AnimationRect* AnimRect;
//	int checker;
//};
//
//
//
//class Collisionengine
//{
//
//public:
//	Collisionengine();
//	~Collisionengine();
//
//
//	// tik당 일어나는 함수들
//	void Update();
//	void UpdateCollision();
//
//
//
//	//충돌 감지 함수 및 시행되었을 때 
//	bool AABB(boxdata* A, boxdata* B);
//	void CollisionEffect(boxdata* A, boxdata* B);
//	void CollisionOverlaps(boxdata* A, boxdata* B);
//
//
//	void UpdateVelocity();
//
//
//	//사라진 적 수 확인
//	int GetEnemycount() { return enemycount; }
//	void SetEnemycount(int count) { this->enemycount = count; }
//
//	//각 구조체의 규격및, 월드버퍼의 포지션을 저장해서 엔진에서 만들어서 사용.
//	void insert(BoundingBox* box, Vector3* position, AnimationRect* AnimRect, int checker);
//	void erase(Vector3* position);
//	void clean();
//
//
//
//private:
//
//
//	vector<Vector3*> Positions;
//	vector<Vector3> tmpPositions;
//	vector<boxdata*> datas;
//	Vector3 Position;
//	int count = -1;
//
//	int enemycount;
//
//
//
//
//	float realTime = 0.0f;
//	float checkerTime = 0.0f;
//
//};