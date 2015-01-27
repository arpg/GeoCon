// Copyright (c) John Morrison, all rights reserved.  See the
// accompanying LICENSE file for more information.

#include <math.h>
#include <Accuracy.h>
#include <CartesianCoordinates.h>
#include <CoordinateConversionException.h>
#include <CoordinateConversionService.h>
#include <GeodeticCoordinates.h>
#include <GeodeticParameters.h>
#include <HeightType.h>
#include <LocalCartesianParameters.h>
#include <geocon/geodetic2local.h>
#include <Eigen/Eigen>
#include <miniglog/logging.h>
#include "geocon_init.h"

namespace geocon {

using namespace MSP::CCS;
using namespace Eigen;

static inline Matrix3d enu_to_ned() {
  // Rotation from ENU to NED.
  static Matrix3d r_en;
  r_en <<
      0, 1, 0,
      1, 0, 0,
      0, 0, -1;
  return r_en;
}

static GeodeticParameters kEllipsoidParameters(
    CoordinateType::geodetic,
    HeightType::ellipsoidHeight);

geodetic2local* geodetic2local::Create(double latitude,
                                       double longitude,
                                       double altitude) {
  if (!init()) {
    LOG(ERROR) << "Failed to initialize GeoCon";
    return nullptr;
  }
  LocalCartesianParameters localParameters(
      CoordinateType::localCartesian,
      // Longitude, latitude, altitude, orientation.
      longitude, latitude, altitude, 0);
  try {
    return new geodetic2local(
        new CoordinateConversionService("WGE", &kEllipsoidParameters,
                                        "WGE", &localParameters));
  } catch (CoordinateConversionException& e) {
    LOG(ERROR) << "Failed to create ccs_: " << e.getMessage();
  }
  return nullptr;
}

geodetic2local::geodetic2local(CoordinateConversionService* ccs) : ccs_(ccs) {}

bool geodetic2local::to_local(
    double latitude, double longitude, double altitude, double std,
    CartesianCoordinates* cart, Accuracy* acc) const {
  CHECK_NOTNULL(cart);
  CHECK_NOTNULL(acc);
  GeodeticCoordinates geo_coords(CoordinateType::geodetic,
                                 longitude, latitude, altitude);
  Accuracy geo_acc;

  // 90% confidence interval is 1.64 STD
  geo_acc.setSphericalError90(std * 1.64);

  try {
    ccs_->convertSourceToTarget(&geo_coords, &geo_acc, *cart, *acc);
  } catch (CoordinateConversionException& e) {
    LOG(ERROR) << "Failed to convert coordinates: " << e.getMessage();
    return false;
  }

  static Matrix3d r_enu_to_ned = enu_to_ned();
  Vector3d v_ned(r_enu_to_ned * Vector3d(cart->x(), cart->y(), cart->z()));
  cart->set(v_ned[0], v_ned[1], v_ned[2]);
  return true;
}

bool geodetic2local::to_geodetic(
    double x, double y, double z, double std,
    GeodeticCoordinates* geo, Accuracy* acc) const {
  CHECK(false);
  return false;
}
}  // namespace geocon
