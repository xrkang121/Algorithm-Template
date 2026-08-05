#include <bits/stdc++.h>
using namespace std;

#define int long long

/*
二维整数几何基础板子

Point a(x, y)              创建一个点或向量
a + b                      向量相加
a - b                      向量相减
a == b                     判断两个点是否相同

cross(a, b)                求向量 a 和 b 的叉积
cross(a, b, c)             求向量 ab 和 ac 的叉积

dot(a, b)                  求向量 a 和 b 的点积
dot(a, b, c)               求向量 ab 和 ac 的点积

dis2(a, b)                 求两点距离的平方

onSegment(p, a, b)         判断点 p 是否在线段 ab 上
segmentIntersect(a,b,c,d)  判断线段 ab 和 cd 是否相交
*/

struct Point
{
    int x;
    int y;

    Point()
    {
        x = 0;
        y = 0;
    }

    Point(int _x, int _y)
    {
        x = _x;
        y = _y;
    }

    Point operator+(const Point &a) const
    {
        return Point(x + a.x, y + a.y);
    }

    Point operator-(const Point &a) const
    {
        return Point(x - a.x, y - a.y);
    }

    Point operator*(int k) const
    {
        return Point(x * k, y * k);
    }

    bool operator==(const Point &a) const
    {
        return x == a.x && y == a.y;
    }

    bool operator<(const Point &a) const
    {
        if (x == a.x)
        {
            return y < a.y;
        }

        return x < a.x;
    }
};

// 求两个向量的叉积
int cross(Point a, Point b)
{
    return a.x * b.y - a.y * b.x;
}

// 求向量 ab 和 ac 的叉积
int cross(Point a, Point b, Point c)
{
    return cross(b - a, c - a);
}

// 求两个向量的点积
int dot(Point a, Point b)
{
    return a.x * b.x + a.y * b.y;
}

// 求向量 ab 和 ac 的点积
int dot(Point a, Point b, Point c)
{
    return dot(b - a, c - a);
}

// 求两点距离的平方
int dis2(Point a, Point b)
{
    int dx = a.x - b.x;
    int dy = a.y - b.y;

    return dx * dx + dy * dy;
}

// 判断点 p 是否在线段 ab 上
bool onSegment(Point p, Point a, Point b)
{
    if (cross(a, b, p) != 0)
    {
        return false;
    }

    if (p.x < min(a.x, b.x) || p.x > max(a.x, b.x))
    {
        return false;
    }

    if (p.y < min(a.y, b.y) || p.y > max(a.y, b.y))
    {
        return false;
    }

    return true;
}

// 判断两个数是否异号
bool differentSign(int a, int b)
{
    if (a > 0 && b < 0)
    {
        return true;
    }

    if (a < 0 && b > 0)
    {
        return true;
    }

    return false;
}

// 判断线段 ab 和 cd 是否相交
bool segmentIntersect(Point a, Point b, Point c, Point d)
{
    int c1 = cross(a, b, c);
    int c2 = cross(a, b, d);
    int c3 = cross(c, d, a);
    int c4 = cross(c, d, b);

    if (c1 == 0 && onSegment(c, a, b))
    {
        return true;
    }

    if (c2 == 0 && onSegment(d, a, b))
    {
        return true;
    }

    if (c3 == 0 && onSegment(a, c, d))
    {
        return true;
    }

    if (c4 == 0 && onSegment(b, c, d))
    {
        return true;
    }

    if (differentSign(c1, c2) && differentSign(c3, c4))
    {
        return true;
    }

    return false;
}
