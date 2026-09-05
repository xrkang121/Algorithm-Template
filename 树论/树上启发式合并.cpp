用途：
- 对每个节点 x，统计“子树 x”里的信息
- 例如：子树不同颜色数、某颜色出现次数、众数、最大出现次数等

适用前提：
- 查询对象是“子树”
- 加入 / 删除一个节点的信息比较容易维护

核心复杂度：
    O(n log n)

核心思想：
    大儿子的统计结果保留
    小儿子的统计结果重新加入


================================================
一、常用数组
================================================

vector<vector<int>> mymap;

vector<int> siz;
vector<int> son;

vector<int> color;
vector<int> cnt;
vector<int> ans;

含义：

    siz[x]   = x 的子树大小
    son[x]   = x 的重儿子（子树最大的儿子）
    color[x] = x 的颜色
    cnt[c]   = 当前保留数据中颜色 c 出现次数
    ans[x]   = x 的答案


================================================
二、第一遍 DFS：求 siz 和重儿子
================================================

void dfs1(int x, int fa)
{
    siz[x] = 1;

    for (int i = 0; i < mymap[x].size(); i++)
    {
        int nxt = mymap[x][i];

        if (nxt == fa)
        {
            continue;
        }

        dfs1(nxt, x);

        siz[x] += siz[nxt];

        if (son[x] == 0 || siz[nxt] > siz[son[x]])
        {
            son[x] = nxt;
        }
    }
}


================================================
三、维护答案：以“不同颜色数”为例
================================================

int now_ans = 0;

void add_point(int x)
{
    if (cnt[color[x]] == 0)
    {
        now_ans++;
    }

    cnt[color[x]]++;
}

void del_point(int x)
{
    cnt[color[x]]--;

    if (cnt[color[x]] == 0)
    {
        now_ans--;
    }
}


================================================
四、加 / 删整棵子树
================================================

void add_tree(int x, int fa)
{
    add_point(x);

    for (int i = 0; i < mymap[x].size(); i++)
    {
        int nxt = mymap[x][i];

        if (nxt == fa)
        {
            continue;
        }

        add_tree(nxt, x);
    }
}

void del_tree(int x, int fa)
{
    del_point(x);

    for (int i = 0; i < mymap[x].size(); i++)
    {
        int nxt = mymap[x][i];

        if (nxt == fa)
        {
            continue;
        }

        del_tree(nxt, x);
    }
}


================================================
五、核心 DFS
================================================

void dfs2(int x, int fa, bool keep)
{
    // 1. 先处理轻儿子，算完清掉
    for (int i = 0; i < mymap[x].size(); i++)
    {
        int nxt = mymap[x][i];

        if (nxt == fa || nxt == son[x])
        {
            continue;
        }

        dfs2(nxt, x, false);
    }

    // 2. 再处理重儿子，并保留
    if (son[x])
    {
        dfs2(son[x], x, true);
    }

    // 3. 把轻儿子重新加回来
    for (int i = 0; i < mymap[x].size(); i++)
    {
        int nxt = mymap[x][i];

        if (nxt == fa || nxt == son[x])
        {
            continue;
        }

        add_tree(nxt, x);
    }

    // 4. 加自己
    add_point(x);

    // 5. 此时当前状态 = x 的整棵子树
    ans[x] = now_ans;

    // 6. 如果父亲不需要保留，就清空
    if (!keep)
    {
        del_tree(x, fa);
    }
}


================================================
六、完整可运行版本：每个子树不同颜色数
================================================

#include <bits/stdc++.h>
using namespace std;

#define int long long

vector<vector<int>> mymap;

vector<int> siz;
vector<int> son;

vector<int> color;
vector<int> cnt;
vector<int> ans;

int now_ans = 0;

void dfs1(int x, int fa)
{
    siz[x] = 1;

    for (int i = 0; i < mymap[x].size(); i++)
    {
        int nxt = mymap[x][i];

        if (nxt == fa)
        {
            continue;
        }

        dfs1(nxt, x);

        siz[x] += siz[nxt];

        if (son[x] == 0 || siz[nxt] > siz[son[x]])
        {
            son[x] = nxt;
        }
    }
}

void add_point(int x)
{
    if (cnt[color[x]] == 0)
    {
        now_ans++;
    }

    cnt[color[x]]++;
}

void del_point(int x)
{
    cnt[color[x]]--;

    if (cnt[color[x]] == 0)
    {
        now_ans--;
    }
}

void add_tree(int x, int fa)
{
    add_point(x);

    for (int i = 0; i < mymap[x].size(); i++)
    {
        int nxt = mymap[x][i];

        if (nxt == fa)
        {
            continue;
        }

        add_tree(nxt, x);
    }
}

void del_tree(int x, int fa)
{
    del_point(x);

    for (int i = 0; i < mymap[x].size(); i++)
    {
        int nxt = mymap[x][i];

        if (nxt == fa)
        {
            continue;
        }

        del_tree(nxt, x);
    }
}

void dfs2(int x, int fa, bool keep)
{
    for (int i = 0; i < mymap[x].size(); i++)
    {
        int nxt = mymap[x][i];

        if (nxt == fa || nxt == son[x])
        {
            continue;
        }

        dfs2(nxt, x, false);
    }

    if (son[x])
    {
        dfs2(son[x], x, true);
    }

    for (int i = 0; i < mymap[x].size(); i++)
    {
        int nxt = mymap[x][i];

        if (nxt == fa || nxt == son[x])
        {
            continue;
        }

        add_tree(nxt, x);
    }

    add_point(x);

    ans[x] = now_ans;

    if (!keep)
    {
        del_tree(x, fa);
    }
}

signed main()
{
    ios::sync_with_stdio(0);
    cin.tie(0), cout.tie(0);

    int n;
    cin >> n;

    mymap.resize(n + 1);

    siz.resize(n + 1);
    son.resize(n + 1);

    color.resize(n + 1);
    ans.resize(n + 1);

    int max_color = 0;

    for (int i = 1; i <= n; i++)
    {
        cin >> color[i];
        max_color = max(max_color, color[i]);
    }

    cnt.resize(max_color + 1);

    for (int i = 1; i < n; i++)
    {
        int x, y;
        cin >> x >> y;

        mymap[x].push_back(y);
        mymap[y].push_back(x);
    }

    dfs1(1, 0);
    dfs2(1, 0, true);

    for (int i = 1; i <= n; i++)
    {
        cout << ans[i] << '\n';
    }

    return 0;
}


================================================
七、你真正需要改的地方
================================================

大多数题：

    dfs1()
    dfs2()
    add_tree()
    del_tree()

都基本不改。

真正按题修改的是：

    add_point()
    del_point()
    ans[x] = ...

所以看到题时先问：

    “加入一个节点后，我能不能快速更新答案？”

如果能，
DSU on Tree 大概率能做。


================================================
八、常见变形
================================================

1. 子树中某颜色 c 出现多少次

直接：

    ans[x] = cnt[c];


2. 子树中不同颜色数

维护：

    cnt[color]
    now_ans


3. 子树最大出现次数

维护：

    cnt[c]
    freq[k] = 当前有多少种颜色出现 k 次
    mx = 当前最大出现次数

最后：

    ans[x] = mx;


4. 子树中“出现次数等于 k”的颜色数

维护：

    freq[k]

最后：

    ans[x] = freq[k];


================================================
九、如果删除很难维护
================================================

有些题：

    加一个点很好维护
    但删除一个点很麻烦

DSU on Tree 里经常可以利用：

    !keep 时整棵都要清空

所以可以：

    只正常维护 add

清空整棵以后直接把全局辅助答案：

    mx = 0;
    sum = 0;

重新置零。

这在“众数颜色编号和”一类题里很常见。


================================================
十、推荐的 DFS 序版本
================================================

如果不喜欢递归 add_tree / del_tree，
可以第一遍 DFS 顺便求：

    dfn[x]
    rev[dfn]
    siz[x]


那么子树 x 在 DFS 序中是连续区间：

    [dfn[x], dfn[x] + siz[x] - 1]


于是：

void add_tree(int x)
{
    for (int i = dfn[x]; i <= dfn[x] + siz[x] - 1; i++)
    {
        add_point(rev[i]);
    }
}


void del_tree(int x)
{
    for (int i = dfn[x]; i <= dfn[x] + siz[x] - 1; i++)
    {
        del_point(rev[i]);
    }
}


这个版本比赛里很舒服，
因为不用反复处理 fa。


================================================
十一、什么时候想到 DSU on Tree
================================================

看到：

    一棵树

并且：

    对每个节点 x
    都要求它“整个子树”的统计信息


例如：

    子树颜色种数
    子树某颜色出现次数
    子树众数
    子树频率统计


并且：

    加一个节点容易维护

就考虑：

    DSU on Tree


================================================
十二、什么时候不要用
================================================

动态修改 + 子树查询：

    DFS 序 + 树状数组 / 线段树


路径查询：

    树链剖分
    LCA
    树上莫队


只求一次子树：

    普通 DFS


================================================
十三、赛场写题顺序
================================================

1.
写 dfs1：

    siz
    son


2.
确定：

    cnt[] 到底维护什么


3.
写：

    add_point()


4.
需要的话写：

    del_point()


5.
写 dfs2，顺序固定：

    轻儿子 keep=false
    ↓
    重儿子 keep=true
    ↓
    轻儿子重新加入
    ↓
    加自己
    ↓
    记录 ans[x]
    ↓
    !keep 清空


================================================
十四、最短速记
================================================

DSU on Tree：

    先轻后重


轻儿子：

    算完清掉


重儿子：

    算完保留


然后：

    轻儿子重新加回来


最后：

    当前状态 = x 的整棵子树


真正按题改：

    add_point()
    del_point()
    ans[x]


看到：

    “对每个节点求子树统计”

优先想：

    DSU on Tree
