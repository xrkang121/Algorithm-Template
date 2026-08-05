/*
格点多边形

boundaryPoints(polygon)
求多边形边界上的格点数量。

interiorPoints(polygon)
求多边形内部的格点数量。

要求：
多边形顶点都是整数点，并且多边形没有自交。
*/

// 求多边形边界上的格点数量
int boundaryPoints(const vector<Point> &polygon)
{
    int n = polygon.size();
    int res = 0;

    for (int i = 0; i < n; i++)
    {
        Point a = polygon[i];
        Point b = polygon[(i + 1) % n];

        int dx = abs(a.x - b.x);
        int dy = abs(a.y - b.y);

        res += gcd(dx, dy);
    }

    return res;
}

// 求多边形内部的格点数量
int interiorPoints(const vector<Point> &polygon)
{
    int area2 = polygonArea2(polygon);
    int boundary = boundaryPoints(polygon);

    return (area2 - boundary + 2) / 2;
}
