#include "distance_relocalization.hpp"

#include <cmath>

namespace relocalize {

bool computePoseFromDistanceSensors(
    const DistanceSensorConfig* sensors,
    int sensorCount,
    const lemlib::Pose& currentPose,
    lemlib::Pose& outPose
) {
    if (sensors == nullptr || sensorCount <= 0) return false;

    const double thetaRad = currentPose.theta * M_PI / 180.0;
    const double cosT = std::cos(thetaRad);
    const double sinT = std::sin(thetaRad);

    double sumX = 0.0;
    int countX = 0;
    double sumY = 0.0;
    int countY = 0;

    for (int i = 0; i < sensorCount; i++) {
        const DistanceSensorConfig& cfg = sensors[i];
        if (cfg.sensor == nullptr) continue;

        const double distMm = static_cast<double>(cfg.sensor->get());
        if (distMm <= 0) continue; // invalid reading

        const double m = distMm * MM_TO_IN; // convert to inches

        // Rotate sensor offset from robot frame to field frame
        const double offXw = cosT * cfg.offset.x - sinT * cfg.offset.y;
        const double offYw = sinT * cfg.offset.x + cosT * cfg.offset.y;

        // Normalize the direction vector to ensure wall projection uses inches.
        const double dirMag = std::sqrt(cfg.dirX_local * cfg.dirX_local + cfg.dirY_local * cfg.dirY_local);
        if (dirMag <= 1e-9) continue; // degenerate direction vector
        const double dirX_local_unit = cfg.dirX_local / dirMag;
        const double dirY_local_unit = cfg.dirY_local / dirMag;

        // Rotate direction vector from robot frame to field frame
        const double dirXw = cosT * dirX_local_unit - sinT * dirY_local_unit;
        const double dirYw = sinT * dirX_local_unit + cosT * dirY_local_unit;

        // P = C + off_w + m * dir_w hits the known wall -> solve for Cx or Cy.
        switch (cfg.wall) {
            case WallAxis::X_POS: {
                if (std::abs(dirXw) <= 1e-6) {break;} // beam not pointing toward the wall line
                const double wallX = FIELD_HALF_IN;
                const double cx = wallX - (offXw + m * dirXw);
                sumX += cx;
                countX++;
                break;
            }
            case WallAxis::X_NEG: {
                if (std::abs(dirXw) <= 1e-6) {break;} // beam not pointing toward the wall line
                const double wallX = -FIELD_HALF_IN;
                const double cx = wallX - (offXw + m * dirXw);
                sumX += cx;
                countX++;
                break;
            }
            case WallAxis::Y_POS: {
                if (std::abs(dirYw) <= 1e-6) {break;} // beam not pointing toward the wall line
                const double wallY = FIELD_HALF_IN;
                const double cy = wallY - (offYw + m * dirYw);
                sumY += cy;
                countY++;
                break;
            }
            case WallAxis::Y_NEG: {
                if (std::abs(dirYw) <= 1e-6) {break;} // beam not pointing toward the wall line
                const double wallY = -FIELD_HALF_IN;
                const double cy = wallY - (offYw + m * dirYw);
                sumY += cy;
                countY++;
                break;
            }
            default:
                break;
        }
    }

    outPose = currentPose;
    bool updated = false;

    if (countX > 0) {
        outPose.x = sumX / static_cast<double>(countX);
        updated = true;
    }

    if (countY > 0) {
        outPose.y = sumY / static_cast<double>(countY);
        updated = true;
    }

    return updated;
}

bool relocalizeWithDistanceSensors(
    lemlib::Chassis& chassis,
    const DistanceSensorConfig* sensors,
    int sensorCount
) {
    lemlib::Pose current = chassis.getPose();
    // Pose requires explicit construction; seed it with the current pose before correction.
    lemlib::Pose corrected(current.x, current.y, current.theta);

    const bool success = computePoseFromDistanceSensors(sensors, sensorCount, current, corrected);
    if (success) {
        chassis.setPose(corrected.x, corrected.y, corrected.theta);
    }

    return success;
}

}