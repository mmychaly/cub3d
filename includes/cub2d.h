#ifndef CUB2D_H
#define CUB2D_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include "../libft/libft.h"

typedef struct s_player
{
	int		y;
	int		x;
	char    direction;
}			t_player;

typedef struct s_data
{
    char    *no;
    char    *so;
    char    *ea;
    char    *we;
    int     f;
    int     c;
    char    **map;
    t_player	player;
}           t_data;

#endif