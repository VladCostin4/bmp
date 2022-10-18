#include "bmp_header.h"
#include "my_header.h"

void edit(char*, FH*, IH*, RGB***);
void createOutputFile(char*);
void save(char*, FH*, IH*, RGB**);
void insert(FH*, IH*, RGB**, FH*, IH*, RGB***, unsigned int, unsigned int);
void set(char*, Brush*);
void touch(Brush, RGB***, int, int, int, int);
void draw_line(Brush, RGB***, int, int, int, int, int, int);
void draw_rectangle(Brush, RGB***, int, int, int, int, int, int);
void fill(Brush, RGB***, int, int, int, int);

int main()
{
  char *command = malloc(CommandSize * sizeof(char));
  char *path = malloc(PathSize * sizeof(char));
  FH *fh = malloc(sizeof(FH));
  IH *ih = malloc(sizeof(IH));
  RGB **image;
  Brush br;

  br.r = br.g = br.b = (char) 0;
  br.width = 1;

  scanf("%s", &(*command));

  while (1) {
    if (strcmp(command, "edit") == 0) {
      scanf("%s", &(*path));
      edit(path, fh, ih, &image);
    } else if (strcmp(command, "save") == 0) {
      scanf("%s", &(*path));
      save(path, fh, ih, image);
    } else if (strcmp(command, "insert") == 0) {
      RGB **newImage;
      unsigned int y, x;
      FH *nfh = malloc(sizeof(FH));
      IH *nih = malloc(sizeof(IH));

      scanf("%s %u %u", &(*path), &y, &x);
      edit(path, nfh, nih, &newImage);
      insert(nfh, nih, newImage, fh, ih, &image, y, x);

      for (int i = 0; i < nih->height; i++) {
        free(newImage[i]);
      }
      free(newImage);
      free(nfh);
      free(nih);
    } else if (strcmp(command, "set") == 0) {
      scanf("%s", &(*command));
      set(command, &br);
    } else if (strcmp(command, "draw") == 0) {
      scanf("%s", &(*command));
      if (strcmp(command, "line") == 0) {
        int y1, x1, y2, x2;
        scanf("%d%d%d%d", &y1, &x1, &y2, &x2);

        draw_line(br, &image, x1, y1, x2, y2, ih->height, ih->width);
      } else if(strcmp(command, "rectangle") == 0) {
        int y, x, w, h;
        scanf("%d%d%d%d", &y, &x, &w, &h);

        draw_rectangle(br, &image, x, y, h, w, ih->height, ih->width);
      } else {
        int x1, y1, x2, y2, x3, y3;
        scanf("%d%d%d%d%d%d", &y1, &x1, &y2, &x2, &y3, &x3);

        draw_line(br, &image, x1, y1, x2, y2, ih->height, ih->width);
        draw_line(br, &image, x2, y2, x3, y3, ih->height, ih->width);
        draw_line(br, &image, x3, y3, x1, y1, ih->height, ih->width);
      }
    } else if (strcmp(command, "fill") == 0) {
      int y, x;
      scanf("%d%d", &y, &x);

      fill(br, &image, x, y, ih->height, ih->width);
    } else {
      for (int i = 0; i < ih->height; i++) {
        free(image[i]);
      }
      free(image);
      free(path);
      free(fh);
      free(ih);
      free(command);

      return 0;
    }

    scanf("%s", &(*command));
  }
}
