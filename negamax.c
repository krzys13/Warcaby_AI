#include"game_engine.h"

int max (int a, int b)
{
  int result = (a >= b ? a : b);
  return result;
}


int negamax (gamestate *g, int depth, link *moves)
{
  int value;
  if (depth == 0)
    {
        int value = analyse (g);
        return value;
    }
  else
    {
        int value = -100000;
        for (int i = 0; (moves+i)->data.na_pole != 66; i++)
    {
        gamestate temp_g = wykonaj_ruch(g, moves[i].data);
        int temp_v = -(negamax (&temp_g, depth - 1, generate_all(&temp_g)));
        value = max (value, temp_v);
    }
    }
   return value;
}

ruch choose_best_move (gamestate *g, link *moves, int depth)
{
  ruch best_move = moves[0].data;
  gamestate first_temp = wykonaj_ruch(g, moves[0].data);
  int best_move_value = -negamax (&first_temp, depth, generate_all(&first_temp));

  for (int i = 1; moves[i].data.na_pole != 66 ; i++)
  {
    gamestate temp_g = wykonaj_ruch(g, moves[i].data);
    int move_value = -negamax(&temp_g, depth - 1, generate_all(&temp_g));
    if (move_value > best_move_value)
    {
      move_value = best_move_value;
      best_move = moves[i].data;
    }
  }
  return best_move;
}
