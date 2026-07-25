vector<vector<int>> diff(n + 2, vector<int>(m + 2, 0));

//对矩形区域的值加上val
diff[x1][y1] += val;
diff[x1][y2 + 1] -= val;
diff[x2 + 1][y1] -= val;
diff[x2 + 1][y2 + 1] += val;

//最后合并
for (int i = 1; i <= n; i++)
{
    for (int j = 1; j <= m; j++)
    {
        diff[i][j] += diff[i - 1][j] + diff[i][j - 1] - diff[i - 1][j - 1];
    }
}
