# sl2
competetive programming library 2

# Installation
Quick installation of both headers and scripts requires building the scripts first:

```sh
$ make
```

Next, to install scripts in `/usr/local/bin/` and to export headers to `/usr/local/include/sl2/`:

```sh
$ sudo make install
```

#### Advanced installation
The scripts, once built, will be placed in `./bin/`. The headers are found under `./src/`. Manually placing these wherever makes most sense is entirely possible, consult `makefile` for details on how normal installation works.

# Uninstallation
To remove all traces of sl2 as installed by `sudo make install`:

```sh
$ sudo make uninstall
```

# Usage
Consult `tm --help` and `prep --help` for details about the scripts. `tm` allows quickly summoning the standard `template.cpp` file. `prep` prepares a source file for online submission by recursively unfolding sl2 headers included.

To use a given header in a source file, you can e.g.:

```c++
#include <sl2/mint> // mint as an example, see content below for an exhaustive list of headers

int main() {
  mi x = 42;
  // etc.
}
```

When compiling locally, the correct version (C++ 20 vs. C++ 17) should be chosen by the compiler correctly automatically. See `prep --help` for details on preparing using the desired C++ version.

# Content
All content has C++ 20 and C++ 17 versions available.

- [C++17 template.cpp](https://github.com/Slemmie/sl2/blob/main/template/c%2B%2B17/template.cpp)
- [C++20 template.cpp](https://github.com/Slemmie/sl2/blob/main/template/c%2B%2B20/template.cpp)

- [script for recursively unfolding a C++ file using sl2 headers for submission](https://github.com/Slemmie/sl2/blob/main/scripts/pre.cpp)
- [script for copying standard sl2 template.cpp file to desired destination](https://github.com/Slemmie/sl2/blob/main/scripts/tem.cpp)

| desciption | C++17 header | C++20 header | wildcard header |
| ---------- | ------------ | ------------ | --------------- |
| mod int class | [mint.h](https://github.com/Slemmie/sl2/blob/main/src/c%2B%2B17/mint.h) | [mint.h](https://github.com/Slemmie/sl2/blob/main/src/c%2B%2B20/mint.h) | [mint](https://github.com/Slemmie/sl2/blob/main/src/mint) |
| mod int class typedeffing `mi` to use 998244353 | [mint9.h](https://github.com/Slemmie/sl2/blob/main/src/c%2B%2B17/mint9.h) | [mint9.h](https://github.com/Slemmie/sl2/blob/main/src/c%2B%2B20/mint9.h) | [mint9](https://github.com/Slemmie/sl2/blob/main/src/mint9) |
| multiplicative inverse function | [inverse.h](https://github.com/Slemmie/sl2/blob/main/src/c%2B%2B17/inverse.h) | [inverse.h](https://github.com/Slemmie/sl2/blob/main/src/c%2B%2B20/inverse.h) | [inverse](https://github.com/Slemmie/sl2/blob/main/src/inverse) |
| euler's totient (phi) function | [phi.h](https://github.com/Slemmie/sl2/blob/main/src/c%2B%2B17/phi.h) | [phi.h](https://github.com/Slemmie/sl2/blob/main/src/c%2B%2B20/phi.h) | [phi](https://github.com/Slemmie/sl2/blob/main/src/phi) |
| disjoint union set class | [dsu.h](https://github.com/Slemmie/sl2/blob/main/src/c%2B%2B17/dsu.h) | [dsu.h](https://github.com/Slemmie/sl2/blob/main/src/c%2B%2B20/dsu.h) | [dsu](https://github.com/Slemmie/sl2/blob/main/src/dsu) |
| segment tree class | [segtree.h](https://github.com/Slemmie/sl2/blob/main/src/c%2B%2B17/segtree.h) | [segtree.h](https://github.com/Slemmie/sl2/blob/main/src/c%2B%2B20/segtree.h) | [segtree](https://github.com/Slemmie/sl2/blob/main/src/segtree) |
| ordered set (extended STL) | [ordered\_set.h](https://github.com/Slemmie/sl2/blob/main/src/c%2B%2B17/ordered_set.h) | [ordered\_set.h](https://github.com/Slemmie/sl2/blob/main/src/c%2B%2B20/ordered_set.h) | [ordered\_set](https://github.com/Slemmie/sl2/blob/main/src/ordered_set) |
| ordered map (extended STL) | [ordered\_map.h](https://github.com/Slemmie/sl2/blob/main/src/c%2B%2B17/ordered_map.h) | [ordered\_map.h](https://github.com/Slemmie/sl2/blob/main/src/c%2B%2B20/ordered_map.h) | [ordered\_map](https://github.com/Slemmie/sl2/blob/main/src/ordered_map) |
| ordered set/map (extended STL) | - | - | [ordered](https://github.com/Slemmie/sl2/blob/main/src/ordered) |

# Todo
- debugger
- fenwick tree
- 2D fenwick tree
- hash map header that beats `std::unordered_map`
- dynamic segment tree
- lazy segment tree
- dynamic lazy segment tree
- persistent segment tree
- persistent lazy segment tree
- 2D segment tree (consider making `Segtree <Segtree <Node>>` possible instead)
- 2D dynamic segment tree (consider making `Dynamic_segtree <Dynamic_segtree <Node>>` possible instead)
- 2D lazy segment tree (consider making `Lazy_segtree <Lazy_segtree <Node>>` possible instead)
- 2D dynamic lazy segment tree (consider making `Dynamic_lazy_segtree <Dynamic_lazy_segtree <Node>>` possible instead)
- 2D persistent segment tree (consider making `Persistent_segtree <Persistent_segtree <Node>>` possible instead)
- 2D persistent lazy segment tree (consider making `Persistent_lazy_segtree <Persistent_lazy_segtree <Node>>` possible instead)
- li chao tree
- convex hull trick
- square matrix object
- ordered set header
- ordered map header
- multi- map/set/ordered_map/ordered_set using std::map or ordered- set/map headers
- sparse table
- all segment trees + sparse table + etc. header(s) with basic operations (e.g. range add/min/etc.)
- static arithmetically reversible (e.g. addition/xor/etc. - not min/max/etc.) sub array queries
- static arithmetically reversible (e.g. addition/xor/etc. - not min/max/etc.) sub matrix queries
- treap
- dsu rollback (persistent dsu)
- 2D point class
- 3D point class
- convex hull
- 3D convex hull
- dynamic convex hull
- 3D dynamic convex hull
- circle class
- circle/circle intersection points
- circle/line intersection points
- circle/polygon intersection points
- circle/polygon intersection area
- external/internel two circle tangent(s)
- closest pair of points
- 3D closest pair of points
- delaunay triangulation
- convex hull diameter
- 3D convex hull diameter
- farthest pair of points
- 3D farthest pair of points
- is point inside polygon
- 3D is point inside of polygon
- line/convex hull intersection
- 3D line/convex hull intersection
- point line projection
- point line reflection
- 3D point line projection
- 3D point line reflection
- point plane projection
- point plane reflection
- euclidian minimum spanning tree
- 3D euclidian minimum spanning tree
- manhattan minimum spanning tree
- 3D manhattan minimum spanning tree
- minimum bounding circle
- minimum bounding sphere
- is point on line segment
- 3D is point on line segment
- is point inside convex hull
- 3D is point inside convex hull
- area of polygon
- 3D area of polygon
- center of mass of polygon
- cut polygon based on one side of given line
- area of $n$ polygons union
- point/line segment distance
- 3D point/line segment distance
- line segment intersection
- 3D line segment intersection
- find circumcircle of triangle
- 2D kd tree
- 3D kd tree
- point/line distance
- 3D point/line distance
- line/line intersection
- 3D line/line intersection
- modify point with linear transformations required to move one line segment to another line segment
- which side of line is point
- spherical distance (2D ponints + 3D points)
- two sat
- bellman ford
- biconnected components finding
- tree helpers (binary lifting, lca, traversal orders, depth, etc.) (extend for forests)
- $\mathcal{O}(VE)$ dfs bipartite matching
- $\mathcal{O}(VElog(MAXCAPACITY))$ maxflow (dinic)
- $\mathcal{O}(Elog(V))$ directed minimum spanning tree with given root
- $\mathcal{O}(VE)$ $k$-edge-coloring where no vertex has degree greater than $k$ (repeat bipartite matching)
- $\mathcal{O}(VE^2)$ maxflow (edmonds karp)
- $\mathcal{O}(V + E)$ undirected euler walk
- $\mathcal{O}(V + E)$ directed euler walk
- $\mathcal{O}(V^3)$ floyd warshall
- general maximum matching (blossom or maybe "micali and vazirani")
- $\mathcal{O}(V^3)$ global minimum cut undirected graph (stoer wagner)
- $\mathcal{O}(V) \cdot \mathcal{O}(FLOWITERATION)$ gomery-hu tree (maybe gusfield's)
- heavy light decomposition (extend for forests)
- $\mathcal{O}(Vlog(V) + Q)$ lowest common ancestor (use sparse table) (extend for forests)
- $\mathcal{O}(log(V))$ amortized all operations link-cut tree
- $\mathcal{O}(3^{V \over 3})$ find all maximal cliques (bron kerbosch)
- $V \le$ ~150 ish; find any maximally sized clique ("maximum clique algorithm")
- maximal independent set
- $\mathcal{O}(E^2)$ minimum cost maximum flow
- retrieval of set of vertices making up left (source) side of minimum cut
- minimum vertex cover bipartite graph
- bipartite graph maximum independent set using minimum vertex cover
- topological sort
- strongly connected components
- $\mathcal{O}(V^2E)$ minimally weighted bipartite matching
- $\mathcal{O}(\sqrt{V}E)$ bipartite maximum matching (hopcroft karp)
- bridge finding
- articulation point finding
- minimum spanning tree with given operator (extend for forests)
- centroid decomposition callback dispatcher (extend for forests)
- other basic tree calculations like leaf count/diameter/degree frequency array/etc. (extend for forests)
- isolated single (or both) centroid finding (extend for forests)
- minimum edge cover
- minimum path cover on DAG
- minimum path cover in tree
- online subtree queries structure (inorder/postorder + range data structure)
- retrieve shortest cycle in graph
- flood fill giving set of vertices in same component as given start vertex
- standard shortest path (seperate implementation for weighted graph) (also include all paths from single source version)
- directed graph (with at most one out edge per vertex) binary lifting
- directed graph negative cycle finding
- k shortest paths finding
- chinese remainder theorem
- fraction $p \over q$ closest approximation of $x \ge 0$ - obeys $|{ p \over q } - x| \le { n \over { q } }, p \le n, q \le n$
- prime sieve (maybe some $\mathcal{O}(n)$ version)
- sieve for all divisors
- single number is prime function (maybe millar rabin)
- single number get prime factors function (maybe use pollard rho)
- single number get divisors function
- big int class including basic operations as well as most number theory implementations that also exist in this library for regular integers
- implementation of binary search in range $[0, 1]$ represented as integer fraction ${ p \over q }, q \le n$.
- include precomputation of modular inverses in mod int class up to given limit
- modular log function
- large modular operations (upwards of $10^{ 18 }$) such as multiplication and exponentiation
- modular square root function such that the result $x$ gives the other solution implicitly $(-x)$
- $\mathcal{O}(log(n))$ arithmetic progression sums under mod
- euclids implementation: find $x, y$ such that $ax + by = \gcd(a, b)$
- phi function for all values less than some limit
- $n$-order linear recurrence relation recovery (berlekamp massey)
- computing determinant of matrix
- computing integer determinant of matrix under mod
- fast fourier transform
- convolution using FFT
- FFT under any mod, strictly doable for $\mod \cdot n \cdot log(n) < 8.6 \cdot 10^{14}$
- convolution using FFT under any mod
- fast subset transform and convolution using FST, i.e. $c_k = \sum\nolimits_{k = i \oplus j} a_i \cdot b_j$ where $\oplus$ is either AND/OR/XOR
- golden section search, i.e. optimized continuous ternary search
- simple integration of given function over an interval (maybe extend with adaptive simpson's rule)
- $\mathcal{O}(n^2log(k))$ linear recurrence (retrieve $k$-th term of $n$-th order)
- compute matrix inverse
- compute matrix inverse under mod
- number theoretic transform
- convolution using NTT
- polynomial interpolation, find $n - 1$ degree polynomial that intersect given $n$ points
- find real roots of polynomial
- polynomial class (consider packing polynomial based functions into this)
- simplex method, solve linear maximization problem
- linear equation solver (i.e. $M \cdot x = b$, where $M$ is a matrix and $x$ and $b$ are vectors), also make version yielding all uniquely determined values of $x$
- dictionary/phrase matching (aho corasick)
- rolling hash
- KMP
- longest palindrome around each index (manacher)
- compute rotation to achieve lexiographically minimum string
- suffix array builder
- suffix tree (maybe online)
- z-function, i.e. longest common prefix of entire string and suffox of every index
- bump allocator, i.e. allocator where freeing does not matter, include 32-bit pointers
- $\mathcal{O}(n \max(w_i))$ knapsack for given set of weights $w$ of size $n$
- fast modulo for constant unknown at compile time
- union interval container that merges intersecting intervals upon insertion $\mathcal{O}(log(n))$ insertion/query, also support removing interval, splitting existing intervals if needed
- smallest set from given set of intervals such that resulting set covers given interval
- retrieve longest increasing subsequence
- implementation of ternary search
