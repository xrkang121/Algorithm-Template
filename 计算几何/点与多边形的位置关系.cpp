/*
判断点与多边形的位置关系

返回值：
0：点在多边形外部
1：点在多边形内部
2：点在多边形边界

使用方法：

int type = pointInPolygon(p, polygon);

if (type == 0)
{
    // 外部
}
else if (type == 1)
{
    // 内部
}
else
{
    // 边界
}
*/

int pointInPolygon(Point p, const vector<Point> &polygon)
{
    int n = polygon.size();
    int inside = 0;

    for (int i = 0; i < n; i++)
    {
        Point a = polygon[i];
        Point b = polygon[(i + 1) % n];

        // 点在线段上
        if (onSegment(p, a, b))
        {
            return 2;
        }

        // 线段从下向上穿过查询点所在水平线
        if (a.y <= p.y && p.y < b.y)
        {
            if (cross(a, b, p) > 0)
            {
                inside ^= 1;
            }
        }

        // 线段从上向下穿过查询点所在水平线
        if (b.y <= p.y && p.y < a.y)
        {
            if (cross(a, b, p) < 0)
            {
                inside ^= 1;
            }
        }
    }

    if (inside == 1)
    {
        return 1;
    }

    return 0;
}
