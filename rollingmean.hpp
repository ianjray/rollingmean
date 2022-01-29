#pragma once

#include <memory>


/// @brief Rolling Mean
/// @discussion Implements a FIFO that excludes outliers (these being the lowest and highest values) when calculating the sum and mean.
class RollingMean
{
    class Impl;
    std::unique_ptr<Impl> pimpl;

public:
    /// @discussion Construct FIFO with the given @c capacity.
    /// The minimum capacity is three since that leaves room for a single value once the outliers are removed.
    RollingMean(size_t capacity);

    RollingMean(const RollingMean &) = delete;
    auto operator=(const RollingMean &) -> RollingMean & = delete;
    RollingMean(RollingMean&& other) = delete;
    auto operator=(RollingMean &&) -> RollingMean & = delete;
    ~RollingMean();

    /// @discussion Erase all elements.
    void clear();

    /// @return True if the FIFO has three or more elements, meaning that valid sum and mean values can be provided.
    /// @see sum
    /// @see mean
    bool ready() const;

    /// @discussion Add a value to the FIFO.
    /// @return Mean value if the FIFO is ready, or given @c value otherwise.
    double add(double value);

    /// @return Sum of all non-outlier elements.
    double sum() const;

    /// @return Mean of all non-outlier elements.
    double mean() const;
};
