#include "DaeTestInputBlueprintFunctionLibrary.h"
#include "DaeTestLogCategory.h"
#include <InputKeyEventArgs.h>
#include <GameFramework/InputSettings.h>
#include <GameFramework/PlayerController.h>
#include <GameFramework/PlayerInput.h>
#include <Kismet/GameplayStatics.h>
#include <EnhancedInputSubsystems.h>
#include <InputAction.h>
#include <InputActionValue.h>
#include <Engine/LocalPlayer.h>

namespace
{
    UEnhancedInputLocalPlayerSubsystem* GetTestEnhancedInputSubsystem(UObject* Context)
    {
        APlayerController* PlayerController = UGameplayStatics::GetPlayerController(Context, 0);
        if (!IsValid(PlayerController))
        {
            return nullptr;
        }

        ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
        if (!IsValid(LocalPlayer))
        {
            return nullptr;
        }

        return LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
    }

    void InjectActionValue(UObject* Context, UInputAction* Action, const FInputActionValue& Value,
                           const TCHAR* Caller)
    {
        if (!IsValid(Action))
        {
            UE_LOG(LogDaeTest, Error, TEXT("%s - Action is null"), Caller);
            return;
        }

        UEnhancedInputLocalPlayerSubsystem* Subsystem = GetTestEnhancedInputSubsystem(Context);
        if (Subsystem == nullptr)
        {
            UE_LOG(LogDaeTest, Error, TEXT("%s - no Enhanced Input subsystem for player 0"), Caller);
            return;
        }

        Subsystem->InjectInputForAction(Action, Value,
                                        TArray<UInputModifier*>(), TArray<UInputTrigger*>());
    }
}

void UDaeTestInputBlueprintFunctionLibrary::ApplyInputAction(
    UObject* Context, const FName& ActionName,
    EInputEvent InputEventType /*= EInputEvent::IE_Pressed*/)
{
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(Context, 0);

    const UInputSettings* InputSettings = GetDefault<UInputSettings>();

    for (const FInputActionKeyMapping& Mapping : InputSettings->GetActionMappings())
    {
        if (Mapping.ActionName == ActionName)
        {
            FInputKeyEventArgs KeyEventArgs = FInputKeyEventArgs::CreateSimulated(Mapping.Key, InputEventType, 1.0f);
            PlayerController->InputKey(KeyEventArgs);
            return;
        }
    }

    UE_LOG(LogDaeTest, Error, TEXT("%s - Input action not found: %s"),
           IsValid(Context) ? *Context->GetName() : TEXT(""), *ActionName.ToString());
}

void UDaeTestInputBlueprintFunctionLibrary::ApplyInputAxis(UObject* Context, const FName& AxisName,
                                                           float AxisValue /*= 1.0f*/)
{
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(Context, 0);

    if (!IsValid(PlayerController))
    {
        return;
    }

    const UInputSettings* InputSettings = GetDefault<UInputSettings>();

    for (const FInputAxisKeyMapping& Mapping : InputSettings->GetAxisMappings())
    {
        if (Mapping.AxisName == AxisName)
        {
            FInputKeyEventArgs KeyEventArgs = FInputKeyEventArgs::CreateSimulated(Mapping.Key, IE_Axis, AxisValue);
            PlayerController->InputKey(KeyEventArgs);
            return;
        }
    }

    UE_LOG(LogDaeTest, Error, TEXT("%s - Input axis not found: %s"),
           IsValid(Context) ? *Context->GetName() : TEXT(""), *AxisName.ToString());
}

void UDaeTestInputBlueprintFunctionLibrary::InjectDigital(UObject* Context, UInputAction* Action, bool bValue)
{
    InjectActionValue(Context, Action, FInputActionValue(bValue), TEXT("InjectDigital"));
}

void UDaeTestInputBlueprintFunctionLibrary::InjectAxis1D(UObject* Context, UInputAction* Action, float Value)
{
    InjectActionValue(Context, Action, FInputActionValue(Value), TEXT("InjectAxis1D"));
}

void UDaeTestInputBlueprintFunctionLibrary::InjectAxis2D(UObject* Context, UInputAction* Action, FVector2D Value)
{
    InjectActionValue(Context, Action, FInputActionValue(Value), TEXT("InjectAxis2D"));
}
