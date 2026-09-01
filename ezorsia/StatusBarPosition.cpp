#include "stdafx.h"
#include "StatusBarPosition.h"
#include "Memory.h"
#include <unknwn.h>
#include <oaidl.h>
#include <intrin.h>

#pragma intrinsic(_ReturnAddress)

namespace {

constexpr int kOriginLT = 0;
constexpr int kOriginCT = 1;
constexpr int kOriginRT = 2;
constexpr int kOriginLC = 3;
constexpr int kOriginCC = 4;
constexpr int kOriginRC = 5;
constexpr int kOriginLB = 6;
constexpr int kOriginCB = 7;
constexpr int kOriginRB = 8;
constexpr int kOriginCount = 9;
constexpr int kStatusBarOrigin = kOriginCB;

constexpr DWORD kCWndCreateWnd = 0x009DE4D2;
constexpr DWORD kCWndManConstructor = 0x009E2C42;
constexpr DWORD kCWndManDestructor = 0x009E3026;
constexpr DWORD kCWndManGetOrgWindow = 0x0048BBA5;

struct IWzArchive;

struct __declspec(uuid("01110bc6-d843-4ff1-918c-9cf64632fb15")) IWzSerialize : IUnknown {
	virtual HRESULT __stdcall get_persistentUOL(BSTR* pVal) = 0;
	virtual HRESULT __stdcall raw_Serialize(IWzArchive* pArchive) = 0;
};

struct __declspec(uuid("4cfb57c7-e3ea-b340-ac98-4b2750e3642a")) IWzShape2D : IWzSerialize {
	virtual HRESULT __stdcall get_item(VARIANT vIndex, VARIANT* pvValue) = 0;
	virtual HRESULT __stdcall get__NewEnum(IUnknown** pVal) = 0;
	virtual HRESULT __stdcall get_count(unsigned int* pVal) = 0;
	virtual HRESULT __stdcall get_x(int* pVal) = 0;
	virtual HRESULT __stdcall put_x(int pVal) = 0;
	virtual HRESULT __stdcall get_y(int* pVal) = 0;
	virtual HRESULT __stdcall put_y(int pVal) = 0;
	virtual HRESULT __stdcall get_x2(int* pVal) = 0;
	virtual HRESULT __stdcall put_x2(int pVal) = 0;
	virtual HRESULT __stdcall get_y2(int* pVal) = 0;
	virtual HRESULT __stdcall put_y2(int pVal) = 0;
	virtual HRESULT __stdcall raw_Move(int nX, int nY) = 0;
	virtual HRESULT __stdcall raw_Offset(int nDX, int nDY) = 0;
	virtual HRESULT __stdcall raw_Scale(int nXMul, int nXDiv, int nYMul, int nYDiv, int nXOrg, int nYOrg) = 0;
	virtual HRESULT __stdcall raw_Insert(VARIANT vIndexOrShape, VARIANT vShape) = 0;
	virtual HRESULT __stdcall raw_Remove(VARIANT vIndex, VARIANT* pvRemoved) = 0;
	virtual HRESULT __stdcall raw_Init(int nX, int nY) = 0;
};

struct __declspec(uuid("f28bd1ed-3deb-4f92-9eec-10ef5a1c3fb4")) IWzVector2D : IWzShape2D {
	virtual HRESULT __stdcall get_currentTime(int* pnTime) = 0;
	virtual HRESULT __stdcall put_currentTime(int pnTime) = 0;
	virtual HRESULT __stdcall get_origin(VARIANT* pvOrigin) = 0;
	virtual HRESULT __stdcall put_origin(VARIANT pvOrigin) = 0;
	virtual HRESULT __stdcall get_rx(int* pVal) = 0;
	virtual HRESULT __stdcall put_rx(int pVal) = 0;
	virtual HRESULT __stdcall get_ry(int* pVal) = 0;
	virtual HRESULT __stdcall put_ry(int pVal) = 0;
	virtual HRESULT __stdcall get_a(double* pVal) = 0;
	virtual HRESULT __stdcall get_ra(double* pVal) = 0;
	virtual HRESULT __stdcall put_ra(double pVal) = 0;
	virtual HRESULT __stdcall get_flipX(int* pVal) = 0;
	virtual HRESULT __stdcall put_flipX(int pVal) = 0;
	virtual HRESULT __stdcall raw__GetSnapshot(int* px, int* py, int* prx, int* pry, int* pxOrg, int* pyOrg, double* pa, double* paOrg, VARIANT vTime) = 0;
	virtual HRESULT __stdcall raw_RelMove(int nX, int nY, VARIANT nTime, VARIANT nType) = 0;
};

struct __declspec(uuid("6dc8c7ce-8e81-4420-b4f6-4b60b7d5fcdf")) IWzGr2DLayer : IWzVector2D {
};

struct __declspec(uuid("e576ea33-d465-4f08-aab1-e78df73ee6d9")) IWzGr2D : IUnknown {
	virtual HRESULT __stdcall raw_Initialize(unsigned int uWidth, unsigned int uHeight, VARIANT vHwnd, VARIANT vBPP, VARIANT vRefreshRate) = 0;
	virtual HRESULT __stdcall raw_Uninitialize() = 0;
	virtual HRESULT __stdcall get_nextRenderTime(int* pnTime) = 0;
	virtual HRESULT __stdcall raw_UpdateCurrentTime(int tTime) = 0;
	virtual HRESULT __stdcall raw_RenderFrame() = 0;
	virtual HRESULT __stdcall raw_SetFrameSkip() = 0;
	virtual HRESULT __stdcall raw_ToggleFpsPanel() = 0;
	virtual HRESULT __stdcall raw_DisableFpsPanel() = 0;
	virtual HRESULT __stdcall get_width(unsigned int* puWidth) = 0;
	virtual HRESULT __stdcall get_height(unsigned int* puHeight) = 0;
	virtual HRESULT __stdcall get_bpp(unsigned int* puBPP) = 0;
	virtual HRESULT __stdcall get_refreshRate(unsigned int* puRefreshRate) = 0;
	virtual HRESULT __stdcall get_fps100(unsigned int* puFps100) = 0;
	virtual HRESULT __stdcall get_currentTime(int* pnCurrentTime) = 0;
	virtual HRESULT __stdcall get_fullScreen(int* pnFullScreen) = 0;
	virtual HRESULT __stdcall put_fullScreen(int pnFullScreen) = 0;
	virtual HRESULT __stdcall get_backColor(unsigned int* puColor) = 0;
	virtual HRESULT __stdcall put_backColor(unsigned int puColor) = 0;
	virtual HRESULT __stdcall get_redTone(IWzVector2D** ppVector) = 0;
	virtual HRESULT __stdcall get_greenBlueTone(IWzVector2D** ppVector) = 0;
	virtual HRESULT __stdcall get_center(IWzVector2D** ppVector) = 0;
};

using CWndCreateWnd_t = void(__fastcall*)(void* pThis, void* edx, int nLeft, int nTop, int nWidth, int nHeight, int z, int bScreenCoord, void* pData, int bSetFocus);
using CWndManConstructor_t = void(__fastcall*)(void* pThis, void* edx, HWND hWnd);
using CWndManDestructor_t = void(__fastcall*)(void* pThis, void* edx);
using CWndManGetOrgWindow_t = IWzVector2D**(__fastcall*)(void* pThis, void* edx, IWzVector2D** result);
using PcCreateObject_t = HRESULT(__cdecl*)(const wchar_t* sUOL, const GUID* riid, void** ppObj, IUnknown* pUnkOuter);

CWndCreateWnd_t CWndCreateWnd = reinterpret_cast<CWndCreateWnd_t>(kCWndCreateWnd);
CWndManConstructor_t CWndManConstructor = reinterpret_cast<CWndManConstructor_t>(kCWndManConstructor);
CWndManDestructor_t CWndManDestructor = reinterpret_cast<CWndManDestructor_t>(kCWndManDestructor);
CWndManGetOrgWindow_t CWndManGetOrgWindow = reinterpret_cast<CWndManGetOrgWindow_t>(kCWndManGetOrgWindow);
IWzVector2D* g_orgWindowEx[kOriginCount] = {};

IWzGr2D* GetGr()
{
	return *reinterpret_cast<IWzGr2D**>(0x00BF14EC);
}

IWzVector2D* GetWndManOrgWindow(void* pWndMan)
{
	if (pWndMan == nullptr) {
		return nullptr;
	}
	return *reinterpret_cast<IWzVector2D**>(reinterpret_cast<DWORD>(pWndMan) + 0xDC);
}

IWzGr2DLayer* GetWndLayer(void* pWnd)
{
	return *reinterpret_cast<IWzGr2DLayer**>(reinterpret_cast<DWORD>(pWnd) + 0x18);
}

void SetVectorOrigin(IWzVector2D* pVector, IUnknown* pOrigin)
{
	if (pVector == nullptr || pOrigin == nullptr) {
		return;
	}

	VARIANT vOrigin;
	VariantInit(&vOrigin);
	V_VT(&vOrigin) = VT_UNKNOWN;
	V_UNKNOWN(&vOrigin) = pOrigin;
	pVector->put_origin(vOrigin);
}

void RelMove(IWzVector2D* pVector, int nX, int nY)
{
	if (pVector == nullptr) {
		return;
	}

	VARIANT vEmpty;
	VariantInit(&vEmpty);
	pVector->raw_RelMove(nX, nY, vEmpty, vEmpty);
}

bool CreateOrgWindows()
{
	auto pComApis = reinterpret_cast<FARPROC*>(0x00BF0CC0);
	if (pComApis == nullptr || pComApis[0] == nullptr) {
		return false;
	}

	auto PcCreateObject = reinterpret_cast<PcCreateObject_t>(pComApis[0]);
	for (int i = 0; i < kOriginCount; ++i) {
		if (g_orgWindowEx[i] != nullptr) {
			continue;
		}

		if (FAILED(PcCreateObject(L"Shape2D#Vector2D", &__uuidof(IWzVector2D), reinterpret_cast<void**>(&g_orgWindowEx[i]), nullptr))) {
			return false;
		}
	}
	return true;
}

void ReleaseOrgWindows()
{
	for (int i = 0; i < kOriginCount; ++i) {
		if (g_orgWindowEx[i] != nullptr) {
			g_orgWindowEx[i]->Release();
			g_orgWindowEx[i] = nullptr;
		}
	}
}

void ResetOrgWindow(void* pWndMan)
{
	if (pWndMan == nullptr || !CreateOrgWindows()) {
		return;
	}

	IWzGr2D* pGr = GetGr();
	IWzVector2D* pCenter = nullptr;
	if (pGr == nullptr || FAILED(pGr->get_center(&pCenter)) || pCenter == nullptr) {
		return;
	}

	const int nScreenWidth = Client::m_nGameWidth;
	const int nScreenHeight = Client::m_nGameHeight;
	IWzVector2D* pOrgWindow = GetWndManOrgWindow(pWndMan);
	SetVectorOrigin(pOrgWindow, pCenter);
	RelMove(pOrgWindow, -(nScreenWidth / 2), -(nScreenHeight / 2));

	for (int i = 0; i < kOriginCount; ++i) {
		int nX = -(nScreenWidth / 2);
		if (i % 3 == kOriginCT) {
			nX += (nScreenWidth - 800) / 2;
		} else if (i % 3 == kOriginRT) {
			nX += nScreenWidth - 800;
		}

		int nY = -(nScreenHeight / 2);
		if (i / 3 == 1) {
			nY += (nScreenHeight - 600) / 2;
		} else if (i / 3 == 2) {
			nY += nScreenHeight - 600;
		}

		SetVectorOrigin(g_orgWindowEx[i], pCenter);
		RelMove(g_orgWindowEx[i], nX, nY);
	}

	pCenter->Release();
}

IWzVector2D* GetOrgWindowEx(void* pWndMan, int nOrigin)
{
	if (nOrigin < 0 || nOrigin >= kOriginCount || !CreateOrgWindows()) {
		return GetWndManOrgWindow(pWndMan);
	}
	return g_orgWindowEx[nOrigin];
}

void __fastcall CWndManConstructor_Hook(void* pThis, void* edx, HWND hWnd)
{
	CWndManConstructor(pThis, edx, hWnd);
	ResetOrgWindow(pThis);
}

void __fastcall CWndManDestructor_Hook(void* pThis, void* edx)
{
	CWndManDestructor(pThis, edx);
	ReleaseOrgWindows();
}

IWzVector2D** __fastcall CWndManGetOrgWindow_Hook(void* pThis, void* edx, IWzVector2D** result)
{
	const DWORD ret = reinterpret_cast<DWORD>(_ReturnAddress());
	IWzVector2D* pOrgWindow = nullptr;

	switch (ret) {
	case 0x008DEB75:
	case 0x008DEE11:
		pOrgWindow = GetOrgWindowEx(pThis, kStatusBarOrigin);
		break;
	default:
		if (ret >= 0x008D01B2 && ret <= 0x008D3ADF) {
			pOrgWindow = GetOrgWindowEx(pThis, kStatusBarOrigin);
		} else {
			pOrgWindow = GetWndManOrgWindow(pThis);
		}
		break;
	}

	*result = pOrgWindow;
	if (pOrgWindow != nullptr) {
		pOrgWindow->AddRef();
	}
	return result;
}

void __fastcall CWndCreateWnd_Hook(void* pThis, void* edx, int nLeft, int nTop, int nWidth, int nHeight, int z, int bScreenCoord, void* pData, int bSetFocus)
{
	CWndCreateWnd(pThis, edx, nLeft, nTop, nWidth, nHeight, z, bScreenCoord, pData, bSetFocus);
	if (!bScreenCoord) {
		return;
	}

	const DWORD ret = reinterpret_cast<DWORD>(_ReturnAddress());
	switch (ret) {
	case 0x0051FA03:
	case 0x008CFD65:
	{
		IWzGr2DLayer* pLayer = GetWndLayer(pThis);
		if (pLayer == nullptr) {
			return;
		}

		auto ppWndMan = reinterpret_cast<void**>(0x00BEC20C);
		if (*ppWndMan == nullptr) {
			return;
		}

		IWzVector2D* pOrigin = GetOrgWindowEx(*ppWndMan, kStatusBarOrigin);
		SetVectorOrigin(pLayer, pOrigin);
		break;
	}
	default:
		break;
	}
}

} // namespace

bool HookStatusBarPosition(bool bEnable)
{
	bool ok = true;
	ok = Memory::SetHook(bEnable, reinterpret_cast<void**>(&CWndManConstructor), CWndManConstructor_Hook) && ok;
	ok = Memory::SetHook(bEnable, reinterpret_cast<void**>(&CWndManDestructor), CWndManDestructor_Hook) && ok;
	ok = Memory::SetHook(bEnable, reinterpret_cast<void**>(&CWndManGetOrgWindow), CWndManGetOrgWindow_Hook) && ok;
	ok = Memory::SetHook(bEnable, reinterpret_cast<void**>(&CWndCreateWnd), CWndCreateWnd_Hook) && ok;
	return ok;
}
