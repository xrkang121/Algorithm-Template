#include <bits/stdc++.h>
using namespace std;

#define int long long

// ==================================================
// 树链剖分板子：节点权值版
//
// 支持操作：
// 1. changePath(x, y, val)  x 到 y 路径上的节点全部加 val
// 2. queryPath(x, y)        查询 x 到 y 路径上的节点权值和
// 3. changeSon(x, val)      x 的整棵子树全部加 val
// 4. querySon(x)            查询 x 的整棵子树权值和
//
// 你需要根据题目修改的地方：
// 1. MAXN：节点数量上限
// 2. main() 中的输入格式和操作编号
// 3. 根节点：现在默认是 1
// 4. 如果维护的不是“区间加、区间和”，修改线段树 Node 和相关函数
//
// 下面这些树链剖分数组和函数通常不用修改：
// father、depth、siz、son、top、id、old_id、dfs1、dfs2
// ==================================================

// ===== 你需要修改：节点数量上限 =====
const int MAXN = 2e5 + 5;

int n, q;

vector<vector<int>> tree(MAXN);

// value[x]：原树中节点 x 的权值
// arr[id[x]]：按照树链剖分新编号排列后的权值
vector<int> value(MAXN);
vector<int> arr(MAXN);

// father[x]：x 的父亲
// depth[x]：x 的深度
// siz[x]：x 的子树节点数量
// son[x]：x 的重儿子
// top[x]：x 所在重链的链顶
// id[x]：x 在线段树中的新编号
// old_id[i]：新编号 i 原来对应哪个节点
vector<int> father(MAXN);
vector<int> depth(MAXN);
vector<int> siz(MAXN);
vector<int> son(MAXN);
vector<int> top(MAXN);
vector<int> id(MAXN);
vector<int> old_id(MAXN);

int cnt = 0;

// 第一次 DFS：计算父亲、深度、子树大小和重儿子
void dfs1(int x, int fa)
{
    father[x] = fa;
    depth[x] = depth[fa] + 1;
    siz[x] = 1;
    son[x] = 0;

    for (int i = 0; i < tree[x].size(); i++)
    {
        int now = tree[x][i];

        if (now == fa)
        {
            continue;
        }

        dfs1(now, x);

        siz[x] += siz[now];

        if (siz[now] > siz[son[x]])
        {
            son[x] = now;
        }
    }
}

// 第二次 DFS：优先访问重儿子，让同一条重链编号连续
void dfs2(int x, int chain_top)
{
    top[x] = chain_top;
    id[x] = ++cnt;
    old_id[cnt] = x;
    arr[cnt] = value[x];

    if (son[x] != 0)
    {
        dfs2(son[x], chain_top);
    }

    for (int i = 0; i < tree[x].size(); i++)
    {
        int now = tree[x][i];

        if (now == father[x] || now == son[x])
        {
            continue;
        }

        // 轻儿子自己成为一条新链的链顶
        dfs2(now, now);
    }
}

// ==================================================
// ===== 你需要根据题目修改：线段树维护的信息 ========
// 当前维护：区间加、区间和
// ==================================================

struct Node
{
    int sum;
    int add;
};

Node tr[MAXN * 4];

void pushup(int rt)
{
    tr[rt].sum = tr[rt * 2].sum + tr[rt * 2 + 1].sum;
}

void build(int rt, int l, int r)
{
    tr[rt].add = 0;

    if (l == r)
    {
        tr[rt].sum = arr[l];
        return;
    }

    int mid = (l + r) / 2;

    build(rt * 2, l, mid);
    build(rt * 2 + 1, mid + 1, r);

    pushup(rt);
}

void addTag(int rt, int l, int r, int val)
{
    tr[rt].sum += (r - l + 1) * val;
    tr[rt].add += val;
}

void pushdown(int rt, int l, int r)
{
    if (tr[rt].add == 0)
    {
        return;
    }

    int mid = (l + r) / 2;

    addTag(rt * 2, l, mid, tr[rt].add);
    addTag(rt * 2 + 1, mid + 1, r, tr[rt].add);

    tr[rt].add = 0;
}

void rangeChange(int rt, int l, int r, int x, int y, int val)
{
    if (x <= l && r <= y)
    {
        addTag(rt, l, r, val);
        return;
    }

    pushdown(rt, l, r);

    int mid = (l + r) / 2;

    if (x <= mid)
    {
        rangeChange(rt * 2, l, mid, x, y, val);
    }

    if (y > mid)
    {
        rangeChange(rt * 2 + 1, mid + 1, r, x, y, val);
    }

    pushup(rt);
}

int query(int rt, int l, int r, int x, int y)
{
    if (x <= l && r <= y)
    {
        return tr[rt].sum;
    }

    pushdown(rt, l, r);

    int mid = (l + r) / 2;
    int res = 0;

    if (x <= mid)
    {
        res += query(rt * 2, l, mid, x, y);
    }

    if (y > mid)
    {
        res += query(rt * 2 + 1, mid + 1, r, x, y);
    }

    return res;
}

// x 到 y 路径上的所有节点加 val
void changePath(int x, int y, int val)
{
    while (top[x] != top[y])
    {
        if (depth[top[x]] < depth[top[y]])
        {
            swap(x, y);
        }

        rangeChange(1, 1, n, id[top[x]], id[x], val);

        x = father[top[x]];
    }

    if (depth[x] > depth[y])
    {
        swap(x, y);
    }

    rangeChange(1, 1, n, id[x], id[y], val);
}

// 查询 x 到 y 路径上的节点权值和
int queryPath(int x, int y)
{
    int res = 0;

    while (top[x] != top[y])
    {
        if (depth[top[x]] < depth[top[y]])
        {
            swap(x, y);
        }

        res += query(1, 1, n, id[top[x]], id[x]);

        x = father[top[x]];
    }

    if (depth[x] > depth[y])
    {
        swap(x, y);
    }

    res += query(1, 1, n, id[x], id[y]);

    return res;
}

// x 的整棵子树全部加 val
void changeSon(int x, int val)
{
    rangeChange(1, 1, n, id[x], id[x] + siz[x] - 1, val);
}

// 查询 x 的整棵子树权值和
int querySon(int x)
{
    return query(1, 1, n, id[x], id[x] + siz[x] - 1);
}

signed main()
{
    ios::sync_with_stdio(0);
    cin.tie(0), cout.tie(0);

    // ==================================================
    // ===== 你需要根据题目修改：输入格式和操作编号 ====
    //
    // 当前示例输入格式：
    // n q
    // n 个节点权值
    // n-1 条无向边
    // q 次操作
    //
    // 操作 1 x y val：x 到 y 路径全部加 val
    // 操作 2 x y：查询 x 到 y 路径和
    // 操作 3 x val：x 的整棵子树全部加 val
    // 操作 4 x：查询 x 的整棵子树和
    // ==================================================

    cin >> n >> q;

    for (int i = 1; i <= n; i++)
    {
        cin >> value[i];
    }

    for (int i = 1; i < n; i++)
    {
        int x, y;
        cin >> x >> y;

        tree[x].push_back(y);
        tree[y].push_back(x);
    }

    // ===== 你需要修改：如果根不是 1，替换这里的 root =====
    int root = 1;

    dfs1(root, 0);
    dfs2(root, root);
    build(1, 1, n);

    while (q--)
    {
        int op;
        cin >> op;

        if (op == 1)
        {
            int x, y, val;
            cin >> x >> y >> val;

            changePath(x, y, val);
        }
        else if (op == 2)
        {
            int x, y;
            cin >> x >> y;

            cout << queryPath(x, y) << "\n";
        }
        else if (op == 3)
        {
            int x, val;
            cin >> x >> val;

            changeSon(x, val);
        }
        else if (op == 4)
        {
            int x;
            cin >> x;

            cout << querySon(x) << "\n";
        }
    }

    return 0;
}
