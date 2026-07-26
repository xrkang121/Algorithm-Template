#include <bits/stdc++.h>
using namespace std;

#define int long long

const int MAXN = 2e5 + 5;
const int LOG = 20;

vector<vector<int>> tree(MAXN);
vector<int> depth(MAXN);

// fa[x][j]：x 向上跳 2^j 步到达的祖先
int fa[MAXN][LOG + 1];

// 预处理深度和倍增祖先
void dfs(int x, int father)
{
    // x 的直接父亲
    fa[x][0] = father;

    // 计算 x 向上跳 2^i 步的祖先
    for (int i = 1; i <= LOG; i++)
    {
        fa[x][i] = fa[fa[x][i - 1]][i - 1];
    }

    for (int i = 0; i < tree[x].size(); i++)
    {
        int now = tree[x][i];

        // 无向边，不能走回父亲
        if (now == father)
        {
            continue;
        }

        depth[now] = depth[x] + 1;
        dfs(now, x);
    }
}

// 求 x 和 y 的最近公共祖先
int lca(int x, int y)
{
    // 保证 x 是深度更大的节点
    if (depth[x] < depth[y])
    {
        swap(x, y);
    }

    // 第一步：让 x 和 y 跳到相同深度
    for (int i = LOG; i >= 0; i--)
    {
        if (depth[fa[x][i]] >= depth[y])
        {
            x = fa[x][i];
        }
    }

    // 如果此时相等，说明 y 原本就是 x 的祖先
    if (x == y)
    {
        return x;
    }

    // 第二步：x 和 y 一起向上跳
    // 保证跳完以后它们仍然不相等
    for (int i = LOG; i >= 0; i--)
    {
        if (fa[x][i] != fa[y][i])
        {
            x = fa[x][i];
            y = fa[y][i];
        }
    }

    // 此时 x 和 y 的父亲就是最近公共祖先
    return fa[x][0];
}

signed main()
{
    ios::sync_with_stdio(0);
    cin.tie(0), cout.tie(0);

    int n;
    cin >> n;

    for (int i = 1; i < n; i++)
    {
        int x, y;
        cin >> x >> y;

        // 无向树，两边都要存
        tree[x].push_back(y);
        tree[y].push_back(x);
    }

    // 规定 1 号节点为根
    // 0 号节点作为不存在的祖先
    depth[0] = 0;
    depth[1] = 1;

    dfs(1, 0);

    int q;
    cin >> q;

    while (q--)
    {
        int x, y;
        cin >> x >> y;

        cout << lca(x, y) << '\n';
    }

    return 0;
}
