#include "rollingmean.hpp"

#include <algorithm>
#include <limits>

struct Node
{
    /// Pointer to node with value greater-than this.
    Node *gt_;
    /// Pointer to node with value less-than-or-equal-to this.
    Node *le_;

    /// Pointer to node that is newer.
    Node *newer_;
    /// Pointer to node that is older.
    Node *older_;

    /// Value.
    double value_;

    Node() : Node{0.}
    {
    }

    Node(double value) : gt_{}, le_{}, newer_{}, older_{}, value_{value}
    {
    }

    /// @discussion Insert node into linked lists.
    void insert(Node * value_successor, Node * time_successor)
    {
        // +-------------+      +------+      +-----------+
        // |             |----->|      |----->|           |
        // | predecessor |      | node |      | successor |
        // |             |<-----|      |<-----|           |
        // +-------------+      +------+      +-----------+
        {
            auto predecessor = value_successor->le_;

            predecessor->gt_ = this;
            gt_ = value_successor;
            value_successor->le_ = this;
            le_ = predecessor;
        }
        {
            auto predecessor = time_successor->older_;

            predecessor->newer_ = this;
            newer_ = time_successor;
            time_successor->older_ = this;
            older_ = predecessor;
        }
    }

    /// @discussion Remove node from linked lists.
    void unlink()
    {
        gt_->le_ = le_;
        le_->gt_ = gt_;

        newer_->older_ = older_;
        older_->newer_ = newer_;
    }
};

class RollingMean::Impl
{
    /// Storage capacity.
    size_t capacity_;

    /// Number of elements in FIFO.
    size_t size_;

    /// Linked list of nodes ordered by value.
    /// This "node" is a sentinel which simplifies linked list management.
    //        +-----+     +-----+     +-----+     +-----+     +----------+
    // ...--->|     |---->|     |---->|     |---->|     |---->|          |-gt-...
    //        | min |     |     |     |     |     | max |     | sentinel |
    // ...----|     |<----|     |<----|     |<----|     |<-le-|          |<---...
    //        +-----+     +-----+     +-----+     +-----+     +----------+
    Node by_size_;

    /// Linked list of nodes ordered by time.
    /// This "node" is a sentinel which simplifies linked list management.
    //           +----------+        +-----+     +-----+     +-----+     +-----+
    // ...------>|          |-newer->|     |---->|     |---->|     |---->|     |----...
    //           | sentinel |        | new |     |     |     |     |     | old |
    // ...-older-|          |<-------|     |<----|     |<----|     |<----|     |<---...
    //           +----------+        +-----+     +-----+     +-----+     +-----+
    Node by_age_;

public:
    Impl(size_t capacity);
    ~Impl();

    Node * successor_of(double value);
    void push(double value);
    void pop();
    void clear();
    bool ready() const;
    double add(double value);
    double sum() const;
    double mean() const;
};

namespace
{

constexpr size_t minimum_size = 3;

} // namespace

RollingMean::Impl::Impl(size_t capacity) : capacity_{std::max(capacity, minimum_size)}, size_{}, by_size_{}, by_age_{}
{
    by_size_.gt_ = &by_size_;
    by_size_.le_ = &by_size_;
    by_size_.value_ = std::numeric_limits<double>::max();

    by_age_.newer_ = &by_age_;
    by_age_.older_ = &by_age_;
}

RollingMean::Impl::~Impl()
{
    clear();
}

Node * RollingMean::Impl::successor_of(double value)
{
    for (auto node = by_size_.gt_; ; node = node->gt_) {
        if (value <= node->value_) {
            return node;
        }
    }
}

void RollingMean::Impl::push(double value)
{
    auto node = new Node{value};
    size_++;

    // Insert newest node in proper place in value size-ordered list, and to front of the time-ordered list.
    node->insert(successor_of(value), by_age_.newer_);
}

void RollingMean::Impl::pop()
{
    if (size_) {
        auto oldest = by_age_.older_;
        oldest->unlink();
        delete oldest;
        size_--;
    }
}

void RollingMean::Impl::clear()
{
    while (size_) {
        pop();
    }
}

bool RollingMean::Impl::ready() const
{
    return size_ >= minimum_size;
}

double RollingMean::Impl::add(double value)
{
    push(value);

    if (!ready()) {
        return value;
    }

    while (size_ > capacity_) {
        pop();
    }

    return mean();
}

double RollingMean::Impl::sum() const
{
    if (!ready()) {
        return 0;
    }

    // Drop lowest and highest values (potential outliers) then sum and average the remaining elements.
    // Calculating the sum like this prevents numeric problems (due to repeated add/subtract).
    double sum = 0;
    for (auto node = by_size_.gt_->gt_; node != by_size_.le_; node = node->gt_) {
        sum += node->value_;
    }

    return sum;
}

double RollingMean::Impl::mean() const
{
    if (!ready()) {
        return 0;
    }

    return sum() / static_cast<double>(size_ - 2);
}


RollingMean::RollingMean(size_t capacity) : pimpl{std::make_unique<Impl>(capacity)}
{
}

RollingMean::~RollingMean() = default;

void RollingMean::clear()
{
    pimpl->clear();
}

bool RollingMean::ready() const
{
    return pimpl->ready();
}

double RollingMean::add(double value)
{
    return pimpl->add(value);
}

double RollingMean::sum() const
{
    return pimpl->sum();
}

double RollingMean::mean() const
{
    return pimpl->mean();
}
