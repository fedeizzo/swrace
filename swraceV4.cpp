#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <fstream>
#include <iostream>

#include "swrace.h"

/* #define EMPTY 0 */
/* #define BLACK 1 */
/* #define WHITE 2 */

#define INPUT "input.txt"
#define OUTPUT "output.txt"
#define EMPTY 0
#define BLACK 66
#define WHITE 87

#define D 3
#define L 4
#define R 5
#define U 6

using namespace std;

int bestSolution = 0;

void readBuildInfo(int *N, int *M, int *B, int *W) {
  char path[] = INPUT;
  int fd = open(path, O_RDONLY);
  char *content = new char[300];
  int index = 0;
  int contentRead = 0;
  char dst = 'a';

  int bytesRead = -1;

  while (bytesRead != 0 && dst != '\n') {
    index = 0;
    while (bytesRead != 0 && dst != ' ') {
      bytesRead = read(fd, &dst, 1);
      if (bytesRead > 0)
        content[index++] = dst;
    }
    dst = 'a';
    switch (contentRead) {
    case 0:
      sscanf(content, "%d", N);
      contentRead++;
      break;
    case 1:
      sscanf(content, "%d", M);
      contentRead++;
      break;
    case 2:
      sscanf(content, "%d", B);
      contentRead++;
      break;
    case 3:
      sscanf(content, "%d", W);
      contentRead++;
      break;
    }
  }
  /* cout << *N << " "; */
  /* cout << *M << " "; */
  /* cout << *B << " "; */
  /* cout << *W << endl << endl; */
  close(fd);

  delete[] content;
}
void readCircle(int **track, int B, int W, int **Barr, int **Warr) {
  ifstream in(INPUT);
  int placeholder;
  in >> placeholder;
  in >> placeholder;
  in >> placeholder;
  in >> placeholder;

  int x, y;
  for (int i = 0; i < B; i++) {
    in >> x;
    in >> y;
    track[x][y] = BLACK;
    Barr[i][0] = x;
    Barr[i][1] = y;
  }

  for (int i = 0; i < W; i++) {
    in >> x;
    in >> y;
    track[x][y] = WHITE;
    Warr[i][0] = x;
    Warr[i][1] = y;
  }
}

int main() {
  int N;
  int M;
  int B;
  int W;
  readBuildInfo(&N, &M, &B, &W);

  int **track = new int *[N];
  for (int i = 0; i < N; i++)
    track[i] = new int[M];

  int **circles = new int *[N];
  for (int i = 0; i < N; i++)
    circles[i] = new int[M];
  int **Barr = new int *[B];
  for (int i = 0; i < B; i++)
    Barr[i] = new int[2];

  int **Warr = new int *[W];
  for (int i = 0; i < W; i++)
    Warr[i] = new int[2];

  readCircle(circles, B, W, Barr, Warr);
  /* cout << "White" << endl; */
  /* for (int i = 0; i < W; i++) { */
  /*   cout << "x:" << Warr[i][0] << " y:" << Warr[i][1] << endl; */
  /* } */

  /* cout << "Black" << endl; */
  /* for (int i = 0; i < B; i++) { */
  /*   cout << "x:" << Barr[i][0] << " y:" << Barr[i][1] << endl; */
  /* } */

  /* if (B == 4) { */
  /*   rectangularPath(N, M, track, circles, B, W, Barr, Warr); */
  /* } else { */
  /*   algoritmoTontolo(N, M, track, circles, B + W, Warr, Barr, W, B); */
  /* } */

  /* for (int i = 0; i < N; i++) { */
  /*   for (int j = 0; j < M; j++) { */
  /*     if (circles[i][j] == 0) */
  /*       cout << "- "; */
  /*     else */
  /*       cout << (char)circles[i][j] << " "; */
  /*   } */
  /*   cout << endl; */
  /* } */

  return 0;
}
