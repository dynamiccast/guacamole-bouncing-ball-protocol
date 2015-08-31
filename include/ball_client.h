#ifndef _BALL_CLIENT_H
#define _BALL_CLIENT_H

#include <guacamole/client.h>

typedef struct ball_client_data {

  guac_layer* ball;

  int ball_x;
  int ball_y;

  int ball_velocity_x;
  int ball_velocity_y;

  guac_timestamp		last_update;
} ball_client_data;

#endif

