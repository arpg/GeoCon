// Copyright (c) John Morrison, all rights reserved.  See the
// accompanying LICENSE file for more information.
#include <gtest/gtest.h>

#include <geocon/geodetic2local.h>

TEST(geodetic2local, north) {
  std::unique_ptr<geocon::geodetic2local> g2l(
      geocon::geodetic2local::Create(0, 0, 0));

  MSP::CCS::CartesianCoordinates cart;
  MSP::CCS::Accuracy acc;
  g2l->to_local(1, 0, 0, 0, &cart, &acc);
  EXPECT_GT(cart.x(), 0.0);
  EXPECT_EQ(0.0, cart.y());
  EXPECT_GT(cart.z(), 0.0);
}

TEST(geodetic2local, east) {
  std::unique_ptr<geocon::geodetic2local> g2l(
      geocon::geodetic2local::Create(0, 0, 0));

  MSP::CCS::CartesianCoordinates cart;
  MSP::CCS::Accuracy acc;
  g2l->to_local(0, 1, 0, 0, &cart, &acc);
  EXPECT_EQ(0.0, cart.x());
  EXPECT_GT(cart.y(), 0.0);
  EXPECT_GT(cart.z(), 0.0);
}

TEST(geodetic2local, down) {
  std::unique_ptr<geocon::geodetic2local> g2l(
      geocon::geodetic2local::Create(0, 0, 1));

  MSP::CCS::CartesianCoordinates cart;
  MSP::CCS::Accuracy acc;
  g2l->to_local(0, 0, 0, 0, &cart, &acc);
  EXPECT_EQ(0.0, cart.x());
  EXPECT_EQ(0.0, cart.y());
  EXPECT_EQ(1.0, cart.z());
}
