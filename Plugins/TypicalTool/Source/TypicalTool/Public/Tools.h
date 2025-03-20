// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Misc/App.h"
#include <string>
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFilemanager.h"
#include "Framework/Application/SlateApplication.h"
#include "Widgets/SWindow.h"
#include "GameFramework/GameUserSettings.h"
#include "Memory.h"
#include "Engine/GameViewportClient.h"
#include "Runtime/Slate/Public/Framework/Application/SlateApplication.h"
#include "Runtime/Launch/Resources/Version.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/PlayerController.h"

#include "Engine/World.h"
#include "Templates/UnrealTemplate.h"


/**
 * 
 */
class TYPICALTOOL_API Tools
{
public:
	Tools();
	~Tools();
};

namespace UETypicalTool {
	template<class Target>
	UUserWidget* GetViewportWidget(APlayerController* PlayerController)
	{
		if (!PlayerController)
		{
			UE_LOG(LogTemp, Warning, TEXT("PlayerController is not properly initialized."));
			return nullptr;
		}

		// 创建一个数组，用于存储找到的 Widget
		TArray<UUserWidget*> FoundWidgets;

		// 调用静态方法获取所有视口上的 Widget
		UWidgetBlueprintLibrary::GetAllWidgetsOfClass(
			PlayerController,
			FoundWidgets,
			Target::StaticClass(),
			false // 是否仅获取可见的 Widget
		);

		if (FoundWidgets.Num() > 0) {
			return FoundWidgets[0];
		}
		else {
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, TEXT("AVehicleTestPawn: 没有找到任何 Widget 对象!"));
			}
		}

		return nullptr;
	}
	template<class Target>
	TArray<UUserWidget*> GetViewportWidgets(APlayerController* PlayerController)
	{
		if (!PlayerController)
		{
			UE_LOG(LogTemp, Warning, TEXT("PlayerController is not properly initialized."));
			return TArray<UUserWidget*>();
		}

		// 创建一个数组，用于存储找到的 Widget
		TArray<UUserWidget*> FoundWidgets;

		// 调用静态方法获取所有视口上的 Widget
		UWidgetBlueprintLibrary::GetAllWidgetsOfClass(
			PlayerController,
			FoundWidgets,
			Target::StaticClass(),
			false // 是否仅获取可见的 Widget
		);

		if (FoundWidgets.Num() > 0) {
			return FoundWidgets;
		}
		else {
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, TEXT("AVehicleTestPawn: 没有找到任何 Widget 对象!"));
			}
		}

		return TArray<UUserWidget*>();
	}

	/*
	* Key: [-1]新消息 | [Num]更换指定消息
	* LogShowTime: [-1]显示一天
	* bNewOnTop: 文本位置 屏幕底(false)/顶(true)
	* TextScale: 文本缩放
	*/
	template<class T = bool>
	void DebugLog(int32 Key, float LogShowTime, FColor TextColor, const FString& Text, bool bNewOnTop = true, FVector2D TextScale = UE::Math::TVector2<double>::UnitVector)
	{
		if (LogShowTime == -1.f) {
			LogShowTime = 3600 * 24; //显示 1 天
		}

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(Key, LogShowTime, TextColor, Text, bNewOnTop, TextScale);
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("UEtytl::DebugLog -> GEngine == nullptr"));
		}
	}
	template<class T = bool>
	void DebugLog(const FString& Text, FColor TextColor = FColor::Yellow, int32 Key = -1.f, float LogShowTime = -1.f, bool bNewOnTop = true, FVector2D TextScale = UE::Math::TVector2<double>::UnitVector)
	{
		if (LogShowTime == -1.f) {
			LogShowTime = 3600 * 24; //显示 1 天
		}

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(Key, LogShowTime, TextColor, Text, bNewOnTop, TextScale);
			if (TextColor == FColor::Red) {
				UE_LOG(LogTemp, Error, TEXT("%s"), *Text);
			}
			else if (TextColor == FColor::Yellow) {
				UE_LOG(LogTemp, Warning, TEXT("%s"), *Text);
			}
			else {
				UE_LOG(LogTemp, Log, TEXT("%s"), *Text);
			}
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("UEtytl::DebugLog -> GEngine == nullptr"));
		}
	}

	//template<class T = bool>
	//bool SaveGameData(const FString& DataPath, const Json::Value& JsonValue)
	//{
	//	// Jsoncpp
	//	UEtytl::JsonManage SaveGame;
	//	SaveGame.Init(Tools::FStringToStdWstring(DataPath), false);
	//	SaveGame.SetJsonValue(JsonValue);

	//	if (SaveGame.WriteJsonFile()) {
	//		return true;
	//	}
	//	else {
	//		UE_LOG(LogTemp, Error, TEXT("Tools::SaveGameData 保存失败! 未成功打开文件[%s]!"), *DataPath);
	//		return false;
	//	}
	//}
	//template<class T = bool>
	//bool LoadGameData(const FString& DataPath, Json::Value& JsonValue, Json::Value& DefaultJsonValue)
	//{
	//	// Jsoncpp
	//	UEtytl::JsonManage SaveGame;
	//	if (SaveGame.Init(Tools::FStringToStdWstring(DataPath), true)) {
	//		JsonValue = SaveGame.GetJsonValue();
	//		return true;
	//	}
	//	else {
	//		UE_LOG(LogTemp, Error, TEXT("Tools::LoadGameData 读取失败! 未成功打开文件[%s]!"), *DataPath);
	//		SaveGame.SetJsonValue(DefaultJsonValue);
	//		if (SaveGame.WriteJsonFile()) {
	//			UE_LOG(LogTemp, Error, TEXT("Tools::LoadGameData 初始化成功!"), *DataPath);
	//		}
	//		else {
	//			UE_LOG(LogTemp, Error, TEXT("Tools::LoadGameData 初始化失败!"), *DataPath);
	//		}
	//		return false;
	//	}
	//}

	/*
	* ErrorMessage: 判断[文件错误 | !文件错误]
	*/
	template<class T = bool>
	bool ReadJsonFile(const TCHAR* JsonFilePath, TSharedPtr<FJsonObject>& ParsedJsonObject, FString& ErrorMessage)
	{
		// 读取文件内容
		FString InputString;
		if (!FFileHelper::LoadFileToString(InputString, JsonFilePath)) {
			ErrorMessage = TEXT("文件错误");
			DebugLog(FString::Printf(TEXT("UEtytl::ReadJsonFile: 读取 JSON 文件失败! 路径: [%s]"), JsonFilePath));
			return false;
		}

		// 反序列化 JSON 字符串
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(InputString);
		if (!FJsonSerializer::Deserialize(Reader, ParsedJsonObject)) {
			ErrorMessage = TEXT("非文件错误");
			DebugLog(FString::Printf(TEXT("UEtytl::ReadJsonFile: 解析 JSON 字符串失败! 错误: {%s}"), *Reader->GetErrorMessage()));
			return false;
		}

		return true;
	}
	/*
	* ErrorMessage: 判断[文件错误 | !文件错误]
	*/
	template<class T = bool>
	bool WriteJsonFile(const TCHAR* JsonFilePath, TSharedPtr<FJsonObject>& ParsedJsonObject, FString& ErrorMessage)
	{
		//序列化
		FString OutputString;
		TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
		if (!FJsonSerializer::Serialize(ParsedJsonObject.ToSharedRef(), Writer)) {
			ErrorMessage = TEXT("非文件错误");
			return false;
		}

		// 保存到文件
		if (!FFileHelper::SaveStringToFile(OutputString, JsonFilePath)) {
			ErrorMessage = TEXT("文件错误");
			UE_LOG(LogTemp, Error, TEXT("UEtytl::WriteJsonFile: 写入 JSON 文件失败! 路径: [%s]"), JsonFilePath);
			return false;
		}

		return true;
	}

	template<class T = bool>
	bool template_GameData()
	{
		// 创建一个 JSON 对象
		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
		{

			// 设置字符串字段
			JsonObject->SetStringField(TEXT("Name"), TEXT("Unreal Engine"));

			// 设置数字字段
			JsonObject->SetNumberField(TEXT("Version"), 5.4);

			// 设置布尔字段
			JsonObject->SetBoolField(TEXT("IsReleased"), true);

			// 设置数组字段
			TArray<TSharedPtr<FJsonValue>> JsonArray;
			JsonArray.Add(MakeShareable(new FJsonValueString(TEXT("Feature1"))));
			JsonArray.Add(MakeShareable(new FJsonValueString(TEXT("Feature2"))));
			JsonObject->SetArrayField(TEXT("Features"), JsonArray);

			// 设置对象字段
			TSharedPtr<FJsonObject> NestedObject = MakeShareable(new FJsonObject());
			NestedObject->SetStringField(TEXT("SubFeature"), TEXT("Nested Feature"));
			JsonObject->SetObjectField(TEXT("NestedObject"), NestedObject);
		}

		//反序列化
		{
			FString InputString = TEXT("{\"Name\":\"Unreal Engine\",\"Version\":5.4,\"IsReleased\":true}");
			TSharedPtr<FJsonObject> ParsedJsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(InputString);

			if (FJsonSerializer::Deserialize(Reader, ParsedJsonObject))
			{
				// 访问字段
				FString Name = ParsedJsonObject->GetStringField(TEXT("Name"));
				int32 Version = ParsedJsonObject->GetNumberField(TEXT("Version"));
				bool IsReleased = ParsedJsonObject->GetBoolField(TEXT("IsReleased"));
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON string"));
			}
		}
	}



	/// <summary>
	/// 调用控制台命令: 关闭 ToneMapping后处理效果
	/// </summary>
	/// <param name="_PlayerController 玩家控制器"></param>
	UFUNCTION(BlueprintCallable)
	template<class T = bool>
	void ConsoleCommand_CloseToneMapping(APlayerController* _PlayerController) {
		if (!_PlayerController) {
			DebugLog(TEXT("ConsoleCommand_CloseToneMapping: 玩家控制器为空!"));
		}
		else {
			FString Command = TEXT("ShowFlag.Tonemapper 0");
			_PlayerController->ConsoleCommand(*Command);
			Command = TEXT("ShowFlag.Tonemapper 0");
			_PlayerController->ConsoleCommand(*Command);
			Command = TEXT("r.TonemapperGamma 0");
			_PlayerController->ConsoleCommand(*Command);
			Command = TEXT("r.TonemapperFilm 0");
			_PlayerController->ConsoleCommand(*Command);
			Command = TEXT("r.Tonemapper.Sharpen 0");
			_PlayerController->ConsoleCommand(*Command);
			Command = TEXT("r.Tonemapper.GrainQuantization 0");
			_PlayerController->ConsoleCommand(*Command);
			Command = TEXT("r.Tonemapper.Quality 0");
			_PlayerController->ConsoleCommand(*Command);
		}
	}


	// 窗口显示与渲染
	// * Show: bool
	// * Render: [0](暂停渲染)/[0<](恢复渲染)
	template<class T = bool>
	void WindowShowAndRender(bool _bShowWindow, int32 _RenderFrameRate)
	{
		// 主窗口隐藏/显示
		if (FSlateApplication::IsInitialized()) {
			TSharedPtr<SWindow> MainWindow = FSlateApplication::Get().GetActiveTopLevelWindow();
			if (MainWindow.IsValid()) {
				if (_bShowWindow) {
					MainWindow->SetTitle(FText::FromString(TEXT("设置")));
					MainWindow->ShowWindow();
				}
				else {
					MainWindow->HideWindow();
				}
			}
		}

		// 暂停渲染
		static IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.SetRenderingEnabled"));
		if (CVar) {
			CVar->Set(_RenderFrameRate); // 0 表示暂停渲染
		}

		// 降低帧率以减少性能消耗
		//if (GEngine)
		//{
		//    GEngine->SetMaxFPS(60); // 将帧率限制为10 FPS
		//}
	}

	/* 设置窗口大小与模式(全屏/窗口/无边框)
	* * _Fullscreen: 0(全屏)/1(无边框)/2(窗口)
	*/
	template<class T = bool>
	void SetWindowResolution(int32 _Width, int32 _Height, EWindowMode::Type _Fullscreen)
	{
		if (GEngine) {
			// 获取游戏用户设置
			UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
			if (UserSettings) {
				// 设置分辨率
				UserSettings->SetScreenResolution(FIntPoint(_Width, _Height));

				// 设置全屏模式
				UserSettings->SetFullscreenMode(_Fullscreen);

				// 应用设置（不检查命令行参数）
				UserSettings->ApplySettings(false);

				DebugLog(FString::Printf(TEXT("UEtytl::SetWindowResolution: Width[%d]/Height[%d]/ScreenMode[%d]!"), _Width, _Height, _Fullscreen));
			}
			else {
				DebugLog(TEXT("UEtytl::SetWindowResolution: 游戏用户设置无效!"));
			}
		}
	}

	template<class T = bool>
	void ExitGame(UWorld* World)
	{
		if (!World) {
			return;
		}

		// 获取第一个玩家控制器
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (!PlayerController) {
			return;
		}

		// 调用 QuitGame 函数退出游戏
		UKismetSystemLibrary::QuitGame(World, PlayerController, EQuitPreference::Quit, false);
	}

	/*
	* _CreateFolderName: 创建的文件夹名称
	* _TargetFolderPath: 创建文件夹的路径(默认为内容文件夹)
	*/
	template<class T = bool>
	bool CreateDirectorys(FString _CreateFolderName, FString _TargetFolderPath = FPaths::ProjectContentDir())
	{
		// 组合成完整的路径: 获取内容目录的路径 + 创建的文件夹名称
		FString NewFolderPath = FPaths::Combine(_TargetFolderPath, _CreateFolderName);
		IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile(); // 获取平台文件管理器

		// 检查文件夹是否存在
		if (!PlatformFile.DirectoryExists(*NewFolderPath))
		{
			bool bSuccess = PlatformFile.CreateDirectory(*NewFolderPath); // 创建文件夹
			if (bSuccess) {
				DebugLog(FString::Printf(TEXT("UEtytl::CreateDirectory: 成功创建文件夹：[%s]"), *NewFolderPath), FColor::Green);

				return true;
			}
			else {
				DebugLog(FString::Printf(TEXT("UEtytl::CreateDirectory: 无法创建文件夹：[%s]"), *NewFolderPath), FColor::Yellow);
				return false;
			}
		}
		else {
			DebugLog(FString::Printf(TEXT("UEtytl::CreateDirectory: 文件夹已存在：[%s]"), *NewFolderPath));
			return true;
		}
	}
}

namespace UEtytl = UETypicalTool;