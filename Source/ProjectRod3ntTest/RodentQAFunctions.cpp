// Fill out your copyright notice in the Description page of Project Settings.


#include "RodentQAFunctions.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

bool URodentQAFunctions::LogPlayerLocation(FString FileName, FVector Location)
{
    // 1. Construct the string: "X, Y, Z" followed by a line break
    // We use CSV format because most heatmap tools prefer it.
    FString TextToSave = FString::Printf(TEXT("%f,%f,%f\n"), Location.X, Location.Y, Location.Z);

    // 2. Define the path: Project/Saved/Heatmaps/FileName.txt
    FString SaveDirectory = FPaths::ProjectSavedDir() + "Heatmaps/";
    FString FilePath = SaveDirectory + FileName;

    // 3. Ensure the directory exists, otherwise writing fails
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    if (!PlatformFile.DirectoryExists(*SaveDirectory))
    {
        PlatformFile.CreateDirectory(*SaveDirectory);
    }

    // 4. Append the string to the file
    // The critical part here is 'EFileWrite::FILEWRITE_Append' so we don't overwrite previous data
    return FFileHelper::SaveStringToFile(TextToSave, *FilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), EFileWrite::FILEWRITE_Append);
}

