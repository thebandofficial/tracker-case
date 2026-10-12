# Domain notes

A **unit** is one deployed device: a camera, a detector and this tracker.
Unit 2 is one of them.

A **frame** is one camera image plus the detections found in it. Frames arrive
in capture order. Each carries `timestamp_ns`: nanoseconds on the camera's
capture clock, monotonic, not wall time. Nothing in this repo assumes a fixed
frame rate. Units in the field run anywhere from 5 to 30 Hz (frames per
second), set by the sensor mode.

A **detection** is one box the detector produced in one frame. It has no
identity. Two detections in consecutive frames are unrelated until the
associator says otherwise.

A **box** is `x, y, w, h` in pixels. `x, y` is the top-left corner, origin at
the top-left of the image, y down. `w, h` are strictly positive.

**IoU** (intersection over union) is the overlap area of two boxes divided by
the area they cover together. 1 means identical, 0 means no overlap. Boxes
that only touch along an edge have zero intersection.

A **track** is the associator's claim that a sequence of detections across
frames is the same object. Its `id` is assigned once and never reused for a
different object. `box` is the most recently matched detection. `last_seen_ns`
is the timestamp of that frame. `misses` counts consecutive frames with no
match. A track with at least one miss is **coasting**.

The **associator** is the code in this repo. **Association** runs once per
frame. A detection matches a track when their IoU is at least
`iou_threshold`. Each detection matches at most one track and each track at
most one detection. A track with no match this frame gains one miss. A track
is dropped once it has `max_misses` consecutive misses. Every detection left
unmatched spawns a new track.

Config defaults: `iou_threshold = 0.3`, `max_misses = 5`.

The **overlay** is the live video the operator watches, with each track's box
and ID drawn on it. The **partner** is the customer whose software receives
the tracks.
