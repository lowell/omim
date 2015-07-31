#pragma once

#include "search/search_query_params.hpp"

#include "indexer/mwm_set.hpp"

#include "geometry/rect2d.hpp"

#include "std/function.hpp"
#include "std/vector.hpp"

class Index;

namespace search
{
class Retrieval
{
public:
  class Callback
  {
  public:
    virtual ~Callback() = default;

    virtual void OnMwmProcessed(MwmSet::MwmId const & id, vector<uint32_t> const & featureIds) = 0;
  };

  // Following class represents a set of retrieval's limits.
  struct Limits
  {
  public:
    Limits();

    // Sets upper bound (inclusive) on a number of features to be
    // retrieved.
    void SetMaxNumFeatures(uint64_t minNumFeatures);
    uint64_t GetMaxNumFeatures() const;

    // Sets upper bound on a maximum viewport's scale.
    void SetMaxViewportScale(double maxViewportScale);
    double GetMaxViewportScale() const;

    inline bool IsMaxNumFeaturesSet() const { return m_maxNumFeaturesSet; }
    inline bool IsMaxViewportScaleSet() const { return m_maxViewportScaleSet; }

  private:
    uint64_t m_maxNumFeatures;
    double m_maxViewportScale;

    bool m_maxNumFeaturesSet : 1;
    bool m_maxViewportScaleSet : 1;
  };

  Retrieval();

  void Init(Index & index, m2::RectD const & viewport, SearchQueryParams const & params,
            Limits const & limits);

  void Go(Callback & callback);

private:
  struct Bucket
  {
    Bucket(MwmSet::MwmHandle && handle);

    MwmSet::MwmHandle m_handle;
    vector<uint32_t> m_addressFeatures;
    vector<uint32_t> m_geometryFeatures;
    vector<uint32_t> m_intersection;
    m2::RectD m_bounds;
    bool m_intersectsWithViewport : 1;
    bool m_coveredByViewport : 1;
    bool m_finished : 1;
  };

  // *NOTE* arguments of successive calls of this method should be
  // *non-decreasing.
  void RetrieveForViewport(m2::RectD const & viewport, Callback & callback);

  bool ViewportCoversAllMwms() const;

  void ReportFeatures(Bucket & bucket, Callback & callback);

  Index * m_index;
  m2::RectD m_viewport;
  SearchQueryParams m_params;
  Limits m_limits;
  uint64_t m_featuresReported;

  vector<Bucket> m_buckets;
};
}  // namespace search