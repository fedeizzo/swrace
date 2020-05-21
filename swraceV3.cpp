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
  cout << *N << " ";
  cout << *M << " ";
  cout << *B << " ";
  cout << *W << endl
       << endl;
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

int blackLost(int **track, const int **circles, const int *i, const int *j,
              const int *N, const int *M, const int *sentinel) {
  // P for current position (i, j)
  // V for visited
  // B for black
  int lost = 0;
  // here a check this case
  // V B P
  if (*j + 2 < *M && track[*i][*j + 2] == *sentinel &&
      circles[*i][*j + 1] == BLACK) {
    lost++;
  }
  // here a check this case
  // P B V
  if (*j - 2 >= 0 && track[*i][*j - 2] == *sentinel &&
      circles[*i][*j - 1] == BLACK) {
    lost++;
  }

  // here a check this case
  // P
  // B
  // V
  if (*i + 2 < *N && track[*i + 2][*j] == *sentinel &&
      circles[*i + 1][*j] == BLACK) {
    lost++;
  }

  // here a check this case
  // V
  // B
  // P
  if (*i - 2 >= 0 && track[*i - 2][*j] == *sentinel &&
      circles[*i - 1][*j] == BLACK) {
    lost++;
  }
  return lost;
}

int whiteLost(int **track, const int **circles, const int *i, const int *j,
              const int *N, const int *M, const int *sentinel) {
  // P for current position (i, j)
  // V for visited
  // W for white
  int lost = 0;
  if (*i - 1 >= 0 && *j - 1 >= 0 && track[*i - 1][*j - 1] == *sentinel) {
    // here a check this case
    // V W
    //   P
    if (circles[*i - 1][*j] == WHITE)
      lost++;
    // here a check this case
    // V
    // W P
    if (circles[*i][*j - 1] == WHITE)
      lost++;
  }

  if (*i + 1 < *N && *j + 1 < *M && track[*i + 1][*j + 1] < *sentinel) {
    // here a check this case
    // P W
    //   V
    if (circles[*i + 1][*j] == WHITE)
      lost++;
    // here a check this case
    // P
    // W V
    if (circles[*i][*j + 1] == WHITE)
      lost++;
  }

  if (*i - 1 >= 0 && *j + 1 < *M && track[*i - 1][*j + 1] > *sentinel) {
    // here a check this case
    // W V
    // P
    if (circles[*i - 1][*j] == WHITE)
      lost++;
    // here a check this case
    //   V
    // P W
    if (circles[*i][*j + 1] == WHITE)
      lost++;
  }

  if (*i + 1 < *N && *j - 1 >= 0 && track[*i + 1][*j - 1] > *sentinel) {
    // here a check this case
    // W P
    // V
    if (circles[*i][*j - 1] == WHITE)
      lost++;
    // here a check this case
    //   P
    // V W
    if (circles[*i + 1][*j] == WHITE)
      lost++;
  }

  return lost;
}

char allowedMovement(char lastMovement, int movementConstraint) {
  if (movementConstraint == 1) {
    if (lastMovement == 'D' || lastMovement || 'U')
      return 'H';
    else
      return 'O';
  } else if (movementConstraint == 2) {
    if (lastMovement == 'D' || lastMovement || 'U')
      return 'O';
    else
      return 'H';
  } else {
    return 'A';
  }
}

// 0 no movement constraint
// 1 ortogonal movement
// 2 parallel movement
void bruteRec(int **track, const int **circles, const int *N, const int *M,
              int i, int j, int sentinel, int movementConstraint,
              const int *tot, int found, int lost, char *soluz,
              int soluzIndex) {
  if (*tot - found - lost < bestSolution - found) {
    return;
  } else {
    lost = lost + whiteLost(track, circles, &i, &j, N, M, &sentinel) +
           blackLost(track, circles, &i, &j, N, M, &sentinel);
    char direction = allowedMovement(soluz[soluzIndex], movementConstraint);
    if (movementConstraint == 1 && direction == 'H') {
      if (i + 1 < *N && circles[i + 1][j] != BLACK &&
          track[i + 1][j] > sentinel) {
        soluz[++soluzIndex] = 'R';
        if (circles[i][j] == WHITE) {
          bruteRec(track, circles, N, M, i + 1, j, sentinel + 1, 0, tot,
                   found + 1, lost, soluz, soluzIndex);
        } else {
          bruteRec(track, circles, N, M, i + 1, j, sentinel + 1, 0, tot, found,
                   lost, soluz, soluzIndex);
        }
        soluzIndex--;
      }
      if (i - 1 >= 0 && circles[i - 1][j] != BLACK &&
          track[i - 1][j] > sentinel) {
        soluz[++soluzIndex] = 'L';
        if (circles[i][j] == WHITE) {
          bruteRec(track, circles, N, M, i - 1, j, sentinel + 1, 0, tot,
                   found + 1, lost, soluz, soluzIndex);
        } else {
          bruteRec(track, circles, N, M, i + 1, j, sentinel + 1, 0, tot, found,
                   lost, soluz, soluzIndex);
        }
        soluzIndex--;
      }
    }
  }
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
          if (j + 1 < M && track[i][j + 1] > sentinel) {
            soluz[++soluzIndex] = 'R';
            blocked = false;
          } else if (j - 1 >= 0 && track[i][j - 1] > sentinel) {
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
  /* algoritmoTontoloRec(out, 0, 0, 0, 0, N, M, 0, totalCircle, track,
   * circles,
   */
  /*                     counter++, soluz, 0, false); */
  for (int i = 0; i < N; i++)
    for (int j = 0; j < M; j++)
      algoritmoTontoloRec(out, i, j, i, j, N, M, 0, totalCircle, track, circles,
                          counter++, soluz, 0, false);

  out.close();
}

void rectangularPath(const int N, const int M, int **track, int **circles, const int B, const int W, int **Barr, int **Warr) {
  ofstream out(OUTPUT);
  char soluz[N * M + 1];
  int soluz_index = 0;
  int circles_counter = 0;

  // The path is drawn from 0 to 1 (first), from 1 to 2 (second), from 2 to 3 (third), from 3 to 0
  // int first, third;
  // if (B == 4) {
  // if (Barr[0][0] == Barr[1][0]) {
  //   cout << "x of 0 eq 1" << endl;
  //   first = 1;
  // } else if (Barr[0][0] == Barr[2][0]) {
  //   cout << "x of 0 eq 2" << endl;
  //   first = 2;
  // } else if (Barr[0][0] == Barr[3][0]) {
  //   cout << "x of 0 eq 3" << endl;
  //   first = 3;
  // } else {
  //   cout << "No circles match same x coordinate" << endl;
  // }

  // if (Barr[0][1] == Barr[1][1]) {
  //   cout << "y of 0 eq 1" << endl;
  //   third = 1;
  // } else if (Barr[0][1] == Barr[2][1]) {
  //   cout << "y of 0 eq 2" << endl;
  //   third = 2;
  // } else if (Barr[0][1] == Barr[3][1]) {
  //   cout << "y of 0 eq 3" << endl;
  //   third = 3;
  // } else {
  //   cout << "No circles match same x coordinate" << endl;
  // }
  // }

  if (B == 4) {
    int top_left_circle, top_left_circle_value = INT16_MAX;
    for (int i = 0; i < 4; i++) {
      if (Barr[i][0] + Barr[i][1] < top_left_circle_value) {
        top_left_circle = i;
        top_left_circle_value = Barr[i][0] + Barr[i][1];
      }
    }

    int x = Barr[top_left_circle][0];
    int y = Barr[top_left_circle][1] + 1;

    // Going right
    while (circles[x][y] != 'B') {
      if (circles[x][y] == 'W') {
        circles_counter++;
      }
      cout << "R " << x << " " << y << endl;
      soluz[soluz_index++] = 'R';
      y++;
    }
    cout << "R " << x << " " << y << endl;
    soluz[soluz_index++] = 'R';

    // Going down
    x++;
    while (circles[x][y] != 'B') {
      if (circles[x][y] == 'W') {
        circles_counter++;
      }
      cout << "D " << x << " " << y << endl;
      soluz[soluz_index++] = 'D';
      x++;
    }
    cout << "D " << x << " " << y << endl;
    soluz[soluz_index++] = 'D';

    // Going left
    y--;
    while (circles[x][y] != 'B') {
      if (circles[x][y] == 'W') {
        circles_counter++;
      }
      cout << "L " << x << " " << y << endl;
      soluz[soluz_index++] = 'L';
      y--;
    }
    cout << "L " << x << " " << y << endl;
    soluz[soluz_index++] = 'L';

    // Going up
    x--;
    while (circles[x][y] != 'B') {
      if (circles[x][y] == 'W') {
        circles_counter++;
      }
      cout << "U " << x << " " << y << endl;
      soluz[soluz_index++] = 'U';
      x--;
    }
    cout << "U " << x << " " << y << endl;
    soluz[soluz_index++] = 'U';

    // Print circes amount
    cout << endl << "circles_counter:" << circles_counter << endl;
    out << circles_counter + 4 << ' '; // circles_counter counts the white

    // Print path lenght
    cout << endl << "path lenght:" << soluz_index << endl;
    out << soluz_index << ' ';

    // Print first cell position
    out << Barr[top_left_circle][0] << ' ' << Barr[top_left_circle][1] << ' ';
    // Print path
    int i = 0;
    while (i < soluz_index) {
      cout << soluz[i];
      out << soluz[i];
      i++;
    }
    out << '#' << endl;
  }

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

  int **Barr = new int *[B];
  for (int i = 0; i < B; i++)
    Barr[i] = new int[2];

  int **Warr = new int *[W];
  for (int i = 0; i < W; i++)
    Warr[i] = new int[2];

  readCircle(circles, B, W, Barr, Warr);

  for (int i = 0; i < N; i++) {
    for (int j = 0; j < M; j++) {
      if (circles[i][j] == 0)
        cout << "- ";
      else
        cout << (char)circles[i][j] << " ";
    }
    cout << endl;
  }

  cout << "White" << endl;
  for (int i = 0; i < W; i++) {
    cout << "x:" << Warr[i][0] << " y:" << Warr[i][1] << endl;
  }

  cout << "Black" << endl;
  for (int i = 0; i < B; i++) {
    cout << "x:" << Barr[i][0] << " y:" << Barr[i][1] << endl;
  }

  rectangularPath(N, M, track, circles, B, W, Barr, Warr);

  return 0;
}
