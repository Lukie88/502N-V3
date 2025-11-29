#pragma once

#include "pros/distance.hpp"
#include "lemlib/api.hpp"

namespace relocalize {

constexpr double FIELD_SIZE_IN = 144.0;
constexpr double FIELD_HALF_IN = FIELD_SIZE_IN / 2.0;
constexpr double MM_TO_IN = 1.0 / 25.4;

struct SensorOffset {
    double x; // inches forward from center (>0 = in front of CoR, <0 = behind)
    double y; // inches left from center (>0 = left, <0 = right)
};

enum class WallAxis {
    // Walls are fixed in field coordinates (not relative to robot heading or start).
    X_POS, // right wall at X = +72
    X_NEG, // left wall at X = -72
    Y_POS, // top wall at Y = +72
    Y_NEG  // bottom wall at Y = -72
};

struct DistanceSensorConfig {
    pros::Distance* sensor;  // pointer to the PROS distance sensor
    SensorOffset offset;     // position of the sensor in ROBOT frame
    double dirX_local;       // sensor-facing direction X component in ROBOT frame (auto-normalized)
    double dirY_local;       // sensor-facing direction Y component in ROBOT frame (auto-normalized)
    WallAxis wall;           // which field wall this sensor is seeing (direction should point toward it)
};

bool computePoseFromDistanceSensors(
    const DistanceSensorConfig* sensors,
    int sensorCount,
    const lemlib::Pose& currentPose,
    lemlib::Pose& outPose
);

bool relocalizeWithDistanceSensors(
    lemlib::Chassis& chassis,
    const DistanceSensorConfig* sensors,
    int sensorCount
);

}