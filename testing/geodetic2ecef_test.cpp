// Copyright (c) John Morrison, all rights reserved.  See the
// accompanying LICENSE file for more information.
#include <gtest/gtest.h>

#include <geocon/geodetic2ecef.h>

TEST(geodetic2ecef, up) {
  std::unique_ptr<geocon::geodetic2ecef> g2e(geocon::geodetic2ecef::Create());

  MSP::CCS::CartesianCoordinates cart;
  MSP::CCS::Accuracy acc;
  g2e->to_ecef(1, 0, 0, 0, &cart, &acc);

  EXPECT_GT(cart.x(), 0.0);
  EXPECT_GT(cart.y(), 0.0);
  EXPECT_GT(cart.z(), 0.0);
}
