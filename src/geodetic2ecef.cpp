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
#include <geocon/geodetic2ecef.h>
#include <Eigen/Eigen>
#include <miniglog/logging.h>
#include "geocon_init.h"

namespace geocon {

using namespace MSP::CCS;
using namespace Eigen;

static GeodeticParameters kGeodeticParameters(
    CoordinateType::geodetic,
    HeightType::ellipsoidHeight);

static GeodeticParameters kECEFParameters(
    CoordinateType::geocentric,
    HeightType::ellipsoidHeight);

geodetic2ecef* geodetic2ecef::Create() {
  if (!init()) {
    LOG(ERROR) << "Failed to initialize GeoCon";
    return nullptr;
  }

  try {
    return new geodetic2ecef(
        new CoordinateConversionService("WGE", &kGeodeticParameters,
                                        "WGE", &kECEFParameters));
  } catch (CoordinateConversionException& e) {
    LOG(ERROR) << "Failed to create ccs_: " << e.getMessage();
  }
  return nullptr;
}

geodetic2ecef::geodetic2ecef(CoordinateConversionService* ccs) : ccs_(ccs) {}

bool geodetic2ecef::to_ecef(
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

  return true;
}

bool geodetic2ecef::to_geodetic(
    double x, double y, double z, double std,
    GeodeticCoordinates* geo, Accuracy* acc) const {
  CHECK(false);
  return false;
}
}  // namespace geocon
