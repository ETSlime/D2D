#include "Util.h"
#include "DirectHelper.h"

const float Values::PI = 3.14159f;

const DirectX::XMFLOAT3 Values::ZeroVec3 = DirectX::XMFLOAT3(0, 0, 0);
const DirectX::XMFLOAT3 Values::OneVec3 = DirectX::XMFLOAT3(1, 1, 1);
const DirectX::XMFLOAT3 Values::LeftVec = DirectX::XMFLOAT3(-1, 0, 0);
const DirectX::XMFLOAT3 Values::RightVec = DirectX::XMFLOAT3(1, 0, 0);
const DirectX::XMFLOAT3 Values::UpVec = DirectX::XMFLOAT3(0, 1, 0);
const DirectX::XMFLOAT3 Values::DownVec = DirectX::XMFLOAT3(0, -1, 0);
const DirectX::XMFLOAT3 Values::FwdVec = DirectX::XMFLOAT3(0, 0, 1);
const DirectX::XMFLOAT3 Values::ScreenSize = DirectX::XMFLOAT3((float)WinMaxWidth, (float)WinMaxHeight, 1);

const DirectX::XMFLOAT2 Values::ZeroVec2 = DirectX::XMFLOAT2(0, 0);
const DirectX::XMFLOAT2 Values::OneVec2 = DirectX::XMFLOAT2(1, 1);
const DirectX::XMFLOAT2 Values::uvLT = DirectX::XMFLOAT2(0, 0);
const DirectX::XMFLOAT2 Values::uvLB = DirectX::XMFLOAT2(0, 1);
const DirectX::XMFLOAT2 Values::uvRT = DirectX::XMFLOAT2(1, 0);
const DirectX::XMFLOAT2 Values::uvRB = DirectX::XMFLOAT2(1, 1);

const DirectX::XMFLOAT4 Values::Red = DirectX::XMFLOAT4(1, 0, 0, 1);
const DirectX::XMFLOAT4 Values::Green = DirectX::XMFLOAT4(0, 1, 0, 1);
const DirectX::XMFLOAT4 Values::Blue = DirectX::XMFLOAT4(0, 0, 1, 1);
const DirectX::XMFLOAT4 Values::Magenta = DirectX::XMFLOAT4(1, 0, 1, 1);
const DirectX::XMFLOAT4 Values::Black = DirectX::XMFLOAT4(0, 0, 0, 1);
const DirectX::XMFLOAT4 Values::White = DirectX::XMFLOAT4(1, 1, 1, 1);
//const DirectX::XMFLOAT4 Values::ClearColor = 0xFF555566;

const DirectX::XMFLOAT4X4 Values::Identity = { 1, 0, 0, 0,
								  0, 1, 0, 0,
								  0, 0, 1, 0,
								  0, 0, 0, 1 };