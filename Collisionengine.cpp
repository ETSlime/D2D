//#include "Collisionengine.h"
//#include "Time.h"
//#include "Character.h"
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
//	//¥Ÿ¿Ω º”µµ ∞ËªÅEª ¿ß«— «ˆ¿ÅE¿ßƒ°∏¶ ¿˙¿ÅE
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
//// √ÊµπΩ√ ¿œæ˚œØ «‡µøµÈ¿ª º≥∏˙„œ¥¬∫Œ∫–
//
//bool Collisionengine::AABB(boxdata* A, boxdata* B)
//{
//	if (A->box->AABB(B->box) == true)
//	{
//		// º”µµ∏¶ ¿ÃøÅE— √Êµπ
//		//CollisionEffect(A, B);
//
//		// ∞„ƒ°¥¬ ∫Œ∫–¿ª ¿ÃøÅE— √Êµπ
//		if (A->checker == 2 && B->checker == 2)
//			CollisionOverlaps(A, B);
//
//
//
//		/*
//		ƒ≥∏Ø≈ÕøÕ ∏ÛΩ∫≈Õ∞° √Êµπ«ﬂ¿ª Ω√ ¿œæ˚œ™¥¬ π›¿¿
//		1. ƒ≥∏Ø≈Õ¿« HP∞° ¥Ÿ∏ß æ÷¿« ∏ÛΩ∫≈Õ¿« µ•πÃ¡ÅE∏∏≈≠ ±¿Ã∞ÅE±◊∞Õ¿ª hpªÛ≈¬πŸø° ¡§∫∏∏¶ ≥—∞‹¡‹.
//		2. ƒ≥∏Ø≈Õø° IsDamaged«‘ºˆ∏¶ ≈ÅEÿº≠ «»ºøºŒ¿Ã¥ıø° ªˆ¿ª πŸ≤„¡‹(ƒ≥∏Ø≈Õ∞° ±Ùπ⁄¿Ã¥¬ »ø∞ÅE
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
//		//√Êµπ«œ¡ÅEæ ¿∏∏ÅE¥ŸΩ√ ø¯∑°¥ÅEŒ µπ∑¡≥ı¿Ω
//		A->AnimRect->isDamaged(0);
//		//B->AnimRect->isDamaged(0);
//
//		return false;
//	}
//
//}
//
////√Êµπ »ƒ π›¿¿ 1. º”µµ π›¿¿
//
//void Collisionengine::CollisionEffect(boxdata* A, boxdata* B)
//{
//	/*
//	tmpAxis : µŒ π∞√º¿« ¡ﬂΩ…¿ª ø¨∞·«ÿ≥ı¿∫ ∫§≈Õ »ƒø° ¡§±‘»≠∏¶ Ω√ƒ—º≠ ±‚¿˙∫§≈Õ∑Œ ªÁøÅEœ∞‘µ»¥Ÿ
//	normalizedtmpAxis : ≈©±‚∞° 1¿Œ ±‚¿˙∫§≈Õ
//	normalizedtmpAxis_normalVector : ¿ß¿« ∫§≈Õ∏¶ 90µµ∑Œ »∏¿ÅEØ»Ø Ω√≈≤ ∫§≈Õ
//
//	Dotproduct_A, Dotproduct_B : A, B¿« º”µµ∫§≈ÕøÕ normalizedtmpAxisøÕ¿« ≥ª¿˚∞™
//
//	Dotproduct_A_normalvector, Dotproduct_B_normalvector A, B¿« º”µµ∫§≈ÕøÕ normalizedtmpAxis_normalVector øÕ¿« ≥ª¿˚∞™
//
//	AbsSum, AbsSum_normalvector : ∞¢ ø˚—ø∑Æ √—«’¿ª ∞¢∞¢¿« ±‚¿˙∫§≈Õ∑Œ ≥™¥©æ˚ÿ≠ ª˝∞¢
//
//
//	*/
//	Vector3 tmpAxis = (*(*A).position) - (*(*B).position);
//	float normalize = sqrt((tmpAxis.x) * (tmpAxis.x) + (tmpAxis.y) * (tmpAxis.y));
//
//
//	Vector3 normalizedtmpAxis = tmpAxis / normalize;
//	// 2¬˜øÅEªÛø°º≠¿« normal vector∏¶ ±∏«œ¥¬ ∞ÅEË∑Œ ±‚¡∏ ∫§≈Õø°º≠ 90µµ 2¬˜øÅE»∏¿ÅEØ»Ø∏∏ «ÿ¡÷∏ÅEµ»¥Ÿ.
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
//// √Êµπ »ƒ π›¿¿ 2. overlapµ«¥¬∫Œ∫–∏∏ µπ∑¡≥ı±ÅE
//
//void Collisionengine::CollisionOverlaps(boxdata* A, boxdata* B)
//{
//
//	/*
//	tmpAxis : µŒ π∞√º¿« ¡ﬂΩ…¿ª ø¨∞·«ÿ≥ı¿∫ ∫§≈Õ »ƒø° ¡§±‘»≠∏¶ Ω√ƒ—º≠ ±‚¿˙∫§≈Õ∑Œ ªÁøÅEœ∞‘µ»¥Ÿ
//	normalizedtmpAxis : ≈©±‚∞° 1¿Œ ±‚¿˙∫§≈Õ
//	normalizedtmpAxis_normalVector : ¿ß¿« ∫§≈Õ∏¶ 90µµ∑Œ »∏¿ÅEØ»Ø Ω√≈≤ ∫§≈Õ
//
//	Overlapdata : ∞„ƒ£ ∫Œ∫–¿ª vector3∑Œ «•Ω√
//
//
//	*/
//
//	Vector3 tmpAxis = (*(*A).AnimRect->GetPos()) - (*(*B).AnimRect->GetPos());
//	float normalize = sqrt((tmpAxis.x) * (tmpAxis.x) + (tmpAxis.y) * (tmpAxis.y));
//	Vector3 normalizedtmpAxis = tmpAxis / normalize;
//	// 2¬˜øÅEªÛø°º≠¿« normal vector∏¶ ±∏«œ¥¬ ∞ÅEË∑Œ ±‚¡∏ ∫§≈Õø°º≠ 90µµ »∏¿ÅEØ»Ø∏∏ «ÿ¡÷∏ÅEµ»¥Ÿ.
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
//// º”µµ∏¶ æ˜µ•¿Ã∆Æ«œ¥¬ ∫Œ∫–
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
////ø¿∫ÅEß∆Æ∏¶ πﬁæ∆µÈ¿Ã¥¬ ∫Œ∫–
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
////π⁄Ω∫µ•¿Ã≈Õ¿« ∫§≈Õø°º≠ ∆Ø¡§ ø¿∫ÅEß∆Æ∏¶ position¿ª ≈ÅEÿº≠ ¡ˆøÅE¬ ∫Œ∫–. (ºˆ¡§« øÅE
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
