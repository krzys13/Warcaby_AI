#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
#include<pthread.h>
#include<string.h>
#include"game_engine.h"

 

int stop_flag = 0; 






    

void engine ()
{
    while(!stop_flag) 
    {

    }
    printf("info string engine stopped\n");
}

void main ()
{
    pthread_t engine_thread;// wątek silnika gry 
    gamestate warcaby;
    warcaby.na_ruchu= BIALY;

    for(int w = 0; w < 3; w++)
    {
        for(int k = w%2 ? 0:1 ; k<KOLUMNY; k=k+2)
            warcaby.plansza.__1D[w*KOLUMNY+k] = 'c';
    }
       for(int w = 0; w < 3; w++)
    {
        for(int k = w%2 ? 1:0 ; k<KOLUMNY; k=k+2)
            warcaby.plansza.__1D[w*KOLUMNY+k] = ' ';
    }
    for(int w = WIERSZE-1; w > 4; w--)
    {
        for(int k = w%2 ? 0:1 ; k<KOLUMNY; k=k+2)
            warcaby.plansza.__1D[w*KOLUMNY+k] = 'b';
    }
     for(int w = WIERSZE-1; w >4 ; w--)
    {
        for(int k = w%2 ? 1:0 ; k<KOLUMNY; k=k+2)
            warcaby.plansza.__1D[w*KOLUMNY+k] = ' ';
    }
     for(int w = 3; w < 5; w++)
    {
        for(int k = 0; k<KOLUMNY; k++)
            warcaby.plansza.__1D[w*KOLUMNY+k] = ' ';
    }
    warcaby.plansza.__1D[33]='B';
   
   char buffer[1024];

    int buffer_length;

    if (pthread_create(&engine_thread, NULL, (void *)engine, NULL))
    {
        printf("info string error: engine couldn't start its thread\n");
        return;
    }

    for(printf("info string engine started\n");;scanf("\n"))
    {
       scanf("%[^\n]", buffer);
        buffer_length = strlen(buffer);

        if (strstr(buffer, "quit") == buffer && buffer_length == 4)
        {
            stop_flag = 1;
            break;
        }
        if (strstr(buffer, "uci") == buffer && buffer_length == 3)
            printf("id name entropy 1.0\nid author szymon oprynski, knowicki\nuciok\n");
        else if (strstr(buffer, "isready") == buffer && buffer_length == 7)
            printf("readyok\n");
        else if(strstr(buffer, "!start") == buffer && buffer_length == 6)
            {
                wypisz(&warcaby);
                link *ptr_lista_ruchow = generate_all(&warcaby);
                gamestate warcaby2 = (wykonaj_ruch(&warcaby,ptr_lista_ruchow->data));
                wypisz(&warcaby2);
                

                
            }    
    }
    pthread_join(engine_thread, NULL);

}
