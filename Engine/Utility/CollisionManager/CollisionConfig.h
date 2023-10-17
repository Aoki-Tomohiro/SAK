#pragma once
#include <cstdint>

const uint32_t kCollisionAttributePlayer = 0b0001;
const uint32_t kCollisionAttributeEnemy = 0b0010;
const uint32_t kCollisionAttributeMissile = 0b0100;
const uint32_t kCollisionAttributeLaser = 0b1000;

const uint32_t kCollisionMaskPlayer = 0b1110;
const uint32_t kCollisionMaskEnemy = 0b0001;
const uint32_t kCollisionMaskMissile = 0b0001;
const uint32_t kCollisionMaskLaser = 0b0001;

//形状
const uint32_t kCollisionPrimitiveSphere = 0b1;
const uint32_t kCollisionPrimitiveAABB = 0b1 << 1;