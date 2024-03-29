// Fill out your copyright notice in the Description page of Project Settings.


#include "VoxelWorld.h"
#include "RawVolume.h"
#include "MarchingCubesSurfaceExtractor.h"
#include "TerrainPager.h"

using namespace PolyVox;

struct FPolyVoxVector : FVector
{
	FORCEINLINE FPolyVoxVector()
	{
	}

	explicit FORCEINLINE FPolyVoxVector(const EForceInit E)
		: FVector(E)
	{
	}

	explicit FORCEINLINE FPolyVoxVector(const float InX, const float InY, const float InZ)
		: FVector(InX, InY, InZ)
	{
	}

	explicit FORCEINLINE FPolyVoxVector(const FVector& InVec)
	{
		FVector::operator=(InVec);
	}

	explicit FORCEINLINE FPolyVoxVector(const Vector3DFloat& InVec)
	{
		operator=(InVec);
	}

	FORCEINLINE FVector& operator=(const Vector3DFloat& Other)
	{
		this->X = Other.getX();
		this->Y = Other.getY();
		this->Z = Other.getZ();

		DiagnosticCheckNaN();

		return *this;
	}
};

// Sets default values
AVoxelWorld::AVoxelWorld()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Generated Mesh"));
	Mesh->bUseAsyncCooking = true;
	RootComponent = Mesh;
}

// Called when the game starts or when spawned
void AVoxelWorld::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AVoxelWorld::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AVoxelWorld::GenerateWorld()
{
	VoxelVolume = MakeShareable(new PagedVolume<float>(new TerrainPager(NoiseGenerator, Amplitude)));

	Region ToExtract(Vector3DInt32(0, 0, 0), Vector3DInt32(127, 127, 127));
	auto ExtractedMesh = extractMarchingCubesMesh(VoxelVolume.Get(), ToExtract);
	auto DecodedMesh = decodeMesh(ExtractedMesh);

	auto Vertices = TArray<FVector>();
	auto Indices = TArray<int32>();
	auto Normals = TArray<FVector>();
	auto UV0 = TArray<FVector2D>();
	auto Colors = TArray<FColor>();
	auto Tangents = TArray<FProcMeshTangent>();

	int32 NumIndices = DecodedMesh.getNoOfIndices();
	UE_LOG(LogTemp, Error, TEXT("%d indices"), NumIndices)

	for (auto i = 0; i < NumIndices; i++)
	{
		auto Index0 = DecodedMesh.getIndex(i++);
		auto Index1 = DecodedMesh.getIndex(i++);
		auto Index2 = DecodedMesh.getIndex(i);

		auto Vertex0 = DecodedMesh.getVertex(Index0);
		auto V0 = FPolyVoxVector(Vertex0.position) * VoxelSize;

		auto Vertex1 = DecodedMesh.getVertex(Index1);
		auto V1 = FPolyVoxVector(Vertex1.position) * VoxelSize;

		auto Vertex2 = DecodedMesh.getVertex(Index2);
		auto V2 = FPolyVoxVector(Vertex2.position) * VoxelSize;

		Indices.Add(Vertices.Add(V2));
		Indices.Add(Vertices.Add(V1));
		Indices.Add(Vertices.Add(V0));

		const auto Edge01 = V1 - V0;
		const auto Tangent = FProcMeshTangent(Edge01.GetSafeNormal(), false);

		Tangents.Add(Tangent);
		Tangents.Add(Tangent);
		Tangents.Add(Tangent);

		if (FlatShading)
		{
			const auto Edge02 = V2 - V0;
			const auto Normal = (Edge01 ^ Edge02).GetSafeNormal();
			Normals.Add(Normal);
			Normals.Add(Normal);
			Normals.Add(Normal);
		}
		else
		{
			Normals.Add(FPolyVoxVector(Vertex2.normal));
			Normals.Add(FPolyVoxVector(Vertex1.normal));
			Normals.Add(FPolyVoxVector(Vertex0.normal));
		}

		UV0.Add(FVector2D(0.0f, 0.0f));
		UV0.Add(FVector2D(0.0f, 0.0f));
		UV0.Add(FVector2D(0.0f, 0.0f));

		Colors.Add(FColor(255, 255, 255));
		Colors.Add(FColor(255, 255, 255));
		Colors.Add(FColor(255, 255, 255));
	}

	// Finally create the mesh
	Mesh->CreateMeshSection(0, Vertices, Indices, Normals, UV0, Colors, Tangents, true);
}
