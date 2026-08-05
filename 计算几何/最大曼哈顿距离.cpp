/*
求所有点中的最大曼哈顿距离

使用方法：

int res = maxManhattan(points);
*/

int maxManhattan(const vector<Point> &points)
{
    if (points.size() <= 1)
    {
        return 0;
    }

    int min_u = (1LL << 62);
    int max_u = -(1LL << 62);

    int min_v = (1LL << 62);
    int max_v = -(1LL << 62);

    for (int i = 0; i < points.size(); i++)
    {
        int u = points[i].x + points[i].y;
        int v = points[i].x - points[i].y;

        min_u = min(min_u, u);
        max_u = max(max_u, u);

        min_v = min(min_v, v);
        max_v = max(max_v, v);
    }

    return max(max_u - min_u, max_v - min_v);
}
