#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

void adjacency_matrix(char fname[STR_MAX], int n, int l, int Adj[n][n])
{
    int u, v, i;
    FILE *fp;

    /* 隣接行列の初期化 */
    for (u = 0; u < n; u++)
    {
        for (v = 0; v < n; v++)
        {
            Adj[u][v] = 0;
        }
    }

    /* 隣接行列の作成 */
    fp = fopen(fname, "r");
    for (i = 0; i < l; i++)
    {
        fscanf(fp, "%d %d\n", &u, &v);
        Adj[u][v] = Adj[v][u] = 1;
    }
    fclose(fp);
}

/* 読み込んだネットワークの頂点iに対する次数を求める */
void get_degree(int n, int d[n], int adj[n][n])
{
    int i, v;

    for (i = 0; i < n; i++)
    {
        d[i] = 0;
    }

    for (v = 0; v < n; v++)
    {
        for (i = 0; i < n; i++)
        {
            if (adj[v][i] == 1)
            {
                d[v]++;
            }
        }
    }
}

int get_random(int min, int max)
{
    return min + (int)(rand() * (max - min + 1.0) / (1.0 + RAND_MAX));
}

void randomize_network(int n, int d[n], int rand_adj[n][n])
{
    int v, k, i;
    int x, y;
    int d_num;
    int check = 1;

    /* 隣接行列の初期化 */
    for (i = 0; i < n; i++)
    {
        for (v = 0; v < n; v++)
        {
            rand_adj[i][v] = 0;
        }
    }

    /* 読み込んだ次数d[i]を元に，頂点iの接続先をランダムに決定 */
    for (i = 0; i < n; i++)
    {
        d_num = d[i];
        for (v = 0; v < d_num; v++)
        {
            y = get_random(0, n - 1);
            while (i == y || rand_adj[i][y] == 1)
            {
                y = get_random(0, n - 1);
            }
            d[i]--;
            d[y]--;
            rand_adj[i][y] = rand_adj[y][i] = 1;
        }
    }

    /* 頂点iに対する次数が正しくない場合，修正する */
    while (check)
    {
        for (i = 0; i < n; i++)
        {
            if (d[i] != 0)
            {
                if (d[i] < 0)
                {
                    for (v = 0; v < n; v++)
                    {
                        if (v != i && d[v] < 0 && rand_adj[i][v] == 1)
                        {
                            d[i]++;
                            d[v]++;
                            rand_adj[i][v] = rand_adj[v][i] = 0;
                        }
                        if (d[i] == 0)
                        {
                            break;
                        }
                    }

                    if (d[i] < 0)
                    {
                        d_num = (-1) * d[i];
                        for (v = 0; v < d_num; v++)
                        {
                            y = get_random(0, n - 1);
                            while (i == y || rand_adj[i][y] == 0)
                            {
                                y = get_random(0, n - 1);
                            }
                            d[i]++;
                            d[y]++;
                            rand_adj[i][y] = rand_adj[y][i] = 0;
                        }
                    }
                }
                else
                {
                    for (v = 0; v < n; v++)
                    {
                        if (v != i && d[v] > 0 && rand_adj[i][v] == 0)
                        {
                            d[i]--;
                            d[v]--;
                            rand_adj[i][v] = rand_adj[v][i] = 1;
                        }
                        if (d[i] == 0)
                        {
                            break;
                        }
                    }
                    if (d[i] > 0)
                    {
                        d_num = d[i];
                        for (v = 0; v < d_num; v++)
                        {
                            y = get_random(0, n - 1);
                            while (i == y || rand_adj[i][y] == 1)
                            {
                                y = get_random(0, n - 1);
                            }
                            d[i]--;
                            d[y]--;
                            rand_adj[i][y] = rand_adj[y][i] = 1;
                        }
                    }
                }
            }
        }

        /* 次数が正しいか判定 */
        for (i = 0; i < n; i++)
        {
            if (d[i] != 0)
            {
                check = 1;
                break;
            }
            else
            {
                check = 0;
            }
        }
    }
}

int check_diff_network(int n, int adj[n][n], int rand_adj[n][n])
{
    int i, v;

    for (i = 0; i < n; i++)
    {
        for (v = 0; v < n; v++)
        {
            if (adj[i][v] != rand_adj[i][v])
                return 0;
        }
    }

    return 1;
}

/* 読み込んだファイル名を元にランダマイズ用のファイル名を取得する */
void get_random_file_name(char fname[STR_MAX])
{
    /* test1.csvを読み込んだ場合，test1_random.csvを作り出す */
    char randfname[STR_MAX] = "_random.csv";
    int i;

    for (i = strlen(fname) - 2; i > 0; i--)
    {
        if (fname[i] == '.')
        {
            fname[i] = '\0';
            break;
        }
        fname[i] = '\0';
    }

    strcat(fname, randfname);
}

void write_file(char fname[STR_MAX], int n, int rand_adj[n][n])
{
    int i, v;
    FILE *outfp;

    outfp = fopen(fname, "w");
    if (outfp == NULL)
    {
        printf("Cannot open\n");
        exit(1);
    }

    for (i = 0; i < n; i++)
    {
        for (v = i; v < n; v++)
        {
            if (rand_adj[i][v] == 1)
            {
                fprintf(outfp, "%d %d\n", i, v);
            }
        }
    }

    fclose(outfp);
}

int main(void)
{
    int N;
    int line;
    char fname[STR_MAX];

    printf("input filename: ");
    fgets(fname, sizeof(fname), stdin); /* 標準入力からファイル名を取得 */
    fname[strlen(fname) - 1] = '\0';    /* 最後の改行コードを除去 */
    fflush(stdin);                      /* 256文字を超えた入力を標準入力から捨てる */

    /* 辺数と頂点数を求める */
    line = get_sides(fname);
    N = get_vertices(fname, line);

    /* 隣接行列を求める */
    int Adj[N][N];
    adjacency_matrix(fname, N, line, Adj);

    /* 読み込んだネットワークをランダマイズする */
    int D[N];
    int rand_Adj[N][N];
    int diff_check = 1;
    srand((unsigned int)time(NULL));

    while (diff_check)
    {
        get_degree(N, D, Adj);
        randomize_network(N, D, rand_Adj);
        diff_check = check_diff_network(N, Adj, rand_Adj);
    }

    /* ランダマイズしたネットワークを別名のファイルに書き込む */
    get_random_file_name(fname);
    write_file(fname, N, rand_Adj);

    printf("Created a randomized network: %s\n", fname);

    return 0;
}
