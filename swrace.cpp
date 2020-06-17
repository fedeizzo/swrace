#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <algorithm>
#include <cmath>
#include <math.h>
#include <utility>
#include <vector>
/* #include "swrace.h" */

/* #define EMPTY 0 */
/* #define BLACK 1 */
/* #define WHITE 2 */

#define INPUT "dataset/input/input11.txt"
#define OUTPUT "output.txt"
#define EMPTY 0
#define BLACK 66
#define WHITE 87

#define D 68
#define L 76
#define R 82
#define U 85

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

void readCircle(int **track, int B, int W, int **Barr, int **Warr, int N,
                int M) {
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
    if (x - 1 >= 0 && track[x - 1][y] == 0) {
      track[x - 1][y] = D;
    }
    if (x + 1 < N && track[x + 1][y] == 0) {
      track[x + 1][y] = U;
    }
    if (y - 1 >= 0 && track[x][y - 1] == 0) {
      track[x][y - 1] = R;
    }
    if (y + 1 < M && track[x][y + 1] == 0) {
      track[x][y + 1] = L;
    }
    track[x][y] = BLACK;
    Barr[i][0] = x;
    Barr[i][1] = y;
  }

  for (int i = 0; i < W; i++) {
    in >> x;
    in >> y;
    if (x - 1 >= 0 && track[x - 1][y] == 0) {
      track[x - 1][y] = D;
    }
    if (x + 1 < N && track[x + 1][y] == 0) {
      track[x + 1][y] = U;
    }
    if (y - 1 >= 0 && track[x][y - 1] == 0) {
      track[x][y - 1] = R;
    }
    if (y + 1 < M && track[x][y + 1] == 0) {
      track[x][y + 1] = L;
    }
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

bool visit(int **track, const int startX, const int startY, int i, int j,
           int *N, int *M, char *soluz, int soluzIndex, int *sentinel) {
  if (i == startX && j == startY) {
    cout << "closed" << endl;
    for (int i = 1; i < soluzIndex; ++i) {
      cout << soluz[i];
    }
    cout << endl;
    return true;
  } else {
    track[i][j] = *sentinel;
    bool returnValue = false;
    if (i + 1 < *N &&
        (track[i + 1][j] < *sentinel || (i + 1 == startX && j == startY))) {
      soluz[++soluzIndex] = 'D';
      returnValue = visit(track, startX, startY, i + 1, j, N, M, soluz,
                          soluzIndex, sentinel);
      --soluzIndex;
    }

    if (!returnValue && i - 1 >= 0 &&
        (track[i - 1][j] < *sentinel || (i - 1 == startX && j == startY))) {
      soluz[++soluzIndex] = 'U';
      returnValue = visit(track, startX, startY, i - 1, j, N, M, soluz,
                          soluzIndex, sentinel);
      --soluzIndex;
    }

    if (!returnValue && j + 1 < *M &&
        (track[i][j + 1] < *sentinel || (i == startX && j + 1 == startY))) {
      soluz[++soluzIndex] = 'R';
      returnValue = visit(track, startX, startY, i, j + 1, N, M, soluz,
                          soluzIndex, sentinel);
      --soluzIndex;
    }

    if (!returnValue && j - 1 >= 0 &&
        (track[i][j - 1] < *sentinel || (i == startX && j - 1 == startY))) {
      soluz[++soluzIndex] = 'L';
      returnValue = visit(track, startX, startY, i, j - 1, N, M, soluz,
                          soluzIndex, sentinel);
      --soluzIndex;
    }
    return returnValue;
  }
}

void closeTrack(ofstream &out, const int startX, const int startY, int i, int j,
                int N, int M, int circleFound, int totalCircle, int **track,
                int **circles, int sentinel, char *soluz, int soluzIndex,
                int restriction, int lastRestriction) {
  char *close = new char[N + M];
  int pX = startX;
  int pY = startX;
  bool impossible = true;
  int circleCount = circleFound - 1;
  if (circles[i][j] == BLACK || circles[i][j] == WHITE) {
    circleCount--;
  }

  int xPlaceholder = startX;
  int yPlaceholder = startY;
  int lenSoluz = soluzIndex;
  cout << "inzio con found cirlce a:" << circleCount << endl;
  for (int i = 1; i <= lenSoluz; i++) {
    if (soluz[i] == 'L') {
      yPlaceholder--;
    } else if (soluz[i] == 'R') {
      yPlaceholder++;
    } else if (soluz[i] == 'D') {
      xPlaceholder++;
    } else if (soluz[i] == 'U') {
      xPlaceholder--;
    }
    cout << xPlaceholder << "-" << yPlaceholder << endl;
    if (circleCount > 0) {
      pX = xPlaceholder;
      pY = yPlaceholder;
    } else {
      cout << "sono uscito qui" << endl;
      soluzIndex--;
      track[xPlaceholder][yPlaceholder] = sentinel - 1;
    }

    if ((circles[xPlaceholder][yPlaceholder] == WHITE ||
         circles[xPlaceholder][yPlaceholder] == BLACK) &&
        circleFound > 0) {
      circleCount--;
      cout << "decrermento " << circleCount << endl;
    }
  }

  /* char choise; */
  /* if (circles[startX][startY] == WHITE) { */
  /* } else if (circles[startX][startY] == BLACK) { */
  /*   if (soluz[1] == 'L' || soluz[1] == 'R') { */
  /*     if (startX - i >= 2) { */
  /*       if (track[pX - 1][pY] < sentinel && track[pX - 2][pY] < sentinel) {
   */
  /*         close[0] = 'U'; */
  /*         close[1] = 'U'; */
  /*         pX -= 2; */
  /*         impossible = false; */
  /*       } */
  /*     } else if (i - startX >= 2) { */
  /*       if (track[pX + 1][pY] < sentinel && track[pX + 2][pY] < sentinel) {
   */
  /*         close[0] = 'D'; */
  /*         close[1] = 'D'; */
  /*         pX += 2; */
  /*         impossible = false; */
  /*       } */
  /*     } */
  /*   } else if (soluz[1] == 'U' || soluz[1] == 'D') { */
  /*     if (startY - j >= 2) { */
  /*       if (track[pX][pY - 1] < sentinel && track[pX][pY - 2] < sentinel) {
   */
  /*         close[0] = 'L'; */
  /*         close[1] = 'L'; */
  /*         pY -= 2; */
  /*         impossible = false; */
  /*       } */
  /*     } else if (j - startY >= 2) { */
  /*       if (track[pX][pY - 1] < sentinel && track[pX][pY - 2] < sentinel) {
   */
  /*         close[0] = 'R'; */
  /*         close[1] = 'R'; */
  /*         pX += 2; */
  /*         impossible = false; */
  /*       } */
  /*     } */
  /*   } */
  /* } */

  for (int i = 0; i < N; i++) {
    for (int j = 0; j < M; j++) {
      if (circles[i][j] == WHITE || circles[i][j] == BLACK) {
        track[i][j] = sentinel;
      }
    }
  }
  cout << "matrice" << endl;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < M; j++) {
      if (track[i][j] == 38)
        cout << track[i][j] << " ";
      else
        cout << "-- ";
    }
    cout << endl;
  }
  cout << "simplified" << endl;
  for (int i = 1; i < soluzIndex; ++i) {
    cout << soluz[i];
  }
  cout << endl;
  cout << "parto da " << pX << "-" << pY << endl;
  cout << "limite " << N << "-" << M << endl;
  visit(track, startX, startY, pX, pY, &N, &M, soluz, soluzIndex, &sentinel);
}

void algoritmoTontoloRec(ofstream &out, const int startX, const int startY,
                         int i, int j, int N, int M, int circleFound,
                         int totalCircle, int **track, int **circles,
                         int sentinel, char *soluz, int soluzIndex,
                         int restriction, int lastRestriction) {
  /* for (int i = 0; i < N; i++) { */
  /*   for (int j = 0; j < M; j++) { */
  /*     cout << track[i][j] << " "; */
  /*   } */
  /*   cout << endl; */
  /* } */
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
          if (circles[i][j] == R) {
            if (blocked && j + 1 < M && track[i][j + 1] > sentinel &&
                circles[i][j + 1] != BLACK) {
              if (restriction != 3 ||
                  (restriction == 3 && soluz[soluzIndex + 1] != 'R')) {
                soluz[++soluzIndex] = 'R';
                blocked = false;
              }
            }
          } else if (circles[i][j] == L) {
            if (blocked && j - 1 >= 0 && track[i][j - 1] > sentinel &&
                circles[i][j - 1] != BLACK) {
              if (restriction != 3 ||
                  (restriction == 3 && soluz[soluzIndex + 1] != 'L')) {
                soluz[++soluzIndex] = 'L';
                blocked = false;
              }
            }
          }

          if (blocked && j + 1 < M && track[i][j + 1] > sentinel &&
              circles[i][j + 1] != BLACK) {
            if (restriction != 3 ||
                (restriction == 3 && soluz[soluzIndex + 1] != 'R')) {
              soluz[++soluzIndex] = 'R';
              blocked = false;
            }
          }
          if (blocked && j - 1 >= 0 && track[i][j - 1] > sentinel &&
              circles[i][j - 1] != BLACK) {
            if (restriction != 3 ||
                (restriction == 3 && soluz[soluzIndex + 1] != 'L')) {
              soluz[++soluzIndex] = 'L';
              blocked = false;
            }
          }
        } else if (chooise == 'V') {
          if (circles[i][j] == D) {
            if (i + 1 < N && track[i + 1][j] < sentinel &&
                circles[i + 1][j] != BLACK) {
              if (restriction != 3 ||
                  (restriction == 3 && soluz[soluzIndex + 1] != 'D')) {
                soluz[++soluzIndex] = 'D';
                blocked = false;
              }
            }
          } else if (circles[i][j] == U) {
            if (blocked && i - 1 >= 0 && track[i - 1][j] < sentinel &&
                circles[i - 1][j] != BLACK) {
              if (restriction != 3 ||
                  (restriction == 3 && soluz[soluzIndex + 1] != 'U')) {
                soluz[++soluzIndex] = 'U';
                blocked = false;
              }
            }
          }
          if (i + 1 < N && track[i + 1][j] < sentinel &&
              circles[i + 1][j] != BLACK) {
            if (restriction != 3 ||
                (restriction == 3 && soluz[soluzIndex + 1] != 'D')) {
              soluz[++soluzIndex] = 'D';
              blocked = false;
            }
          }
          if (blocked && i - 1 >= 0 && track[i - 1][j] < sentinel &&
              circles[i - 1][j] != BLACK) {
            if (restriction != 3 ||
                (restriction == 3 && soluz[soluzIndex + 1] != 'U')) {
              soluz[++soluzIndex] = 'U';
              blocked = false;
            }
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
      chooise = 'b';
    } else if (circles[i][j] == WHITE &&
               (restriction != 1 ||
                (restriction == 3 && lastRestriction != 1))) {
      circleFound++;
      if (soluzIndex > 1)
        chooise = whiteChooise(soluz[soluzIndex], soluz[soluzIndex - 1]);
      else
        chooise = 's';
      /* cout << "\t la scelta e' stat di: " << chooise << endl; */

      if (j + 1 < M && track[i][j + 1] < sentinel &&
          (soluzIndex == 0 || soluz[soluzIndex] == 'R')) {
        if (restriction != 3 ||
            (restriction == 3 && soluz[soluzIndex + 1] != 'R')) {
          soluz[++soluzIndex] = 'R';
          blocked = false;
        }
      } else if (i + 1 < N && track[i + 1][j] < sentinel &&
                 (soluzIndex == 0 || soluz[soluzIndex] == 'D')) {
        if (restriction != 3 ||
            (restriction == 3 && soluz[soluzIndex + 1] != 'D')) {
          soluz[++soluzIndex] = 'D';
          blocked = false;
        }
      } else if (j - 1 >= 0 && track[i][j - 1] < sentinel &&
                 (soluzIndex == 0 || soluz[soluzIndex] == 'L')) {
        if (restriction != 3 ||
            (restriction == 3 && soluz[soluzIndex + 1] != 'L')) {
          soluz[++soluzIndex] = 'L';
          blocked = false;
        }
      } else if (i - 1 >= 0 && track[i - 1][j] < sentinel &&
                 (soluzIndex == 0 || soluz[soluzIndex] == 'U')) {
        if (restriction != 3 ||
            (restriction == 3 && soluz[soluzIndex + 1] != 'U')) {
          soluz[++soluzIndex] = 'U';
          blocked = false;
        }
      }
    } else if (circles[i][j] != WHITE && circles[i][j] != BLACK) {
      chooise = 'e';
      if (restriction == 0 || (restriction == 3 && lastRestriction == 0)) {
        // suggested right
        if (blocked && j + 1 < M && track[i][j + 1] < sentinel) {
          if (circles[i][j] == R) {
            if (soluzIndex > 0) {
              if (soluz[soluzIndex] == 'U' || soluz[soluzIndex] == 'D') {
                if (circles[i][j + 1] != BLACK) {
                  if (restriction != 3 ||
                      (restriction == 3 && soluz[soluzIndex + 1] != 'R')) {
                    soluz[++soluzIndex] = 'R';
                    blocked = false;
                  }
                }
              } else {
                if (restriction != 3 ||
                    (restriction == 3 && soluz[soluzIndex + 1] != 'R')) {
                  soluz[++soluzIndex] = 'R';
                  blocked = false;
                }
              }
            } else {
              if (restriction != 3 ||
                  (restriction == 3 && soluz[soluzIndex + 1] != 'R')) {
                soluz[++soluzIndex] = 'R';
                blocked = false;
              }
            }
          }
        }

        // suggested left
        if (blocked && j - 1 >= 0 && track[i][j - 1] < sentinel) {
          if (circles[i][j] == L) {
            if (soluzIndex > 0) {
              if (soluz[soluzIndex] == 'U' || soluz[soluzIndex] == 'D') {
                if (circles[i][j - 1] != BLACK) {
                  if (restriction != 3 ||
                      (restriction == 3 && soluz[soluzIndex + 1] != 'L')) {
                    soluz[++soluzIndex] = 'L';
                    blocked = false;
                  }
                }
              } else {
                if (restriction != 3 ||
                    (restriction == 3 && soluz[soluzIndex + 1] != 'L')) {
                  soluz[++soluzIndex] = 'L';
                  blocked = false;
                }
              }
            } else {
              if (restriction != 3 ||
                  (restriction == 3 && soluz[soluzIndex + 1] != 'L')) {
                soluz[++soluzIndex] = 'L';
                blocked = false;
              }
            }
          }
        }

        // suggested up
        if (blocked && i - 1 >= 0 && track[i - 1][j] < sentinel) {
          if (circles[i][j] == U) {
            if (soluzIndex > 0) {
              if (soluz[soluzIndex] == 'L' || soluz[soluzIndex] == 'R') {
                if (circles[i - 1][j] != BLACK) {
                  if (restriction != 3 ||
                      (restriction == 3 && soluz[soluzIndex + 1] != 'U')) {
                    soluz[++soluzIndex] = 'U';
                    blocked = false;
                  }
                }
              } else {
                if (restriction != 3 ||
                    (restriction == 3 && soluz[soluzIndex + 1] != 'U')) {
                  soluz[++soluzIndex] = 'U';
                  blocked = false;
                }
              }
            } else {
              if (restriction != 3 ||
                  (restriction == 3 && soluz[soluzIndex + 1] != 'U')) {
                soluz[++soluzIndex] = 'U';
                blocked = false;
              }
            }
          }
        }

        // suggested down
        if (blocked && i + 1 < N && track[i + 1][j] < sentinel) {
          if (circles[i][j] == D) {
            if (soluzIndex > 0) {
              if (soluz[soluzIndex] == 'L' || soluz[soluzIndex] == 'R') {
                if (circles[i + 1][j] != BLACK) {
                  if (restriction != 3 ||
                      (restriction == 3 && soluz[soluzIndex + 1] != 'D')) {
                    soluz[++soluzIndex] = 'D';
                    blocked = false;
                  }
                }
              } else {
                if (restriction != 3 ||
                    (restriction == 3 && soluz[soluzIndex + 1] != 'D')) {
                  soluz[++soluzIndex] = 'D';
                  blocked = false;
                }
              }
            } else {
              if (restriction != 3 ||
                  (restriction == 3 && soluz[soluzIndex + 1] != 'D')) {
                soluz[++soluzIndex] = 'D';
                blocked = false;
              }
            }
          }
        }

        if (blocked && j + 1 < M && track[i][j + 1] < sentinel) {
          if (soluzIndex > 0) {
            if (soluz[soluzIndex] == 'D' || soluz[soluzIndex] == 'U') {
              if (circles[i][j + 1] != BLACK) {
                if (restriction != 3 ||
                    (restriction == 3 && soluz[soluzIndex + 1] != 'R')) {
                  soluz[++soluzIndex] = 'R';
                  blocked = false;
                }
              }
            } else {
              if (restriction != 3 ||
                  (restriction == 3 && soluz[soluzIndex + 1] != 'R')) {
                soluz[++soluzIndex] = 'R';
                blocked = false;
              }
            }
          } else {
            if (restriction != 3 ||
                (restriction == 3 && soluz[soluzIndex + 1] != 'R')) {
              soluz[++soluzIndex] = 'R';
              blocked = false;
            }
          }
        }

        if (blocked && i + 1 < N && track[i + 1][j] < sentinel) {
          if (soluzIndex > 0) {
            if (soluz[soluzIndex] == 'L' || soluz[soluzIndex] == 'R') {
              if (circles[i + 1][j] != BLACK) {
                if (restriction != 3 ||
                    (restriction == 3 && soluz[soluzIndex + 1] != 'D')) {
                  soluz[++soluzIndex] = 'D';
                  blocked = false;
                }
              }
            } else {
              if (restriction != 3 ||
                  (restriction == 3 && soluz[soluzIndex + 1] != 'D')) {
                soluz[++soluzIndex] = 'D';
                blocked = false;
              }
            }
          } else {
            if (restriction != 3 ||
                (restriction == 3 && soluz[soluzIndex + 1] != 'D')) {
              soluz[++soluzIndex] = 'D';
              blocked = false;
            }
          }
        }

        if (blocked && j - 1 >= 0 && track[i][j - 1] < sentinel) {
          if (soluzIndex > 0) {
            if (soluz[soluzIndex] == 'U' || soluz[soluzIndex] == 'D') {
              if (circles[i][j - 1] != BLACK) {
                if (restriction != 3 ||
                    (restriction == 3 && soluz[soluzIndex + 1] != 'L')) {
                  soluz[++soluzIndex] = 'L';
                  blocked = false;
                }
              }
            } else {
              if (restriction != 3 ||
                  (restriction == 3 && soluz[soluzIndex + 1] != 'L')) {
                soluz[++soluzIndex] = 'L';
                blocked = false;
              }
            }
          } else {
            if (restriction != 3 ||
                (restriction == 3 && soluz[soluzIndex + 1] != 'L')) {
              soluz[++soluzIndex] = 'L';
              blocked = false;
            }
          }
        }

        if (blocked && i - 1 >= 0 && track[i - 1][j] < sentinel) {
          if (soluzIndex > 0) {
            if (soluz[soluzIndex] == 'L' || soluz[soluzIndex] == 'R') {
              if (circles[i - 1][j] != BLACK) {
                if (restriction != 3 ||
                    (restriction == 3 && soluz[soluzIndex + 1] != 'U')) {
                  soluz[++soluzIndex] = 'U';
                  blocked = false;
                }
              }
            } else {
              if (restriction != 3 ||
                  (restriction == 3 && soluz[soluzIndex + 1] != 'U')) {
                soluz[++soluzIndex] = 'U';
                blocked = false;
              }
            }
          } else {
            if (restriction != 3 ||
                (restriction == 3 && soluz[soluzIndex + 1] != 'U')) {
              soluz[++soluzIndex] = 'U';
              blocked = false;
            }
          }
        }
      } else if (restriction == 1 ||
                 (restriction == 3 && lastRestriction == 1)) {
        if (soluzIndex > 0 &&
            (soluz[soluzIndex] == 'D' || soluz[soluzIndex] == 'U')) {
          if (j + 1 < M && track[i][j + 1] < sentinel &&
              circles[i][j + 1] != BLACK) {
            if (restriction != 3 ||
                (restriction == 3 && soluz[soluzIndex + 1] != 'R')) {
              soluz[++soluzIndex] = 'R';
              blocked = false;
            }
          } else if (j - 1 >= 0 && track[i][j - 1] < sentinel &&
                     circles[i][j - 1] != BLACK) {
            if (restriction != 3 ||
                (restriction == 3 && soluz[soluzIndex + 1] != 'L')) {
              soluz[++soluzIndex] = 'L';
              blocked = false;
            }
          }
        } else if (soluzIndex > 0 &&
                   (soluz[soluzIndex] == 'L' || soluz[soluzIndex] == 'R')) {
          if (i + 1 < N && track[i + 1][j] < sentinel &&
              circles[i + 1][j] != BLACK) {
            if (restriction != 3 ||
                (restriction == 3 && soluz[soluzIndex + 1] != 'D')) {
              soluz[++soluzIndex] = 'D';
              blocked = false;
            }
          } else if (i - 1 >= 0 && track[i - 1][j] < sentinel &&
                     circles[i - 1][j] != BLACK) {
            if (restriction != 3 ||
                (restriction == 3 && soluz[soluzIndex + 1] != 'U')) {
              soluz[++soluzIndex] = 'U';
              blocked = false;
            }
          }
        }
      } else {
        if (soluzIndex > 0 &&
            (soluz[soluzIndex] == 'D' || soluz[soluzIndex] == 'U')) {
          if (i + 1 < N && track[i + 1][j] < sentinel) {
            if (restriction != 3 ||
                (restriction == 3 && soluz[soluzIndex + 1] != 'D')) {
              soluz[++soluzIndex] = 'D';
              blocked = false;
            }
          } else if (i - 1 >= 0 && track[i - 1][j] < sentinel) {
            if (restriction != 3 ||
                (restriction == 3 && soluz[soluzIndex + 1] != 'U')) {
              soluz[++soluzIndex] = 'U';
              blocked = false;
            }
          }
        } else if (soluzIndex > 0 &&
                   (soluz[soluzIndex] == 'L' || soluz[soluzIndex] == 'R')) {
          if (j + 1 < M && track[i][j + 1] < sentinel) {
            if (restriction != 3 ||
                (restriction == 3 && soluz[soluzIndex + 1] != 'R')) {
              soluz[++soluzIndex] = 'R';
              blocked = false;
            }
          } else if (j - 1 >= 0 && track[i][j - 1] < sentinel) {
            if (restriction != 3 ||
                (restriction == 3 && soluz[soluzIndex + 1] != 'L')) {
              soluz[++soluzIndex] = 'L';
              blocked = false;
            }
          }
        }
      }
    }
    if (blocked == false) {
      int rest = 0;
      track[i][j] = sentinel;
      /* cout << "\tultimo val: " << soluz[soluzIndex] << endl; */
      /* cout << "\tcon chooise: " << chooise << endl; */
      /* cout << "\ttotal cirlces: " << circleFound << endl; */
      if (chooise == 's') {
        rest = 1;
      } else if (chooise == 'b') {
        rest = 2;
      }
      if (restriction == 3)
        restriction = lastRestriction;

      if (soluz[soluzIndex] == 'R') {
        algoritmoTontoloRec(out, startX, startY, i, j + 1, N, M, circleFound,
                            totalCircle, track, circles, sentinel, soluz,
                            soluzIndex, rest, restriction);
      } else if (soluz[soluzIndex] == 'D') {
        algoritmoTontoloRec(out, startX, startY, i + 1, j, N, M, circleFound,
                            totalCircle, track, circles, sentinel, soluz,
                            soluzIndex, rest, restriction);
      } else if (soluz[soluzIndex] == 'L') {
        algoritmoTontoloRec(out, startX, startY, i, j - 1, N, M, circleFound,
                            totalCircle, track, circles, sentinel, soluz,
                            soluzIndex, rest, restriction);
      } else if (soluz[soluzIndex] == 'U') {
        algoritmoTontoloRec(out, startX, startY, i - 1, j, N, M, circleFound,
                            totalCircle, track, circles, sentinel, soluz,
                            soluzIndex, rest, restriction);
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
        /* closeTrack(out, startX, startY, i, j, N, M, circleFound, totalCircle,
         */
        /* track, circles, sentinel, soluz, soluzIndex, 0, 0); */
        bestSolution = circleFound;
      }
      if (false) {
        track[i][j] = 0;
        if (circles[i][j] == BLACK || circles[i][j] == BLACK)
          circleFound--;
        --soluzIndex;
        if (soluzIndex > 0 && restriction != 3) {
          if (soluz[soluzIndex + 1] == 'R') {
            if (circles[i][j - 1] == WHITE || circles[i][j - 1] == BLACK)
              circleFound--;

            algoritmoTontoloRec(out, startX, startY, i, j - 1, N, M,
                                circleFound, totalCircle, track, circles,
                                sentinel, soluz, soluzIndex, 3,
                                lastRestriction);
          } else if (soluz[soluzIndex + 1] == 'D') {
            if (circles[i - 1][j] == WHITE || circles[i - 1][j] == BLACK)
              circleFound--;
            algoritmoTontoloRec(out, startX, startY, i - 1, j, N, M,
                                circleFound, totalCircle, track, circles,
                                sentinel, soluz, soluzIndex, 3,
                                lastRestriction);
          } else if (soluz[soluzIndex + 1] == 'L') {
            if (circles[i][j + 1] == WHITE || circles[i][j + 1] == BLACK)
              circleFound--;
            algoritmoTontoloRec(out, startX, startY, i, j + 1, N, M,
                                circleFound, totalCircle, track, circles,
                                sentinel, soluz, soluzIndex, 3,
                                lastRestriction);
          } else if (soluz[soluzIndex + 1] == 'U') {
            if (circles[i + 1][j] == WHITE || circles[i + 1][j] == BLACK)
              circleFound--;
            algoritmoTontoloRec(out, startX, startY, i + 1, j, N, M,
                                circleFound, totalCircle, track, circles,
                                sentinel, soluz, soluzIndex, 3,
                                lastRestriction);
          }
        }
      }
    }
  }
  /* out << "ciao"; */
}

void algoritmoTontolo(const int N, const int M, int **track, int **circles,
                      const int totalCircle, int **Warr, int **Barr, int W,
                      int B) {
  ofstream out(OUTPUT);
  char soluz[N * M + 1];

  int counter = 1;
  if (true) {
    for (int i = 0; i < W; i++) {
      algoritmoTontoloRec(out, Warr[i][0], Warr[i][1], Warr[i][0], Warr[i][1],
                          N, M, 0, totalCircle, track, circles, counter, soluz,
                          0, 0, 0);
      algoritmoTontoloRec(out, Warr[i][0], Warr[i][1], Warr[i][0], Warr[i][1],
                          N, M, 0, totalCircle, track, circles, counter, soluz,
                          0, 0, 0);
      algoritmoTontoloRec(out, Warr[i][0], Warr[i][1], Warr[i][0], Warr[i][1],
                          N, M, 0, totalCircle, track, circles, counter, soluz,
                          0, 0, 0);
      algoritmoTontoloRec(out, Warr[i][0], Warr[i][1], Warr[i][0], Warr[i][1],
                          N, M, 0, totalCircle, track, circles, counter++,
                          soluz, 0, 0, 0);
    }

    for (int i = 0; i < B; i++) {
      algoritmoTontoloRec(out, Barr[i][0], Barr[i][1], Barr[i][0], Barr[i][1],
                          N, M, 0, totalCircle, track, circles, counter, soluz,
                          0, 0, 0);
      algoritmoTontoloRec(out, Barr[i][0], Barr[i][1], Barr[i][0], Barr[i][1],
                          N, M, 0, totalCircle, track, circles, counter, soluz,
                          0, 0, 0);
      algoritmoTontoloRec(out, Barr[i][0], Barr[i][1], Barr[i][0], Barr[i][1],
                          N, M, 0, totalCircle, track, circles, counter, soluz,
                          0, 0, 0);
      algoritmoTontoloRec(out, Barr[i][0], Barr[i][1], Barr[i][0], Barr[i][1],
                          N, M, 0, totalCircle, track, circles, counter++,
                          soluz, 0, 0, 0);
    }
    for (int i = 0; i < N; i++)
      for (int j = 0; j < M; j++) {
        algoritmoTontoloRec(out, i, j, i, j, N, M, 0, totalCircle, track,
                            circles, counter, soluz, 0, 0, 0);
        algoritmoTontoloRec(out, i, j, i, j, N, M, 0, totalCircle, track,
                            circles, counter, soluz, 0, 0, 0);
        algoritmoTontoloRec(out, i, j, i, j, N, M, 0, totalCircle, track,
                            circles, counter, soluz, 0, 0, 0);
        algoritmoTontoloRec(out, i, j, i, j, N, M, 0, totalCircle, track,
                            circles, counter++, soluz, 0, 0, 0);
      }
  }

  out.close();
}

void rectangularPath(const int N, const int M, int **track, int **circles,
                     const int B, const int W, int **Barr, int **Warr) {
  ofstream out(OUTPUT);
  char soluz[N * M + 1];
  int soluz_index = 0;
  int circles_counter = 0;

  // The path is drawn from 0 to 1 (first), from 1 to 2 (second), from 2 to 3
  // (third), from 3 to 0 int first, third; if (B == 4) { if (Barr[0][0] ==
  // Barr[1][0]) {
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
      /* cout << "R " << x << " " << y << endl; */
      soluz[soluz_index++] = 'R';
      y++;
    }
    /* cout << "R " << x << " " << y << endl; */
    soluz[soluz_index++] = 'R';

    // Going down
    x++;
    while (circles[x][y] != 'B') {
      if (circles[x][y] == 'W') {
        circles_counter++;
      }
      /* cout << "D " << x << " " << y << endl; */
      soluz[soluz_index++] = 'D';
      x++;
    }
    /* cout << "D " << x << " " << y << endl; */
    soluz[soluz_index++] = 'D';

    // Going left
    y--;
    while (circles[x][y] != 'B') {
      if (circles[x][y] == 'W') {
        circles_counter++;
      }
      /* cout << "L " << x << " " << y << endl; */
      soluz[soluz_index++] = 'L';
      y--;
    }
    /* cout << "L " << x << " " << y << endl; */
    soluz[soluz_index++] = 'L';

    // Going up
    x--;
    while (circles[x][y] != 'B') {
      if (circles[x][y] == 'W') {
        circles_counter++;
      }
      /* cout << "U " << x << " " << y << endl; */
      soluz[soluz_index++] = 'U';
      x--;
    }
    /* cout << "U " << x << " " << y << endl; */
    soluz[soluz_index++] = 'U';

    // Print circes amount
    /* cout << endl << "circles_counter:" << circles_counter << endl; */
    out << circles_counter + 4 << ' ';  // circles_counter counts the white

    // Print path lenght
    /* cout << endl << "path lenght:" << soluz_index << endl; */
    out << soluz_index << ' ';

    // Print first cell position
    out << Barr[top_left_circle][0] << ' ' << Barr[top_left_circle][1] << ' ';
    // Print path
    int i = 0;
    while (i < soluz_index) {
      /* cout << soluz[i]; */
      out << soluz[i];
      i++;
    }
    out << '#' << endl;
  }

  out.close();
}

void superAlgorithmForInput12(int **Barr, int **circles, int N, int M) {
  ofstream out(OUTPUT);
  char soluz[N * M + 1];
  int soluz_index = 0;

  int posX = Barr[0][0];
  int posY = Barr[0][1];
  int startX = posX;
  int startY = posY;
  int foundCircles = 0;

  do {
    for (int i = 0; i < M; ++i) {
      if (circles[posX][i] == BLACK) {
        if (i < posY) {
          while (posY != i) {
            soluz[soluz_index++] = 'L';
            posY--;
            if (circles[posX][posY] == WHITE)
              foundCircles++;
          }
          foundCircles++;
          break;
        } else if (i > posY) {
          while (posY != i) {
            soluz[soluz_index++] = 'R';
            posY++;
            if (circles[posX][posY] == WHITE)
              foundCircles++;
          }
          foundCircles++;
          break;
        }
      }
    }

    for (int i = 0; i < N; ++i) {
      if (circles[i][posY] == BLACK) {
        if (i < posX) {
          while (posX != i) {
            soluz[soluz_index++] = 'U';
            posX--;
            if (circles[posX][posY] == WHITE)
              foundCircles++;
          }
          foundCircles++;
          break;
        } else if (i > posX) {
          while (posX != i) {
            soluz[soluz_index++] = 'D';
            posX++;
            if (circles[posX][posY] == WHITE)
              foundCircles++;
          }
          foundCircles++;
          break;
        }
      }
    }
  } while (posX != startX && posY != startY);

  out << foundCircles;
  out << " ";
  out << soluz_index;
  out << " ";
  out << startX;
  out << " ";
  out << startY;
  out << " ";
  for (int i = 0; i < soluz_index; i++) {
    out << soluz[i];
  }
  out << "#";
  bestSolution = foundCircles;
}

bool LeaveMySpaghettiCodeAlone(int **Barr, int **circles, int N, int M) {
  ofstream out(OUTPUT);
  char soluz[N * M + 1];
  int soluz_index = 0;

  int posX = Barr[0][0];
  int posY = Barr[0][1];
  int startX = posX;
  int startY = posY;
  bool isCircleFreakingNotCompleted = true;
  bool solutionImpossible = false;
  if (posX - 1 >= 0 && circles[posX - 1][posY] == WHITE) {
    soluz[soluz_index++] = 'U'; posX--;
  } else if (posX + 1 < N && circles[posX + 1][posY] == WHITE) {
    soluz[soluz_index++] = 'D'; posX++;
  } else if (posY - 1 >= 0 && circles[posX][posY - 1] == WHITE) {
    soluz[soluz_index++] = 'L'; posY--;
  } else if (posY + 1 < M && circles[posX][posY + 1] == WHITE) {
    soluz[soluz_index++] = 'R'; posY++;
  }

  while (!solutionImpossible && isCircleFreakingNotCompleted && (circles[posX][posY] != WHITE || circles[posX][posY] != BLACK) && posY >= 0 && posY < M && posX >= 0 && posX < N) {
    if (circles[posX][posY] == WHITE) {
      if (soluz[soluz_index - 1] == 'U') {
        soluz[soluz_index++] = 'U'; posX--;
        if (circles[posX][posY] == BLACK) {
          if (posX == startX && posY == startY) {
            isCircleFreakingNotCompleted = false;
          } else if(posY - 1 >= 0 && circles[posX][posY - 1] == WHITE) {
            soluz[soluz_index++] = 'L'; posY--;
          } else if(posY + 1 < M && circles[posX][posY + 1] == WHITE) {
            soluz[soluz_index++] = 'R'; posY++;
          }
        } else if (circles[posX][posY] == WHITE) {
          // White after a white, now doing nothing
        } else if (posX - 1 >= 0 && posY - 1 >= 0 && circles[posX - 1][posY - 1] == WHITE) {
          soluz[soluz_index++] = 'L'; posY--;
          soluz[soluz_index++] = 'U'; posX--;
        } else if (posX - 1 >= 0 && posY + 1 < M && circles[posX - 1][posY + 1] == WHITE) {
          soluz[soluz_index++] = 'R'; posY++;
          soluz[soluz_index++] = 'U'; posX--;
        }
      } else if (soluz[soluz_index - 1] == 'L') {
        soluz[soluz_index++] = 'L'; posY--;
        if (circles[posX][posY] == BLACK) {
          if (posX == startX && posY == startY) {
            isCircleFreakingNotCompleted = false;
          } else if(posX + 1 < N && circles[posX + 1][posY] == WHITE) {
            soluz[soluz_index++] = 'D'; posX++;
          } else if(posX - 1 >= 0 && circles[posX - 1][posY] == WHITE) {
            soluz[soluz_index++] = 'U'; posX--;
          }
        } else if (circles[posX][posY] == WHITE) {
          // White after a white, now doing nothing
        } else if (posX - 1 >= 0 && posY - 1 >= 0 && circles[posX - 1][posY - 1] == WHITE) {
          soluz[soluz_index++] = 'U'; posX--;
          soluz[soluz_index++] = 'L'; posY--;
        } else if (posX + 1 < N && posY - 1 >= 0 && circles[posX + 1][posY - 1] == WHITE) {
          soluz[soluz_index++] = 'D'; posX++;
          soluz[soluz_index++] = 'L'; posY--;
        }
      } else if (soluz[soluz_index - 1] == 'D') {
        soluz[soluz_index++] = 'D'; posX++;
        if (circles[posX][posY] == BLACK) {
          if (posX == startX && posY == startY) {
            isCircleFreakingNotCompleted = false;
          } else if(posY - 1 >= 0 && circles[posX][posY - 1] == WHITE) {
            soluz[soluz_index++] = 'L'; posY--;
          } else if (posY + 1 < M && circles[posX][posY + 1] == WHITE) {
            soluz[soluz_index++] = 'R'; posY++;
          }
        } else if (circles[posX][posY] == WHITE) {
          // White after a white, now doing nothing
        } else if (posX + 1 < N && posY - 1 >= 0 && circles[posX + 1][posY - 1] == WHITE) {
          soluz[soluz_index++] = 'L'; posY--;
          soluz[soluz_index++] = 'D'; posX++;
        } else if (posX + 1 < N && posY + 1 < M && circles[posX + 1][posY + 1] == WHITE) {
          soluz[soluz_index++] = 'R'; posY++;
          soluz[soluz_index++] = 'D'; posX++;
        }
      } else if (soluz[soluz_index - 1] == 'R') {
        soluz[soluz_index++] = 'R'; posY++;
        if (circles[posX][posY] == BLACK) {
          if (posX == startX && posY == startY) {
            isCircleFreakingNotCompleted = false;
          } else if(posX + 1 < N && circles[posX + 1][posY] == WHITE) {
            soluz[soluz_index++] = 'D'; posX++;
          } else if(posX - 1 >= 0 && circles[posX - 1][posY] == WHITE) {
            soluz[soluz_index++] = 'U'; posX--;
          }
        } else if (circles[posX][posY] == WHITE) {
          // White after a white, now doing nothing
        } else if (posX - 1 >= 0 && posY + 1 < M && circles[posX - 1][posY + 1] == WHITE) {
          soluz[soluz_index++] = 'U'; posX--;
          soluz[soluz_index++] = 'R'; posY++;
        } else if (posX + 1 < N && posY + 1 < M && circles[posX + 1][posY + 1] == WHITE) {
          soluz[soluz_index++] = 'D'; posX++;
          soluz[soluz_index++] = 'R'; posY++;
        }
      }
    } else if (circles[posX][posY] == BLACK) {
      if (posX == startX && posY == startY) {
        isCircleFreakingNotCompleted = false;
      } else {
        int theNextBlack = -1;
        char nextDirection = 'x';
        if (soluz[soluz_index - 1] == 'U' || soluz[soluz_index - 1] == 'D'){ // Check row
          if(posY - 1 >= 0 && circles[posX][posY - 1] == WHITE) {
            soluz[soluz_index++] = 'L'; posY--;
          } else if(posY + 1 < M && circles[posX][posY + 1] == WHITE) {
            soluz[soluz_index++] = 'R'; posY++;
          } else {
            if (posY != M-1) {
              for (int i = posY+1; i < M; i++){ // Increasing, going right
                if (circles[posX][i] == BLACK && theNextBlack == -1) {
                  theNextBlack = i;
                  soluz[soluz_index++] = 'R'; posY++;
                }
              }
            }
            if (posY != 0) {
              for (int i = posY-1; i >= 0; i--){ // Descreasing, going left
                if (circles[posX][i] == BLACK && theNextBlack == -1) {
                  theNextBlack = i;
                  soluz[soluz_index++] = 'L'; posY--;
                }
              }
            }
          }
        } else if (soluz[soluz_index - 1] == 'R' || soluz[soluz_index - 1] == 'L') { // Check column
          if(posX - 1 >= 0 && circles[posX - 1][posY] == WHITE) {
            soluz[soluz_index++] = 'U'; posX--;
          } else if(posX + 1 < N && circles[posX + 1][posY] == WHITE) {
            soluz[soluz_index++] = 'D'; posX++;
          } else {
            if (posX != N-1) {
              for (int i = posX+1; i < N; i++){ // Increasing, going down
                if (circles[i][posY] == BLACK && theNextBlack == -1) {
                  theNextBlack = i;
                  soluz[soluz_index++] = 'D'; posX++;
                }
              }
            }
            if (posX != 0) {
              for (int i = posX-1; i >= 0; i--){ // Descreasing, going up
                if (circles[i][posY] == BLACK && theNextBlack == -1) {
                  theNextBlack = i;
                  soluz[soluz_index++] = 'U'; posX--;
                }
              }
            }
          }
        } else {
          cout << "Error: What shoud it be?";
        }
      }
    } else {
      if (posY - 1 >= 0 && soluz[soluz_index - 1] == 'L') {
        soluz[soluz_index++] = 'L'; posY--;
      } else if (posY + 1 < M && soluz[soluz_index - 1] == 'R') {
        soluz[soluz_index++] = 'R'; posY++;
      } else if (posX + 1 < N && soluz[soluz_index - 1] == 'D') {
        soluz[soluz_index++] = 'D'; posX++;
      } else if (posX - 1 >= 0 && soluz[soluz_index - 1] == 'U') {
        soluz[soluz_index++] = 'U'; posX--;
      } else {
        solutionImpossible = true;
      }
    }
  }
  posX = startX;
  posY = startY;
  int foundCircles = 0;
  for (int i = 0; i < soluz_index; i++) {
    if (circles[posX][posY] == BLACK || circles[posX][posY] == WHITE)
      foundCircles++;
    if (soluz[i] == 'U')
      posX--;
    else if (soluz[i] == 'D')
      posX++;
    else if (soluz[i] == 'L')
      posY--;
    else if (soluz[i] == 'R')
      posY++;
  }

  out << foundCircles;
  out << " ";
  out << soluz_index;
  out << " ";
  out << startX;
  out << " ";
  out << startY;
  out << " ";
  for (int i = 0; i < soluz_index; i++) {
    out << soluz[i];
  }
  out << "#";
  bestSolution = foundCircles;
  return !isCircleFreakingNotCompleted;
}
int shorterIsBetterCounter = 0;
int shorterIsBetter(int **Warr, int **visited, int W, int x, int y) {
  shorterIsBetterCounter++;
  vector<pair<int, int>> distances;
  for (int i = 0; i < W; ++i) {
    pair<int, int> tmp;
  
  
  if (visited[Warr[i][0]][Warr[i][1]] != 1) {
      int deltaX = abs(Warr[i][0] - x);
      int deltaY = abs(Warr[i][1] - y);
      int realFucinkgMagnificentIDK = abs(deltaX - deltaY);
      if (realFucinkgMagnificentIDK <= 1 || deltaX == 0 || deltaY == 0) {
        double distance = sqrt(pow(x - Warr[i][0], 2) + pow(y - Warr[i][1], 2));
        tmp.first = distance;
        tmp.second = i;
        distances.push_back(tmp);
      }
    }
  }
  // if (distances.size() == 0) {
  //   for (int i = 0; i < W; ++i) {
  //     pair<int, int> tmp;

  //     if (visited[Warr[i][0]][Warr[i][1]] != 1) {
  //       double distance = sqrt(pow(x - Warr[i][0], 2) + pow(y - Warr[i][1], 2));
  //       tmp.first = distance;
  //       tmp.second = i;
  //       distances.push_back(tmp);
  //     }
  //   }
  // }
  sort(distances.begin(), distances.end());
  return distances[0].second;
}
void goUp(char *soluz, int *soluz_index, int **visited, int *posX, int *posY) {
  soluz[(*soluz_index)++] = 'U';
  (*posX)--;
  visited[*posX][*posY] = 1;
}
void goDown(char *soluz, int *soluz_index, int **visited, int *posX,
            int *posY) {
  soluz[(*soluz_index)++] = 'D';
  (*posX)++;
  visited[*posX][*posY] = 1;
}
void goLeft(char *soluz, int *soluz_index, int **visited, int *posX,
            int *posY) {
  soluz[(*soluz_index)++] = 'L';
  (*posY)--;
  visited[*posX][*posY] = 1;
}
void goRight(char *soluz, int *soluz_index, int **visited, int *posX,
             int *posY) {
  soluz[(*soluz_index)++] = 'R';
  (*posY)++;
  visited[*posX][*posY] = 1;
}


void drawLine(char *soluz, int *soluz_index, int **visited, int *posX,
              int *posY, int nextX, int nextY, int **circles, int N, int M) {
  if (*posY<nextY && * posX> nextX) { // To right up
    bool theRealFlag = false;
    bool theRealFlag2 = false;
    if (nextX - 1 >= 0 && circles[nextX - 1][nextY] == BLACK) {
      theRealFlag = true;
    } else if (nextY + 1 < M && circles[nextX][nextY + 1] == WHITE) {
      theRealFlag2 = true;
    }

    if (soluz[*soluz_index - 1] == 'R') {
      goUp(soluz, soluz_index, visited, posX, posY);
    }
    goRight(soluz, soluz_index, visited, posX, posY);
    int finish = nextX;
    if (theRealFlag)
      finish++;
    else if (theRealFlag2)
      finish++;
    while (*posX != finish) {
      goUp(soluz, soluz_index, visited, posX, posY);
    }
    if (theRealFlag2) {
      goRight(soluz, soluz_index, visited, posX, posY);
      goUp(soluz, soluz_index, visited, posX, posY);
    }
    while (*posY != nextY) {
      goRight(soluz, soluz_index, visited, posX, posY);
    }
    if (theRealFlag)
      goUp(soluz, soluz_index, visited, posX, posY);
  } else if (*posY < nextY && *posX < nextX) { // To down right
    bool theRealFlag = false;
    bool theRealFlag2 = false;
    if (nextX + 1 < N && circles[nextX + 1][nextY] == BLACK) {
      theRealFlag = true;
    } else if (nextY + 1 < M && circles[nextX][nextY + 1] == WHITE) {
      theRealFlag2 = true;
    }
    if (soluz[*soluz_index - 1] == 'R') {
      goDown(soluz, soluz_index, visited, posX, posY);
    }
    goRight(soluz, soluz_index, visited, posX, posY);
    int finish = nextX;
    if (theRealFlag)
      finish--;
    else if (theRealFlag2)
      finish--;
    while (*posX != finish) {
      goDown(soluz, soluz_index, visited, posX, posY);
    }
    if (theRealFlag2) {
      goRight(soluz, soluz_index, visited, posX, posY);
      goDown(soluz, soluz_index, visited, posX, posY);
    }
    while (*posY != nextY) {
      goRight(soluz, soluz_index, visited, posX, posY);
    }
    if (theRealFlag)
      goDown(soluz, soluz_index, visited, posX, posY);
  } else if (*posY > nextY && *posX < nextX) { // To down left
    bool theRealFlag = false;
    bool theRealFlag2 = false;
    if (nextX + 1 < N && circles[nextX + 1][nextY] == BLACK) {
      theRealFlag = true;
    } else if (nextY - 1 >= 0 && circles[nextX][nextY - 1] == WHITE) {
      theRealFlag2 = true;
    }

    if (soluz[*soluz_index - 1] == 'L') {
      goDown(soluz, soluz_index, visited, posX, posY);
    }
    goLeft(soluz, soluz_index, visited, posX, posY);
    int finish = nextX;
    if (theRealFlag)
      finish--;
    else if (theRealFlag2)
      finish--;
    while (*posX != finish) {
      goDown(soluz, soluz_index, visited, posX, posY);
    }
    if (theRealFlag2) {
      goLeft(soluz, soluz_index, visited, posX, posY);
      goDown(soluz, soluz_index, visited, posX, posY);
    }
    while (*posY != nextY) {
      goLeft(soluz, soluz_index, visited, posX, posY);
    }
    if (theRealFlag)
      goDown(soluz, soluz_index, visited, posX, posY);
  } else if (*posY > nextY && *posX > nextX) { // To Up Left
    bool theRealFlag = false;
    bool theRealFlag2 = false;
    if (nextX - 1 >= 0 && circles[nextX - 1][nextY] == BLACK) {
      theRealFlag = true;
    } else if (nextY - 1 >= 0 && circles[nextX][nextY - 1] == WHITE) {
      theRealFlag2 = true;
    }

    if (soluz[*soluz_index - 1] == 'L') {
      goUp(soluz, soluz_index, visited, posX, posY);
    }
    goLeft(soluz, soluz_index, visited, posX, posY);
    int finish = nextX;
    if (theRealFlag)
      finish++;
    else if (theRealFlag2)
      finish++;
    while (*posX != finish) {
      goUp(soluz, soluz_index, visited, posX, posY);
    }
    if (theRealFlag2) {
      goLeft(soluz, soluz_index, visited, posX, posY);
      goUp(soluz, soluz_index, visited, posX, posY);
    }
    while (*posY != nextY) {
      goLeft(soluz, soluz_index, visited, posX, posY);
    }
    if (theRealFlag)
      goUp(soluz, soluz_index, visited, posX, posY);
  }
}


bool TheSpaghettinoRevenge(int **Warr, int **Barr, int **circles, int N, int M, int B, int W, int **visited) {
  ofstream out(OUTPUT);
  char soluz[N * M + 1];
  int soluz_index = 0;
  bool solutionImpossible = false;
  bool isCircleFreakingNotCompleted = true;
  int posX, posY;
  int startX, startY;
  if (circles[0][0] == BLACK) { // Go right or down
    startX = posX = 0;
    startY = posY = 0;
    goRight(soluz, &soluz_index, visited, &posX, &posY);
    goRight(soluz, &soluz_index, visited, &posX, &posY);
  } else if (circles[N-1][0] == BLACK) { // Up or right
    startX = posX = N-1;
    startY = posY = 0;
    goRight(soluz, &soluz_index, visited, &posX, &posY);
    goRight(soluz, &soluz_index, visited, &posX, &posY);
  } else if (circles[0][M-1] == BLACK) { // Down or left
    startX = posX = 0;
    startY = posY = M-1;
    goLeft(soluz, &soluz_index, visited, &posX, &posY);
    goLeft(soluz, &soluz_index, visited, &posX, &posY);
  } else if (circles[N-1][M-1] == BLACK) { // Up or left
    startX = posX = N-1;
    startY = posY = M-1;
    goLeft(soluz, &soluz_index, visited, &posX, &posY);
    goLeft(soluz, &soluz_index, visited, &posX, &posY);
  } else  {
    solutionImpossible = true;
  }

  if (solutionImpossible == true) {
    cout << "solutionImpossible " << posX << " " << posY << endl; 
  }
  // 0 non visited
  // 1 visited
  while (!solutionImpossible && isCircleFreakingNotCompleted && posY >= 0 && posY < M && posX >= 0 && posX < N) {
    visited[posX][posY] = 1;

    if (circles[posX][posY] == WHITE) {
      cout << "White " << posX << " " << posY << endl;
      if (soluz[0] == 'L') {
        if (startX - posX == 1 && startY == posY ){
          goDown(soluz, &soluz_index, visited, &posX, &posY);
          break;
        }
      } else if (soluz[0] = 'U') {
        if (startX == posX && startY - posY == 1 ){
          goRight(soluz, &soluz_index, visited, &posX, &posY);
          break;
        }
      } else if (soluz[0] = 'U') {
        if (startX == posX && startY - posY == -1 ){
          goLeft(soluz, &soluz_index, visited, &posX, &posY);
          break;
        }
      }
      int nextIndex = shorterIsBetter(Warr, visited, W, posX, posY);
      int nextX = Warr[nextIndex][0];
      int nextY = Warr[nextIndex][1];
      
      if (soluz[soluz_index - 1] == 'U') {
        goUp(soluz, &soluz_index, visited, &posX, &posY);
        if (circles[posX][posY] == BLACK) {
          if (posX == startX && posY == startY) {
            isCircleFreakingNotCompleted = false;
          } else if(posY - 1 >= 0 && circles[posX][posY - 1] == WHITE) {
            goLeft(soluz, &soluz_index, visited, &posX, &posY);
          } else if(posY + 1 < M && circles[posX][posY + 1] == WHITE) {
            goRight(soluz, &soluz_index, visited, &posX, &posY);
          }
        } else if (circles[posX][posY] == WHITE) {
          // White after a white, now doing nothing
        } else if (posX - 1 >= 0 && posY - 1 >= 0 && circles[posX - 1][posY - 1] == WHITE) {
          goLeft(soluz, &soluz_index, visited, &posX, &posY);
          goUp(soluz, &soluz_index, visited, &posX, &posY);
        } else if (posX - 1 >= 0 && posY + 1 < M && circles[posX - 1][posY + 1] == WHITE) {
          goRight(soluz, &soluz_index, visited, &posX, &posY);
          goUp(soluz, &soluz_index, visited, &posX, &posY);
        } else if (posY - 2 >= 0 && circles[posX][posY - 2] == WHITE) {
          goLeft(soluz, &soluz_index, visited, &posX, &posY);
          goLeft(soluz, &soluz_index, visited, &posX, &posY);
        } else if (posY + 2 < M && circles[posX][posY + 2] == WHITE) {
          goRight(soluz, &soluz_index, visited, &posX, &posY);
          goRight(soluz, &soluz_index, visited, &posX, &posY);
        } else {
          drawLine(soluz, &soluz_index, visited, &posX, &posY, nextX, nextY, circles, N, M);
        }
      } else if (soluz[soluz_index - 1] == 'L') {
        goLeft(soluz, &soluz_index, visited, &posX, &posY);
        if (circles[posX][posY] == BLACK) {
          if (posX == startX && posY == startY) {
            isCircleFreakingNotCompleted = false;
          } else if(posX + 1 < N && circles[posX + 1][posY] == WHITE) {
            goDown(soluz, &soluz_index, visited, &posX, &posY);
          } else if(posX - 1 >= 0 && circles[posX - 1][posY] == WHITE) {
            goUp(soluz, &soluz_index, visited, &posX, &posY);
          }
        } else if (circles[posX][posY] == WHITE) {
          // White after a white, now doing nothing
        } else if (posX - 1 >= 0 && posY - 1 >= 0 && circles[posX - 1][posY - 1] == WHITE) {
          goUp(soluz, &soluz_index, visited, &posX, &posY);
          goLeft(soluz, &soluz_index, visited, &posX, &posY);
        } else if (posX + 1 < N && posY - 1 >= 0 && circles[posX + 1][posY - 1] == WHITE) {
          goDown(soluz, &soluz_index, visited, &posX, &posY);
          goLeft(soluz, &soluz_index, visited, &posX, &posY);
        } else if (posX - 2 >= 0 && circles[posX - 2][posY] == WHITE) {
          goUp(soluz, &soluz_index, visited, &posX, &posY);
          goUp(soluz, &soluz_index, visited, &posX, &posY);
        } else if (posX + 2 < N && circles[posX + 2][posY] == WHITE) {
          goDown(soluz, &soluz_index, visited, &posX, &posY);
          goDown(soluz, &soluz_index, visited, &posX, &posY);
        } else {
          drawLine(soluz, &soluz_index, visited, &posX, &posY, nextX, nextY, circles, N, M);
        }
      } else if (soluz[soluz_index - 1] == 'D') {
        goDown(soluz, &soluz_index, visited, &posX, &posY);
        if (circles[posX][posY] == BLACK) {
          if (posX == startX && posY == startY) {
            isCircleFreakingNotCompleted = false;
          } else if(posY - 1 >= 0 && circles[posX][posY - 1] == WHITE) {
            goLeft(soluz, &soluz_index, visited, &posX, &posY);
          } else if (posY + 1 < M && circles[posX][posY + 1] == WHITE) {
            goRight(soluz, &soluz_index, visited, &posX, &posY);
          }
        } else if (circles[posX][posY] == WHITE) {
          // White after a white, now doing nothing
        } else if (posX + 1 < N && posY - 1 >= 0 && circles[posX + 1][posY - 1] == WHITE) {
          soluz[soluz_index++] = 'L'; posY--;
          goDown(soluz, &soluz_index, visited, &posX, &posY);
        } else if (posX + 1 < N && posY + 1 < M && circles[posX + 1][posY + 1] == WHITE) {
          goRight(soluz, &soluz_index, visited, &posX, &posY);
          goDown(soluz, &soluz_index, visited, &posX, &posY);
        } else if (posY - 2 >= 0 && circles[posX][posY - 2] == WHITE) {
          goLeft(soluz, &soluz_index, visited, &posX, &posY);
          goLeft(soluz, &soluz_index, visited, &posX, &posY);
        } else if (posY + 2 < M && circles[posX][posY + 2] == WHITE) {
          goRight(soluz, &soluz_index, visited, &posX, &posY);
          goRight(soluz, &soluz_index, visited, &posX, &posY);
        } else {
          drawLine(soluz, &soluz_index, visited, &posX, &posY, nextX, nextY, circles, N, M);
        }
      } else if (soluz[soluz_index - 1] == 'R') {
        goRight(soluz, &soluz_index, visited, &posX, &posY);
        if (circles[posX][posY] == BLACK) {
          if (posX == startX && posY == startY) {
            isCircleFreakingNotCompleted = false;
          } else if(posX + 1 < N && circles[posX + 1][posY] == WHITE) {
            goDown(soluz, &soluz_index, visited, &posX, &posY);
          } else if(posX - 1 >= 0 && circles[posX - 1][posY] == WHITE) {
            goUp(soluz, &soluz_index, visited, &posX, &posY);
          }
        } else if (circles[posX][posY] == WHITE) {
          // White after a white, now doing nothing
        } else if (posX - 1 >= 0 && posY + 1 < M && circles[posX - 1][posY + 1] == WHITE) {
          goUp(soluz, &soluz_index, visited, &posX, &posY);
          goRight(soluz, &soluz_index, visited, &posX, &posY);
        } else if (posX + 1 < N && posY + 1 < M && circles[posX + 1][posY + 1] == WHITE) {
          goDown(soluz, &soluz_index, visited, &posX, &posY);
          goRight(soluz, &soluz_index, visited, &posX, &posY);
        } else if (posX - 2 >= 0 && circles[posX - 2][posY] == WHITE) {
          goUp(soluz, &soluz_index, visited, &posX, &posY);
          goUp(soluz, &soluz_index, visited, &posX, &posY);
        } else if (posX + 2 < N && circles[posX + 2][posY] == WHITE) {
          goDown(soluz, &soluz_index, visited, &posX, &posY);
          goDown(soluz, &soluz_index, visited, &posX, &posY);
        } else {
          drawLine(soluz, &soluz_index, visited, &posX, &posY, nextX, nextY, circles, N, M);
        }
      }
    } else if (circles[posX][posY] == BLACK) {
      cout << "Black " << posX << " " << posY << endl;
      if (posX == startX && posY == startY) {
        isCircleFreakingNotCompleted = false;
      } else {
        int theNextBlack = -1;
        char nextDirection = 'x';
        if (soluz[soluz_index - 1] == 'U' || soluz[soluz_index - 1] == 'D'){ // Check row
          if(posY - 1 >= 0 && circles[posX][posY - 1] == WHITE) {
            goLeft(soluz, &soluz_index, visited, &posX, &posY);
          } else if(posY + 1 < M && circles[posX][posY + 1] == WHITE) {
            goRight(soluz, &soluz_index, visited, &posX, &posY);
          } else {
            if (posY != M-1) {
              for (int i = posY+1; i < M; i++){ // Increasing, going right
                if (circles[posX][i] == BLACK && theNextBlack == -1) {
                  theNextBlack = i;
                  goRight(soluz, &soluz_index, visited, &posX, &posY);
                }
              }
            }
            if (posY != 0) {
              for (int i = posY-1; i >= 0; i--){ // Descreasing, going left
                if (circles[posX][i] == BLACK && theNextBlack == -1) {
                  theNextBlack = i;
                  goLeft(soluz, &soluz_index, visited, &posX, &posY);
                }
              }
            }
          }
        } else if (soluz[soluz_index - 1] == 'R' || soluz[soluz_index - 1] == 'L') { // Check column
          if(posX - 1 >= 0 && circles[posX - 1][posY] == WHITE) {
            goUp(soluz, &soluz_index, visited, &posX, &posY);
          } else if(posX + 1 < N && circles[posX + 1][posY] == WHITE) {
            goDown(soluz, &soluz_index, visited, &posX, &posY);
          } else {
            if (posX != N-1) {
              for (int i = posX+1; i < N; i++){ // Increasing, going down
                if (circles[i][posY] == BLACK && theNextBlack == -1) {
                  theNextBlack = i;
                  goDown(soluz, &soluz_index, visited, &posX, &posY);
                }
              }
            }
            if (posX != 0) {
              for (int i = posX-1; i >= 0; i--){ // Descreasing, going up
                if (circles[i][posY] == BLACK && theNextBlack == -1) {
                  theNextBlack = i;
                  goUp(soluz, &soluz_index, visited, &posX, &posY);
                }
              }
            }
          }
        } else {
          cout << "Error: What shoud it be?";
        }
      }
    } else {
      cout << "Nor white nor black " << posX << " " << posY << endl;
      if (posY - 1 >= 0 && soluz[soluz_index - 1] == 'L') {
        if (
          circles[posX][posY] != WHITE && circles[posX][posY] != BLACK &&
          circles[posX][posY - 1] != WHITE && circles[posX][posY - 1] != BLACK
          ) {
          if (
            circles[posX - 1][posY] != WHITE && circles[posX - 1][posY] != BLACK &&
            circles[posX - 1][posY - 1] != WHITE && circles[posX - 1][posY - 1] != BLACK
          ) {
            goUp(soluz, &soluz_index, visited, &posX, &posY);
            goLeft(soluz, &soluz_index, visited, &posX, &posY);
            goDown(soluz, &soluz_index, visited, &posX, &posY);
          } 
        }
        goLeft(soluz, &soluz_index, visited, &posX, &posY);
      } else if (posY + 1 < M && soluz[soluz_index - 1] == 'R') {
        if (
          circles[posX][posY] != WHITE && circles[posX][posY] != BLACK &&
          circles[posX][posY + 1] != WHITE && circles[posX][posY + 1] != BLACK
          ) {
          if (
            circles[posX - 1][posY] != WHITE && circles[posX - 1][posY] != BLACK &&
            circles[posX - 1][posY + 1] != WHITE && circles[posX - 1][posY + 1] != BLACK
          ) {
            goUp(soluz, &soluz_index, visited, &posX, &posY);
            goRight(soluz, &soluz_index, visited, &posX, &posY);
            goDown(soluz, &soluz_index, visited, &posX, &posY);
          } 
        }
        goRight(soluz, &soluz_index, visited, &posX, &posY);
      } else if (posX + 1 < N && soluz[soluz_index - 1] == 'D') {



        goDown(soluz, &soluz_index, visited, &posX, &posY);
      } else if (posX - 1 >= 0 && soluz[soluz_index - 1] == 'U') {
        goUp(soluz, &soluz_index, visited, &posX, &posY);
      } else {
        solutionImpossible = true;
      }
    }
  }

  // Count circles
  posX = startX;
  posY = startY;
  int foundCircles = 0;
  for (int i = 0; i < soluz_index; i++) {
    if (circles[posX][posY] == BLACK || circles[posX][posY] == WHITE)
      foundCircles++;
    if (soluz[i] == 'U')
      posX--;
    else if (soluz[i] == 'D')
      posX++;
    else if (soluz[i] == 'L')
      posY--;
    else if (soluz[i] == 'R')
      posY++;
  }

  out << foundCircles;
  out << " ";
  out << soluz_index;
  out << " ";
  out << startX;
  out << " ";
  out << startY;
  out << " ";
  for (int i = 0; i < soluz_index; i++) {
    out << soluz[i];
  }
  out << "#";
  bestSolution = foundCircles;
  return !isCircleFreakingNotCompleted;
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

  int **visited = new int *[N];
  for (int i = 0; i < N; i++)
    visited[i] = new int[M];

  int **circles = new int *[N];
  for (int i = 0; i < N; i++)
    circles[i] = new int[M];

  int **Barr = new int *[B];
  for (int i = 0; i < B; i++)
    Barr[i] = new int[2];

  int **Warr = new int *[W];
  for (int i = 0; i < W; i++)
    Warr[i] = new int[2];

  readCircle(circles, B, W, Barr, Warr, N, M);

  // for (int i = 0; i < N; i++) {
  //   for (int j = 0; j < M; j++) {
  //     if (circles[i][j] == 0)
  //       cout << "- ";
  //     else
  //       cout << (char)circles[i][j] << " ";
  //   }
  //   cout << endl;
  // }
  // cout << endl;

  /* cout << "White" << endl; */
  /* for (int i = 0; i < W; i++) { */
  /*   cout << "x:" << Warr[i][0] << " y:" << Warr[i][1] << endl; */
  /* } */

  /* cout << "Black" << endl; */
  /* for (int i = 0; i < B; i++) { */
  /*   cout << "x:" << Barr[i][0] << " y:" << Barr[i][1] << endl; */
  /* } */

  // bool modified = true;
  // while (modified) {
  //   modified = false;
  //   for (int i = 0; i < N; i++) {
  //     for (int j = 0; j < M; j++) {
  //       if (circles[i][j] == 0) {
  //         if (j + 1 < M && circles[i][j + 1] != 0) {
  //           if (circles[i][j + 1] != BLACK && circles[i][j + 1] != WHITE) {
  //             circles[i][j] = circles[i][j + 1];
  //             modified = true;
  //           }
  //         } else if (j - 1 >= 0 && circles[i][j - 1] != 0) {
  //           if (circles[i][j - 1] != BLACK && circles[i][j - 1] != WHITE) {
  //             circles[i][j] = circles[i][j - 1];
  //             modified = true;
  //           }
  //         } else if (i + 1 < N && circles[i + 1][j] != 0) {
  //           if (circles[i + 1][j] != BLACK && circles[i + 1][j] != WHITE) {
  //             circles[i][j] = circles[i + 1][j];
  //             modified = true;
  //           }
  //         } else if (i - 1 >= 0 && circles[i - 1][j] != 0) {
  //           if (circles[i - 1][j] != BLACK && circles[i - 1][j] != WHITE) {
  //             circles[i][j] = circles[i - 1][j];
  //             modified = true;
  //           }
  //         }
  //       }
  //     }
  //   }
  // }

  // Caso 12
  int circlesCount = B;
  bool supremeFlagOfTheGod = false;
  for (int i = 0; i < B; i++) {
    int x = Barr[i][0];
    int y = Barr[i][1];
    int blackCount = 0;
    for (int j = 0; j < N; j++) {
      if (circles[x][j] == BLACK)
        blackCount++;
    }
    if (blackCount != 2) {
      supremeFlagOfTheGod = true;
      break;
    }
    blackCount = 0;
    for (int j = 0; j < M; j++) {
      if (circles[j][y] == BLACK)
        blackCount++;
    }
    if (blackCount != 2) {
      supremeFlagOfTheGod = true;
      break;
    }

    if (x - 1 >= 0 && circles[x - 1][y] == WHITE) {
      circlesCount++;
    }
    if (x + 1 < N && circles[x + 1][y] == WHITE) {
      circlesCount++;
    }
    if (y - 1 >= 0 && circles[x][y - 1] == WHITE) {
      circlesCount++;
    }
    if (y + 1 < M && circles[x][y + 1] == WHITE) {
      circlesCount++;
    }
  }
  
  // LeaveMySpaghettiCodeAlone(Barr, circles, N, M);

  if (LeaveMySpaghettiCodeAlone(Barr, circles, N, M)) {
    // Buona
  } else if (TheSpaghettinoRevenge(Warr, Barr, circles, N, M, B, W, visited)) {
    // Another case
  }
  //  else if (B == 4) { // Added also if it's slightly useless
  //   rectangularPath(N, M, track, circles, B, W, Barr, Warr);
  // } else if (circlesCount == B + W && !supremeFlagOfTheGod) {
  //   superAlgorithmForInput12(Barr, circles, N, M);
  // } else {
  //   algoritmoTontolo(N, M, track, circles, B + W, Warr, Barr, W, B);
  // }

  // for (int i = 0; i < N; i++) {
  //   for (int j = 0; j < M; j++) {
  //     cout << (char)circles[i][j] << " ";
  //   }
  //   cout << endl;
  // }

  return 0;
}
