// Copyright (c) John Morrison, all rights reserved.  See the
// accompanying LICENSE file for more information.

#pragma once

#include <memory>
#include <Accuracy.h>
#include <CartesianCoordinates.h>
#include <GeodeticCoordinates.h>
#include <CoordinateConversionService.h>

namespace geocon {

/**
 * Convert easily between an Earth Centered Earth Fixed (ECEF)
 * coordinate system and WGS84 Geodetic coordinates.
 *
 */
class geodetic2ecef {
 public:
  virtual ~geodetic2ecef() {}

  /**
   * Create using initial geodetic coordinates in radians.
   *
   * Can return nullptr if initialization fails (possibly because of
   * missing data).
   */
  static geodetic2ecef* Create();

  /**
   * Convert from WGS84 geodetic to ECEF.
   *
   * @param latitude In radians
   * @param longitude In radians
   * @param altitude In meters. Techinically, Height Above Ellipsoid.
   * @param std Spherical standard deviation of position error in meters.
   */
  bool to_ecef(
      double latitude, double longitude, double altitude, double std,
      MSP::CCS::CartesianCoordinates* cart, MSP::CCS::Accuracy* acc) const;

  /**
   * Convert from ECEF into WGS84 Geodetic coordinates
   */
  bool to_geodetic(
      double x, double y, double z, double std,
      MSP::CCS::GeodeticCoordinates* geo, MSP::CCS::Accuracy* acc) const;

 private:
  // Create using initial geodetic coordinates in radians
  geodetic2ecef(MSP::CCS::CoordinateConversionService* ccs);

  std::unique_ptr<MSP::CCS::CoordinateConversionService> ccs_;
};
}  // namespace geocon
