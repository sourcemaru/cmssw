# TrackSoA Data Formats

`DataFormat`s meant to be used on Host (CPU) or Device (GPU) for
storing information about `TrackSoA`s created during the Pixel-local Reconstruction
chain. It stores data in an SoA manner.  

The host format is inheriting from `DataFormats/Portable/interface/PortableHostCollection.h`,
while the device format is inheriting from `DataFormats/Portable/interface/PortableDeviceCollection.h`

Both formats use the same SoA Layout (`TrackSoA::Layout`) which is generated
via the `GENERATE_SOA_LAYOUT` macro in the `TrackDefinitions.h` file.

## Notes

-`hitIndices` and `detIndices`, instances of `HitContainer`, have been added into the
layout as `SOA_SCALAR`s, meaning that they manage their own data independently from the SoA
`Layout`. This could be improved in the future, if `HitContainer` (aka a `OneToManyAssoc` of fixed size)
is replaced, but there don't seem to be any conflicts in including it in the `Layout` like this.
- Host and Device classes should **not** be created via inheritance, as they're done here,
but via composition. See [this discussion](https://github.com/cms-sw/cmssw/pull/40465#discussion_r1066039309).

## TracksHost

The version of the data format to be used for storing `TrackSoA` data on the CPU. 
Instances of this class are to be used for:

- Having a place to copy data to host from device, via `Memcpy`, or
- Running host-side algorithms using data stored in an SoA manner.

## TracksDevice

The version of the data format to be used for storing `TrackSoA` data on the GPU.

Instances of `TracksDevice` are to be created on host and be
used on device only. To do so, the instance's `view()` method is to be called
to pass a `View` to any kernel launched. Accessing data from the `view()` is not
possible on the host side.

## TracksSoACollection

Depending on the Alpaka accelerator back-end enabled, `TrackSoACollection` is an alias to either the Host or Device SoA:

```cpp
template <typename TrackerTraits>
  using TrackSoACollection = std::conditional_t<std::is_same_v<Device, alpaka::DevCpu>,
                                                          TrackSoAHost<TrackerTraits>,
                                                          TrackSoADevice<TrackerTraits, Device>>;
```

## Utilities

`alpaka/TrackUtilities.h` contains a collection of methods which were originally
defined as class methods inside either `TrackSoAHeterogeneousT` and `TrajectoryStateSoAT`
which have been adapted to operate on `View` instances, so that they are callable
from within `__global__` kernels, on both CPU and CPU. 

## Use case

See `test/TrackSoAHeterogeneous_test.cpp` for a simple example of instantiation,
processing and copying from device to host.
