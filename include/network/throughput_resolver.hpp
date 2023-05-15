#pragma once
#include "network/resolver.hpp"
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#include <unordered_map>
#include <chrono>
//---------------------------------------------------------------------------
// AnyBlob - Universal Cloud Object Storage Library
// Dominik Durner, 2022
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
// If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
// SPDX-License-Identifier: MPL-2.0
//---------------------------------------------------------------------------
namespace anyblob {
//---------------------------------------------------------------------------
namespace network {
//---------------------------------------------------------------------------
/// Implements the AWS Resolver logic
class ThroughputResolver : public network::Resolver {
    /// Order statistic tree
    typedef __gnu_pbds::tree<
        double,
        __gnu_pbds::null_type,
        std::greater<double>,
        __gnu_pbds::rb_tree_tag,
        __gnu_pbds::tree_order_statistics_node_update>
        map_t;
    /// The order statistic tree
    map_t _throughputTree;
    /// The seconds vector as stable ring buffer
    std::vector<double> _throughput;
    /// The map between fd and start time
    std::unordered_map<int, std::pair<unsigned, std::chrono::steady_clock::time_point>> _fdMap;
    /// The seconds vector iterator
    uint64_t _throughputIterator;
    /// The maximum history
    const unsigned _maxHistory = 1024;

    public:
    /// The constructor
    explicit ThroughputResolver(unsigned entries);
    /// The address resolving
    virtual unsigned resolve(std::string hostname, std::string port, bool& reuse) override;
    /// Start the timing
    virtual void startSocket(int fd, unsigned ipAsInt) override;
    /// Stop the timing
    virtual void stopSocket(int fd, uint64_t bytes) override;
    /// The destructor
    virtual ~ThroughputResolver() noexcept = default;
};
//---------------------------------------------------------------------------
}; // namespace network
//---------------------------------------------------------------------------
}; // namespace anyblob
