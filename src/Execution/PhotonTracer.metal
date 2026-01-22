#include <metal_stdlib>
using namespace metal;

// ---------- Constants ----------

#define WINDOW_W 820
#define WINDOW_H 820

constant int NUM_PIXELS = WINDOW_W * WINDOW_H;

// ---------- KERNEL ----------

kernel void TracePhoton(
    device 		uint* 		pixelBuffer 		[[ buffer(0) ]],
    uint pixel [[ thread_position_in_grid ]]
) {
	pixelBuffer[pixel] = 0xFFFF0000;
}
