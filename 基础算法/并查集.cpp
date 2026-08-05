/*
并查集使用方法：

1. 初始化：
fa.resize(n + 1);
siz.assign(n + 1, 1);

for (int i = 1; i <= n; i++)
{
    fa[i] = i;
}

2. 连接两个点：
join(x, y);

3. 查询一个点所在连通块的根：
int rt = find(x);

4. 判断两个点是否连通：
if (find(x) == find(y))
{
    // x 和 y 连通
}
*/

vector<int> fa;
vector<int> siz;

int find(int x)
{
    if (fa[x] == x)
    {
        return x;
    }

    int rt = find(fa[x]);
    fa[x] = rt;

    return rt;
}

void join(int x, int y)
{
    x = find(x);
    y = find(y);

    if (x == y)
    {
        return;
    }

    // 小连通块连接到大连通块
    if (siz[x] > siz[y])
    {
        swap(x, y);
    }

    fa[x] = y;
    siz[y] += siz[x];
}
