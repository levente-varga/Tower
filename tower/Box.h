#pragma once
#include "Drawable.h"
#include <random>

class Box : public Drawable {
public:
	Box(
		Graphics& graphics,
		std::mt19937& rng,
		std::uniform_real_distribution<float> adist,
		std::uniform_real_distribution<float> ddist,
		std::uniform_real_distribution<float> odist,
		std::uniform_real_distribution<float> rdist
	);

	void Update(float delta) noexcept override;
	DirectX::XMMATRIX GetTransform() const noexcept;

private:
	float r;
	float roll = 0;
	float pitch = 0;
	float yaw = 0;
	float theta;
	float phi;
	float chi;

	float dRoll;
	float dPitch;
	float dYaw;
	float dTheta;
	float dPhi;
	float dChi;
};