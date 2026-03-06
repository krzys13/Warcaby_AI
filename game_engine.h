#ifndef game_engine

#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>


#define KOLUMNY 8 //macro kolumny
#define WIERSZE 8 //macro wiersze

typedef enum { BIALY,CZARNY } Kolor;







typedef struct gamestate
{
    
    union// będąca tablicą pionków i informacji o nich 
    {
        char __2D[WIERSZE][KOLUMNY]; //tablica do której odwołujemy się poprzez dwa indeksy
        char __1D[WIERSZE * KOLUMNY]; //tablica do której odwołujemy się poprzez jeden indeks
        
    }plansza;
    
    Kolor na_ruchu;
    
    int bicie; //dana strona ma bicie wartość 1 nie ma bicia wartośc 0
}gamestate;

typedef struct ruch
{
    int z_pola, na_pole;
    int czy_bicie; //wartość 1 jeśli ruch jest biciem
} ruch;

void wypisz (gamestate *g)
{
    char ozna_w = 'A'; // do numeracji wiersza
    int element = 0;
    for (int i = 0; i < WIERSZE + 1; i++)
    {
        for (int j = 0; j < KOLUMNY; j++)
        {
            if(i == 0 && j == 0)
                printf("    0   1   2   3   4   5   6   7    \n");
            if(j == 0)
                printf("  ");
            printf("+---");
        }
        printf("+\n");
        for (int m = 0; m < KOLUMNY; m++)
            {   
                if(i == 8)
                    break;
                if(m == 0 )
                    printf("%c ", ozna_w + i);
                printf("| %c ", g->plansza.__1D[element]);
                element++;
            }
        if(i != WIERSZE)
            printf("|\n");
    }
}



typedef struct link
{
    ruch data;
    struct link *next, *previous;
} link;

link* create_first(ruch first_data)
{
    link *first = malloc(sizeof(link));
    *first = (link) {.data = first_data, .previous = NULL, .next = NULL};
    return first;
}

void add(link *head, ruch new_data)
{
    
    while(head->next != NULL)
        head = head->next;
    link *new = malloc(sizeof(link));
    *new = (link) {.data = new_data, .previous = head, .next = NULL};
    head->next = new;
    
}

link *element_index(link *head, int index)
{
    for (int i = 0; i < index; i++)
    {
        head = head->next;
    }
    return head;
}

link* remove_link(link *head, int index)
{
    link *removed = element_index(head, index);
    
    if (removed->previous == NULL)
    {
        link *right = removed->next;
        right->previous = NULL;
        free(removed);
        head = right;
        return head;
    }
    else
    if (removed->next == NULL)
        {
            link *left = removed->previous;
            left->next = NULL;
            free(removed);
            return head;
        }
    else
        {
            link *left = removed->previous;
            link *right = removed->next;
            left->next = right;
            right->previous = left;
            free(removed);
            return head;
        }
}

void swap(link *head, int index1, int index2)
{
    link *remember_first = head;

    for (int i = 0; i < index1; i++)
    {
        head = head->next;
    }

    ruch temp_data1 = head->data;
    link *remember1 = head;
    head = remember_first;

    for (int i = 0; i < index2; i++)
    {
        head = head->next;
    }

    ruch temp_data2 = head->data;
    head->data = temp_data1;
    head = remember1;
    head->data = temp_data2;
}

link* insert_before(link *head, int index, ruch inserted_data)
{
    link *inserted_link = malloc(sizeof(link));
    *inserted_link = (link) {.data = inserted_data, .next = NULL, .previous = NULL};

    link *before_what = element_index(head, index);
    if (before_what->previous == NULL)
    {
        inserted_link->next = before_what;
        before_what->previous = inserted_link;
        return inserted_link;
    }
    else
    {
        before_what->previous->next = inserted_link;
        before_what->previous = inserted_link;
        inserted_link->previous = before_what->previous;
        inserted_link->next = before_what;
        return head;
    }
}

link* insert_after(link *head, int index, ruch inserted_data)
{
    link *inserted_link = malloc(sizeof(link));
    *inserted_link = (link) {.data = inserted_data, .next = NULL, .previous = NULL};

    link *after_what = element_index(head, index);
    if (after_what->next == NULL)
    {
        after_what->next = inserted_link;
        inserted_link->previous = after_what;
        return head;
    }
    else
    {
        inserted_link->next = after_what->next;
        after_what->next->previous = inserted_link;
        after_what->next = inserted_link;
        inserted_link->previous = after_what;
        return head;
    }
}

void destroy(link *head)
{
    if (head->next == NULL)
        free (head);
    else
    {
       destroy (head->next);
    }
}

int length(link *head)
{
    int list_length = 1;
    while (head->next != NULL)
    {
        head = head->next;
        list_length++;
    }
    return list_length;
}

int analyse (gamestate *g)
{
    int result = 0;
    for (int i = 0; i < WIERSZE * KOLUMNY; i++)
    {
        if (g->plansza.__1D[i] == ' ')
            continue;
        if (g->plansza.__1D[i] == (g->na_ruchu == BIALY ? 'b' : 'c')) // analize pod pionki
        {
            result += 20;
            if ((i % WIERSZE == 0) || (i % WIERSZE == 7)) //jeśli jest na krawędzi to dodatkowe                
                result += 5;
            if (g->plansza.__1D[i] == 'b')
                {
                    result += 2*(WIERSZE - (i / WIERSZE)); // biały szedł do tyłu
                }
            if (g->plansza.__1D[i] == 'c') // czarny do przodu
                {
                    result += 2*((i / WIERSZE) + 1);
                }
            if (g->plansza.__1D[i - WIERSZE - 1] == (g->na_ruchu == BIALY ? 'b' : 'c') && (i - WIERSZE - 1) >= 0 && (i - WIERSZE - 1) % WIERSZE < 7) //dodatkowe punkty za grupy pionków
                result++; 
            if (g->plansza.__1D[i - WIERSZE - 1] == (g->na_ruchu == BIALY ? 'b' : 'c') && (i - WIERSZE + 1) >= 0 && (i - WIERSZE + 1) % WIERSZE > 0)
                result++;
            if (g->plansza.__1D[i - WIERSZE - 1] == (g->na_ruchu == BIALY ? 'b' : 'c') && (i + WIERSZE - 1) < WIERSZE * KOLUMNY && (i - WIERSZE - 1) % WIERSZE < 7)
                result++;
            if (g->plansza.__1D[i - WIERSZE - 1] == (g->na_ruchu == BIALY ? 'b' : 'c') && (i + WIERSZE + 1) < WIERSZE * KOLUMNY && (i - WIERSZE - 1) % WIERSZE > 0)
                result++;      
            continue;
        }
        if (g->plansza.__1D[i] == (g->na_ruchu == BIALY ? 'c' : 'b')) //analiza pionków przeciwnika
        {
            result -= 20;
            if ((i % WIERSZE == 0) || (i % WIERSZE == 7))                
                result -= 5;
            if (g->plansza.__1D[i] == 'b')
                {
                    result -= 2*(WIERSZE - (i / WIERSZE)); 
                }
            if (g->plansza.__1D[i] == 'c') 
                {
                    result -= 2*((i / WIERSZE) + 1);
                }
            if (g->plansza.__1D[i - WIERSZE - 1] == (g->na_ruchu == BIALY ? 'c' : 'b') && (i - WIERSZE - 1) >= 0 && (i - WIERSZE - 1) % WIERSZE < 7) 
                result--; 
            if (g->plansza.__1D[i - WIERSZE - 1] == (g->na_ruchu == BIALY ? 'c' : 'b') && (i - WIERSZE + 1) >= 0 && (i - WIERSZE + 1) % WIERSZE > 0)
                result--;
            if (g->plansza.__1D[i - WIERSZE - 1] == (g->na_ruchu == BIALY ? 'c' : 'b') && (i + WIERSZE - 1) < WIERSZE * KOLUMNY && (i - WIERSZE - 1) % WIERSZE < 7)
                result--;
            if (g->plansza.__1D[i - WIERSZE - 1] == (g->na_ruchu == BIALY ? 'c' : 'b') && (i + WIERSZE + 1) < WIERSZE * KOLUMNY && (i - WIERSZE - 1) % WIERSZE > 0)
                result--;
            continue;
        }
        if (g->plansza.__1D[i] == (g->na_ruchu == BIALY ? 'B' : 'C')) // analiza pod damke
        {   
            result += 100;
            if ((i % WIERSZE == 0) || (i % WIERSZE == 7))
                result += 5;
            continue;
        }
        if (g->plansza.__1D[i] == (g->na_ruchu == BIALY ? 'C' : 'B')) // analiza damek przeciwnika
        {   
            result -= 100;
            if ((i % WIERSZE == 0) || (i % WIERSZE == 7))
                result -= 5;
        }
    }
    
    return result;
}


                

gamestate wykonaj_ruch(gamestate *g, ruch ruch) 
{
    
    gamestate nowy_stan_gry = *g;


    if(ruch.czy_bicie == 1) // ruch będący biciem
    {
        if (g->plansza.__1D[ruch.z_pola]== 'c') // bicie zwykłęgo pionka czarnego
        {
            nowy_stan_gry.plansza.__1D[ruch.na_pole] = (ruch.na_pole >= 56 && ruch.na_pole<=63) ? 'C' :'c';
            nowy_stan_gry.plansza.__1D[ruch.z_pola] = ' ';
            if(ruch.na_pole==ruch.z_pola+2*WIERSZE+2)//przód prawo 
                nowy_stan_gry.plansza.__1D[ruch.z_pola+WIERSZE+1]= ' ';
            else if(ruch.na_pole==ruch.z_pola+2*WIERSZE-2) //przód lewo
                nowy_stan_gry.plansza.__1D[ruch.z_pola+WIERSZE-1]= ' ';
            else if (ruch.na_pole==ruch.z_pola-2*WIERSZE-2) //tył lewp
                nowy_stan_gry.plansza.__1D[ruch.z_pola-WIERSZE-1]= ' ';
            else if (ruch.na_pole==ruch.z_pola-2*WIERSZE+2) // tył prawo
                nowy_stan_gry.plansza.__1D[ruch.z_pola-WIERSZE+1]= ' ';
                    
        }
        if (g->plansza.__1D[ruch.z_pola]== 'b') // bicie zwykłego pionka białego 
        {
            nowy_stan_gry.plansza.__1D[ruch.na_pole] = (ruch.na_pole >= 0 && ruch.na_pole <=7) ? 'B' :'b';
            nowy_stan_gry.plansza.__1D[ruch.z_pola] = ' ';
            if(ruch.na_pole==ruch.z_pola+2*WIERSZE+2)//przód prawo 
                nowy_stan_gry.plansza.__1D[ruch.z_pola+WIERSZE+1]= ' ';
            else if(ruch.na_pole==ruch.z_pola+2*WIERSZE-2)
                nowy_stan_gry.plansza.__1D[ruch.z_pola+WIERSZE-1]= ' ';
            else if (ruch.na_pole==ruch.z_pola-2*WIERSZE-2)
                nowy_stan_gry.plansza.__1D[ruch.z_pola-WIERSZE-1]= ' ';
            else if (ruch.na_pole==ruch.z_pola-2*WIERSZE+2)

                nowy_stan_gry.plansza.__1D[ruch.z_pola-WIERSZE+1]= ' ';
        
        
        
        
        
        }

        if(g->plansza.__1D[ruch.z_pola]== 'B') // bicie damek Białych
        {
            nowy_stan_gry.plansza.__1D[ruch.na_pole] = g->plansza.__1D[ruch.z_pola];    
            nowy_stan_gry.plansza.__1D[ruch.z_pola]= ' ';

            if(ruch.na_pole-ruch.z_pola< 0  && (ruch.na_pole-ruch.z_pola)%7 == 0) // prawo tył damka biała
            {
                for (int i = ruch.z_pola-7; i>ruch.na_pole; i=i-7)
                {
                    if(g->plansza.__1D[i]== 'c'&& g->plansza.__1D[i]=='C')
                        nowy_stan_gry.plansza.__1D[i] = ' ';
                }
            }
            
            if(ruch.na_pole-ruch.z_pola< 0  && (ruch.na_pole-ruch.z_pola) % 9 == 0 ) // lewo tył damka biała
            {
                for (int i = ruch.z_pola-9; i>ruch.na_pole; i=i-9)
                {
                    if(g->plansza.__1D[i]== 'c'|| g->plansza.__1D[i]== 'C')
                        nowy_stan_gry.plansza.__1D[i] = ' ';
                }
            }


            if(ruch.na_pole-ruch.z_pola > 0  && (ruch.na_pole-ruch.z_pola)%9 == 0) // prawo przód  damka biała
            {
                for (int i = ruch.z_pola+9; i<ruch.na_pole; i=i+9)
                {
                    if(g->plansza.__1D[i]== 'c'|| g->plansza.__1D[i]== 'C')
                        nowy_stan_gry.plansza.__1D[i] = ' ';
                }
            }

            if(ruch.na_pole-ruch.z_pola > 0  && (ruch.na_pole-ruch.z_pola)%7==0) // prawo przód damka biała
            {
                for (int i = ruch.z_pola+7; i<ruch.na_pole; i=i+7)
                {
                    if(g->plansza.__1D[i]== 'c'|| g->plansza.__1D[i]== 'C')
                        nowy_stan_gry.plansza.__1D[i] = ' ';
                }
            }
        }
        
        if(g->plansza.__1D[ruch.z_pola]== 'C') // bicie damek Czarnych
        {
            nowy_stan_gry.plansza.__1D[ruch.na_pole] = g->plansza.__1D[ruch.z_pola];    
            nowy_stan_gry.plansza.__1D[ruch.z_pola]= ' ';

            if(ruch.na_pole-ruch.z_pola< 0  && (ruch.na_pole-ruch.z_pola)%7 == 0) // prawo tył damka czarna
            {
                for (int i = ruch.z_pola-7; i>ruch.na_pole; i=i-7)
                {
                    if(g->plansza.__1D[i]== 'b'|| g->plansza.__1D[i]== 'B')
                        nowy_stan_gry.plansza.__1D[i] = ' ';
                }
            }
            
            if(ruch.na_pole-ruch.z_pola< 0  && (ruch.na_pole-ruch.z_pola)%9 == 0 ) // lewo tył damka czarna
            {
                for (int i = ruch.z_pola-9; i>ruch.na_pole; i=i-9)
                {
                    if(g->plansza.__1D[i]== 'b'|| g->plansza.__1D[i]== 'B')
                        nowy_stan_gry.plansza.__1D[i] = ' ';
                }
            }


            if(ruch.na_pole-ruch.z_pola > 0  && (ruch.na_pole-ruch.z_pola)%9 == 0) // prawo przód  damka czarna
            {
                for (int i = ruch.z_pola+9; i<ruch.na_pole; i=i+9)
                {
                    if(g->plansza.__1D[i]== 'b'|| g->plansza.__1D[i]== 'B')
                        nowy_stan_gry.plansza.__1D[i] = ' ';
                }
            }

            if(ruch.na_pole-ruch.z_pola > 0  && (ruch.na_pole-ruch.z_pola)%7==0) // prawo przód damka czarna
            {
                for (int i = ruch.z_pola+7; i<ruch.na_pole; i=i+7)
                {
                    if(g->plansza.__1D[i]== 'b' || g->plansza.__1D[i]== 'B')
                        nowy_stan_gry.plansza.__1D[i] = ' ';
                }
            }
        }


           
            

        
        

    
    
    
    
    
    
    }

    
    else if (ruch.czy_bicie==0)
    {

    
    
    
        if(ruch.na_pole >=56 && g->plansza.__1D[ruch.z_pola] == 'c')
        {
            nowy_stan_gry.plansza.__1D[ruch.na_pole]= 'C';
            nowy_stan_gry.plansza.__1D[ruch.z_pola] = ' ';
            nowy_stan_gry.na_ruchu= g->na_ruchu=BIALY ? CZARNY:BIALY;
            return nowy_stan_gry;
        }

        if(ruch.na_pole <=7 && g->plansza.__1D[ruch.z_pola] == 'b' )
        {    
            nowy_stan_gry.plansza.__1D[ruch.na_pole]= 'B';
            nowy_stan_gry.plansza.__1D[ruch.z_pola] = ' ';
            nowy_stan_gry.na_ruchu= g->na_ruchu=BIALY ? CZARNY:BIALY;
            return nowy_stan_gry;
        }    


        else 
        {
            nowy_stan_gry.plansza.__1D[ruch.na_pole] = g->plansza.__1D[ruch.z_pola];
            nowy_stan_gry.plansza.__1D[ruch.z_pola] = ' ';
            nowy_stan_gry.na_ruchu= g->na_ruchu=BIALY ? CZARNY:BIALY;
            return nowy_stan_gry;
        }



    

    }


nowy_stan_gry.na_ruchu= g->na_ruchu=BIALY ? CZARNY:BIALY;
return nowy_stan_gry;   
        
}

void generate_damka_bicia (gamestate  *g, link *ptr_lista_ruchów, int pole_pierwotne_dam, int *ptr_inx_tab)
{
                                                    //bicie damka czarne gdy przekaże damke o kolorze czarnym 
    for (int pole_aktualne_dam  = pole_pierwotne_dam ;g->plansza.__1D[pole_pierwotne_dam]== 'C'&& 
    pole_aktualne_dam/WIERSZE-2 >= 0 && pole_aktualne_dam%WIERSZE -2>= 0  ; pole_aktualne_dam-=(WIERSZE+1))// tył lewo
        {       
            if ((g->plansza.__1D[pole_aktualne_dam-WIERSZE-1] ==('b') || g->plansza.__1D[pole_aktualne_dam-WIERSZE-1] ==('B') )&& g->plansza.__1D[pole_aktualne_dam-2*WIERSZE-2] == ' ' )
            {  
                (ptr_lista_ruchów+*ptr_inx_tab)->data = (ruch){ .z_pola=pole_pierwotne_dam, .na_pole= pole_aktualne_dam-2*WIERSZE-2, .czy_bicie =1 };
                (*ptr_inx_tab)++;
                g->bicie=1;
                break;
            }
            else if (g->plansza.__1D[pole_aktualne_dam-WIERSZE-1] ==' ')
                continue;
            else if (g->plansza.__1D[pole_aktualne_dam-WIERSZE-1] != ' ' && g->plansza.__1D[pole_aktualne_dam-2*WIERSZE-2] != ' ')
                break;
        }
        
        for ( int pole_aktualne_dam  = pole_pierwotne_dam ;g->plansza.__1D[pole_pierwotne_dam]== 'C'&& pole_aktualne_dam/WIERSZE-2 >= 0 && pole_aktualne_dam%WIERSZE +2 <WIERSZE  ; pole_aktualne_dam-=(WIERSZE-1))// tył prawo
        {       
            if ((g->plansza.__1D[pole_aktualne_dam-WIERSZE-1] ==('b') || g->plansza.__1D[pole_aktualne_dam-WIERSZE+1] ==('B') )&& g->plansza.__1D[pole_aktualne_dam-2*WIERSZE+2] == ' ' )
            {  
                (ptr_lista_ruchów+*ptr_inx_tab)->data = (ruch){ .z_pola=pole_pierwotne_dam, .na_pole= pole_aktualne_dam-2*WIERSZE+2, .czy_bicie =1 };
                (*ptr_inx_tab)++;
                g->bicie=1;
                break;
            }
            else if (g->plansza.__1D[pole_aktualne_dam-WIERSZE+1] ==' ')
                continue;
            else if (g->plansza.__1D[pole_aktualne_dam-WIERSZE+1] != ' ' && g->plansza.__1D[pole_aktualne_dam-2*WIERSZE+2] != ' ')
                break;
        }

        for ( int pole_aktualne_dam  = pole_pierwotne_dam ;g->plansza.__1D[pole_pierwotne_dam]== 'C'&& pole_aktualne_dam/WIERSZE+2 <WIERSZE && pole_aktualne_dam%WIERSZE -2 >= 0  ; pole_aktualne_dam+=(WIERSZE-1))// przód lewo
        {       
            if ((g->plansza.__1D[pole_aktualne_dam+WIERSZE-1] ==('b') || g->plansza.__1D[pole_aktualne_dam+WIERSZE-1] ==('B') )&& g->plansza.__1D[pole_aktualne_dam+2*WIERSZE-2] == ' ' )
            {  
                (ptr_lista_ruchów+*ptr_inx_tab)->data = (ruch){ .z_pola=pole_pierwotne_dam, .na_pole= pole_aktualne_dam+2*WIERSZE-2, .czy_bicie =1 };
                (*ptr_inx_tab)++;
                g->bicie=1;
                break;
            }
            else if (g->plansza.__1D[pole_aktualne_dam+WIERSZE-1] ==' ')
                continue;
            else if (g->plansza.__1D[pole_aktualne_dam+WIERSZE-1] != ' ' && g->plansza.__1D[pole_aktualne_dam+2*WIERSZE-2] != ' ')
                break;
    
        }   

        for ( int pole_aktualne_dam  = pole_pierwotne_dam ;g->plansza.__1D[pole_pierwotne_dam]== 'C'&& pole_aktualne_dam/WIERSZE+2 <WIERSZE && pole_aktualne_dam%WIERSZE +2 <WIERSZE   ; pole_aktualne_dam+=(WIERSZE+1))// przód prawo
        {       
            if ((g->plansza.__1D[pole_aktualne_dam+WIERSZE+1] ==('b') || g->plansza.__1D[pole_aktualne_dam+WIERSZE+1] ==('B') )&& g->plansza.__1D[pole_aktualne_dam+2*WIERSZE+2] == ' ' )
            {  
                (ptr_lista_ruchów+*ptr_inx_tab)->data = (ruch){ .z_pola=pole_pierwotne_dam, .na_pole= pole_aktualne_dam+2*WIERSZE+2, .czy_bicie =1 };
                (*ptr_inx_tab)++;
                g->bicie=1;
                break;
            }
            else if (g->plansza.__1D[pole_aktualne_dam+WIERSZE+1] ==' ')
                continue;
            else if (g->plansza.__1D[pole_aktualne_dam+WIERSZE+1] != ' ' && g->plansza.__1D[pole_aktualne_dam+2*WIERSZE+2] != ' ')
                break;
        }
                                                                /*bice dla białych gdy przekaże damke o kolrze białym*/     
        for ( int pole_aktualne_dam  = pole_pierwotne_dam ;g->plansza.__1D[pole_pierwotne_dam]== 'B'&& pole_aktualne_dam/WIERSZE-2 >= 0 && pole_aktualne_dam%WIERSZE -2>= 0  ; pole_aktualne_dam-=(WIERSZE+1))// tył lewo
        {       
            if ((g->plansza.__1D[pole_aktualne_dam-WIERSZE-1] ==('c') || g->plansza.__1D[pole_aktualne_dam-WIERSZE-1] ==('C') )&& g->plansza.__1D[pole_aktualne_dam-2*WIERSZE-2] == ' ' )
            {  
                (ptr_lista_ruchów+*ptr_inx_tab)->data = (ruch){ .z_pola=pole_pierwotne_dam, .na_pole= pole_aktualne_dam-2*WIERSZE-2, .czy_bicie =1 };
                (*ptr_inx_tab)++;
                g->bicie=1;
                break;
            }
            else if (g->plansza.__1D[pole_aktualne_dam-WIERSZE-1] ==' ')
                continue;
            else if (g->plansza.__1D[pole_aktualne_dam-WIERSZE-1] != ' ' && g->plansza.__1D[pole_aktualne_dam-2*WIERSZE-2] != ' ')
                break;
        }
        
        for ( int pole_aktualne_dam  = pole_pierwotne_dam ;g->plansza.__1D[pole_pierwotne_dam]== 'B'&& pole_aktualne_dam/WIERSZE-2 >= 0 && pole_aktualne_dam%WIERSZE +2 <WIERSZE  ; pole_aktualne_dam-=(WIERSZE-1))// tył prawo
        {       
            if ((g->plansza.__1D[pole_aktualne_dam-WIERSZE+1] ==('c') || g->plansza.__1D[pole_aktualne_dam-WIERSZE+1] ==('C') )&& g->plansza.__1D[pole_aktualne_dam-2*WIERSZE+2] == ' ' )
            {  
                (ptr_lista_ruchów+*ptr_inx_tab)->data = (ruch){ .z_pola=pole_pierwotne_dam, .na_pole= pole_aktualne_dam-2*WIERSZE+2, .czy_bicie =1 };
                (*ptr_inx_tab)++;
                g->bicie=1;
                break;
            }
            else if (g->plansza.__1D[pole_aktualne_dam-WIERSZE+1] ==' ')
                continue;
            else if (g->plansza.__1D[pole_aktualne_dam-WIERSZE+1] != ' ' && g->plansza.__1D[pole_aktualne_dam-2*WIERSZE+2] != ' ')
                break;
        }

        for ( int pole_aktualne_dam  = pole_pierwotne_dam ;g->plansza.__1D[pole_pierwotne_dam]== 'B'&& pole_aktualne_dam/WIERSZE+2 <WIERSZE && pole_aktualne_dam%WIERSZE -2 >= 0  ; pole_aktualne_dam+=(WIERSZE-1))// przód lewo
        {       
            if ((g->plansza.__1D[pole_aktualne_dam+WIERSZE-1] ==('c') || g->plansza.__1D[pole_aktualne_dam+WIERSZE-1] ==('C') )&& g->plansza.__1D[pole_aktualne_dam+2*WIERSZE-2] == ' ' )
            {  
                (ptr_lista_ruchów+*ptr_inx_tab)->data = (ruch){ .z_pola=pole_pierwotne_dam, .na_pole= pole_aktualne_dam+2*WIERSZE-2, .czy_bicie =1 };
                (*ptr_inx_tab)++;
                g->bicie=1;
                break;
            }
            else if (g->plansza.__1D[pole_aktualne_dam+WIERSZE-1] ==' ')
                continue;
            else if (g->plansza.__1D[pole_aktualne_dam+WIERSZE-1] != ' ' && g->plansza.__1D[pole_aktualne_dam+2*WIERSZE-2] != ' ')
                break;
            
        }   

        for ( int pole_aktualne_dam  = pole_pierwotne_dam ;g->plansza.__1D[pole_pierwotne_dam]== 'B'&& pole_aktualne_dam/WIERSZE+2 <WIERSZE && pole_aktualne_dam%WIERSZE +2 <WIERSZE   ; pole_aktualne_dam+=(WIERSZE+1))// przód prawo
        {       
            if ((g->plansza.__1D[pole_aktualne_dam+WIERSZE+1] ==('c') || g->plansza.__1D[pole_aktualne_dam+WIERSZE+1] ==('C') )&& g->plansza.__1D[pole_aktualne_dam+2*WIERSZE+2] == ' ' )
            {  
                (ptr_lista_ruchów+*ptr_inx_tab)->data = (ruch){ .z_pola=pole_pierwotne_dam, .na_pole= pole_aktualne_dam+2*WIERSZE+2, .czy_bicie =1 };
                (*ptr_inx_tab)++;
                g->bicie=1;
                break;
            }
            else if (g->plansza.__1D[pole_aktualne_dam+WIERSZE+1] ==' ')
                continue;
            else if (g->plansza.__1D[pole_aktualne_dam+WIERSZE+1] != ' ' && g->plansza.__1D[pole_aktualne_dam+2*WIERSZE+2] != ' ')
                break;
        }  
}


void generate_damka_moves(gamestate  *g, link *ptr_lista_ruchów, int pole_pierwotne_dam, int *ptr_inx_tab)
{
    
    for ( int pole_aktualne_dam  = pole_pierwotne_dam ; pole_aktualne_dam/WIERSZE-1 >= 0 && pole_aktualne_dam%WIERSZE -1 >= 0 && g->plansza.__1D[pole_aktualne_dam-WIERSZE-1] == ' ' ; pole_aktualne_dam-=(WIERSZE+1))// tył lewo
        {       
                
            (ptr_lista_ruchów+*ptr_inx_tab)->data = (ruch){ .z_pola=pole_pierwotne_dam, .na_pole= pole_aktualne_dam-WIERSZE-1, .czy_bicie =0 };
            (*ptr_inx_tab)++;
                
                
        }
    for ( int pole_aktualne_dam  = pole_pierwotne_dam ; pole_aktualne_dam/WIERSZE-1 >= 0 && pole_aktualne_dam%WIERSZE +1 <WIERSZE && g->plansza.__1D[pole_aktualne_dam-WIERSZE+1] == ' ' ; pole_aktualne_dam-=(WIERSZE-1)) // tył prawo
        {
                
            (ptr_lista_ruchów+*ptr_inx_tab)->data = (ruch){ .z_pola=pole_pierwotne_dam, .na_pole= pole_aktualne_dam-WIERSZE+1, .czy_bicie =0 };
            (*ptr_inx_tab)++;
                
                
        }
        for ( int pole_aktualne_dam  = pole_pierwotne_dam ; pole_aktualne_dam/WIERSZE+1 <WIERSZE && pole_aktualne_dam%WIERSZE -1 >= 0 && g->plansza.__1D[pole_aktualne_dam+WIERSZE-1] == ' ' ; pole_aktualne_dam+=(WIERSZE-1)) // przód lewo
        {       
                
            (ptr_lista_ruchów+*ptr_inx_tab)->data = (ruch){ .z_pola=pole_pierwotne_dam, .na_pole= pole_aktualne_dam+WIERSZE-1, .czy_bicie =0 };
            (*ptr_inx_tab)++;
                
                
        }
        for ( int pole_aktualne_dam  = pole_pierwotne_dam ; pole_aktualne_dam/WIERSZE+1 <WIERSZE && pole_aktualne_dam%WIERSZE +1 <WIERSZE  && g->plansza.__1D[pole_aktualne_dam+WIERSZE+1] == ' ' ; pole_aktualne_dam+=(WIERSZE+1)) // przód prawo
        {       
                
            (ptr_lista_ruchów+*ptr_inx_tab)->data = (ruch){ .z_pola=pole_pierwotne_dam, .na_pole= pole_aktualne_dam+WIERSZE+1, .czy_bicie =0 };
            (*ptr_inx_tab)++;
                
                
        }
        
    
}
 

link *generate_moves (gamestate *g , link *ptr_lista_ruchow)
{
  
   int index_tablica=0;
   //for ( int i = 0, i<WIERSZE*KOLUMNY; i++ )
            //if(g->plansza.__1D[i] = 'C'||'B')

    for ( int i =0 ; i<WIERSZE*KOLUMNY && g->na_ruchu==CZARNY; i++)  
    {
        if(g->plansza.__1D[i] == 'C')
            generate_damka_moves(g,ptr_lista_ruchow,i,&index_tablica);
    }

    for ( int i =0 ; i<WIERSZE*KOLUMNY && g->na_ruchu==BIALY; i++)  
    {
        if(g->plansza.__1D[i] == 'B')
            generate_damka_moves(g,ptr_lista_ruchow,i,&index_tablica);
    } 
    
    

     
    for (int i = 0; i < WIERSZE * KOLUMNY; i++) // i to pole w tej iteracji
    {
        if(g->na_ruchu==CZARNY)// generuje gdy jest kolejka czarnych 
        {
                                                    /* WARUNKI DLA CZARMYCH*/
            if (g->plansza.__1D[i] == 'c' && (i/WIERSZE +1 < WIERSZE) && (i%WIERSZE-1 >=0) && (g->plansza.__1D[i+WIERSZE-1]==' ') )   // przód lewo    
                {
                    *(ptr_lista_ruchow+index_tablica) = (link) {.data={.z_pola=i ,.na_pole=i+WIERSZE-1 ,.czy_bicie=0}};
                    index_tablica++;
                }
        
        
            if (g->plansza.__1D[i] == 'c' && (i/WIERSZE +1 < WIERSZE) && (i%WIERSZE+1 <WIERSZE) && (g->plansza.__1D[i+WIERSZE+1]==' ') )  //przód prawo      
            {
                
                *(ptr_lista_ruchow+index_tablica) = (link) {.data={.z_pola=i ,.na_pole=i+WIERSZE+1 ,.czy_bicie=0}};
                index_tablica++;
            }
        }

        if (g->na_ruchu==BIALY)
        {
                                                    /*WARUNKI DLA BIAŁYCH*/
            if (g->plansza.__1D[i] == 'b' && (i/WIERSZE -1 >=0 ) && (i%WIERSZE-1 >=0) && (g->plansza.__1D[i-WIERSZE-1]==' ') )   // tył lewo    
                {
                    *(ptr_lista_ruchow+index_tablica) = (link) {.data={.z_pola=i ,.na_pole=i-WIERSZE-1 ,.czy_bicie=0}};
                    index_tablica++;            
                }
            
            if (g->plansza.__1D[i] == 'b' && (i/WIERSZE -1 >=0 ) && (i%WIERSZE+1 <WIERSZE) && (g->plansza.__1D[i-WIERSZE+1]==' ') )   // tył prawo
                {
                    *(ptr_lista_ruchow+index_tablica) = (link) {.data={.z_pola=i ,.na_pole=i-WIERSZE+1 ,.czy_bicie=0}};
                    index_tablica++;
                }
        
        }
    }
        
                

    return (ptr_lista_ruchow);
}


link *generate_all (gamestate *g)
{
    int index_tablica=0;
    g->bicie=0;
    link *ptr_lista_ruchow= malloc(1024*sizeof(link));
    for (int index_pole = 0; index_pole<WIERSZE*KOLUMNY; index_pole++)
    {
        *(ptr_lista_ruchow+index_pole)= (link) {.data.na_pole =66 , .data.z_pola=66 };
    }
    
    for(int i =0 ; (( i<WIERSZE*KOLUMNY)&&(g->na_ruchu==CZARNY))  ; i++)
    {   
        if(g->plansza.__1D[i]=='C')
            generate_damka_bicia(g,ptr_lista_ruchow,i,&index_tablica);
    }

    for(int i =0 ; (( i<WIERSZE*KOLUMNY)&&(g->na_ruchu==BIALY)); i++)
    {
        if(g->plansza.__1D[i]=='B')
            generate_damka_bicia(g,ptr_lista_ruchow,i,&index_tablica);
    }
    
                                                /*BICIA NORMALNYCH PIONKÓW*/
    for ( int index_pole =0; index_pole < WIERSZE*KOLUMNY; index_pole++)
    {
                                        
        if(g->na_ruchu==CZARNY) // generuje bicia dla ruchu czarnych  
        {
                                                                        /*CZARNE*/
            if ((index_pole/8 /*liczba całkowita z dzielenia */+2 <WIERSZE &&(index_pole%8)+2<WIERSZE) &&
            g->plansza.__1D[index_pole]== 'c' &&(g->plansza.__1D[index_pole+WIERSZE+1]== 'b'|| g->plansza.__1D[index_pole+WIERSZE+1]=='B')&& g->plansza.__1D[index_pole+2*WIERSZE+2]==' ' )//przód prawo
            {
                
                g->bicie=1;
                *(ptr_lista_ruchow+index_tablica)= (link) {.data.z_pola=index_pole ,.data.na_pole=index_pole+2*WIERSZE+2,.data.czy_bicie=1};
                index_tablica++;
            }

            if ((index_pole/8 +2  <WIERSZE &&(index_pole%8) -2 >=0) &&
            g->plansza.__1D[index_pole]== 'c' &&(g->plansza.__1D[index_pole+WIERSZE-1]== 'b'||g->plansza.__1D[index_pole+WIERSZE-1]=='B')&& g->plansza.__1D[index_pole+2*WIERSZE-2]==' ' ) //przód lewo
            {        
                g->bicie=1;
                *(ptr_lista_ruchow+index_tablica)= (link) {.data.z_pola=index_pole ,.data.na_pole=index_pole+2*WIERSZE-2, .data.czy_bicie=1};
                index_tablica++;
            }
            if ((index_pole/8 -2  >=0 &&(index_pole%8)-2 >= 0) &&
            g->plansza.__1D[index_pole]== 'c' &&( g->plansza.__1D[index_pole-WIERSZE-1]== 'b'||g->plansza.__1D[index_pole-WIERSZE-1]=='B') && g->plansza.__1D[index_pole-2*WIERSZE-2]==' ' ) //tył lewo
                {
                g->bicie=1;    
                *(ptr_lista_ruchow+index_tablica)= (link) {.data.z_pola=index_pole ,.data.na_pole=index_pole-2*WIERSZE-2, .data.czy_bicie=1};
                index_tablica++;
                }

            if ((index_pole/8 -2 >=0 &&(index_pole%8)+2 <WIERSZE) &&
            g->plansza.__1D[index_pole]== 'c' &&(g->plansza.__1D[index_pole-WIERSZE+1]== 'b'|| g->plansza.__1D[index_pole-WIERSZE+1]=='B')&& g->plansza.__1D[index_pole-2*WIERSZE+2]==' ' ) //tył prawo
                {    
                    g->bicie=1;
                    *(ptr_lista_ruchow+index_tablica)= (link) {.data.z_pola=index_pole ,.data.na_pole=index_pole-2*WIERSZE+2, .data.czy_bicie=1};
                    index_tablica++;
                }
        }




        if(g->na_ruchu==BIALY) // generowanie ruchów dla białych na ruchu 
        {
                                                                        /*BIALE*/  
            if ((index_pole/8 +2 <WIERSZE &&(index_pole%8)+2<WIERSZE)&&
            g->plansza.__1D[index_pole]== 'b' &&(g->plansza.__1D[index_pole+WIERSZE+1]== 'c'|| g->plansza.__1D[index_pole+WIERSZE+1]=='C') && g->plansza.__1D[index_pole+2*WIERSZE+2]==' ' )//przód prawo
            {
                g->bicie=1;
                *(ptr_lista_ruchow+index_tablica)= (link) {.data.z_pola=index_pole ,.data.na_pole=index_pole+2*WIERSZE+2, .data.czy_bicie=1};
                index_tablica++;
            }

            if ((index_pole/8 +2 <WIERSZE &&(index_pole%8) -2 >=0) && 
            g->plansza.__1D[index_pole]== 'b' &&(g->plansza.__1D[index_pole+WIERSZE-1]== 'c'||g->plansza.__1D[index_pole+WIERSZE-1]=='C')&& g->plansza.__1D[index_pole+2*WIERSZE-2]==' ' ) //przód lewo
            {        
                g->bicie=1;
                *(ptr_lista_ruchow+index_tablica)= (link) {.data.z_pola=index_pole ,.data.na_pole=index_pole+2*WIERSZE-2, .data.czy_bicie=1};
                index_tablica++;
            }
            if ((index_pole/8 -2 >=0 &&(index_pole%8)-2 >= 0) &&
            g->plansza.__1D[index_pole]== 'b' &&(g->plansza.__1D[index_pole-WIERSZE-1]== 'c'|| g->plansza.__1D[index_pole-WIERSZE-1]== 'C')&& g->plansza.__1D[index_pole-2*WIERSZE-2]==' ' ) //tył lewo
                {
                    g->bicie=1;
                    *(ptr_lista_ruchow+index_tablica)= (link) {.data.z_pola=index_pole ,.data.na_pole=index_pole-2*WIERSZE-2, .data.czy_bicie=1};
                    index_tablica++;
                }
            if (index_pole/8 -2 >=0  &&(index_pole%8)+2 <WIERSZE &&
            g->plansza.__1D[index_pole]== 'b' &&(g->plansza.__1D[index_pole-WIERSZE+1]== 'c' || g->plansza.__1D[index_pole-WIERSZE+1]== 'C')&& g->plansza.__1D[index_pole-2*WIERSZE+2]==' ' ) //tył prawo
                {    
                    g->bicie=1;
                    *(ptr_lista_ruchow+index_tablica)= (link) {.data.z_pola=index_pole ,.data.na_pole=index_pole-2*WIERSZE+2, .data.czy_bicie=1};
                    index_tablica++;
            }
        }
    }

    

    if(g->bicie==0)
    {
        generate_moves(g,ptr_lista_ruchow);
    }
    
    return(ptr_lista_ruchow);
}    







#endif

