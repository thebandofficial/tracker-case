---
title: Clocks and synchronization in GStreamer
...

# Clocks and synchronization in GStreamer

*Excerpt from the GStreamer Application Development Manual, gst-docs
`markdown/application-development/advanced/clocks.md` @5c8ad24c. Open
Publication License v1.0.*

## Clock running-time

In a typical computer, there are many sources that can be used as a time
source, e.g., the system time, soundcards, CPU performance counters, etc.
For this reason, GStreamer has many `GstClock` implementations available.
Note that clock time doesn't have to start from 0 or any other known
value. Some clocks start counting from a particular start date, others
from the last reboot, etc.

A `GstClock` returns the **absolute-time** according to that clock with
`gst_clock_get_time ()`. The **absolute-time** (or clock time) of a clock
is monotonically increasing.

A **running-time** is the difference between a previous snapshot of the
**absolute-time** called the **base-time**, and any other **absolute-time**.

```
running-time = absolute-time - base-time
```

A GStreamer `GstPipeline` object maintains a `GstClock` object and a
base-time when it goes to the `PLAYING` state. The pipeline gives a handle
to the selected `GstClock` to each element in the pipeline along with
selected base-time. The pipeline will select a base-time in such a way
that the running-time reflects the total time spent in the `PLAYING`
state. As a result, when the pipeline is `PAUSED`, the running-time stands
still.

Because all objects in the pipeline have the same clock and base-time,
they can thus all calculate the running-time according to the pipeline
clock.

## Buffer running-time

To calculate a buffer running-time, we need a buffer timestamp and the
`SEGMENT` event that preceded the buffer. First we can convert the
`SEGMENT` event into a `GstSegment` object and then we can use the
`gst_segment_to_running_time ()` function to perform the calculation of
the buffer running-time.

Synchronization is now a matter of making sure that a buffer with a
certain running-time is played when the clock reaches the same
running-time. Usually, this task is performed by sink elements. These
elements also have to take into account the configured pipeline's latency
and add it to the buffer running-time before synchronizing to the pipeline
clock.

Non-live sources timestamp buffers with a running-time starting from 0.
After a flushing seek, they will produce buffers again from a
running-time of 0.

Live sources need to timestamp buffers with a running-time matching the
pipeline running-time when the first byte of the buffer was captured.

## Buffer stream-time

The buffer stream-time, also known as the position in the stream, is
a value between 0 and the total duration of the media and it's calculated
from the buffer timestamps and the preceding `SEGMENT` event.

The stream-time is used in:

  - Report the current position in the stream with the `POSITION` query.

  - The position used in the seek events and queries.

  - The position used to synchronize controlled values.

The stream-time is never used to synchronize streams, this is only done
with the running-time.
