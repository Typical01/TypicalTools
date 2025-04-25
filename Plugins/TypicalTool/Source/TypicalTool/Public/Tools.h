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
#include "Misc/CommandLine.h"
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
#include "HAL/PlatformMisc.h"
#include "Misc/ScopedSlowTask.h"

#include "Engine/World.h"
#include "Templates/UnrealTemplate.h"


/**
 * 
 */
class TYPICALTOOL_API Tools
{
public:
	FString LogFilePath { FPaths::Combine(FPaths::ProjectContentDir(), TEXT("Config")) };
	FString LogFileName { TEXT("Log_TypicalTool.log") };
	bool bInit = false;
	FCriticalSection FileWriteMutex;

public:
	Tools();
	~Tools();

public:
	void Init();

	bool WriteTextToFile(const FString& Text);
};

namespace UETypicalTool {
	inline Tools ToolsPtr;

	/// <summary>
	/// 
	/// </summary>
	/// <param name="Key:	[-1]新消息 | [Num]更换指定消息"></param>
	/// <param name="LogShowTime:	[-1]显示一天"></param>
	/// <param name="TextColor:	文本颜色"></param>
	/// <param name="Text:	文本"></param>
	/// <param name="bNewOnTop:	文本位置 屏幕底(false)/顶(true)"></param>
	/// <param name="TextScale:	文本缩放"></param>
	UFUNCTION(BlueprintCallable, Category = "UEtytl")
	TYPICALTOOL_API void DebugLog(int32 Key, float LogShowTime, FColor TextColor, const FString& Text, bool bNewOnTop = true, FVector2D TextScale = UE::Math::TVector2<double>::UnitVector);
	/// <summary>
	/// 
	/// </summary>
	/// <param name="Key:	[-1]新消息 | [Num]更换指定消息"></param>
	/// <param name="LogShowTime:	[-1]显示一天"></param>
	/// <param name="TextColor:	文本颜色"></param>
	/// <param name="Text:	文本"></param>
	/// <param name="bNewOnTop:	文本位置 屏幕底(false)/顶(true)"></param>
	/// <param name="TextScale:	文本缩放"></param>
	UFUNCTION(BlueprintCallable, Category = "UEtytl")
	TYPICALTOOL_API void DebugLog(const FString& Text, FColor TextColor = FColor::Yellow, int32 Key = -1.f, float LogShowTime = -1.f,
		bool bNewOnTop = true, FVector2D TextScale = UE::Math::TVector2<double>::UnitVector);

	/// <summary>
	/// 获取命令行参数: ParamValue 为空时, 不获取参数值
	/// </summary>
	/// <param name="Param:	参数"></param>
	/// <param name="ParamValue:	参数的值"></param>
	/// <returns></returns>
	TYPICALTOOL_API bool GetCommandLineArgument(const FString& Param, FString* ParamValue = nullptr);

	/// <summary>
	/// 获取视口上的 Widget
	/// </summary>
	/// <typeparam name="Target: 获取对象的类型"></typeparam>
	/// <param name="PlayerController"></param>
	/// <returns></returns>
	template<class Target>
	UUserWidget* GetViewportWidget(APlayerController* PlayerController)
	{
		if (!PlayerController) {
			DebugLog(TEXT("PlayerController is not properly initialized."));
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
			DebugLog(TEXT("AVehicleTestPawn: 没有找到任何 Widget 对象!"));
		}

		return nullptr;
	}

	/// <summary>
	/// 获取视口上的 Widget
	/// </summary>
	/// <typeparam name="Target: 获取对象的类型"></typeparam>
	/// <param name="PlayerController"></param>
	/// <returns></returns>
	template<class Target>
	TArray<UUserWidget*> GetViewportWidgets(APlayerController* PlayerController)
	{
		if (!PlayerController)
		{
			DebugLog(TEXT("PlayerController is not properly initialized."));
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
			DebugLog(TEXT("AVehicleTestPawn: 没有找到任何 Widget 对象!"));
		}

		return TArray<UUserWidget*>();
	}

	/// <summary>
	/// <para>读取Json文件</para>
	/// <para>FJsonObject* JsonObject = MakeShareable(new FJsonObject());	//Json对象</para>
	/// <para>SetStringField(TEXT("Name"), TEXT("Unreal Engine"));			//字符串</para>
	/// <para>SetNumberField(TEXT("Version"), 5.4);							//数字</para>
	/// <para>SetBoolField(TEXT("IsReleased"), true);						//布尔</para>
	/// <para>FJsonValue* JsonArray[];										//数组</para>
	/// <para>Add(MakeShareable(new FJsonValueString(TEXT("Feature1"))));	//数组成员</para>
	/// <para>SetArrayField(TEXT("Features"), JsonArray);</para>
	/// <para>SetStringField(TEXT("SubFeature"), TEXT("Nested Feature"));</para>
	/// <para>SetObjectField(TEXT("NestedObject"), NestedObject);			//嵌套Json对象</para>
	/// </summary>
	/// <param name="JsonFilePath:		Json文件路径"></param>
	/// <param name="ParsedJsonObject:  返回的Json对象"></param>
	/// <param name="ErrorMessage:		错误信息"></param>
	/// <returns></returns>
	TYPICALTOOL_API bool ReadJsonFile(const TCHAR* JsonFilePath, TSharedPtr<FJsonObject>& ParsedJsonObject, FString& ErrorMessage);
	/// <summary>
	/// <para>写入Json文件</para>
	/// <para>FJsonObject* ParsedJsonObject;		//Json对象</para>
	/// <para>GetStringField(TEXT("Name"));			//字符串</para>
	/// <para>GetNumberField(TEXT("Version"));		//数字</para>
	/// <para>GetBoolField(TEXT("IsReleased"));		//布尔</para>
	/// </summary>
	/// <param name="JsonFilePath:		Json文件路径"></param>
	/// <param name="ParsedJsonObject:  返回的Json对象"></param>
	/// <param name="ErrorMessage:		错误信息"></param>
	/// <returns></returns>
	TYPICALTOOL_API bool WriteJsonFile(const TCHAR* JsonFilePath, TSharedPtr<FJsonObject>& ParsedJsonObject, FString& ErrorMessage);

	/// <summary>
	/// 调用控制台命令: 关闭 ToneMapping后处理效果
	/// </summary>
	/// <param name="_PlayerController: 玩家控制器"></param>
	UFUNCTION(BlueprintCallable)
	TYPICALTOOL_API void ConsoleCommand_CloseToneMapping(APlayerController* _PlayerController);
	/// <summary>
	/// 窗口显示与渲染
	/// </summary>
	/// <param name="_bShowWindow: 显示/隐藏窗口"></param>
	/// <param name="_RenderFrameRate: [0](暂停渲染)/[!=0](恢复渲染)"></param>
	/// <param name="bShowLog: 显示日志信息"></param>
	TYPICALTOOL_API void WindowShowAndRender(bool _bShowWindow, int32 _RenderFrameRate, bool bShowLog = false);
	
	/// <summary>
	/// 获取当前视口的窗口
	/// </summary>
	/// <returns></returns>
	TYPICALTOOL_API TSharedPtr<SWindow> GetWindow();

	/// <summary>
	/// 设置窗口大小与模式
	/// </summary>
	/// <param name="_Width: 宽"></param>
	/// <param name="_Height: 高"></param>
	/// <param name="_Fullscreen: 窗口模式([0]全屏/[1]窗口/[2]无边框)"></param>
	TYPICALTOOL_API void SetWindowResolution(int32 _Width, int32 _Height, EWindowMode::Type _Fullscreen);

	/// <summary>
	/// 退出程序
	/// </summary>
	/// <param name="_CallStack:	回调位置"></param>
	TYPICALTOOL_API void QuitGame(FString CallStack = TEXT("NULL"));

#undef CreateDirectory
	/// <summary>
	/// 创建文件夹(UE)
	/// </summary>
	/// <param name="_CreateFolderName: 创建的文件夹名称"></param>
	/// <param name="_TargetFolderPath: 创建文件夹的路径(默认为内容文件夹)"></param>
	/// <returns></returns>
	TYPICALTOOL_API bool CreateDirectory(FString _CreateFolderName, FString _TargetFolderPath = FPaths::ProjectContentDir());
}

namespace UEtytl = UETypicalTool;