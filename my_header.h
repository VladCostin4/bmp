#ifndef MY_HEADER
#define MY_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CommandSize 20
#define PathSize 100

typedef bmp_fileheader FH;
typedef bmp_infoheader IH;

typedef struct {
  char r, g, b;
} RGB;
typedef struct {
  char r, g, b;
  int width;
} Brush;

void edit(char *p, FH *file, IH *info, RGB ***mat)
{
  FILE *input = fopen(p, "rb");

  fread(&file->fileMarker1, sizeof(unsigned char), 1, input);
  fread(&file->fileMarker2, sizeof(unsigned char), 1, input);
  fread(&file->bfSize, sizeof(unsigned int), 1, input);
  fread(&file->unused1, sizeof(unsigned short), 1, input);
  fread(&file->unused2, sizeof(unsigned short), 1, input);
  fread(&file->imageDataOffset,sizeof(unsigned int), 1, input);

  fread(&info->biSize, sizeof(unsigned int), 1, input);
  fread(&info->width, sizeof(int), 1, input);
  fread(&info->height, sizeof(int), 1, input);
  fread(&info->planes, sizeof(unsigned short), 1, input);
  fread(&info->bitPix, sizeof(unsigned short), 1, input);
  fread(&info->biCompression, sizeof(unsigned int), 1, input);
  fread(&info->biSizeImage, sizeof(unsigned int), 1, input);
  fread(&info->biXPelsPerMeter, sizeof(int), 1, input);
  fread(&info->biYPelsPerMeter, sizeof(int), 1, input);
  fread(&info->biClrUsed, sizeof(unsigned int), 1, input);
  fread(&info->biClrImportant, sizeof(unsigned int), 1, input);

  int h = info->height;
  int w = info->width;

  RGB element;

  *mat = malloc(h * sizeof(RGB*));

  for (int i = 0; i < h; i++) {
   (*mat)[i] = malloc(w * sizeof(RGB));

    for (int j = 0; j < w; j++) {
      fread(&element.b, sizeof(unsigned char), 1, input);
      fread(&element.g, sizeof(unsigned char), 1, input);
      fread(&element.r, sizeof(unsigned char), 1, input);
      (*mat)[i][j] = element;
    }

    if (w % 4 != 0) {
      fseek(input, 3 * (4 - (w % 4)), SEEK_CUR);
    }
  }

  fclose(input);
}

void createOutputFile(char *p)
{
  int n = strlen(p) - 1;
  int val;

  char *d = malloc(PathSize * sizeof(char));

  while (n >= 0 && p[n] != '/') {
    n--;
  }

  for (int i = 0; i < n; i++) {
    d[i] = p[i];
  }

  d[n] = '\0';

  char *createDirectory = malloc(PathSize * sizeof(char));
  val = snprintf(createDirectory, 7 + strlen(d) + 11 + strlen(d) + 4, "%s %s %s %s", "test -d", d, "|| mkdir -p", d);

  if (val < 0) {
    return;
  }

  system(createDirectory);

  char *createFile = malloc(PathSize * sizeof(char));
  val = snprintf(createFile, 5 + strlen(p) + 2, "%s %s", "touch", p);

  if (val < 0) {
    return;
  }

  system(createFile);

  free(d);
  free(createFile);
  free(createDirectory);
}

void save(char *p, FH *file, IH *info, RGB **mat)
{
  createOutputFile(p);

  FILE *output = fopen(p, "wb");

  fwrite(&file->fileMarker1, sizeof(unsigned char), 1, output);
  fwrite(&file->fileMarker2, sizeof(unsigned char), 1, output);
  fwrite(&file->bfSize, sizeof(unsigned int), 1, output);
  fwrite(&file->unused1, sizeof(unsigned short), 1, output);
  fwrite(&file->unused2, sizeof(unsigned short), 1, output);
  fwrite(&file->imageDataOffset, sizeof(unsigned int), 1, output);

  fwrite(&info->biSize, sizeof(unsigned int), 1, output);
  fwrite(&info->width, sizeof(int), 1, output);
  fwrite(&info->height, sizeof(int), 1, output);
  fwrite(&info->planes, sizeof(unsigned short), 1, output);
  fwrite(&info->bitPix, sizeof(unsigned short), 1, output);
  fwrite(&info->biCompression, sizeof(unsigned int), 1, output);
  fwrite(&info->biSizeImage, sizeof(unsigned int), 1, output);
  fwrite(&info->biXPelsPerMeter, sizeof(int), 1, output);
  fwrite(&info->biYPelsPerMeter, sizeof(int), 1, output);
  fwrite(&info->biClrUsed, sizeof(unsigned int), 1, output);
  fwrite(&info->biClrImportant, sizeof(unsigned int), 1, output);

  int h = info->height;
  int w = info->width;

  unsigned char white = (char) 0;

  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      fwrite(&mat[i][j].b, sizeof(unsigned char), 1, output);
      fwrite(&mat[i][j].g, sizeof(unsigned char), 1, output);
      fwrite(&mat[i][j].r, sizeof(unsigned char), 1, output);
    }

    if (w % 4 != 0) {
      for (int j = w % 4; j < 4; j++) {
        fwrite(&white, sizeof(unsigned char), 1, output);
        fwrite(&white, sizeof(unsigned char), 1, output);
        fwrite(&white, sizeof(unsigned char), 1, output);
      }
    }
  }

  fclose(output);
}

void insert(FH *nFile, IH *nInfo, RGB **nMat,
            FH *file, IH *info, RGB ***mat,
            unsigned int y, unsigned int x)
{
  int nh = nInfo->height;
  int nw = nInfo->width;
  int h = info->height;
  int w = info->width;

  for (int i = x; i < h && i - x < nh; i++) {
    for (int j = y; j < w && j - y < nw; j++) {
      (*mat)[i][j] = nMat[i - x][j - y];
    }
  }
}

void set(char *c, Brush *br)
{
  if (strcmp(c,"draw_color") == 0) {
    int r, g, b;
    scanf("%d%d%d", &r, &g, &b);

    br->r = r;
    br->g = g;
    br->b = b;
  } else {
    int w;
    scanf("%d", &w);

    br->width = w;
  }
}

void touch(Brush br, RGB ***mat, int x, int y, int H, int W)
{
  int x1 = x - br.width / 2, x2 = x + br.width / 2;
  int y1 = y - br.width / 2, y2 = y + br.width / 2;

  if (x1 < 0) {
    x1 = 0;
  }

  if (x2 >= H) {
    x2 = H - 1;
  }

  if (y1 < 0) {
    y1 = 0;
  }

  if (y2 >= W) {
    y2 = W - 1;
  }

  for (int i = x1; i <= x2; i++) {
    for (int j = y1; j <= y2; j++) {
      (*mat)[i][j].r = (char) br.r;
      (*mat)[i][j].g = (char) br.g;
      (*mat)[i][j].b = (char) br.b;
    }
  }
}

void draw_line(Brush br, RGB ***mat, int x1, int y1, int x2, int y2, int H, int W) {
  if (x1 == x2) {
    if (y1 > y2) {
      int aux = y1;
      y1 = y2;
      y2 = aux;
    }

    for (int i = y1; i <= y2; i++) {
      touch(br, mat, x1, i, H, W);
    }
  } else if (y1 == y2) {
    if (x1 > x2) {
      int aux = x1;
      x1 = x2;
      x2 = aux;
    }

    for (int i = x1; i <= x2; i++) {
      touch(br, mat, i, y1, H, W);
    }
  } else {
    int intx = abs(x2 - x1);
    int inty = abs(y2 - y1);

    if (intx > inty) {
      if (x1 > x2) {
        int aux = x1;
        x1 = x2;
        x2 = aux;

        aux = y1;
        y1 = y2;
        y2 = aux;
      }

      for (int i = x1; i <= x2; i++) {
        int y = (y1 * (x2 - x1) + (y2 - y1) * (i - x1)) / (x2 - x1);
        touch(br, mat, i, y, H, W);
      }
    } else {
      if (y1 > y2) {
        int aux = y1;
        y1 = y2;
        y2 = aux;

        aux = x1;
        x1 = x2;
        x2 = aux;
      }

      for (int i = y1; i <= y2; i++) {
        int x = (x1 * (y2 - y1) + (x2 - x1) * (i - y1)) / (y2 - y1);
        touch(br, mat, x, i, H, W);
      }
    }
  }
}

void draw_rectangle(Brush br, RGB ***mat, int x, int y, int h, int w, int H, int W)
{
  for (int i = y; i <= y + w; i++) {
    touch(br, mat, x, i, H, W);
  }

  for (int i = y; i <= y + w; i++) {
    touch(br, mat, x + h, i, H, W);
  }

  for (int i = x; i <= x + h; i++) {
    touch(br, mat, i, y, H, W);
  }

  for (int i = x; i <= x + h; i++) {
    touch(br, mat, i, y + w, H, W);
  }
}

void fill(Brush br, RGB ***mat, int x, int y, int H, int W)
{
  RGB cp = (*mat)[x][y];

  (*mat)[x][y].r = br.r;
  (*mat)[x][y].g = br.g;
  (*mat)[x][y].b = br.b;

  if (x + 1 < H) {
    if ((*mat)[x + 1][y].r == cp.r && (*mat)[x + 1][y].g == cp.g && (*mat)[x + 1][y].b == cp.b) {
      fill(br, mat, x + 1, y, H, W);
    }
  }

  if (x > 0) {
    if ((*mat)[x - 1][y].r == cp.r && (*mat)[x - 1][y].g == cp.g && (*mat)[x - 1][y].b == cp.b) {
      fill(br, mat, x - 1, y, H, W);
    }
  }

  if (y + 1 < W) {
    if ((*mat)[x][y + 1].r == cp.r && (*mat)[x][y + 1].g == cp.g && (*mat)[x][y + 1].b == cp.b) {
      fill(br, mat, x, y + 1, H, W);
    }
  }

  if (y > 0) {
    if ((*mat)[x][y - 1].r == cp.r && (*mat)[x][y - 1].g == cp.g && (*mat)[x][y - 1].b == cp.b) {
      fill(br, mat, x, y - 1, H, W);
    }
  }
}

#endif
