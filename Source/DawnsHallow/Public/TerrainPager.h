// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PagedVolume.h"
#include "UFNNoiseGenerator.h"

class DAWNSHALLOW_API TerrainPager : public PolyVox::PagedVolume<float>::Pager
{
public:
	TerrainPager(UUFNNoiseGenerator* NoiseGenerator, float Amplitude);
	~TerrainPager();

	void pageIn(const PolyVox::Region& Region, PolyVox::PagedVolume<float>::Chunk* Chunk) override;
	void pageOut(const PolyVox::Region& Region, PolyVox::PagedVolume<float>::Chunk* Chunk) override;

private:
	UUFNNoiseGenerator* NoiseGenerator;

	float Amplitude;
};
