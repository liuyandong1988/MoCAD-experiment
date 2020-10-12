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

#include "RoadRunnerTrafficJunction.h"
#include "Components/MeshComponent.h"

DEFINE_LOG_CATEGORY(RoadRunnerTraffic);

////////////////////////////////////////////////////////////////////////////////
// Sets default values for this component's properties
URoadRunnerTrafficJunction::URoadRunnerTrafficJunction()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

////////////////////////////////////////////////////////////////////////////////
// Called when the game starts
void URoadRunnerTrafficJunction::BeginPlay()
{
	Super::BeginPlay();
}

////////////////////////////////////////////////////////////////////////////////
// Called every frame
void URoadRunnerTrafficJunction::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Phases.Num() == 0)
		return;

	FSignalPhase& signalPhase = GetCurrentPhase();

	if (signalPhase.Intervals.Num() == 0)
		return;

	FLightInterval& interval = GetCurrentInterval();

	Timer += DeltaTime;

	// Set lightbulb state
	for (auto& signalState : interval.SignalStates)
	{
		for (auto& lightInstanceState : signalState.LightInstanceStates)
		{
			if (!lightInstanceState.MeshComponent)
			{
				FComponentReference compRef;
				compRef.ComponentProperty = FName(*lightInstanceState.ComponentName);
				compRef.OtherActor = NULL;
				lightInstanceState.MeshComponent = Cast<UMeshComponent>(compRef.GetComponent(GetOwner()));
			}
			
			if (lightInstanceState.MeshComponent)
			{
				lightInstanceState.MeshComponent->SetVisibility(lightInstanceState.State);
			}
			else
			{
				if (!ErrorFlag)
				{
					UE_LOG(RoadRunnerTraffic, Error, TEXT("Light State not set up properly in %s."), *GetName());
					ErrorFlag = true;
				}
			}
		}
	}

	// Update current state
	if (Timer > interval.Time)
	{
		Timer = 0;
		CurrentInterval++;
		if (CurrentInterval >= signalPhase.Intervals.Num())
		{
			CurrentInterval = 0;
			CurrentPhase++;
			if (CurrentPhase >= Phases.Num())
			{
				CurrentPhase = 0;
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////

FSignalPhase& URoadRunnerTrafficJunction::GetCurrentPhase()
{
	return Phases[CurrentPhase];
}

////////////////////////////////////////////////////////////////////////////////

FLightInterval& URoadRunnerTrafficJunction::GetCurrentInterval()
{
	return GetCurrentPhase().Intervals[CurrentInterval];
}

////////////////////////////////////////////////////////////////////////////////

void URoadRunnerTrafficJunction::SetPhases(FJunction importedData)
{
	Phases = importedData.SignalPhases;
}
