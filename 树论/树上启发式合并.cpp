#include <bits/stdc++.h>
using namespace std;

#define int long long

/*
==================================================
DSU on Tree（树上启发式合并）板子
==================================================

当前功能：
求每个节点 x 的子树中有多少种不同颜色。

使用方法：

1. 输入节点颜色和树边。
2. 对颜色进行离散化。
3. 调用：

dfs1(root, 0);
dfs2(root, 0, 1);

4. 此时：

ans[x] = 节点 x 的子树中不同颜色的数量。

--------------------------------------------------
dfs2(x, father, keep) 中 keep 的含义
--------------------------------------------------

keep = 1：计算完成后保留 x 子树的数据。
keep = 0：计算完成后清空 x 子树的数据。

标准顺序：

1. 计算所有轻儿子，计算后清空。
2. 计算重儿子，计算后保留。
3. 重新加入所有轻儿子的整棵子树。
4. 加入当前节点 x。
5. 计算 ans[x]。
6. keep=0 时清空 x 的整棵子树。

--------------------------------------------------
你需要根据题目修改的地方
--------------------------------------------------

1. MAXN：节点数量上限。
2. main()：输入和输出格式。
3. root：根节点，现在默认是 1。
4. changeNode()：加入和删除一个节点时，维护哪些信息。
5. dfs2() 中的 ans[x] = kind：当前节点的答案怎么计算。

下面这些内容通常不用修改：

siz、son、id、old_id
dfs1()
changeSubtree()
dfs2() 的整体流程

--------------------------------------------------
如果题目不是统计不同颜色
--------------------------------------------------

只需要修改两个位置：

一、changeNode(x, val)

val = 1：加入节点 x。
val = -1：删除节点 x。

在这里维护题目需要的全局统计信息。

二、dfs2() 中：

ans[x] = kind;

把 kind 换成题目要求的当前统计答案。

--------------------------------------------------
当前输入格式
--------------------------------------------------

n
n 个节点的颜色
n-1 条无向边

输出：
每个节点子树中不同颜色的数量。
==================================================
*/

// ===== 你需要修改：节点数量上限 =====
const int MAXN = 2e5 + 5;

int n;

vector<vector<int>> tree(MAXN);

// raw_color[x]：输入的原始颜色
// color[x]：离散化后的颜色
vector<int> raw_color(MAXN);
vector<int> color(MAXN);

// father[x]：x 的父亲
// siz[x]：x 的子树大小
// son[x]：x 的重儿子
// id[x]：x 的 DFS 序编号
// old_id[i]：DFS 序编号 i 对应的原节点
vector<int> father(MAXN);
vector<int> siz(MAXN);
vector<int> son(MAXN);
vector<int> id(MAXN);
vector<int> old_id(MAXN);

// cnt[c]：当前保留的数据中，颜色 c 出现次数
// ans[x]：节点 x 的答案
vector<int> cnt(MAXN);
vector<int> ans(MAXN);

int dfs_cnt = 0;
int kind = 0;

// 计算子树大小、重儿子和 DFS 序
void dfs1(int x, int fa)
{
    father[x] = fa;
    siz[x] = 1;
    son[x] = 0;

    id[x] = ++dfs_cnt;
    old_id[dfs_cnt] = x;

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

// ==================================================
// ===== 你需要根据题目修改：加入和删除节点 =========
//
// val = 1：加入节点 x
// val = -1：删除节点 x
//
// 当前维护的是不同颜色数量 kind。
// ==================================================
void changeNode(int x, int val)
{
    int c = color[x];

    if (val == 1)
    {
        if (cnt[c] == 0)
        {
            kind++;
        }

        cnt[c]++;
    }
    else
    {
        cnt[c]--;

        if (cnt[c] == 0)
        {
            kind--;
        }
    }
}

// 加入或者删除节点 x 的整棵子树
// 因为 DFS 序中一棵子树对应一个连续区间，所以可以直接遍历
void changeSubtree(int x, int val)
{
    int l = id[x];
    int r = id[x] + siz[x] - 1;

    for (int i = l; i <= r; i++)
    {
        int now = old_id[i];

        changeNode(now, val);
    }
}

void dfs2(int x, int fa, int keep)
{
    // 1. 先处理所有轻儿子，处理完成后清空
    for (int i = 0; i < tree[x].size(); i++)
    {
        int now = tree[x][i];

        if (now == fa || now == son[x])
        {
            continue;
        }

        dfs2(now, x, 0);
    }

    // 2. 再处理重儿子，处理完成后保留
    if (son[x] != 0)
    {
        dfs2(son[x], x, 1);
    }

    // 3. 重新加入所有轻儿子的整棵子树
    for (int i = 0; i < tree[x].size(); i++)
    {
        int now = tree[x][i];

        if (now == fa || now == son[x])
        {
            continue;
        }

        changeSubtree(now, 1);
    }

    // 4. 加入当前节点
    changeNode(x, 1);

    // ==================================================
    // ===== 你需要根据题目修改：记录当前节点答案 =====
    // 当前统计结构中正好是 x 的整棵子树。
    // ==================================================
    ans[x] = kind;

    // 5. 如果不需要保留，就清空当前整棵子树
    if (keep == 0)
    {
        changeSubtree(x, -1);
    }
}

signed main()
{
    ios::sync_with_stdio(0);
    cin.tie(0), cout.tie(0);

    // ===== 你需要根据题目修改：输入格式 =====
    cin >> n;

    vector<int> nums;

    for (int i = 1; i <= n; i++)
    {
        cin >> raw_color[i];
        nums.push_back(raw_color[i]);
    }

    // 颜色离散化，原始颜色很大或者为负数也可以处理
    sort(nums.begin(), nums.end());
    nums.erase(unique(nums.begin(), nums.end()), nums.end());

    for (int i = 1; i <= n; i++)
    {
        color[i] = lower_bound(nums.begin(), nums.end(), raw_color[i]) - nums.begin() + 1;
    }

    for (int i = 1; i < n; i++)
    {
        int x, y;
        cin >> x >> y;

        tree[x].push_back(y);
        tree[y].push_back(x);
    }

    // ===== 你需要修改：如果根不是 1，修改 root =====
    int root = 1;

    dfs1(root, 0);

    // 根节点没有父亲，并且处理完成后不需要清空，所以 keep=1
    dfs2(root, 0, 1);

    // ===== 你需要根据题目修改：输出格式 =====
    for (int i = 1; i <= n; i++)
    {
        cout << ans[i] << " ";
    }
    cout << "\n";

    return 0;
}
