// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "functional"
#include "util/PerlinNoise.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "promap.generated.h"

UCLASS()
class SANDSTORM_API Apromap : public AActor
{
    GENERATED_BODY()
    
public:
    
    // ------------- Properties
    // Generated mesh
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Promap")
    UProceduralMeshComponent* mesh;
    
    //configuration
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Promap")
    int32 width;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Promap")
    int32 height;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Promap")
    int32 uv_scaling;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Promap")
    FVector2D origin;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Promap")
    float node_spacing;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Promap")
    float senoid_amplitude;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Promap")
    float senoid_stretching;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Promap")
    float noise_amplitude_mul;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Promap")
    float noise_stretching_mul;
    
    float generated = false;
    
    int32 n_nodes;
    
    int32 n_triangles;
    
    PerlinNoise noise;
    
    // parameter comparison
    int32 last_width;
    int32 last_height;
    int32 last_uv_scaling;
    FVector2D last_origin;
    float last_node_spacing;
    float last_senoid_amplitude;
    float last_senoid_stretching;
    float last_noise_amplitude_mul;
    float last_noise_stretching_mul;
    
    //generation components
    TArray<FVector> vertices;
    TArray<int32> triangles;
    TArray<FVector> normals;
    TArray<FVector2D> UVs;
    //    TArray<FColor>* vertexColors;
    //    TArray<FProcMeshTangent>* tangents;
    
    
    // ------------- Callables
    
    // Sets default values for this actor's properties
    Apromap();
    
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    
    // Called every frame
    virtual void Tick( float DeltaSeconds ) override;
    
    UFUNCTION(BlueprintCallable, Category = "Promap")
    void GenerateMesh();
    
    UFUNCTION(BlueprintCallable, Category = "Promap")
    bool ParametersChanged();
    
    UFUNCTION(BlueprintCallable, Category = "Promap")
    void SaveCurrentParameters();
    
    void node_iter(std::function<void(int, int, int)> l);
    
    float heightmap(double x, double y);
    
    double senoidal(double x, double y);
};
