#pragma once

#include <CoreMinimal.h>
#include <Engine/EngineBaseTypes.h>
#include <Kismet/BlueprintFunctionLibrary.h>
#include "DaeTestInputBlueprintFunctionLibrary.generated.h"

class UInputAction;

/** Utility functions for simulating input in automated tests. */
UCLASS(meta = (RestrictedToClasses = "DaeTestActor"))
class DAEDALICTESTAUTOMATIONPLUGIN_API UDaeTestInputBlueprintFunctionLibrary
    : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /** Applies the input action with the specified name once. */
    UFUNCTION(BlueprintCallable, Category = "TestAutomation|Input",
              meta = (HidePin = "Context", DefaultToSelf = "Context"))
    static void ApplyInputAction(UObject* Context, const FName& ActionName,
                                 EInputEvent InputEventType = EInputEvent::IE_Pressed);

    /** Applies the input axis with the specified name. Pass AxisValue 0.0f to reset the input axis. */
    UFUNCTION(BlueprintCallable, Category = "TestAutomation|Input",
              meta = (HidePin = "Context", DefaultToSelf = "Context"))
    static void ApplyInputAxis(UObject* Context, const FName& AxisName, float AxisValue = 1.0f);

    // --- Enhanced Input (the modern UE5 path; drives UInputAction assets, not legacy mappings) ---
    // These inject a value for a UInputAction on player 0's UEnhancedInputLocalPlayerSubsystem.
    // NOTE: an injected value lasts a SINGLE input tick. To simulate a sustained hold, re-call
    // every frame across the latent Act (e.g. inside a DelayFrames loop); stop calling to release.

    /** Enhanced Input: inject a digital (bool) value for a UInputAction on player 0. */
    UFUNCTION(BlueprintCallable, Category = "TestAutomation|Input",
              meta = (HidePin = "Context", DefaultToSelf = "Context"))
    static void InjectDigital(UObject* Context, UInputAction* Action, bool bValue = true);

    /** Enhanced Input: inject an Axis1D (float) value for a UInputAction on player 0. */
    UFUNCTION(BlueprintCallable, Category = "TestAutomation|Input",
              meta = (HidePin = "Context", DefaultToSelf = "Context"))
    static void InjectAxis1D(UObject* Context, UInputAction* Action, float Value = 1.0f);

    /** Enhanced Input: inject an Axis2D (Vector2D) value for a UInputAction on player 0. */
    UFUNCTION(BlueprintCallable, Category = "TestAutomation|Input",
              meta = (HidePin = "Context", DefaultToSelf = "Context"))
    static void InjectAxis2D(UObject* Context, UInputAction* Action, FVector2D Value);
};
