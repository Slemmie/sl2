#pragma once

#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

using namespace __gnu_pbds;

template <typename KEY, typename VALUE, typename COMPARISON = std::less <KEY>>
using ordered_map = tree <KEY, VALUE, COMPARISON, rb_tree_tag, tree_order_statistics_node_update>;
