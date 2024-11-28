### Код
```
int main(int argc, char **argv)
{
int main(int argc, char **argv) {
    ...double a[ISIZE][JSIZE],
    b[ISIZE][JSIZE];
    int i, j;
    FILE *ff;
    for (i=0; i<ISIZE; i++){
        for (j=0; j<JSIZE; j++){
            a[i][j] = 10*i +j;
            b[i][j] = 0;
        }
    }
    //начало измерения времени
    for (i=0; i<ISIZE; i++){
        for (j = 0; j < JSIZE; j++){
            a[i][j] = sin(0.002*a[i][j]);
        }
    }
    for (i=0; i<ISIZE-4; i++){
        for (j = 1; j < JSIZE; j++){
            b[i][j] = a[i+4][j-1]*1.5;
        }
    }
    //окончание измерения времени
    ff = fopen("result.txt","w");
    for(i=0; i < ISIZE; i++){
        for (j=0; j < JSIZE; j++){
            fprintf(ff,"%f ", b[i][j]);
        }
        fprintf(ff,"\n");
    }
    fclose(ff);
}
}
```
### Вектор направлений
<"=","="> <"=","=">
### Как контрить
а зачем 2 фазы?
просто делим на клетки и работаем, разве, что лучше это вообще в 1 цикл спихнуть, паралельность тут импакта особого не даёт.
