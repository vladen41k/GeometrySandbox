// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGeometryActor.h"
#include "Engine/Engine.h"
#include "TimerManager.h"
#include "Materials/MaterialInstanceDynamic.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseGeometry, All, All);

// Sets default values
ABaseGeometryActor::ABaseGeometryActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Вот этим вот мы задаём Mesh и у нас появляется физический объект
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	SetRootComponent(BaseMesh);
}

// Called when the game starts or when spawned
void ABaseGeometryActor::BeginPlay() {
	Super::BeginPlay();

	InitialLocation = GetActorLocation();

	SetColor(GeometryData.Color);

	GetWorldTimerManager().SetTimer(TimerHandle, this, &ABaseGeometryActor::OnTimerFired, GeometryData.TimerRate, true);



	FTransform Transform = GetActorTransform();
	FVector Location = Transform.GetLocation();
	FRotator Rotation = Transform.Rotator();
	FVector Scale = Transform.GetScale3D();

	UE_LOG(LogBaseGeometry, Warning, TEXT("Actor name %s"), *GetName());
	UE_LOG(LogBaseGeometry, Warning, TEXT("Transform %s"), *Transform.ToString());
	UE_LOG(LogBaseGeometry, Warning, TEXT("Location %s"), *Location.ToString());
	UE_LOG(LogBaseGeometry, Warning, TEXT("Rotator %s"), *Rotation.ToString());
	UE_LOG(LogBaseGeometry, Warning, TEXT("Scale %s"), *Scale.ToString());

	UE_LOG(LogBaseGeometry, Error, TEXT("Human Transform %s"), *Transform.ToHumanReadableString());

	//printStringTypes();
	//printTipes();
}

// Called every frame
void ABaseGeometryActor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	switch (GeometryData.MoveType) {
		case EMovementType::Sin: {
			FVector CurrentLocation = GetActorLocation();
			float time = GetWorld()->GetTimeSeconds();
			CurrentLocation.Z = InitialLocation.Z + GeometryData.Amplitude * FMath::Sin(GeometryData.Frequency * time);

			SetActorLocation(CurrentLocation);
		}
		break;
		case EMovementType::Static: break;
		default: break;
	}
}

void ABaseGeometryActor::printTipes()
{
	UE_LOG(LogBaseGeometry, Warning, TEXT("Actor name %s"), *GetName());
	UE_LOG(LogBaseGeometry, Warning, TEXT("Weapon num: %d, Kills num: %i"), WeaponsNum, KillsNum);
	UE_LOG(LogBaseGeometry, Warning, TEXT("Health: %.2f"), Health);
	UE_LOG(LogBaseGeometry, Warning, TEXT("Is Death: %d"), IsDeath);
	UE_LOG(LogBaseGeometry, Warning, TEXT("Has Weapons: %d"), static_cast<int>(HasWeapons));
}

void ABaseGeometryActor::printStringTypes() {
	UE_LOG(LogBaseGeometry, Display, TEXT("info"));

	FString Name = "Jon Conor";

	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, Name, true, FVector2D(1.5f, 1.5f));
}

void ABaseGeometryActor::SetColor(const FLinearColor& Color) {
	UMaterialInstanceDynamic* DynMaterial = BaseMesh->CreateAndSetMaterialInstanceDynamic(0);
	if (DynMaterial) {
		DynMaterial->SetVectorParameterValue("Color", Color);
	}
}

void ABaseGeometryActor::OnTimerFired() {
	if (++TimerCount <= MaxTimerCount) {
		const FLinearColor NewColor = FLinearColor::MakeRandomColor();
		UE_LOG(LogBaseGeometry, Display, TEXT("Timer count: %i, Color to set up: %s"), TimerCount, *NewColor.ToString());
		SetColor(NewColor);
	} else {
		UE_LOG(LogBaseGeometry, Display, TEXT("Timer has been stopped!"))
		GetWorldTimerManager().ClearTimer(TimerHandle);
	}
}	