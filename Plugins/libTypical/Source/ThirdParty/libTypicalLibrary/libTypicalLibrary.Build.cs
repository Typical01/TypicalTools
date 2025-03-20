// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using UnrealBuildTool;

public class libTypicalLibrary : ModuleRules
{
	public libTypicalLibrary(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;
		PublicSystemIncludePaths.Add("$(ModuleDir)/Public");

        string IncludePath_libTypical = Path.Combine(ModuleDirectory, "Include");
        string LibPath_libTypical = Path.Combine(ModuleDirectory, "Lib");

        // 区分平台
        string LibraryFilePath_libTypical = "";
        string ConfigurationFolder_DebugAndRelease = ""; //Debug/Release...Debug_Win/Release_Win...

        // 根据配置选择库路径
        if (Target.Platform == UnrealTargetPlatform.Win64) //Win
        {
            PublicDefinitions.Add("_WINDOWS");

            PublicDefinitions.Add("_CRT_SECURE_NO_WARNINGS");
            PublicDefinitions.Add("_SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING");

            ConfigurationFolder_DebugAndRelease = Target.Configuration == UnrealTargetConfiguration.Debug ? "Debug_Win" : "Release_Win";
            LibraryFilePath_libTypical = Path.Combine(LibPath_libTypical, ConfigurationFolder_DebugAndRelease, "libTypical.lib");
        }
        else
        {
            ConfigurationFolder_DebugAndRelease = Target.Configuration == UnrealTargetConfiguration.Debug ? "Debug" : "Release";
            LibraryFilePath_libTypical = Path.Combine(LibPath_libTypical, ConfigurationFolder_DebugAndRelease, "libTypical.lib");
        }


        // 添加包含路径
        //PublicIncludePaths.Add(IncludePath_libTypical);
        PublicIncludePaths.AddRange(new string[] { IncludePath_libTypical });
        // 添加库文件
        PublicAdditionalLibraries.AddRange(new string[] { LibraryFilePath_libTypical });

        //if (Target.Platform == UnrealTargetPlatform.Win64)
        //{
        //	// Add the import library
        //	PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "x64", "Release", "ExampleLibrary.lib"));

        //	// Delay-load the DLL, so we can load it from the right place first
        //	PublicDelayLoadDLLs.Add("ExampleLibrary.dll");

        //	// Ensure that the DLL is staged along with the executable
        //	RuntimeDependencies.Add("$(PluginDir)/Binaries/ThirdParty/libTypicalLibrary/Win64/ExampleLibrary.dll");
        //}
        //else if (Target.Platform == UnrealTargetPlatform.Mac)
        //{
        //	PublicDelayLoadDLLs.Add(Path.Combine(ModuleDirectory, "Mac", "Release", "libExampleLibrary.dylib"));
        //	RuntimeDependencies.Add("$(PluginDir)/Source/ThirdParty/libTypicalLibrary/Mac/Release/libExampleLibrary.dylib");
        //}
        //else if (Target.Platform == UnrealTargetPlatform.Linux)
        //{
        //	string ExampleSoPath = Path.Combine("$(PluginDir)", "Binaries", "ThirdParty", "libTypicalLibrary", "Linux", "x86_64-unknown-linux-gnu", "libExampleLibrary.so");
        //	PublicAdditionalLibraries.Add(ExampleSoPath);
        //	PublicDelayLoadDLLs.Add(ExampleSoPath);
        //	RuntimeDependencies.Add(ExampleSoPath);
        //}
    }
}
