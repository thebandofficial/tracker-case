#pragma once

#include <cstdint>
#include <vector>

namespace tracker {

struct Box {
  double x;
  double y;
  double w;
  double h;
};

struct Detection {
  Box box;
};

struct Frame {
  std::uint64_t timestamp_ns;
  std::vector<Detection> detections;
};

struct Track {
  int id;
  Box box;
  std::uint64_t last_seen_ns;
  int misses;
  double vx = 0.0;
  double vy = 0.0;

  bool coasting() const { return misses > 0; }
  Box predicted() const { return Box{box.x + vx, box.y + vy, box.w, box.h}; }
};

struct Config {
  double iou_threshold = 0.3;
  int max_misses = 3;
};

double iou(const Box& a, const Box& b);

class Associator {
 public:
  explicit Associator(Config config);

  void update(const Frame& frame);
  const std::vector<Track>& tracks() const { return tracks_; }

 private:
  void match_detections(const Frame& frame, std::vector<bool>& detection_used);
  void coast_unmatched_tracks();
  void spawn_tracks(const Frame& frame, const std::vector<bool>& detection_used);

  Config config_;
  std::vector<Track> tracks_;
  std::vector<bool> track_matched_;
  int next_id_ = 1;
};

}
