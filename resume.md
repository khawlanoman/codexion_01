# Codexion Bug Fixes Resume

This document outlines the logical bugs identified in the project and the specific code changes implemented to resolve them.

## 1. Race Condition in Burnout Timer
**File Modified**: `codexion_src/timer.c`

**The Issue**: 
The monitor thread checked for a burned-out coder using a greater-than-or-equal condition (`>=`). This created a race condition where a coder could be declared dead on the *exact millisecond* they acquired their dongles and started compiling, causing false-positive burnouts in tightly scheduled test cases.

**The Fix**:
Modified the check to use a strict greater-than (`>`) inequality. This grants the coders the necessary margin to survive if they secure their resources exactly on their deadline.
```c
// Changed from:
if (now - read_last_active(data, &i) >= data->args.time_to_burnout)
// Changed to:
if (now - read_last_active(data, &i) > data->args.time_to_burnout)
```

## 2. Serialization and Starvation in FIFO Scheduler
**Files Modified**: 
- `codexion_src/head.h`
- `codexion_src/main_helper.c`
- `codexion_src/timer_helper1.c`
- `codexion_src/f_h_thread_f.c`
- `codexion_src/f_h_lock_dongles.c`

**The Issue**: 
The FIFO scheduler suffered from severe synchronization issues that forced the concurrent coders to run sequentially one by one. With 5 coders taking 600ms per round, a strict sequential queue took exactly 3000ms. Since the `time_to_burnout` was also 3000ms, coders inherently burned out from starvation. The underlying `fifo_groups` logic was designed to prevent this by interleaving odd/even coders, but it contained several logical flaws and was missing from the main loop.

**The Fixes**:
1. **Fixed Group Initialization**: In `data_init` (`main_helper.c`), `data->group_count_two` was initialized to `0` instead of the correct number of even coders. This caused the controller thread to instantly skip the even group entirely. I fixed it to initialize to `data->args.number_of_coders / 2`.
2. **Added Phase Tracking**: I introduced an `int phase` variable to the `t_data` struct in `head.h` (and initialized it in `main_helper.c`). This keeps track of which global "round" the system is currently executing.
3. **Synchronized Phases**: In `update_group` (`timer_helper1.c`), I added logic to increment `data->phase` whenever a full round completes (when group 1 transitions back to group 0).
4. **Prevented Lapping**: I updated `fifo_groups` (`f_h_thread_f.c`) to require that a coder's `compile_count` strictly matches the current `data->phase`. This ensures fast coders cannot re-enter the queue and steal dongles from coders that haven't eaten yet.
5. **Applied the Gates**: I inserted the calls to `fifo_groups(coder)` (before entering the heap) and `fifo_group(coder)` (after removing from the heap) into the core `coder_cycle` function (`f_h_lock_dongles.c`). 

**Result**: 
These changes allow non-conflicting coders to acquire dongles and compile concurrently, reducing the total cycle time drastically and passing the 3000ms edge cases with flawless execution.
