# Thread: track IDs swapping on our unit 2

---

**From:** Dana Reyes, Integration Lead, Northline Systems
**To:** tracker team
**Subject:** track IDs swapping on our unit 2

Hi,

Our operators report that when two objects cross on the 5 Hz unit, the labels
swap and stay swapped. This happened three times during yesterday's session.
Our downstream logic keys on the track ID, so after a swap we are following
the wrong object until someone notices.

Can you confirm this is a known issue, and when can we expect a fix? We have a
customer demo on the 15th.

Dana

---

**From:** Dana Reyes
**Subject:** RE: track IDs swapping on our unit 2

One more thing while you are in there. We also lose tracks when an object goes
behind a pole or a tree for a second. Can you make tracks never drop? Just keep
the last position until the object shows up again, however long that takes. Our
side would be much simpler if the ID is always there.

Dana
