#include <stdlib.h>
#include <unistd.h>
#include <guacamole/client.h>
#include <guacamole/protocol.h>
#include <guacamole/socket.h>
#include <guacamole/timestamp.h>
#include "ball_client.h"

/* Client plugin arguments */
const char* GUAC_CLIENT_ARGS[] = { NULL };

static int				ball_client_handle_messages(guac_client* client) {

  /* Get data */
  ball_client_data*		data = (ball_client_data*)client->data;
  guac_timestamp		current;
  int					delta_t;

  /* Deal with timing */
  usleep(30000);
  current = guac_timestamp_current();
  delta_t = current - data->last_update;

  /* Update position */
  data->ball_x += data->ball_velocity_x * delta_t / 1000;
  data->ball_y += data->ball_velocity_y * delta_t / 1000;

  /* Bounce if necessary */
  if (data->ball_x < 0) {
	data->ball_x = -data->ball_x;
	data->ball_velocity_x = -data->ball_velocity_x;
  }
  else if (data->ball_x >= 1024-128) {
	data->ball_x = (2*(1024-128)) - data->ball_x;
	data->ball_velocity_x = -data->ball_velocity_x;
  }

  if (data->ball_y < 0) {
	data->ball_y = -data->ball_y;
	data->ball_velocity_y = -data->ball_velocity_y;
  }
  else if (data->ball_y >= (768-128)) {
	data->ball_y = (2*(768-128)) - data->ball_y;
	data->ball_velocity_y = -data->ball_velocity_y;
  }

  guac_protocol_send_move(client->socket,
						  data->ball,
						  GUAC_DEFAULT_LAYER,
						  data->ball_x,
						  data->ball_y,
						  0);

  data->last_update = current;

  return 0;
}

int						guac_client_init(guac_client* client, int argc, char** argv)
{
  guac_layer*			texture = guac_client_alloc_buffer(client);
  ball_client_data*		data = malloc(sizeof(ball_client_data));

  data->last_update = guac_timestamp_current();
  data->ball = guac_client_alloc_layer(client);

  /* Send the name of the connection */
  guac_protocol_send_name(client->socket, "Bouncing Ball");

  /* Send the display size */
  guac_protocol_send_size(client->socket, GUAC_DEFAULT_LAYER, 1024, 768);

  /* Fill with solid color */
  guac_protocol_send_rect(client->socket, GUAC_DEFAULT_LAYER,
						  0, 0, 1024, 768);

  guac_protocol_send_rect(client->socket, texture, 0, 0, 64, 64);
  guac_protocol_send_cfill(client->socket, GUAC_COMP_OVER, texture,
						   0x88, 0x88, 0x88, 0xFF);

  guac_protocol_send_rect(client->socket, texture, 0, 0, 32, 32);
  guac_protocol_send_cfill(client->socket, GUAC_COMP_OVER, texture,
						   0xDD, 0xDD, 0xDD, 0xFF);

  guac_protocol_send_rect(client->socket, texture, 32, 32, 32, 32);
  guac_protocol_send_cfill(client->socket, GUAC_COMP_OVER, texture,
						   0xDD, 0xDD, 0xDD, 0xFF);

  guac_protocol_send_lfill(client->socket,
						   GUAC_COMP_OVER, GUAC_DEFAULT_LAYER,
						   texture);

  /* Set up our data->ball layer */
  guac_protocol_send_size(client->socket, data->ball, 128, 128);

  /* Fill with solid color */
  guac_protocol_send_arc(client->socket, data->ball,
						 64, 64, 62, 0, 6.28, 0);

  guac_protocol_send_close(client->socket, data->ball);

  guac_protocol_send_cstroke(client->socket,
							 GUAC_COMP_OVER, data->ball,
							 GUAC_LINE_CAP_ROUND, GUAC_LINE_JOIN_ROUND, 4,
							 0x00, 0x00, 0x00, 0xFF);

  guac_protocol_send_cfill(client->socket,
						   GUAC_COMP_OVER, data->ball,
						   0x00, 0x80, 0x80, 0x80);

  /* Start data->ball at upper left */
  data->ball_x = 0;
  data->ball_y = 0;

  /* Move at a reasonable pace to the lower right */
  data->ball_velocity_x = 200; /* pixels per second */
  data->ball_velocity_y = 200;   /* pixels per second */

  /* Flush buffer */
  guac_socket_flush(client->socket);

  /* Set handlers */
  client->data = data;
  client->handle_messages = ball_client_handle_messages;

  /* Done */
  return 0;
}
