/*
 * @lc app=leetcode.cn id=1584 lang=cpp
 *
 * [1584] 连接所有点的最小费用
 */

// @lc code=start
class DisjointSetUnion {
private:
    vector<int> f;     // f[x]: x的父节点
    vector<int> rank;  // rank[x]: 按秩合并，记录集合大小，用于树的平衡
    int n;

public:
    DisjointSetUnion(int _n) {
        n = _n;
        f.resize(n);
        rank.resize(n, 1);
        for(int i = 0; i < n; i++)
            f[i] = i;
    }

    // 查找函数
    int Find(int x) {
        int root = x;
        while(f[root] != root){
            root = f[root];
        }
        //路径压缩：把路径上所有点直接指向root
        while(f[x] != root){
            int next = f[x];
            f[x] = root;
            x = next;
        }
        return root;
    }

    // 合并函数
    bool Union(int x, int y) {
        int fx = Find(x);
        int fy = Find(y);
        if(fx == fy) return false;
        if(rank[fx] < rank[fy])  { // 合并到y
            rank[fy] += rank[fx];
            f[fx] = fy;
        }
        else {  // 合并到x
            rank[fx] += rank[fy];
            f[fy] = fx;
        }
        return true;
    }
};

class Solution {
public:
    // 获得曼哈顿距离
    int getDist(vector<vector<int>>& points, int x, int y) {
        return abs(points[x][0] - points[y][0]) + abs(points[x][1] - points[y][1]);
    }

    struct Edge {
        int len;
        int from;
        int to;
    };


    int minCostConnectPoints(vector<vector<int>>& points) {
        int n = points.size();

        vector<Edge> edges;
        for(int i = 0; i < n; i++) {
            for(int j = i + 1; j < n; j++) {
                Edge e;
                e.len = getDist(points, i, j);
                e.from = i;
                e.to = j;
                edges.push_back(e);
            }
        }

        // 比较函数
        auto cmp = [&](Edge& x, Edge& y) {
            return x.len < y.len;
        };
        sort(edges.begin(), edges.end(), cmp);

        // 并查集初始化
        DisjointSetUnion dsu(n);

        int res = 0;
        int count = 0;
        for(Edge e : edges) {
            if(dsu.Union(e.from, e.to)) {
                res += e.len;
                count++;
                if(count == n) break;
            }
        }
        return res;
    }
};
// @lc code=end

// 示例
class DisjointSetUnion {
private:
    vector<int> f;     // f[x]: x的父节点
    vector<int> rank;  // rank[x]: 按秩合并，记录集合大小，用于树的平衡
    int n;

public:
    // 初始化n个元素，每个元素自己是一个集合
    DisjointSetUnion(int _n) {
        n = _n;
        rank.resize(n, 1);
        f.resize(n);
        for (int i = 0; i < n; i++) {
            f[i] = i;   // 初始父节点指向自己
        }
    }

    // find：查找x的根，带路径压缩
    int find(int x) {
        return f[x] == x ? x : f[x] = find(f[x]);
    }

    // 合并x,y所在集合；返回true代表真正合并成功，false代表本来就在同一个集合
    bool unionSet(int x, int y) {
        int fx = find(x);
        int fy = find(y);
        if (fx == fy) {
            return false; // 同集合，出现环，不选这条边
        }
        // 按秩合并：小集合合并到大集合下，树不会太高
        if (rank[fx] < rank[fy]) {
            swap(fx, fy);
        }
        rank[fx] += rank[fy];
        f[fy] = fx;
        return true;
    }
};

// 边结构体：边的长度len，连接点x、点y
struct Edge {
    int len, x, y;
    Edge(int len, int x, int y) : len(len), x(x), y(y) {
    }
};


class Solution {
public:
    // 计算曼哈顿距离，代替lambda
    int getDist(const vector<vector<int>>& points, int x, int y) {
        return abs(points[x][0] - points[y][0]) + abs(points[x][1] - points[y][1]);
    }

    int minCostConnectPoints(vector<vector<int>>& points) {
        int n = points.size();
        DisjointSetUnion dsu(n);
        vector<Edge> edges;

        // 生成全部点对之间的边：i<j避免重复生成(i,j)和(j,i)
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                edges.emplace_back(getDist(points, i, j), i, j);
            }
        }

        // Kruskal核心：边按长度从小到大升序排序
        sort(edges.begin(), edges.end(), [](Edge a, Edge b) -> int {
            return a.len < b.len;
        });

        int ret = 0;   // 累加最小生成树总费用
        int num = 1;   // 当前已经选入MST的点的数量，初始1个点

        // 遍历从小到大所有边，尝试选边
        for (auto& e : edges) {
            int len = e.len;
            int x = e.x;
            int y = e.y;

            // 如果两点不在同一集合，选这条边，加入MST
            if (dsu.unionSet(x, y)) {
                ret += len;
                num++;
                // MST有n个点，选够n个点直接退出，不用遍历剩余边
                if (num == n) {
                    break;
                }
            }
        }
        return ret;
    }
};