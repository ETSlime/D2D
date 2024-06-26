//#include "Collisionengine.h"
//#include "Time.h"
//#include "GameEvent.h"
//#include "AnimationRect.h"
//
//Collisionengine::Collisionengine()
//{
//	realTime = 0.0f;
//	checkerTime = 0.0f;
//}
//
//Collisionengine::~Collisionengine()
//{
//
//}
//
//void Collisionengine::Update()
//{
//
//
//	realTime += Time::Delta();
//	checkerTime += Time::Delta();
//
//
//	UpdateCollision();
//	UpdateVelocity();
//
//
//
//
//	//다음 속도 계퍊E� 위한 현픸E위치를 저픸E
//	vector<Vector3*>::iterator iter2 = Positions.begin();
//	vector<Vector3>::iterator iter3 = tmpPositions.begin();
//	for (; iter2 != Positions.end(); iter2++)
//	{
//		(*iter3).x = (**iter2).x;
//		(*iter3).y = (**iter2).y;
//		(*iter3).z = (**iter2).z;
//
//
//		iter3++;
//	}
//
//}
//
//void Collisionengine::UpdateCollision()
//{
//	vector<boxdata*>::iterator iter = datas.begin();
//	vector<boxdata*>::iterator iter2;
//	for (; iter != datas.end(); iter++)
//	{
//		for (iter2 = iter + 1; iter2 != datas.end(); iter2++)
//		{
//
//			if ((*iter)->AnimRect->GetisLive() == true && (*iter2)->AnimRect->GetisLive() == true)
//			{
//				AABB((*iter), (*iter2));
//
//				if ((*iter2)->AnimRect->GetHp() <= 0)
//				{
//					(*iter2)->AnimRect->SetisLive(false);
//
//					if ((*iter)->checker == 2)
//						enemycount--;
//
//				}
//			}
//
//
//
//		}
//	}
//
//}
//
//
//
//
//
//// 충돌시 일엉柩 행동들을 설뫄豺는부분
//
//bool Collisionengine::AABB(boxdata* A, boxdata* B)
//{
//	if (A->box->AABB(B->box) == true)
//	{
//		// 속도를 이퓖E� 충돌
//		//CollisionEffect(A, B);
//
//		// 겹치는 부분을 이퓖E� 충돌
//		if (A->checker == 2 && B->checker == 2)
//			CollisionOverlaps(A, B);
//
//
//
//		/*
//		캐릭터와 몬스터가 충돌했을 시 일엉廐는 반응
//		1. 캐릭터의 HP가 다름 애의 몬스터의 데미햨E만큼 깎이컖E그것을 hp상태바에 정보를 넘겨줌.
//		2. 캐릭터에 IsDamaged함수를 흟E漫� 픽셀셰이더에 색을 바꿔줌(캐릭터가 깜박이는 효컖E
//
//		*/
//		if (A->checker == 1 && B->checker == 2 && checkerTime > 1.0f)
//		{
//			if (A->AnimRect->GetHp() > 0)
//			{
//				A->AnimRect->SetHp(A->AnimRect->GetHp() - B->AnimRect->GetAttack());
//				checkerTime = 0;
//				A->AnimRect->isDamaged(1);
//			}
//		}
//		else if (B->checker == 1 && A->checker == 2 && checkerTime > 1.0f)
//		{
//			if (B->AnimRect->GetHp() > 0)
//			{
//				B->AnimRect->SetHp(B->AnimRect->GetHp() - A->AnimRect->GetAttack());
//				checkerTime = 0;
//				B->AnimRect->isDamaged(1);
//			}
//		}
//
//		else if (A->checker == 3 && B->checker == 2)
//		{
//
//
//			B->AnimRect->SetHp(B->AnimRect->GetHp() - A->AnimRect->GetAttack());
//			A->AnimRect->SetisLive(false);
//			A->AnimRect->SetPos({ 0,0,1 });
//
//
//
//		}
//		else if (A->checker == 2 && B->checker == 3)
//		{
//
//
//			A->AnimRect->SetHp(B->AnimRect->GetHp() - A->AnimRect->GetAttack());
//			B->AnimRect->SetisLive(false);
//			B->AnimRect->SetPos({ 0,0,1 });
//
//
//
//
//		}
//
//
//		return true;
//	}
//	else
//	{
//		//충돌하햨E않으툈E다시 원래큱E� 돌려놓음
//		A->AnimRect->isDamaged(0);
//		//B->AnimRect->isDamaged(0);
//
//		return false;
//	}
//
//}
//
////충돌 후 반응 1. 속도 반응
//
//void Collisionengine::CollisionEffect(boxdata* A, boxdata* B)
//{
//	/*
//	tmpAxis : 두 물체의 중심을 연결해놓은 벡터 후에 정규화를 시켜서 기저벡터로 사퓖E構鍍홱�
//	normalizedtmpAxis : 크기가 1인 기저벡터
//	normalizedtmpAxis_normalVector : 위의 벡터를 90도로 회픸E�환 시킨 벡터
//
//	Dotproduct_A, Dotproduct_B : A, B의 속도벡터와 normalizedtmpAxis와의 내적값
//
//	Dotproduct_A_normalvector, Dotproduct_B_normalvector A, B의 속도벡터와 normalizedtmpAxis_normalVector 와의 내적값
//
//	AbsSum, AbsSum_normalvector : 각 웝肌량 총합을 각각의 기저벡터로 나누엉幕 생각
//
//
//	*/
//	Vector3 tmpAxis = (*(*A).position) - (*(*B).position);
//	float normalize = sqrt((tmpAxis.x) * (tmpAxis.x) + (tmpAxis.y) * (tmpAxis.y));
//
//
//	Vector3 normalizedtmpAxis = tmpAxis / normalize;
//	// 2차퓖E상에서의 normal vector를 구하는 컖E瓮� 기존 벡터에서 90도 2차퓖E회픸E�환만 해주툈E된다.
//	Vector3 normalizedtmpAxis_normalVector = Vector3((-1) * normalizedtmpAxis.y, normalizedtmpAxis.x, normalizedtmpAxis.z);
//
//	float Dotproduct_A = D3DXVec3Dot(&normalizedtmpAxis, &(*A).velocity);
//	float Dotproduct_B = D3DXVec3Dot(&normalizedtmpAxis, &(*B).velocity);
//
//	float Dotproduct_A_normalvector = D3DXVec3Dot(&normalizedtmpAxis_normalVector, &(*A).velocity);
//	float Dotproduct_B_normalvector = D3DXVec3Dot(&normalizedtmpAxis_normalVector, &(*B).velocity);
//
//
//	float AbsSum = abs(Dotproduct_A) + abs(Dotproduct_B);
//	float AbsSum_normalvector = abs(Dotproduct_A_normalvector) + abs(Dotproduct_B_normalvector);
//
//	if (((AbsSum)*normalizedtmpAxis.x) * (Time::Delta()) < 5 && ((AbsSum)*normalizedtmpAxis.y) * (Time::Delta()) < 5)
//	{
//		(*(*B).position) = (*(*B).position) - (Time::Delta()) * (*B).velocity - 0.5 * (Time::Delta()) * (AbsSum)*normalizedtmpAxis - 0.5 * (Time::Delta()) * (AbsSum_normalvector)*normalizedtmpAxis_normalVector;
//		(*(*A).position) = (*(*A).position) - (Time::Delta()) * (*A).velocity + 0.5 * (Time::Delta()) * (AbsSum)*normalizedtmpAxis + 0.5 * (Time::Delta()) * (AbsSum_normalvector)*normalizedtmpAxis_normalVector;
//	}
//	else
//	{
//		(*(*B).position) = (*(*B).position) - (Time::Delta()) * (*B).velocity - 0.5 * (Time::Delta()) * (5) * normalizedtmpAxis - 0.5 * (Time::Delta()) * (5) * normalizedtmpAxis_normalVector;
//		(*(*A).position) = (*(*A).position) - (Time::Delta()) * (*A).velocity + 0.5 * (Time::Delta()) * (5) * normalizedtmpAxis + 0.5 * (Time::Delta()) * (5) * normalizedtmpAxis_normalVector;
//	}
//}
//
//// 충돌 후 반응 2. overlap되는부분만 돌려놓콅E
//
//void Collisionengine::CollisionOverlaps(boxdata* A, boxdata* B)
//{
//
//	/*
//	tmpAxis : 두 물체의 중심을 연결해놓은 벡터 후에 정규화를 시켜서 기저벡터로 사퓖E構鍍홱�
//	normalizedtmpAxis : 크기가 1인 기저벡터
//	normalizedtmpAxis_normalVector : 위의 벡터를 90도로 회픸E�환 시킨 벡터
//
//	Overlapdata : 겹친 부분을 vector3로 표시
//
//
//	*/
//
//	Vector3 tmpAxis = (*(*A).AnimRect->GetPos()) - (*(*B).AnimRect->GetPos());
//	float normalize = sqrt((tmpAxis.x) * (tmpAxis.x) + (tmpAxis.y) * (tmpAxis.y));
//	Vector3 normalizedtmpAxis = tmpAxis / normalize;
//	// 2차퓖E상에서의 normal vector를 구하는 컖E瓮� 기존 벡터에서 90도 회픸E�환만 해주툈E된다.
//	Vector3 normalizedtmpAxis_normalVector = Vector3((-1) * normalizedtmpAxis.y, normalizedtmpAxis.x, normalizedtmpAxis.z);
//
//	Vector3 Overlapdata = { 0,0,0 };
//
//	if (((*A).box->GetEdge()->RB.x - (*B).box->GetEdge()->LT.x) < ((*B).box->GetEdge()->RB.x - (*A).box->GetEdge()->LT.x))
//	{
//		Overlapdata.x = ((*A).box->GetEdge()->RB.x - (*B).box->GetEdge()->LT.x);
//	}
//	else
//	{
//		Overlapdata.x = ((*B).box->GetEdge()->RB.x - (*A).box->GetEdge()->LT.x);
//	}
//
//	if (((*A).box->GetEdge()->LT.y - (*B).box->GetEdge()->RB.y) < ((*B).box->GetEdge()->LT.y - (*A).box->GetEdge()->RB.y))
//	{
//		Overlapdata.y = ((*A).box->GetEdge()->LT.y - (*B).box->GetEdge()->RB.y);
//	}
//	else
//	{
//		Overlapdata.y = ((*B).box->GetEdge()->LT.y - (*A).box->GetEdge()->RB.y);
//	}
//
//
//	float Overlapsize = sqrt((Overlapdata.x) * (Overlapdata.x) + (Overlapdata.y) * (Overlapdata.y));
//
//	//(*(*B).position) = (*(*B).position) - 0.1*(Overlapsize)* Vector3( 1, 0, 0 ) -0.1*(Overlapsize)*normalizedtmpAxis_normalVector;
//	//(*(*A).position) = (*(*A).position) + 0.1*(Overlapsize)*normalizedtmpAxis + 0.1*(Overlapsize)*normalizedtmpAxis_normalVector;
//
//	(*(*B).position) = (*(*B).position) - 0.1 * (Overlapsize)*normalizedtmpAxis - 0.1 * (Overlapsize)*normalizedtmpAxis_normalVector;
//	(*(*A).position) = (*(*A).position) + 0.1 * (Overlapsize)*normalizedtmpAxis + 0.1 * (Overlapsize)*normalizedtmpAxis_normalVector;
//	(*(*B).position).z = 0.0f;
//	(*(*A).position).z = 0.0f;
//
//
//
//
//}
//
//
//
//
//
//// 속도를 업데이트하는 부분
//
//void Collisionengine::UpdateVelocity()
//{
//	vector<boxdata*>::iterator iter = datas.begin();
//	vector<Vector3*>::iterator iter2 = Positions.begin();
//	vector<Vector3>::iterator iter3 = tmpPositions.begin();
//	for (; iter != datas.end(); iter++)
//	{
//		float vel_X = ((*iter2)->x - (*iter3).x) / Time::Delta();
//		float vel_Y = ((*iter2)->y - (*iter3).y) / Time::Delta();
//
//		(*iter)->velocity.x = vel_X;
//		(*iter)->velocity.y = vel_Y;
//		(*iter)->velocity.z = 0;
//
//
//
//		iter2++;
//		iter3++;
//	}
//}
//
////오틒E㎷�를 받아들이는 부분
//
//void Collisionengine::insert(BoundingBox* box, Vector3* position, AnimationRect* AnimRect, int checker)
//{
//
//	datas.push_back(new boxdata());
//	count++;
//	datas[count]->box = box;
//	datas[count]->position = position;
//	datas[count]->checker = checker;
//	datas[count]->velocity = { 0.0f, 0.0f, 0.0f };
//	datas[count]->AnimRect = AnimRect;
//
//	this->Position = *position;
//
//
//
//	Positions.push_back(position);
//
//
//	tmpPositions.push_back({ 0,0,0 });
//
//
//}
//
////박스데이터의 벡터에서 특정 오틒E㎷�를 position을 흟E漫� 지퓖E� 부분. (수정필퓖E
//
//void Collisionengine::erase(Vector3* position)
//{
//	vector<boxdata*>::iterator iter = datas.begin();
//	for (; iter != datas.end(); iter++)
//	{
//		if ((*iter)->position == position)
//		{
//			delete (*iter);
//			iter = datas.erase(iter);
//		}
//	}
//	vector<Vector3*>::iterator iter2 = (Positions.begin());
//	vector<Vector3>::iterator iter3 = tmpPositions.begin();
//	for (; iter2 != Positions.end(); iter2++)
//	{
//		if (*iter2 == position)
//		{
//			delete (*iter2);
//			iter2 = Positions.erase(iter2);
//			delete (*iter3);
//			iter3 = tmpPositions.erase(iter3);
//		}
//		else
//		{
//			iter3++;
//		}
//	}
//	count--;
//
//}
//
//void Collisionengine::clean()
//{
//}
