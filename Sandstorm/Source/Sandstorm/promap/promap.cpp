// Fill out your copyright notice in the Description page of Project Settings.

#include "Sandstorm.h"
#include "promap.h"
#include "util/PerlinNoise.h"
#include "util/tools.h"
#include <iostream>
#include "math.h"

// Sets default values
Apromap::Apromap()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;
    this->RootComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
    
    this->mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
    
    this->SetActorEnableCollision(true);
    
    width = 100;
    height = 100;
    origin = FVector2D(0, 0);
    node_spacing = 50;
    senoid_amplitude = 5;
    noise_amplitude_mul = 300;
    noise_stretching_mul = 3;
    
    this->mesh->AttachTo(RootComponent);
    
    this->GenerateMesh();
}

void Apromap::GenerateMesh()
{
    // cleanup
    vertices.Reset();
    triangles.Reset();
    
    n_nodes = width * height;
    
    vertices.SetNum(n_nodes);
    triangles.SetNum(6 * (width-1) * (height-1));
    
    int min = 99999999;
    int max = 0;
    
    node_iter([&](int x, int y, int i){
        
        int z = heightmap(x, y);
        if (z < min) {
            min = z;
        }
        if (z > max) {
            max = z;
        }
        vertices[i] = FVector(origin.X + x * node_spacing, origin.Y + y * node_spacing, z);
        
    });
    
    node_iter([&](int x, int y, int i){
        
        vertices[i].Z -= min + (max - min)/2;
        
    });
    
    node_iter([&](int x, int y, int i){
        
        int j = x * (width-1) + y;
        if (x <= height-2 && y <= width-2)
        {
            triangles[6*j+0] = i;
            triangles[6*j+1] = i + 1;
            triangles[6*j+2] = i + width;
            
            triangles[6*j+3] = i + 1;
            triangles[6*j+4] = i + 1 + width;
            triangles[6*j+5] = i + width;
        }
    });
    
    // With default options
    this->mesh->CreateMeshSection(1, vertices, triangles, TArray<FVector>(), TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>(), true);
    //this->mesh->CreateMeshSection(1, *vertices, *triangles, *normals, *UV0, *vertexColors, *tangents, false);
}

void Apromap::node_iter(std::function<void(int, int, int)> l)
{
    for (int x : range(height))
    {
        for (int y : range(width))
        {
            int i = x * width + y;
            l(x, y, i);
        }
    }
}

float Apromap::heightmap(double x, double y)
{
    double x_scale = 4;
    double y_scale = 4;
    
    double sin_x = (senoid_amplitude / 2) * sin( x/(float)height * PI * 2 * x_scale * 4 );
    
    double cos_y = (senoid_amplitude / 2) * cos( y/(float)width * PI * 2 * y_scale * 4 );
    
    double noise_x = x/(x_scale * noise_stretching_mul);
    double noise_y = y/(y_scale * noise_stretching_mul);
    
    double znoise = noise_amplitude_mul * this->noise.noise(noise_x, noise_y, 0.8);
    
    
    return znoise + sin_x + cos_y;
}

// Called when the game starts or when spawned
void Apromap::BeginPlay()
{
    Super::BeginPlay();
    
}

// Called every frame
void Apromap::Tick( float DeltaTime )
{
    Super::Tick( DeltaTime );
    
}

// CTOR
//    normals = new TArray<FVector>;
//    UV0 = new TArray<FVector2D>;
//    vertexColors = new TArray<FColor>;
//    tangents = new TArray<FProcMeshTangent>;

// GEN
//    normals->Reset();
//    UV0->Reset();
//    vertexColors->Reset();
//    tangents->Reset();
//    vertices->Add(FVector(0, 0, 0));
//    vertices->Add(FVector(0, 100, 0));
//    vertices->Add(FVector(10, 50, 150));
//
//    triangles->Add(0);
//    triangles->Add(1);
//    triangles->Add(2);
//
//    normals->Add(FVector(1, 0, 0));
//    normals->Add(FVector(1, 0, 0));
//    normals->Add(FVector(1, 0, 0));
//
//    UV0->Add(FVector2D(0, 0));
//    UV0->Add(FVector2D(0, 10));
//    UV0->Add(FVector2D(10 ,10));
//
//    vertexColors->Add(FColor(100,100,100,100));
//    vertexColors->Add(FColor(100, 100, 100, 100));
//    vertexColors->Add(FColor(100, 100, 100, 100));
//
//    tangents->Add(FProcMeshTangent(1, 1, 1));
//    tangents->Add(FProcMeshTangent(1, 1, 1));
//    tangents->Add(FProcMeshTangent(1, 1, 1));

//this->mesh->CreateMeshSection(1, *vertices, *triangles, *normals, *UV0, *vertexColors, *tangents, false);