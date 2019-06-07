#include <stdio.h>
#include <gsl/gsl_fit.h>
#include <stdlib.h>
#include <string.h>


void get_two_dimension(char* line, double** data, char *filename);
void print_two_dimension(double** data, int row, int col);
int get_row(char *filename);
int get_col(char *filename);

int main()
{
    char filename[] = "./data/spring_data.csv"; 
    char line[1024];
    double **data;
    int row, col;
    row = get_row(filename);
    col = get_col(filename);
    data = (double **)malloc(row * sizeof(int *));
    for (int i = 0; i < row; ++i){
        data[i] = (double *)malloc(col * sizeof(double));
    }//动态申请二维数组 
    get_two_dimension(line, data, filename);
    printf("row = %d\n", row);
    printf("col = %d\n", col);
    //print_two_dimension(data, row, col);

    double c0, c1, cov00, cov01, cov11, sumsq;
    
    //int n=19;
    double dis[19];
    double mas[19];
    for(int i=0; i<row-1; i++)
    {
       mas[i] = data[i+1][1]*9.81;
       dis[i] = data[i+1][0];
       printf("%lf\t",mas[i]);
       printf("%lf\n",dis[i]);
    }
    
    gsl_fit_linear (mas,1,dis,1,row-1 , &c0, &c1, &cov00, &cov01, &cov11, &sumsq);
    
    printf ("best fit: Y = %g + %g X\n", c0, c1);
    printf ("covariance matrix:\n");
    printf ("[ %g, %g\n  %g, %g]\n",
          cov00, cov01, cov01, cov11);
    printf ("sumsq = %g\n", sumsq);

    printf ("\n");

      FILE *pipe = popen("gnuplot -persist", "w"); // Open a pipe to gnuplot
  if (pipe) // If gnuplot is found
  { 
     fprintf(pipe, "set term wx\n");         // set the terminal
     fprintf(pipe, "set xlabel '|Force| (Newtons)'\n");
     fprintf(pipe, "set ylabel 'Distance (meters)'\n");
     fprintf(pipe, "set xrange [0:10]\n");
     fprintf(pipe, "set yrange [0:0.6]\n");
     fprintf(pipe, "set title '<mas,dis> and Linear fit:y=%.4f*x+%.4f'\n",c1,c0);
   
     // 1 sending gnuplot the plot '-' command
     fprintf(pipe, "plot '-' title '<mas,dis>' with points  pt 7 lc rgb 'blue',\
                         '-' title 'Fit Line' with  linespoints  pt  6 lc rgb 'red'\n");
     
     // 2 followed by data points: <x,y>
     for (int i = 0; i < row-1; i++)
     {
        fprintf(pipe, "%lf %lf\n", mas[i], dis[i]);
     }
     // 3 followed by the letter "e" 
     fprintf(pipe, "e");
     
     // linear fit
     fprintf(pipe,"\n"); // start a new draw item
     fprintf(pipe, "%lf %lf\n", 0.0, c0+c1*0,0);
     for (int i = 0; i < row-1; i++)
     {
        fprintf(pipe, "%lf %lf\n", mas[i], c0+c1*mas[i]);
     }
     //fprintf(pipe, "%lf %lf\n", 10,c0+c1*10,0);
     fprintf(pipe, "e");
      
     fflush(pipe);
     fprintf(pipe, "exit \n"); // exit gnuplot
     pclose(pipe);             //close pipe
  }
    return 0;
    
}

void get_two_dimension(char* line, double** data, char *filename)
{
    FILE* stream = fopen(filename, "r");
    int i = 0;
    while (fgets(line, 1024, stream))//逐行读取
    {
        int j = 0;
        char *tok;
        char* tmp = strdup(line);
        for (tok = strtok(line, ","); tok && *tok; j++, tok = strtok(NULL, ",\n")){
            data[i][j] = atof(tok);//转换成浮点数 
        }//字符串拆分操作 
        i++;
        free(tmp);
    }
    fclose(stream);//文件打开后要进行关闭操作
}

void print_two_dimension(double** data, int row, int col)
{
    int i, j;
    for(i=1; i<row; i++){
        for(j=0; j<col; j++){
            printf("%f\t", data[i][j]);
        }
        printf("\n");
    }//打印的时候不打印第一行，第一行全是字符
  
}

int get_row(char *filename)
{
    char line[1024];
    int i;
    FILE* stream = fopen(filename, "r");
    while(fgets(line, 1024, stream)){
        i++;
    }
    fclose(stream);
    return i;
}

int get_col(char *filename)
{
    char line[1024];
    int i = 0;
    FILE* stream = fopen(filename, "r");
    fgets(line, 1024, stream);
    char* token = strtok(line, ",");
    while(token){
        token = strtok(NULL, ",");
        i++;
    }
    fclose(stream);
    return i;
}
