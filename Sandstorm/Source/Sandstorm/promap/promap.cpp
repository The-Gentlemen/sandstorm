// Fill out your copyright notice in the Description page of Project Settings.

#include "Sandstorm.h"
#include "promap.h"
#include "util/PerlinNoise.h"
#include "util/tools.h"
#include <iostream>
#include <cmath>
#include <ctime>

// Sets default values
Apromap::Apromap()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;
    this->RootComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
    
    this->mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
    
    this->SetActorEnableCollision(true);
    
    generated = false;
    // PARAMETERS
    width = 100;
    height = 100;
    uv_scaling = 1;
    origin = FVector2D(0, 0);
    node_spacing = 50;
    senoid_amplitude = 5;
    senoid_stretching = 5;
    noise_amplitude_mul = 300;
    noise_stretching_mul = 3;
    // END PARAMETERS
    
    this->mesh->CreateMeshSection(1, TArray<FVector>(), TArray<int32>(), TArray<FVector>(), TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>(), true);
    this->mesh->AttachTo(RootComponent);
}

bool Apromap::ParametersChanged()
{
    bool changed = false;
    
    changed = changed || width != last_width;
    changed = changed || height != last_height;
    changed = changed || uv_scaling != last_uv_scaling;
    changed = changed || origin != last_origin;
    changed = changed || node_spacing != last_node_spacing;
    changed = changed || senoid_amplitude != last_senoid_amplitude;
    changed = changed || senoid_stretching != last_senoid_stretching;
    changed = changed || noise_amplitude_mul != last_noise_amplitude_mul;
    changed = changed || noise_amplitude_mul != last_noise_amplitude_mul;
    
    return changed;
}

void Apromap::SaveCurrentParameters()
{
    last_width = width;
    last_height = height;
    last_uv_scaling = uv_scaling;
    last_origin = origin;
    last_node_spacing = node_spacing;
    last_senoid_amplitude = senoid_amplitude;
    last_senoid_stretching = senoid_stretching;
    last_noise_amplitude_mul = noise_amplitude_mul;
    last_noise_amplitude_mul = noise_amplitude_mul;
}


void Apromap::GenerateMesh()
{
    if (generated && !ParametersChanged()) {
        return;
    }
    generated = true;
    
    // set the number of nodes
    n_nodes = width * height;
    n_triangles = 6 * (width-1) * (height-1);
    
    auto begin_t = time(NULL);
    
    generated = true;
    // cleanup
    vertices.Reset();
    triangles.Reset();
    normals.Reset();
    UVs.Reset();
    
    
    vertices.SetNum(n_nodes);
    triangles.SetNum(n_triangles);
    normals.SetNum(n_nodes);
    UVs.SetNum(n_nodes);
    
    int min = 99999999;
    int max = 0;
    
    // calculate z for each vertex in the grid
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
    
    // remove the z offset, leaving median(z) = 0
    node_iter([&](int x, int y, int i){
        
        vertices[i].Z -= min + (max - min)/2;
        
    });
    
    // generate the face triangles (striped)
    node_iter([&](int x, int y, int i){
        
        int j = x * (width-1) + y;
        if (x <= height-2 && y <= width-2)
        {
            triangles[6*j+0] = i;
            triangles[6*j+1] = i + width;
            triangles[6*j+2] = i + 1;
            
            triangles[6*j+3] = i + 1;
            triangles[6*j+4] = i + width;
            triangles[6*j+5] = i + width + 1;
        }
    });
    
    // generate the normal vectors
    node_iter([&](int x, int y, int i){
        
        auto& v = vertices[i]; // current vertex
        auto& vyp = i - 1 >= 0          ? vertices[i - 1]     : vertices[i]; // previous vertex on y direction
        auto& vyn = i + 1 < n_nodes     ? vertices[i + 1]     : vertices[i]; // next vertex in y direction
        auto& vxp = i - width >= 0      ? vertices[i - width] : vertices[i]; // previous vertex on x direction
        auto& vxn = i + width < n_nodes ? vertices[i + width] : vertices[i]; // next vertex in x direction
        
        double dya  = FVector::Dist(vyp, v); // length of the previous y segment
        double dyb = FVector::Dist(v, vyn); // length of the next y segment
        double dyc = FVector::Dist(vyp, vyn); // length of the next y segment
        
        double dxa  = FVector::Dist(vxp, v); // length of the previous x segment
        double dxb = FVector::Dist(v, vxn); // length of the next x segment
        double dxc = FVector::Dist(vxp, vxn); // length of the next x segment
        
        double yalpha = ((dya * dya) + (dyb * dyb) - (dyc * dyc)) / (2 * dya * dyb); // angle between the previous y-segment and the next y-segment
        double xalpha = ((dxa * dxa) + (dxb * dxb) - (dxc * dxc)) / (2 * dxa * dxb); // angle between the previous x-segment and the next x-segment
        
        //get rid of floating point errors
        if(yalpha > 1) { yalpha = 1;}
        if(yalpha < -1) { yalpha = -1;}
        if(xalpha > 1) { xalpha = 1;}
        if(xalpha < -1) { xalpha = -1;}
        
        yalpha = acos(yalpha);
        xalpha = acos(xalpha);
        
        double ybeta = ((2 * PI) - yalpha) / 2; // angle of the normal along the y axis
        double xbeta = ((2 * PI) - xalpha) / 2; // angle of the normal along the x axis
        
        auto ynorm = FVector(0, cos(ybeta), sin(ybeta));
        auto xnorm = FVector(cos(ybeta), 0, sin(ybeta));
        
        auto norm = ((xnorm + ynorm) / 2);
        
        normals[i] = norm.ContainsNaN() ? FVector(0,0,0) : norm;
        //normals[i] = FVector(0,0,1);
        //std::cout << "dya =" << dya << " dyb =" << dyb << " dyc =" << dyc << " dxa =" << dxa << " dxb =" << dxb << " dxc =" << dxc << " yalpha =" << yalpha << " xalpha =" << xalpha << " ybeta =" << ybeta << " xbeta =" << xbeta << "\n";
        //std::cout << "X =" << normals[i].X << " Y =" << normals[i].Y << " Z =" << normals[i].Z << "\n";
        
    });
    
    // generate UV map
    float curX[width]; for(int i = 0; i < height; i++) { curX[i] = 0; };
    float curY = 0;
    node_iter([&](int x, int y, int i){
        
        if (y==0) {
            curY = 0;
        }
        
        auto& v1 = vertices[i];
        auto& v2 = i + width < n_nodes ? vertices[i + width] : vertices[i];
        auto& v3 = i + 1 < n_nodes     ? vertices[i + 1]     : vertices[i];
        
        auto wa = FVector::Dist(v1, v3) / (width * node_spacing * uv_scaling); // width of the first triangle in the square
        auto ha = FVector::Dist(v1, v2) / (height * node_spacing * uv_scaling); // height of the first triangle in the square
        
        UVs[i] = FVector2D(curX[y], curY);
        
        curY += wa;
        curX[y] += ha;
        
        
    });
    
    // With default options
    //this->mesh->CreateMeshSection(1, vertices, triangles, TArray<FVector>(), TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>(), true);
    //this->mesh->CreateMeshSection(1, *vertices, *triangles, *normals, *UV0, *vertexColors, *tangents, false);
    
    this->mesh->CreateMeshSection(1, vertices, triangles, normals, UVs, TArray<FColor>(), TArray<FProcMeshTangent>(), true);
    
    auto end_t = time(NULL);
    
    auto took = end_t - begin_t;
    
    
    
    std::cout << "\n\n\n" << "Promap generation took:" << took << "\n\n\n";
}

void Apromap::node_iter(std::function<void(int, int, int)> l)
{
    for (int x : range(height))
    {
        for (int y : range(width))
        {
            int i = x + y * width;
            l(x, y, i);
        }
    }
}

float Apromap::heightmap(double x, double y)
{
    double x_scale = 4;
    double y_scale = 4;
    
    double sen = senoid_amplitude * senoidal(x / senoid_stretching, y / senoid_stretching);
    
    double noise_x = x/(x_scale * noise_stretching_mul);
    double noise_y = y/(y_scale * noise_stretching_mul);
    
    double znoise = noise_amplitude_mul * this->noise.noise(noise_x, noise_y, 0.8);
    
    
    return znoise + sen;
}

double Apromap::senoidal(double x, double y)
{
    double coef[] = {0.6, 0.4, 0.3, 0.1, 0.8};

    double s1 = sin(x)*sin(y); // high fequency (2pi)

    double s2 = sin(x/89)*sin(y/93); // low fequency (~90)

    double s3_a = sin( (x+y) / 23.4 ) * sin( (x-y) / 17.8); // medium frequency ~66
    double s3_b = pow( ( sin(x+y+33)+1.001 ), ( std::abs(cos( (x+y+33)/11)) ) ) / 2; // noise

    double s3 = s3_a * s3_b;

    double px = std::abs((double)x)+0.1;
    double py = std::abs((double)x)+0.1;
    double s4 = cos( -sqrt(x*y) ) * cos( pow((x/py),2) ) * cos( pow((y/px),2) ); // variable frequecy, tends to stability on x=y, noisy on x=0 and y=0

    double theta = (sin( (x*y)/10 ) + 1)/2;
    double s5 = sin((x*theta + y*(1-theta))) * sin((x*0.5 + y*0.5)); // high variation noise with very long period. somewhat stable along x=y, but its negligible

    auto zmax = 0;
    for (auto &&c : coef)
    {
        zmax += 2 * std::abs((double)c);
    }

    return ( s1 * coef[0] + s2 * coef[1] + s3 * coef[2] + s4 * coef[3] + s5 * coef[4]) / zmax;
}

// Called when the game starts or when spawned
void Apromap::BeginPlay()
{
    Super::BeginPlay();
    GenerateMesh();
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