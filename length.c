#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STR_MAX 256
#define inf 1000000
#define WHITE 0
#define GRAY 1
#define BLACK 2

struct cell {
  int vertex;
  int next;
};

unsigned long int get_sides(char fname[STR_MAX]) {
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

double combination(int n, int r) {
  if (r == 0 || r == n)
    return 1;
  else if (r == 1)
    return n;
  return (combination(n - 1, r - 1) + combination(n - 1, r));
}

double characteristic_path_length(char fname[STR_MAX], int n, int l) {
  int d[n], p[n], color[n];
  int Q[n], A[n];
  int head, tail;
  int u, v, v0, i, e, a;
  unsigned long int D = 0;
  struct cell *List;
  FILE *fp;

  List = (struct cell *)malloc(sizeof(struct cell) * (2 * l));

  /* 幅優先探索の準備 */
  for (u = 0; u < n; u++) {
    A[u] = -1;
  }

  for (u = 0; u < 2 * l; u++) {
    List[u].next = -1;
  }

  fp = fopen(fname, "r");
  for (i = 0; i < 2 * l; i++) {
    fscanf(fp, " %d %d", &u, &v);
    if (A[u] == -1) {
      List[i].vertex = v;
      A[u] = i;
    } else {
      e = A[u];
      while (List[e].next != -1) {
        e = List[e].next;
      }
      List[e].next = i;
      List[i].vertex = v;
    }
    i++;
    if (A[v] == -1) {
      List[i].vertex = u;
      A[v] = i;
    } else {
      e = A[v];
      while (List[e].next != -1) {
        e = List[e].next;
      }
      List[e].next = i;
      List[i].vertex = u;
    }
  }
  fclose(fp);

  /* 幅優先探索で最短距離を求め，合計する */
  for (v0 = 0; v0 < n; v0++) {
    for (v = 0; v < n; v++) {
      color[v] = WHITE;
      d[v] = inf;
      p[v] = -1;
    }
    head = 0;
    tail = 0;
    Q[tail] = v0;
    tail++;
    if (tail + 1 == n) {
      tail = 0;
    }
    if (tail == head) {
      printf("ERROR:Overflow\n");
      exit(1);
    }

    color[v0] = GRAY;
    d[v0] = 0;
    p[v0] = -1;
    while (head != tail) {
      if (head == tail) {
        printf("ERROR:Underflow\n");
        exit(1);
      } else {
        a = Q[head];
        head++;
        if (head + 1 == n) {
          head = 0;
        }
      }
      v = a;
      e = A[v];
      while (e != -1) {
        if (color[List[e].vertex] == WHITE) {
          Q[tail] = List[e].vertex;
          tail++;
          if (tail + 1 == n) {
            tail = 0;
          }
          if (tail == head) {
            printf("ERROR:Overflow\n");
            exit(1);
          }
          color[List[e].vertex] = GRAY;
          d[List[e].vertex] = d[v] + 1;
          p[List[e].vertex] = v;
        }
        e = List[e].next;
      }
      color[v] = BLACK;
    }

    /* 最短距離を合計 */
    for (v = v0; v < n; v++) {
      if (d[v] == inf)
        continue;
      else
        D += d[v];
    }
  }

  return D / combination(n, 2);
}

int main(void) {
  int line;
  int N;
  double L;
  char fname[STR_MAX];

  printf("input filename: ");
  fgets(fname, sizeof(fname), stdin);
  fname[strlen(fname) - 1] = '\0';
  fflush(stdin);

  /* 辺数と頂点数を求める */
  line = get_sides(fname);
  N = get_vertices(fname, line);

  printf("頂点数: %d\n", N);

  /* 平均頂点間距離を求める */
  L = characteristic_path_length(fname, N, line);

  printf("平均頂点間距離: %lf\n", L);

  return 0;
}