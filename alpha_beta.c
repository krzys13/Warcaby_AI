#include "game_engine.h"


ruch *alpha_beta (ruch* ptr_lista_ruchow, gamestate*g, int deepth , int czy_max   ) // max =1 gdzy max robie a =0 gdy robi  min
{
    if ( deepth == 0)
    {
        return (ptr_lista_ruchow);
 
    }
           
   if (czy_max) //maximum 1
   {
    for ( int pozycja =0; pozycja <WIERSZE*KOLUMNY; pozycja ++)
        for ( int index_ruch=0 ; (ptr_lista_ruchow+index_ruch)->na_pole != 66; index_ruch++)
        {
            
            gamestate nowy_stan_gry = wykonaj_ruch(g,*(ptr_lista_ruchow+index_ruch));
            int ocena_ruch_max;
            ruch *ptr_ruch_max;
            
            ruch *ptr_ruch_check;
            

            if ( index_ruch==0 )
            {
                ocena_ruch_max = analyse(&nowy_stan_gry);
                ptr_ruch_max = ptr_lista_ruchow;
                
            }

            if(deepth ==1 && index_ruch>0)
            {
                gamestate ocena_gamestate_check=wykonaj_ruch(&nowy_stan_gry,*(ptr_lista_ruchow+index_ruch));
                int ocena_ruch_check = analyse(&nowy_stan_gry);
                if ( ocena_ruch_max < ocena_ruch_check )
                {
                    ocena_ruch_max=ocena_ruch_check;
                    ptr_ruch_max = (ptr_lista_ruchow+index_ruch);
                    
                }
                return(ptr_ruch_max);   
            }
            
            if ( deepth >1)
            {
                ruch *ptr_max =alpha_beta((&generate_all(&nowy_stan_gry)->data),&nowy_stan_gry,deepth-1, 1);
                gamestate ocena_gameste_check =wykonaj_ruch(&nowy_stan_gry,*ptr_max);
                int ocena_ruch_check = analyse(&ocena_gameste_check);
                if ( ocena_ruch_max < ocena_ruch_check )
                {
                    ocena_ruch_max=ocena_ruch_check;
                    ptr_ruch_max = (ptr_lista_ruchow+index_ruch);
                    
                }  
                return(ptr_ruch_max); 
            }
             
                
        }
   }

}


