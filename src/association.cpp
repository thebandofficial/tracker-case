#include "tracker/association.hpp"

#include <algorithm>
#include <cstddef>
#include <limits>

namespace tracker {

namespace {

constexpr std::size_t kNone = std::numeric_limits<std::size_t>::max();

double area(const Box& b) { return b.w * b.h; }

}

double iou(const Box& a, const Box& b) {
  const double left = std::max(a.x, b.x);
  const double top = std::max(a.y, b.y);
  const double right = std::min(a.x + a.w, b.x + b.w);
  const double bottom = std::min(a.y + a.h, b.y + b.h);
  if (right <= left || bottom <= top) {
    return 0.0;
  }
  const double inter = (right - left) * (bottom - top);
  return inter / (area(a) + area(b) - inter);
}

Associator::Associator(Config config) : config_(config) {}

void Associator::update(const Frame& frame) {
  std::vector<bool> detection_used(frame.detections.size(), false);
  track_matched_.assign(tracks_.size(), false);

  match_detections(frame, detection_used);
  coast_unmatched_tracks();
  spawn_tracks(frame, detection_used);
}

void Associator::match_detections(const Frame& frame, std::vector<bool>& detection_used) {
  for (std::size_t t = 0; t < tracks_.size(); ++t) {
    double best_score = config_.iou_threshold;
    std::size_t best_detection = kNone;
    for (std::size_t d = 0; d < frame.detections.size(); ++d) {
      if (detection_used[d]) {
        continue;
      }
      const double score = iou(tracks_[t].box, frame.detections[d].box);
      if (score >= best_score) {
        best_score = score;
        best_detection = d;
      }
    }
    if (best_detection == kNone) {
      continue;
    }
    Track& track = tracks_[t];
    track.box = frame.detections[best_detection].box;
    track.last_seen_ns = frame.timestamp_ns;
    track.misses = 0;
    detection_used[best_detection] = true;
    track_matched_[t] = true;
  }
}

void Associator::coast_unmatched_tracks() {
  for (std::size_t t = 0; t < tracks_.size(); ++t) {
    if (!track_matched_[t]) {
      ++tracks_[t].misses;
    }
  }
  tracks_.erase(std::remove_if(tracks_.begin(), tracks_.end(),
                               [this](const Track& track) { return track.misses >= config_.max_misses; }),
                tracks_.end());
}

void Associator::spawn_tracks(const Frame& frame, const std::vector<bool>& detection_used) {
  for (std::size_t d = 0; d < frame.detections.size(); ++d) {
    if (detection_used[d]) {
      continue;
    }
    tracks_.push_back(Track{next_id_++, frame.detections[d].box, frame.timestamp_ns, 0});
  }
}

}
