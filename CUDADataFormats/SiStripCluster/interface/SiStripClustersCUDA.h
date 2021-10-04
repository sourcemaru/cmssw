#ifndef CUDADataFormats_SiStripCluster_interface_SiStripClustersCUDA_h
#define CUDADataFormats_SiStripCluster_interface_SiStripClustersCUDA_h

#include "DataFormats/SiStripCluster/interface/SiStripClustersSOABase.h"
#include "HeterogeneousCore/CUDAUtilities/interface/device_unique_ptr.h"
#include "HeterogeneousCore/CUDAUtilities/interface/host_unique_ptr.h"
#include "HeterogeneousCore/CUDAUtilities/interface/cudaCompat.h"

namespace cms {
  template <typename T>
  using observer_ptr = T *;
}

#include <cuda_runtime.h>

class SiStripClustersCUDADevice : public SiStripClustersSOABase<cms::cuda::device::unique_ptr> {
public:
  SiStripClustersCUDADevice() = default;
  explicit SiStripClustersCUDADevice(size_t maxClusters, int clustersPerStrip, cudaStream_t stream);
  ~SiStripClustersCUDADevice() override = default;

  SiStripClustersCUDADevice(const SiStripClustersCUDADevice &) = delete;
  SiStripClustersCUDADevice &operator=(const SiStripClustersCUDADevice &) = delete;
  SiStripClustersCUDADevice(SiStripClustersCUDADevice &&) = default;
  SiStripClustersCUDADevice &operator=(SiStripClustersCUDADevice &&) = default;

  struct DeviceView {
    uint32_t *clusterIndex_;
    uint32_t *clusterSize_;
    uint8_t *clusterADCs_;
    stripgpu::detId_t *clusterDetId_;
    stripgpu::stripId_t *firstStrip_;
    bool *trueCluster_;
    float *barycenter_;
    float *charge_;
    uint32_t nClusters_;
  };

  DeviceView *view() const { return view_d.get(); }
  int nClustersHost() const { return nClusters_h; }
  int *nClustersHostPtr() { return &nClusters_h; }

private:
  cms::cuda::device::unique_ptr<DeviceView> view_d;  // "me" pointer
  int nClusters_h;
};

class SiStripClustersCUDAHost : public SiStripClustersSOABase<cms::cuda::host::unique_ptr> {
public:
  SiStripClustersCUDAHost() = default;
  explicit SiStripClustersCUDAHost(const SiStripClustersCUDADevice &clusters_d,
                                   int clustersPerStrip,
                                   cudaStream_t stream);
  ~SiStripClustersCUDAHost() override = default;

  SiStripClustersCUDAHost(const SiStripClustersCUDAHost &) = delete;
  SiStripClustersCUDAHost &operator=(const SiStripClustersCUDAHost &) = delete;
  SiStripClustersCUDAHost(SiStripClustersCUDAHost &&) = default;
  SiStripClustersCUDAHost &operator=(SiStripClustersCUDAHost &&) = default;
};

#endif
