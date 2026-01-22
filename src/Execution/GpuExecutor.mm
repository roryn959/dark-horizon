#import <Metal/Metal.h>
#import <Foundation/Foundation.h>

#include "Execution/GpuExecutor.h"

struct GpuExecutor::Impl {
    id<MTLDevice> device;
    id<MTLCommandQueue> queue;
    id<MTLComputePipelineState> pipeline;
};

GpuExecutor::GpuExecutor()
{
	@autoreleasepool {
		impl = new Impl{};

		InitialiseDevice();
		if (!impl->device) std::exit(1);

		InitialiseCommandQueue();
		if (!impl->queue) std::exit(1);

		CheckCommandBuffer();

		InitialisePipeline();
	}
}

GpuExecutor::~GpuExecutor() {
    delete impl;
}

void GpuExecutor::InitialiseDevice() {
	impl->device = MTLCreateSystemDefaultDevice();
	if (impl->device) return;

	NSArray<id<MTLDevice>> *devices = nil;

	devices = MTLCopyAllDevices();
	if (!devices || devices.count == 0) {
		std::cerr << "Copy returned no devices.\n";
		std::exit(1);
	}

	impl->device = devices[0];
}

void GpuExecutor::InitialiseCommandQueue() {
	impl->queue = [impl->device newCommandQueue];
	if (!impl->queue) {
		std::cerr << "Failed to create command queue\n";
		impl->device = nil;
		std::exit(1);
	}
}

void GpuExecutor::CheckCommandBuffer() {
	id<MTLCommandBuffer> cb = [impl->queue commandBuffer];
	[cb commit];
	[cb waitUntilCompleted];

	if (cb.status != MTLCommandBufferStatusCompleted) {
		std::cerr << "Command buffer failure.\n";
		std::exit(1);
	}
}

void GpuExecutor::InitialisePipeline()
{
    NSURL* url = [NSURL fileURLWithPath:@"PhotonTracer.metallib"];
	NSError* error = nil;
	id<MTLLibrary> library = [impl->device newLibraryWithURL:url error:&error];
    if (!library) {
        std::cerr << "Failed to load metallib: " << [[error localizedDescription] UTF8String] << '\n';
        return;
    }

    id<MTLFunction> kernel = [library newFunctionWithName:@"TracePhoton"];
    if (!kernel) {
        std::cerr << "Kernel 'TracePhoton' not found.\n";
        return;
    }

    impl->pipeline = [impl->device newComputePipelineStateWithFunction:kernel error:&error];
    if (!impl->pipeline) {
        std::cerr << "Failed to create pipeline: " << [[error localizedDescription] UTF8String] << '\n';
        return;
    }
}

void GpuExecutor::TracePhotons(uint32_t* pixels) {

	id<MTLBuffer> pixelBuffer = [
		impl->device
		newBufferWithBytesNoCopy:pixels
        length:sizeof(uint32_t) * NUM_PIXELS
        options:MTLResourceStorageModeShared
        deallocator:nil
	];

	id<MTLCommandBuffer> cmd = [impl->queue commandBuffer];
    id<MTLComputeCommandEncoder> enc = [cmd computeCommandEncoder];

    MTLSize gridSize = MTLSizeMake(NUM_PIXELS, 1, 1);
    NSUInteger tgSize = impl->pipeline.maxTotalThreadsPerThreadgroup;
    if (tgSize > NUM_PIXELS) tgSize = NUM_PIXELS;
    MTLSize threadgroupSize = MTLSizeMake(tgSize, 1, 1);

	[enc setComputePipelineState:impl->pipeline];

    [enc setBuffer:pixelBuffer offset:0 atIndex:0];

    [enc dispatchThreads:gridSize threadsPerThreadgroup:threadgroupSize];

	[enc endEncoding];
    [cmd commit];
    [cmd waitUntilCompleted];

    if (cmd.status != MTLCommandBufferStatusCompleted) {
        std::cerr << "Compute dispatch failed.\n";
        return;
    }
}