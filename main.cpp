//======================================================
//
//メイン処理[main.cpp]
//Author:石原颯馬
//
//======================================================
#include <Windows.h>
#include <string.h>
#include "main.h"
#include "manager.h"

//マクロ定義
#define WINDOW_NAME			"逃走者ピクト 市街の破壊者"
#define FPS_SPEED			(500)	//FPS計測時間

//プロトタイプ宣言
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//========================
//メイン関数
//========================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int CmdShow)
{
	CManager* pManager = nullptr;
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };	//ウィンドウの領域（書き換え不可の定数）

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),					//メモリサイズ
		CS_CLASSDC,							//ウィンドウのスタイル
		WindowProc,							//ウィンドウプロシージャ
		0,									//使用しない
		0,									//使用しない
		hInstance,							//インスタンスハンドル
		LoadIcon(nullptr,IDI_APPLICATION),		//タスクバーのアイコン
		LoadCursor(nullptr,IDC_ARROW),			//マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1),			//クライアント領域の背景色
		nullptr,								//メニューバー
		CLASS_NAME,							//クラスの名前
		LoadIcon(nullptr,IDI_APPLICATION)		//アプリケーションのアイコン
	};

	HWND hWnd;	//ウィンドウハンドル
	MSG msg;	//メッセージ格納

	//時刻関係
	DWORD dwCurrentTime;		//現在時刻
	DWORD dwExecLastTime;		//最後に処理した時刻
	DWORD dwFPSTime;			//最後にFPS計測した時刻

	//ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	//クライアント領域を指定サイズに調整
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//ウィンドウを生成
	hWnd = CreateWindowEx(
		0,									//拡張ウィンドウスタイル
		CLASS_NAME,							//ウィンドウクラスの名前
		WINDOW_NAME,						//ウィンドウの名前
		WS_OVERLAPPEDWINDOW,				//ウィンドウスタイル
		CW_USEDEFAULT,						//ウィンドウの左上X座標
		CW_USEDEFAULT,						//ウィンドウの左上Y座標
		(rect.right - rect.left),			//ウィンドウの幅
		(rect.bottom - rect.top),			//ウィンドウの高さ
		nullptr,								//親ウィンドウのハンドル
		nullptr,								//メニューハンドルまたは子ウィンドウID
		hInstance,							//インスタンスハンドル
		nullptr								//ウィンドウ作成データ
	);

	//マネージャ生成
	pManager = new CManager;

	//マネージャ初期化
	if (FAILED(pManager->Init(hInstance, hWnd, TRUE)))
	{
		return -1;
	}

	//分解能設定
	timeBeginPeriod(1);

	dwCurrentTime = 0;		//初期化
	dwExecLastTime = timeGetTime();
	dwFPSTime = timeGetTime();

	//ウィンドウの表示
	ShowWindow(hWnd, CmdShow);
	UpdateWindow(hWnd);

	//メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) != 0)
		{//windowsの処理
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				//メッセージの設定
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{//DirectXの処理
			dwCurrentTime = timeGetTime();
			if (dwCurrentTime - dwFPSTime >= FPS_SPEED)
			{
				pManager->CheckFPS(dwCurrentTime, dwFPSTime);
				dwFPSTime = dwCurrentTime;
			}
			if ((dwCurrentTime - dwExecLastTime) >= PROC_SPEED)
			{//60分の1秒経過
				dwExecLastTime = dwCurrentTime;
				//更新処理
				pManager->Update();

				//描画処理
				pManager->Draw();
			}
		}
	}

	//マネージャ破棄
	if (pManager != nullptr)
	{
		//終了
		pManager->Uninit();
		delete pManager;
		pManager = nullptr;
	}

	//クラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//========================
//ウィンドウプロシージャ
//========================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;

	switch (uMsg)
	{
	case WM_DESTROY:
		//WM_QUITメッセージを送る
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO);

			if (nID == IDYES)
			{
				DestroyWindow(hWnd);	//Destroyメッセージを送る
			}
			break;
		}
		break;
	case WM_CLOSE:
		nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO);

		if (nID == IDYES)
		{
			DestroyWindow(hWnd);	//Destroyメッセージを送る
		}
		else
		{
			return 0;
		}
		break;
	case WM_CREATE:
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}