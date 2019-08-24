// Fill out your copyright notice in the Description page of Project Settings.


#include "TerrainPager.h"

using namespace PolyVox;

TerrainPager::TerrainPager(UUFNNoiseGenerator* NoiseGenerator, const float Amplitude): Pager(),
                                                                                       NoiseGenerator(NoiseGenerator),
                                                                                       Amplitude(Amplitude)
{
}

TerrainPager::~TerrainPager()
{
}

void TerrainPager::pageIn(const Region& Region, PagedVolume<float>::Chunk* Chunk)
{
	for (auto X = Region.getLowerX(); X <= Region.getUpperX(); X++)
	{
		for (auto Y = Region.getLowerY(); Y <= Region.getUpperY(); Y++)
		{
			const auto Noise = NoiseGenerator->GetNoise2D(X, Y);
			for (auto Z = Region.getLowerZ(); Z <= Region.getUpperZ(); Z++)
			{
				const auto Value = Z - Noise * Amplitude;
				Chunk->setVoxel(X - Region.getLowerX(),
				                Y - Region.getLowerY(),
				                Z - Region.getLowerZ(),
				                Value * -1);
			}
		}
	}
}

void TerrainPager::pageOut(const Region& Region, PagedVolume<float>::Chunk* Chunk)
{
}
