#include <stdio.h>
#include <stdlib.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>


int col;
int fil;

int ** matriz;
int *  filas;
int * columnas;
int * estados;

int opciones[4];
int *spanning;
int begin[2];
int spanningSize =0;
int last_one = 0;

int Entry[2];
int Exit[2];

int RATA[2];
int pRATA[2]; //--> previous rat
int hasBegun =0;


//_________________________________MANEJO DE LOS DATOS_____________________________________________________
void initMaze()
{
    //printf("\nInitMaze");
    for(int i=0; i<fil; i++)
    {
        for(int j=0; j<col; j++)
        {
            //printf("\nFILA: %d, COL: %d", i, j);
            matriz[i][j] = 0;
        }
    }
}

void initReferences()
{
    for(int i=0; i<fil; i++)
    {
        for(int j=0; j<col; j++)
        {
            filas[ (i* (col-1) )+(i+j) ]=i;
            columnas[ (i*(col-1))+(i+j) ] = j;
        }

    }
}

void initEstados()
{
    for(int k=0; k<fil*col;k++)
    {
        estados[k] = 0;
    }
}

void tagBorders(int fila, int columna)
{
    //printf("\n\tEmpezamos");
    int index;
    if( fila-1 > -1 )
    {
        index = ( (fila-1)*(col-1)+(fila-1)+(columna) );
        if( !estados[index] )
        {
            estados[index] = 2;
        }
    }
    if( fila+1 < fil )
    {
        index = ( (fila+1)*(col-1)+(fila+1)+(columna) );
        if( !estados[index] )
        {
            estados[index] = 2;
        }
    }
    if( columna-1 > -1 )
    {
        index = ( (fila)*(col-1)+(fila)+(columna-1) );
        if( !estados[index] )
        {
            estados[index] = 2;
        }
    }
    if( columna+1 < col )
    {
        index = ( (fila)*(col-1)+(fila)+(columna+1) );
        if( !estados[index] )
        {
            estados[index] = 2;
        }
    }
    //printf("\n\tFinal");
}

int adyacentValidCount(int fila, int columna)
{
    int validCount = 0;
    int index;
    if( fila-1 > -1 )
    {
        index = ( (fila-1)*(col-1)+(fila-1)+(columna) );
        if( estados[index]==2 )
        {
            validCount++;
        }
    }
    if( fila+1 < fil )
    {
        index = ( (fila+1)*(col-1)+(fila+1)+(columna) );
        if( estados[index]==2)
        {
            validCount++;
        }
    }
    if( columna-1 > -1 )
    {
        index = ( (fila)*(col-1)+(fila)+(columna-1) );
        if( estados[index]==2)
        {
            validCount++;
        }
    }
    if( columna+1 < col )
    {
        index = ( (fila)*(col-1)+(fila)+(columna+1) );
        if( estados[index]==2 )
        {
            validCount++;
        }
    }

    return validCount;
}


int calcElections(int fila, int columna)
{
    int opcionesSize = 0;
    if( adyacentValidCount(fila,columna) == 0 ) return -1;
    int index;
    if( fila-1 > -1 )
    {
        index = ( (fila-1)*(col-1)+(fila-1)+(columna) );
        if( estados[index]==2 )
        {
            opciones[opcionesSize] = 0;
            opcionesSize++;
        }
    }
    if( fila+1 < fil )
    {
        index = ( (fila+1)*(col-1)+(fila+1)+(columna) );
        if( estados[index]==2)
        {
            opciones[opcionesSize] = 1;
            opcionesSize++;
        }
    }

    if( columna-1 > -1 )
    {
        index = ( (fila)*(col-1)+(fila)+(columna-1) );
        if( estados[index]==2)
        {
            opciones[opcionesSize] = 2;
            opcionesSize++;
        }
    }
    if( columna+1 < col )
    {
        index = ( (fila)*(col-1)+(fila)+(columna+1) );
        if( estados[index]==2 )
        {
            opciones[opcionesSize] = 3;
            opcionesSize++;
        }
    }

    return opcionesSize;

}

void beginConection(int fila, int columna)
{
    if( adyacentValidCount(fila,columna)==0 )
    {
        spanningSize--;
        return;
    }
    int option = opciones[ rand() % ( calcElections(fila, columna) )  ];

    int index;
    if(!option)
    {
        matriz[fila][columna] += 8;
        matriz[fila-1][columna] +=2;
        index =( (fila-1)*(col-1)) + (fila-1+columna);
        estados[ index ] = 1;
        spanningSize++;
        spanning[spanningSize] = index;
    }
    if(option == 1)
    {
        matriz[fila][columna] += 2;
        matriz[fila+1][columna] +=8;
        index =( (fila+1)*(col-1)) + (fila+1+columna);
        estados[ index ] = 1;
        spanningSize++;
        spanning[spanningSize] = index;
    }
    if(option == 2)
    {
        matriz[fila][columna] += 4;
        matriz[fila][columna-1] +=1;

        index =( (fila)*(col-1)) + (fila+columna-1);
        estados[ index ] = 1;
        spanningSize++;
        spanning[spanningSize] = index;
    }
    if(option == 3)
    {
        matriz[fila][columna] += 1;
        matriz[fila][columna+1] += 4;

        index =( (fila)*(col-1)) + (fila+columna+1);
        estados[ index ] = 1;
        spanningSize++;
        spanning[spanningSize] = index;
    }
}

int allOne()
{
    for(int k= last_one; k<fil*col;k++)
    {
        if(estados[k] != 1)
        {
            last_one=k;
            return 0;
        }

    }
    return 1;
}

void start_final_conection()
{
    int opcion = rand() % 1;

    if(!opcion)
    {
        matriz[0][0] += 4;
        matriz[fil-1][col-1] += 1;
        Entry[0] =0;
        Entry[1] =0;
        Exit[0] = fil-1;
        Exit[1] = col-1;
    }
    if(opcion)
    {
        matriz[0][col-1] += 1;
        matriz[fil-1][0] += 4;
        Exit[0] =0;
        Exit[1] =col-1;
        Entry[0] = fil-1;
        Entry[1] = 0;
    }
}


//____________________________________________VERIFICACIÓN-----> LOS PRINTS_________________________________________________


void printMaze()
{
    printf("\n\tMAZE");
    for(int i=0; i<fil; i++)
    {
        for(int j=0; j<col; j++) printf("\nM[%d][%d] = %d",i, j, matriz[i][j] );
    }
}

void printReferences()
{
    printf("\n\tREFERENCIAS");
    for(int i=0; i<fil; i++)
    {
        for(int j=0; j<col; j++)
        {
            int index = (i*(col-1))+(i+j);
            printf("\nR[%d] = %d , %d", index , filas[index], columnas[index] );
        }
    }
}

void printEstados()
{
    printf("\n\tESTADOS");
    for(int i=0; i<fil; i++)
    {
        for(int j=0; j<col; j++)
        {
            int index = (i*(col-1))+(i+j);
            printf("\nM[%d][%d] = %d", i,j,estados[index]);
        }
    }
}
//____________________________________________________RESOLUCIÓN DE PROBLEMAS[SUBRUTINAS]________________________________________________

int calcChoice(int fila, int colum)
{
    int wallCount = matriz[fila][colum];
    int option_Count = 0;
    //printf("\nWall COunt = %d\n", wallCount);
    if( wallCount >= 8 )
    {
        opciones[option_Count] = 8;
        option_Count++;
        wallCount -=8;
    }
    if(wallCount >= 4 )
    {
        opciones[option_Count] = 4;
        option_Count++;
        wallCount -=4;
    }
    if(wallCount >= 2)
    {
        opciones[option_Count] = 2;
        option_Count++;
        wallCount -=2;
    }
    if(wallCount >= 1 )
    {
        opciones[option_Count] = 1;
        option_Count++;
        wallCount -=1;
    }
    return option_Count;

}

void random_chose( )
{
    int fila = RATA[0];
    int colum = RATA[1];
    //printf("IN ramdom, [%d][%d]\n", fila, colum);
    int opciones_size = calcChoice(fila, colum);
    int option = opciones[ rand()%opciones_size ];
    pRATA[0]=RATA[0];
    pRATA[1]= RATA[1];
    if( option == 1 )
    {
        RATA[1]++;
    //    printf("Derecha\n");
    }
    else if( option == 2 )
    {
        RATA[0]++;
  //      printf("Abajo\n");
    }
    else if( option == 4 )
    {
        RATA[1]--;
//        printf("Izquierda\n");
    }
    else if( option == 8 )
    {
        RATA[0]--;
//        printf("Arriba\n");
    }
}

int hasFront(int direction)
{
    int wallCount = matriz[ RATA[0] ][ RATA[1] ];
    if(wallCount >=8)
    {
        if(direction==90) return 1;
        else wallCount -=8;
    }
    if(wallCount >=4)
    {
        if(direction==180) return 1;
        else wallCount -=4;
    }
    if(wallCount >=2)
    {
        if(direction==270) return 1;
        else wallCount -=2;
    }
    if(wallCount >=1)
    {
        if(direction==0) return 1;
        else wallCount -=1;
    }
    return 0;
}

int hasRigth(int direction)
{
    int wallCount = matriz[ RATA[0] ][ RATA[1] ];
    // printf("\nHas_rigth_wall count = %d", wallCount);
    if(wallCount >=8)
    {
        if(direction==180) return 0;
        else wallCount -=8;
    }
    if(wallCount >=4)
    {
        if(direction==270) return 0;
        else wallCount -=4;
    }
    if(wallCount >=2)
    {
        if(direction==0) return 0;
        else wallCount -=2;
    }
    if(wallCount >=1)
    {
        if(direction==90) return 0;
        else wallCount -=1;
    }
    return 1;
}

int adelante(int direction)
{
    pRATA[0] = RATA[0];
    pRATA[1] = RATA[1];
    if(direction==0) RATA[1]++;
    else if(direction==90) RATA[0]--;
    else if(direction==180) RATA[1]--;
    else if(direction=270) RATA[0]++;
}


//_________________________________________________SOLUCIONES PRINCIPALES_____________________________________________-
void randomMouse()
{
    RATA[0] = Entry[0];
    RATA[1] = Entry[1];
    if( !hasBegun )
    {
        matriz[ Entry[0] ][ Entry[1] ] -=4;
        matriz[ Exit[0] ][ Exit[1] ] -=1;
        hasBegun++;
    }
    int cant_nodos = 1;
    //printf("\nEntrada [%d][%d]\nSalida [%d][%d]\n",Entry[0], Entry[1], Exit[0], Exit[1]);

    while( RATA[0] != Exit[0] || RATA[1] != Exit[1] )
    {
        printf("\nCurrent [%d][%d]\n", RATA[0], RATA[1]);
        random_chose();
        cant_nodos++;
    }
    //printf("Salimos --> Current [%d][%d]\n", RATA[0], RATA[1]);
    printf("Cant Nodos = %d\n", cant_nodos);
}

void rigth_hand()
{
    if( !hasBegun )
    {
        matriz[ Entry[0] ][ Entry[1] ] -=4;
        matriz[ Exit[0] ][ Exit[1] ] -=1;
        hasBegun++;
    }
    RATA[0] = Entry[0];
    RATA[1] = Entry[1];
    int cant_nodos=1;
    int front_D=0;
    int rigth_D=270;
    //front_D = define_front();
    //rigth_D = calc_rigth(front_D);
    int wallCount = matriz[ RATA[0] ][ RATA[1] ];
    //printf("\nBegin_WallCount = %d", wallCount);
    //printf("\nDirection = %d", direction);

    while( RATA[0] != Exit[0] || RATA[1] != Exit[1] )
    {
        int wallCount = matriz[ RATA[0] ][ RATA[1] ];
        /*printf("\n%d) ", cant_nodos);
        printf("\ncurrent = [%d][%d]",RATA[0], RATA[1]);
        printf("\nDirection = %d",front_D);
        printf("\nWallCount = %d", wallCount);
        printf("\n");*/
        if( hasRigth(front_D) )
        {
          //  printf("\nHas rigth");
            if( hasFront(front_D) )
            {
                adelante(front_D);
                cant_nodos++;
            }
            else
            {
                rigth_D = front_D;
                front_D = (rigth_D+90)%360;
            }
        }
        else
        {
            front_D = rigth_D;
            rigth_D = (front_D+270)%360;
            adelante(front_D);
            cant_nodos++;
        }
    }
    printf("\nCant Nodo = %d\n", cant_nodos);
}


//____________________________________________________EL_MAIN()___________________________________________________________-

int main()
{
    srand((unsigned) time(NULL));
    fil=2048;
    col=2048;
    matriz = (int**)malloc(fil * sizeof(int*));
    for(int e=0; e<fil; e++)matriz[e]=(int* )malloc(col * sizeof(int* ));
    filas = (int*)malloc( fil*col*sizeof(int*) );
    columnas = (int*)malloc( fil*col*sizeof(int*) );
    estados = (int*)malloc( fil*col*sizeof(int*) );

    initEstados();
    initMaze();
    initReferences();
    spanning = (int*)malloc( col*fil*sizeof(int*) );

    //______________________________________MANEJO_DE_DATOS_____________________________________________________
    begin[0] = rand() % fil;
    begin[1] = rand() % col;
    int index =(begin[0]*(col-1)) + (begin[0]+begin[1]);
    estados[ index ] = 1;
    spanning[spanningSize] = index;
    int actualFil = filas[ spanning[spanningSize] ];
    int actualCol = columnas[ spanning[spanningSize] ];
    tagBorders(actualFil, actualCol);
    int i=0;
    last_one=0;
    while( !allOne() )
    {
        tagBorders(actualFil, actualCol);
        beginConection( actualFil , actualCol );
        actualFil = filas[ spanning[spanningSize] ];
        actualCol = columnas[ spanning[spanningSize] ];
        i++;

    }
    start_final_conection();

    //randomMouse();
    /*printMaze();
    int pausa;
    scanf( "%d",&pausa );*/
    rigth_hand();
    //printEstados();
    //printMaze();
}


//hacer que todas las listas empiecen en 2048
