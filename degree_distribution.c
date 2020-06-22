#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STR_MAX 256

int get_sides(char fname[STR_MAX]) {
  int l;
  char buf[STR_MAX];
  FILE *fp;

  fp = fopen(fname, "r");
  while (fgets(buf, STR_MAX, fp) != NULL) {
    l++;
  }
  fclose(fp);

  return l;
}

int get_vertices(char fname[STR_MAX], int l) {
  int i, u, v;
  int bigger, max;
  FILE *fp;

  fp = fopen(fname, "r");
  for (i = 0; i < l; i++) {
    fscanf(fp, "%d %d\n", &u, &v);
    if (u > v)
      bigger = u;
    else
      bigger = v;

    if (max < bigger) max = bigger;
  }
  fclose(fp);

  return max + 1;
}

void adjacency_matrix(char fname[STR_MAX], int n, int l, int **adj) {
  int u, v, i;
  FILE *fp;

  /* 隣接行列の初期化 */
  for (u = 0; u < n; u++) {
    for (v = 0; v < n; v++) {
      adj[u][v] = 0;
    }
  }

  /* 隣接行列の作成 */
  fp = fopen(fname, "r");
  for (i = 0; i < l; i++) {
    fscanf(fp, "%d %d\n", &u, &v);
    adj[u][v] = adj[v][u] = 1;
  }
  fclose(fp);
}

void get_degree(int n, int d[n], int **adj) {
  int i, v;

  for (i = 0; i < n; i++) {
    d[i] = 0;
  }

  for (v = 0; v < n; v++) {
    for (i = 0; i < n; i++) {
      if (adj[v][i] == 1) {
        d[v]++;
      }
    }
  }
}

void get_degree_distribution(int n, int d[n], double p[n]) {
  int v;

  for (v = 0; v < n; v++) {
    p[v] = 0;
  }

  for (v = 0; v < n; v++) {
    p[d[v]]++;
  }

  for (v = 0; v < n; v++) {
    p[v] /= n;
  }
}

void get_degree_file_name(char fname[STR_MAX]) {
  /* test1.csvを読み込んだ場合，test1_degree.csvを作り出す */
  char randfname[STR_MAX] = "_degree.csv";
  int i;

  for (i = strlen(fname) - 2; i > 0; i--) {
    if (fname[i] == '.') {
      fname[i] = '\0';
      break;
    }
    fname[i] = '\0';
  }

  strcat(fname, randfname);
}

void write_file(char fname[STR_MAX], int n, double p[n]) {
  int i, v;
  FILE *outfp;

  outfp = fopen(fname, "w");
  if (outfp == NULL) {
    printf("Cannot open\n");
    exit(1);
  }

  fprintf(outfp, "k    P(k)\n");

  for (i = 0; i < n; i++) {
    fprintf(outfp, "%d    %lf\n", i, p[i]);
  }

  fclose(outfp);
}

int main(void) {
  int N;
  int line;
  int **Adj, i;
  double C;
  char fname[STR_MAX];

  printf("input filename: ");
  fgets(fname, sizeof(fname), stdin);
  fname[strlen(fname) - 1] = '\0';
  fflush(stdin);

  /* 辺数と頂点数を求める */
  line = get_sides(fname);
  N = get_vertices(fname, line);

  /* 隣接行列の作成 */
  Adj = (int **)malloc(sizeof(int *) * N);
  for (i = 0; i < N; i++) {
    Adj[i] = (int *)malloc(sizeof(int) * N);
  }
  adjacency_matrix(fname, N, line, Adj);

  /* 次数分布を求める */
  double P[N];
  int D[N];
  get_degree(N, D, Adj);
  get_degree_distribution(N, D, P);

  get_degree_file_name(fname);
  write_file(fname, N, P);

  printf("Created a degree distribution file: %s\n", fname);

  return 0;
}
