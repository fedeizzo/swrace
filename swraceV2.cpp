#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "swrace.h"

/* #define EMPTY 0 */
/* #define BLACK 1 */
/* #define WHITE 2 */

#define INPUT "i2.txt"
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

/* void readCircle(int **track, int B, int W) { */
/*   char path[] = "input.txt"; */
/*   int fd = open(path, O_RDONLY); */
/*   char *content = new char[300]; */
/*   int index = 0; */
/*   int lineCounter = 0; */
/*   char dst = 'a'; */

/*   int bytesRead = -1; */
/*   int n, m; */
/*   int axisCount = 0; */
/*   for (int i = 0; i < B + W; i++) { */
/*     cout << "ho ciclato " << lineCounter << endl; */
/*     index = 0; */
/*     while (bytesRead != 0 && (dst != ' ' || dst != '\n')) { */
/*       bytesRead = read(fd, &dst, 1); */
/*       if (bytesRead > 0) */
/*         content[index++] = dst; */
/*     } */
/*     content[--index] = '\0'; */
/*     /1* cout << "il ocntenutop e: " << content << endl; *1/ */
/*     if (lineCounter > 4) { */
/*       switch (axisCount) { */
/*       case 0: */
/*         axisCount = 1; */
/*         sscanf(content, "%d", &n); */
/*         cout << "ho letto: " << n << "="; */
/*         break; */
/*       case 1: */
/*         axisCount = 0; */
/*         sscanf(content, "%d", &m); */
/*         cout << "ho letto: " << m << endl; */
/*         if (B > 0) { */
/*           /1* track[n][m] = BLACK; *1/ */
/*           B--; */
/*         } else { */
/*         } */
/*         /1* track[n][m] = WHITE; *1/ */
/*         break; */
/*       } */
/*       dst = 'a'; */
/*     } */
/*     lineCounter++; */
/*   } */

/*   close(fd); */
/* } */

void readCircle(int **track, int B, int W) {
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
  }

  for (int i = 0; i < W; i++) {
    in >> x;
    in >> y;
    track[x][y] = WHITE;
  }
}

char blackChooise(char c) {
  switch (c) {
  case 'U':
    return 'H';
    break;
  case 'R':
    return 'V';
    break;
  case 'D':
    return 'H';
    break;
  case 'L':
    return 'V';
    break;
  default:
    return 'A';
  }
}

char whiteChooise(char lastChar, char c) {
  // S senza restrizioni
  // s con restrizione
  if (c == 'U' && (lastChar == 'R' || lastChar == 'L'))
    return 'S';
  else if (c == 'U' && lastChar == 'U')
    return 's';
  else if (c == 'D' && (lastChar == 'R' || lastChar == 'L'))
    return 'S';
  else if (c == 'D' && lastChar == 'D')
    return 's';
  else if (c == 'L' && (lastChar == 'U' || lastChar == 'D'))
    return 'S';
  else if (c == 'L' && lastChar == 'L')
    return 's';
  else if (c == 'R' && (lastChar == 'U' || lastChar == 'D'))
    return 'S';
  else if (c == 'R' && lastChar == 'R')
    return 's';
  else
    return 'A';
}

void algoritmoTontoloRec(ofstream &out, const int startX, const int startY,
                         int i, int j, int N, int M, int circleFound,
                         int totalCircle, int **track, int **circles,
                         int sentinel, char *soluz, int soluzIndex,
                         bool restriction) {
  // non mi muovo
  if (false) {
    // piu cerchi del massimo quindi salvo soluz
    /* cout << "sono entrato qui" << endl; */
  } else {
    /* cout << "partenza in " << i << " " << j << endl; */
    /* cout << "\trestizioni? " << restriction << endl; */
    bool blocked = true;
    char chooise;
    if (circles[i][j] == BLACK) {
      circleFound++;
      if (soluzIndex > 0) {
        chooise = blackChooise(soluz[soluzIndex]);
        if (chooise == 'H') {
          if (j + 1 < M && track[i][j + 1] < sentinel) {
            soluz[++soluzIndex] = 'R';
            blocked = false;
          } else if (j - 1 >= 0 && track[i][j - 1] < sentinel) {
            soluz[++soluzIndex] = 'L';
            blocked = false;
          }
        } else if (chooise == 'V') {
          if (i + 1 < N && track[i + 1][j] < sentinel) {
            soluz[++soluzIndex] = 'D';
            blocked = false;
          } else if (i - 1 >= 0 && track[i - 1][j] < sentinel) {
            soluz[++soluzIndex] = 'U';
            blocked = false;
          }
        }
      } else {
        chooise = 'k';
        if (j + 1 < M && track[i][j + 1] < sentinel) {
          soluz[++soluzIndex] = 'R';
          blocked = false;
        } else if (i + 1 < N && track[i + 1][j] < sentinel) {
          soluz[++soluzIndex] = 'D';
          blocked = false;
        } else if (j - 1 >= 0 && track[i][j - 1] < sentinel) {
          soluz[++soluzIndex] = 'L';
          blocked = false;
        } else if (i - 1 >= 0 && track[i - 1][j] < sentinel) {
          soluz[++soluzIndex] = 'U';
          blocked = false;
        }
      }
    } else if (circles[i][j] == WHITE && restriction == false) {
      circleFound++;
      if (soluzIndex > 1)
        chooise = whiteChooise(soluz[soluzIndex], soluz[soluzIndex - 1]);
      else
        chooise = 's';
      /* cout << "\t la scelta e' stat di: " << chooise << endl; */

      if (j + 1 < M && track[i][j + 1] < sentinel &&
          (soluzIndex == 0 || soluz[soluzIndex] == 'R')) {
        soluz[++soluzIndex] = 'R';
        blocked = false;
      } else if (i + 1 < N && track[i + 1][j] < sentinel &&
                 (soluzIndex == 0 || soluz[soluzIndex] == 'D')) {
        soluz[++soluzIndex] = 'D';
        blocked = false;
      } else if (j - 1 >= 0 && track[i][j - 1] < sentinel &&
                 (soluzIndex == 0 || soluz[soluzIndex] == 'L')) {
        soluz[++soluzIndex] = 'L';
        blocked = false;
      } else if (i - 1 >= 0 && track[i - 1][j] < sentinel &&
                 (soluzIndex == 0 || soluz[soluzIndex] == 'U')) {
        soluz[++soluzIndex] = 'U';
        blocked = false;
      }
    } else if (circles[i][j] == EMPTY) {
      chooise = 'e';
      if (restriction == false) {
        if (j + 1 < M && track[i][j + 1] < sentinel) {
          soluz[++soluzIndex] = 'R';
          blocked = false;
        } else if (i + 1 < N && track[i + 1][j] < sentinel) {
          soluz[++soluzIndex] = 'D';
          blocked = false;
        } else if (j - 1 >= 0 && track[i][j - 1] < sentinel) {
          soluz[++soluzIndex] = 'L';
          blocked = false;
        } else if (i - 1 >= 0 && track[i - 1][j] < sentinel) {
          soluz[++soluzIndex] = 'U';
          blocked = false;
        }
      } else {
        if (soluzIndex > 0 &&
            (soluz[soluzIndex] == 'D' || soluz[soluzIndex] == 'U')) {
          if (j + 1 < M && track[i][j + 1] < sentinel) {
            soluz[++soluzIndex] = 'R';
            blocked = false;
          } else if (j - 1 >= 0 && track[i][j - 1] < sentinel) {
            soluz[++soluzIndex] = 'L';
            blocked = false;
          }
        } else if (soluzIndex > 0 &&
                   (soluz[soluzIndex] == 'L' || soluz[soluzIndex] == 'R')) {
          if (i + 1 < N && track[i + 1][j] < sentinel) {
            soluz[++soluzIndex] = 'D';
            blocked = false;
          } else if (i - 1 >= 0 && track[i - 1][j] < sentinel) {
            soluz[++soluzIndex] = 'U';
            blocked = false;
          }
        }
      }
    }
    if (blocked == false) {
      bool rest = false;
      track[i][j] = sentinel;
      /* cout << "\tultimo val: " << soluz[soluzIndex] << endl; */
      /* cout << "\tcon chooise: " << chooise << endl; */
      /* cout << "\ttotal cirlces: " << circleFound << endl; */
      if (chooise == 's') {
        rest = true;
      }

      if (soluz[soluzIndex] == 'R') {
        algoritmoTontoloRec(out, startX, startY, i, j + 1, N, M, circleFound,
                            totalCircle, track, circles, sentinel, soluz,
                            soluzIndex, rest);
      } else if (soluz[soluzIndex] == 'D') {
        algoritmoTontoloRec(out, startX, startY, i + 1, j, N, M, circleFound,
                            totalCircle, track, circles, sentinel, soluz,
                            soluzIndex, rest);
      } else if (soluz[soluzIndex] == 'L') {
        algoritmoTontoloRec(out, startX, startY, i, j - 1, N, M, circleFound,
                            totalCircle, track, circles, sentinel, soluz,
                            soluzIndex, rest);
      } else if (soluz[soluzIndex] == 'U') {
        algoritmoTontoloRec(out, startX, startY, i - 1, j, N, M, circleFound,
                            totalCircle, track, circles, sentinel, soluz,
                            soluzIndex, rest);
      }

    } else {
      if (circleFound > bestSolution) {
        out << endl;
        out << circleFound << " ";
        out << soluzIndex << " ";
        out << startX << " ";
        out << startY << " ";
        for (int i = 1; i <= soluzIndex; i++) {
          out << soluz[i];
        }
        out << "#";
        bestSolution = circleFound;
      }
    }
  }
  /* out << "ciao"; */
}

void algoritmoTontolo(const int N, const int M, int **track, int **circles,
                      const int totalCircle) {
  ofstream out(OUTPUT);
  char soluz[N * M + 1];

  int counter = 1;
  /* cout << circles[4][3] << endl; */
  /* cout << circles[3][4] << endl; */
  /* algoritmoTontoloRec(out, 0, 0, 0, 0, N, M, 0, totalCircle, track, circles,
   */
  /*                     counter++, soluz, 0, false); */
  for (int i = 0; i < N; i++)
    for (int j = 0; j < M; j++)
      algoritmoTontoloRec(out, i, j, i, j, N, M, 0, totalCircle, track, circles,
                          counter++, soluz, 0, false);

  out.close();
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

  readCircle(circles, B, W);

  for (int i = 0; i < N; i++) {
    for (int j = 0; j < M; j++) {
      if (circles[i][j] == 0)
        cout << "- ";
      else
        cout << (char)circles[i][j] << " ";
    }
    cout << endl;
  }

  algoritmoTontolo(N, M, track, circles, B + W);
  /* for (int i = 0; i < N; i++) { */
  /*   for (int j = 0; j < M; j++) */
  /*     cout << track[i][j] << " "; */
  /*   cout << endl; */
  /* } */
  for (int i = 0; i < N; i++)
    delete[] track[i];
  delete[] track;
  return 0;
}
