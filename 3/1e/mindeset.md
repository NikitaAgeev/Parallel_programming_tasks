### Код
```
int main(int argc, char **argv)
{
    double a[ISIZE][JSIZE];
    int i, j;
    FILE *ff;
    for (i=0; i<ISIZE; i++){
        for (j=0; j<JSIZE; j++){
            a[i][j] = 10*i +j;
        }
    }
    //начало измерения времени
    for (i=1; i<ISIZE; i++){
        for (j = 8; j < JSIZE; j++){
            a[i][j] = sin(5*a[i-1][j-8]);
        }
    }
    //окончание измерения времени
    ff = fopen("result.txt","w");
    for(i= 0; i < ISIZE; i++){
        for (j= 0; j < JSIZE; j++){
            fprintf(ff,"%f ",a[i][j]);
        }
        fprintf(ff,"\n");
    }
    fclose(ff);
}
```
### Вектор направлений
<">",">">
### Как контрить
разбить массив на прямоугольники по i для уменьшения данных при передачи между потоками, и дальше сделать каскад.(пока нулевой прямоугольник по i считает стрку j, перый считает j-1 и т.д.)
