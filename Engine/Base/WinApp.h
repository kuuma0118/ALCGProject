#pragma once
#include <Windows.h>
#include <cstdint>

#include "Externals/imgui/imgui_impl_win32.h"
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

/// <summary>
/// ウィンドウズアプリケーション
/// </summary>
class WinApp {
public:
	//ウィンドウサイズ
	static const int32_t kClientWidth = 1280;
	static const int32_t kClientHeight = 720;

	/// <summary>
	/// シングルトンインスタンスを取得
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
	static WinApp* GetInstance();

	/// <summary>
	/// ウィンドウプロシージャ
	/// </summary>
	/// <param name="hwnd">ウィンドウハンドル</param>
	/// <param name="msg">メッセージ番号</param>
	/// <param name="wparam">メッセージ情報1</param>
	/// <param name="lparam">メッセージ情報2</param>
	/// <returns>成否</returns>
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	/// <summary>
	/// ゲームウィンドウの作成
	/// </summary>
	/// <param name="title">ウィンドウタイトル</param>
	/// <param name="clientWidth">クライアント領域の幅</param>
	/// <param name="clientHeight">クライアント領域の高さ</param>
	void CreateGameWindow(const wchar_t* title, int32_t clientWidth, int32_t clientHeight);

	/// <summary>
	/// ゲームウィンドウを閉じる
	/// </summary>
	void CloseGameWindow();

	/// <summary>
	/// メッセージの処理
	/// </summary>
	/// <returns>終了かどうか</returns>
	bool ProcessMessage();

	/// <summary>
	/// ウィンドウハンドルを取得
	/// </summary>
	/// <returns>ウィンドウハンドル</returns>
	HWND GetHwnd() const { return hwnd_; };

	/// <summary>
	/// インスタンスハンドルの取得
	/// </summary>
	/// <returns>インスタンスハンドル</returns>
	HINSTANCE GetHInstance() { return wc_.hInstance; };

private:
	WinApp() = default;
	~WinApp() = default;
	WinApp(const WinApp&) = delete;
	const WinApp& operator=(const WinApp&) = delete;

private:
	//ウィンドウクラス
	WNDCLASS wc_{};
	//ウィンドウサイズ
	RECT wrc_{};
	//ウィンドウハンドル
	HWND hwnd_{};

};

