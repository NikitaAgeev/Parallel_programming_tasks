### Код
```
int main(int argc, char **argv)
{
    double a[ISIZE][JSIZE];
    int i, j;
    FILE *ff;
    for (i=0; i< ISIZE; i++){
        for (j=0; j<JSIZE; j++){
            a[i][j] = 10*i +j;
        }
    }
    //начало измерения времени
    for (i=0; i< SIZE-4; i++){
        for (j = 0; j < JSIZE-2; j++){
            a[i][j] = sin(0.1*a[i+4][j+2]);
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
<"<","<">
### Как контрить
проблем с распаралеливанием нет, можно хоть прямоугольники, хот клеточка, хоть что только нужно больше памяти, сточки зрения памяти опитмально будет сделать прямоугольники с разлением по j, так как меньше пересечений данных. Единстевеная трудность это создание дополнительной области памяти для копирования вводных данных