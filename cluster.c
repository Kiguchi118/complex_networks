#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STR_MAX 256

int get_sides(char fname[STR_MAX])
{
    int l;
    char buf[STR_MAX];
    FILE *fp;

    fp = fopen(fname, "r");
    while (fgets(buf, STR_MAX, fp) != NULL)
    {
        l++;
    }
    fclose(fp);

    return l;
}

int get_vertices(char fname[STR_MAX], int l)
{
    int i, u, v;
    int bigger, max;
    FILE *fp;

    fp = fopen(fname, "r");
    for (i = 0; i < l; i++)
    {
        fscanf(fp, "%d %d\n", &u, &v);
        if (u > v)
            bigger = u;
        else
            bigger = v;

        if (max < bigger)
            max = bigger;
    }
    fclose(fp);

    return max + 1;
}

double combination(int n, int r)
{
    if (r == 0 || r == n)
        return 1;
    else if (r == 1)
        return n;
    return (combination(n - 1, r - 1) + combination(n - 1, r));
}

void adjacency_matrix(char fname[STR_MAX], int n, int l, int **adj)
{
    int u, v, i;
    FILE *fp;

    /* 隣接行列の初期化 */
    for (u = 0; u < n; u++)
    {
        for (v = 0; v < n; v++)
        {
            adj[u][v] = 0;
        }
    }

    /* 隣接行列の作成 */
    fp = fopen(fname, "r");
    for (i = 0; i < l; i++)
    {
        fscanf(fp, "%d %d\n", &u, &v);
        adj[u][v] = adj[v][u] = 1;
    }
    fclose(fp);
}

double cluster_coefficient(int n, int **adj)
{
    int v0, i, u, k;
    double c;
    int adj_point[n];
    int triangle_num;

    for (v0 = 0; v0 < n; v0++)
    {
        k = 0;
        triangle_num = 0;
        for (i = 0; i < n; i++)
        {
            adj_point[i] = 0;
        }

        /* 次数kと隣接点を求める */
        for (i = 0; i < n; i++)
        {
            if (adj[v0][i] == 1)
            {
                adj_point[k] = i;
                k++;
            }
        }

        /* クラスタを求める */
        for (i = 0; i < k; i++)
        {
            for (u = 0; u < k; u++)
            {
                if (adj[adj_point[i]][adj_point[u]] == 1)
                    triangle_num++;
            }
        }

        if (triangle_num != 0)
        {
            triangle_num /= 2;
            c += triangle_num / combination(k, 2);
        }
    }

    return c / n;
}

int main(void)
{
    int N;
    int line;
    int **Adj, i;
    double C;
    char fname[STR_MAX];

    printf("input filename: ");
    fgets(fname, sizeof(fname), stdin); /* 標準入力からファイル名を取得 */
    fname[strlen(fname) - 1] = '\0';    /* 最後の改行コードを除去 */
    fflush(stdin);                      /* 256文字を超えた入力を標準入力から捨てる */

    /* 辺数と頂点数を求める */
    line = get_sides(fname);
    N = get_vertices(fname, line);

    printf("頂点数: %d\n", N);

    /* 隣接行列の作成 */
    Adj = (int **)malloc(sizeof(int *) * N);
    for (i = 0; i < N; i++)
    {
        Adj[i] = (int *)malloc(sizeof(int) * N);
    }
    adjacency_matrix(fname, N, line, Adj);

    /* クラスタ係数を求める */
    C = cluster_coefficient(N, Adj);

    printf("クラスタ係数: %f\n", C);

    return 0;
}
