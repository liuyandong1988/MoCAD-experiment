// This is free and unencumbered software released into the public domain.
//
// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non - commercial, and by any
// means.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/MeshComponent.h"
#include "RoadRunnerTrafficJunction.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(RoadRunnerTraffic, Log, All);

// Various structs to organize data and make it edittable in Unreal

////////////////////////////////////////////////////////////////////////////////
// Represents the "LightState" element in the metadata
USTRUCT()
struct FLightBulbState
{
	GENERATED_USTRUCT_BODY()

	// Original name of the lightbulb mesh
	UPROPERTY(EditAnywhere, Category = "Traffic")
	FString Name;

	// Whether the lightbulb should be on or not
	UPROPERTY(EditAnywhere, Category = "Traffic")
	bool State = false;
};

////////////////////////////////////////////////////////////////////////////////
// Represents the "Configuration" element in the metadata
USTRUCT()
struct FSignalConfiguration
{
	GENERATED_USTRUCT_BODY()

	// Name of the configuration.
	UPROPERTY(EditAnywhere, Category = "Traffic")
	FString Name;

	// List of light bulb states
	UPROPERTY(EditAnywhere, Category = "Traffic")
	TArray<FLightBulbState> LightBulbStates;
};

////////////////////////////////////////////////////////////////////////////////
// Represents the "Signal" element under "SingalAssets" in the metadata
USTRUCT()
struct FSignalAsset
{
	GENERATED_USTRUCT_BODY()

	// Holds a reference to a lightbulb's static mesh component in the imported scene's actor
	UPROPERTY(EditAnywhere, Category = "Traffic")
	FString Id;

	// Whether the lightbulb should be on or not
	UPROPERTY(EditAnywhere, Category = "Traffic")
	TArray<FSignalConfiguration> SignalConfigurations;
};

////////////////////////////////////////////////////////////////////////////////
// Generated from translating the IDs of FSignalState for use during runtime
USTRUCT()
struct FLightInstanceState
{
	GENERATED_USTRUCT_BODY()

	// Set after first lookup during runtime
	UPROPERTY(EditAnywhere, Category = "Traffic")
	UMeshComponent* MeshComponent = nullptr;

	// Modified lightbulb name, set during import
	UPROPERTY(EditAnywhere, Category = "Traffic")
	FString ComponentName;

	// Whether the lightbulb should be on or not
	UPROPERTY(EditAnywhere, Category = "Traffic")
	bool State = false;
};

////////////////////////////////////////////////////////////////////////////////
// Represents the "Signal" state element under "Interval" in the metadata
USTRUCT()
struct FSignalState
{
	GENERATED_USTRUCT_BODY()

	// ID of the signal object
	UPROPERTY(EditAnywhere, Category = "Traffic")
	FString Id;

	// ID of the signal asset
	UPROPERTY(EditAnywhere, Category = "Traffic")
	FString SignalAssetId;

	// Index into the configuration list in the associated signal asset
	UPROPERTY(EditAnywhere, Category = "Traffic")
	int Configuration = 0;

	// Runtime translation of the imported data
	UPROPERTY(EditAnywhere, Category = "Traffic")
	TArray<FLightInstanceState> LightInstanceStates;
};

////////////////////////////////////////////////////////////////////////////////
// Represents the "Interval" element in the metadata
USTRUCT()
struct FLightInterval
{
	GENERATED_USTRUCT_BODY()

	// Duration of the interval
	UPROPERTY(EditAnywhere, Category = "Traffic")
	float Time = 0.0f;

	// List of signal states for this interval
	UPROPERTY(EditAnywhere, Category = "Traffic")
	TArray<FSignalState> SignalStates;
};

////////////////////////////////////////////////////////////////////////////////
// Represents the "SignalPhase" element in the metadata
USTRUCT()
struct FSignalPhase
{
	GENERATED_USTRUCT_BODY()

	// List of intervals (i.e. green, yellow, red)
	UPROPERTY(EditAnywhere, Category = "Traffic")
	TArray<FLightInterval> Intervals;
};

////////////////////////////////////////////////////////////////////////////////
// Represents the "Junction" element in the metadata
USTRUCT()
struct FJunction
{
	GENERATED_USTRUCT_BODY()

	// ID of the junction
	UPROPERTY(EditAnywhere, Category = "Traffic")
	FString Id;

	// List of signal phases for this junction
	UPROPERTY(EditAnywhere, Category = "Traffic")
	TArray<FSignalPhase> SignalPhases;
};

////////////////////////////////////////////////////////////////////////////////
// URoadRunnerTrafficJunction handles the behaviour of each junction to 
// change the traffic lights
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROADRUNNERRUNTIME_API URoadRunnerTrafficJunction : public UActorComponent
{
	GENERATED_BODY()

public:

	// Holds a list of phases to loop through (i.e. Traffic going east/west -> Traffic going north/south -> Left Turns)
	UPROPERTY(EditAnywhere, Category = "Traffic")
	TArray<FSignalPhase> Phases;

	// Timer to keep track of interval switches
	UPROPERTY()
	float Timer = 0.0f;

	// Keeps track of which phase the junction is on
	UPROPERTY(EditAnywhere, Category = "Traffic")
	int CurrentPhase = 0;

	// Keeps track of which phase the junction is on
	UPROPERTY(EditAnywhere, Category = "Traffic")
	int CurrentInterval = 0;

	// Sets default values for this component's properties
	URoadRunnerTrafficJunction();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Get a reference to the current phase
	FSignalPhase& GetCurrentPhase();

	// Get a reference to the current interval
	FLightInterval& GetCurrentInterval();

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Import data from FJunction created from xml
	void SetPhases(FJunction importedData);

private:
	bool ErrorFlag = false;
};
