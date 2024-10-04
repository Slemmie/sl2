#pragma once

#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

using namespace __gnu_pbds;

template <typename KEY, typename COMPARISON = std::less <KEY>>
using ordered_set = tree <KEY, null_type, COMPARISON, rb_tree_tag, tree_order_statistics_node_update>;
