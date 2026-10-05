#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "tracker/association.hpp"

#include <cstdint>
#include <vector>

namespace tracker {
namespace {

constexpr std::uint64_t kMsToNs = 1'000'000;

Frame frame_at_ms(std::uint64_t ms, std::vector<Box> boxes) {
  Frame frame{ms * kMsToNs, {}};
  for (const Box& box : boxes) {
    frame.detections.push_back(Detection{box});
  }
  return frame;
}

TEST_CASE("iou: identical boxes score one") {
  CHECK(iou({0, 0, 10, 10}, {0, 0, 10, 10}) == 1.0);
}

TEST_CASE("iou: disjoint boxes score zero") {
  CHECK(iou({0, 0, 10, 10}, {20, 20, 10, 10}) == 0.0);
}

TEST_CASE("iou: half-shifted squares score one third") {
  CHECK(iou({0, 0, 2, 2}, {1, 0, 2, 2}) == doctest::Approx(1.0 / 3.0));
}

TEST_CASE("associator: first frame creates one track per detection") {
  Associator associator{Config{}};
  associator.update(frame_at_ms(0, {{0, 0, 60, 60}, {300, 0, 60, 60}}));
  CHECK(associator.tracks().size() == 2);
}

TEST_CASE("associator: stationary detection keeps track count") {
  Associator associator{Config{}};
  for (std::uint64_t ms = 0; ms < 1000; ms += 33) {
    associator.update(frame_at_ms(ms, {{100, 100, 60, 60}}));
  }
  CHECK(associator.tracks().size() == 1);
}

TEST_CASE("associator: detection below threshold spawns new track") {
  Associator associator{Config{}};
  associator.update(frame_at_ms(0, {{0, 0, 60, 60}}));
  associator.update(frame_at_ms(33, {{55, 0, 60, 60}}));
  CHECK(associator.tracks().size() == 2);
}

TEST_CASE("associator: track survives one missed frame") {
  Associator associator{Config{}};
  associator.update(frame_at_ms(0, {{0, 0, 60, 60}}));
  associator.update(frame_at_ms(33, {}));
  REQUIRE(associator.tracks().size() == 1);
  CHECK(associator.tracks()[0].coasting());
}

TEST_CASE("associator: track dropped after many missed frames") {
  Associator associator{Config{}};
  associator.update(frame_at_ms(0, {{0, 0, 60, 60}}));
  for (std::uint64_t ms = 33; ms <= 330; ms += 33) {
    associator.update(frame_at_ms(ms, {}));
  }
  CHECK(associator.tracks().empty());
}

TEST_CASE("associator: two targets crossing at 30 Hz stay two tracks") {
  Associator associator{Config{}};
  std::uint64_t ms = 0;
  for (double step = 0; step <= 200; step += 5, ms += 33) {
    associator.update(frame_at_ms(ms, {{step, 0, 60, 60}, {200 - step, 20, 60, 60}}));
  }
  CHECK(associator.tracks().size() == 2);
}

TEST_CASE("associator: two targets crossing at 5 Hz keep their ids") {
  Associator associator{Config{}};
  std::uint64_t ms = 0;
  associator.update(frame_at_ms(ms, {{0, 0, 60, 60}, {200, 20, 60, 60}}));
  const int left_id = associator.tracks()[0].id;
  const int right_id = associator.tracks()[1].id;
  for (double step = 30; step <= 210; step += 30) {
    ms += 200;
    associator.update(frame_at_ms(ms, {{step, 0, 60, 60}, {200 - step, 20, 60, 60}}));
  }
  REQUIRE(associator.tracks().size() == 2);
  for (const Track& track : associator.tracks()) {
    if (track.id == left_id) CHECK(track.box.x == 210);
    if (track.id == right_id) CHECK(track.box.x == -10);
  }
}

TEST_CASE("associator: match exactly at threshold is rejected") {
  Associator associator{Config{0.5, 3}};
  associator.update(frame_at_ms(0, {{0, 0, 4, 4}}));
  associator.update(frame_at_ms(33, {{0, 0, 4, 2}}));
  CHECK(associator.tracks().size() == 2);
}

}
}
