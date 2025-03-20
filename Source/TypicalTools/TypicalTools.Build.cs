// Copyright Epic Games, Inc. All Rights Reserved.

using System.Collections.Generic;
using System.IO;
using UnrealBuildTool;

public class TypicalTools : ModuleRules
{
	public TypicalTools(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        //// 定义路径
        //string ThirdPartyPath = Path.Combine(ModuleDirectory, "ThirdParty");
        //string IncludePath = Path.Combine(ThirdPartyPath, "Include"); //通用的引用头文件的路径
        //string LibPath = Path.Combine(ThirdPartyPath, "Lib"); //通用的引用头文件的路径

        ////头文件路径
        //string LibPath_libTypical = Path.Combine(LibPath, "libTypical");

        //// 区分平台
        //string LibraryFilePath_libTypical = "";

        //string ConfigurationFolder_Win = ""; // Debug_Win/Release_Win/Debug/Release
        //string ConfigurationFolder_DebugAndRelease = ""; //只有 Debug/Release
        //string FileExtension_Win = ".lib";

        //ConfigurationFolder_DebugAndRelease = Target.Configuration == UnrealTargetConfiguration.Debug ? "Debug" : "Release";
        //if (Target.Platform == UnrealTargetPlatform.Win64) //Win
        //{
        //    // 根据配置选择库路径
        //    PublicDefinitions.Add("_WINDOWS");

        //    ConfigurationFolder_Win = Target.Configuration == UnrealTargetConfiguration.Debug ? "Debug_Win" : "Release_Win";
        //    LibraryFilePath_libTypical = Path.Combine(LibPath_libTypical, ConfigurationFolder_Win, "libTypical" + FileExtension_Win);
        //}

        //// 添加包含路径
        //PublicIncludePaths.Add(ModuleDirectory + "/ThirdParty/Include");
        //PublicIncludePaths.AddRange(new string[] { IncludePath });

        if (Target.Platform == UnrealTargetPlatform.Win64) //Win
        {
            PublicDefinitions.Add("_WINDOWS");
            PublicAdditionalLibraries.Add("Advapi32.lib");
        }


        PublicIncludePaths.AddRange(new string[] { ModuleDirectory });
        //// 添加库文件
        //PublicAdditionalLibraries.AddRange(new string[] { LibraryFilePath_libTypical });


        PublicDependencyModuleNames.AddRange(new string[] { 
            "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG", "Slate", "SlateCore", "Json", "JsonUtilities", "TypicalTool", "libTypicalLibrary"
        });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
