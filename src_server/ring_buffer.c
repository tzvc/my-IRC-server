/*
** ring_buffer.c for myirc in /home/rootkid/rendu/psu/PSU_2016_myirc
** 
** Made by theo champion
** Login   <theo.champion@epitech.eu>
** 
** Started on  Tue May 30 14:23:22 2017 theo champion
** Last update Wed May 31 14:42:52 2017 theo champion
*/

#include "irc_server.h"

static void	inc_ptr(t_rb *rb, char **ptr)
{
  if (*ptr == rb->buf + BUF_SIZE)
    *ptr = rb->buf;
  else
    (*ptr)++;
}

t_rb	*rb_init(void)
{
  t_rb	*rb;

  if ((rb = malloc(sizeof(t_rb))) == NULL)
    return (NULL);
  if ((rb->buf = (char *)malloc(BUF_SIZE)) == NULL)
    return (NULL);
  rb->wend = rb->buf;
  rb->rend = rb->buf;
  log_msg(INFO, "ring buffer initialized.");
  return (rb);
}

size_t	rb_get_space(t_rb *rb)
{
  return ((rb->wend >= rb->rend)
          ? (BUF_SIZE - (size_t)(rb->wend - rb->rend))
          : (size_t)(rb->rend - rb->wend));
}

void	rb_write(t_rb *rb, char *data)
{
  int	i;

  i = 0;
  while (data[i])
    {
      printf("%c\n", data[i]);
      if (data[i] == '\n')
	printf("nl found\n");
      if (data[i] != '\r')
	{
	  *rb->wend = data[i];
	  inc_ptr(rb, &rb->wend);
	}
	i++;
    }
}

char		*rb_readline(t_rb *rb)
{
  size_t	ct;
  char		*tmp;
  char		*line;

  ct = 0;
  tmp = rb->rend;
  while (tmp != rb->wend)
    {
      if (*tmp == '\n')
        break;
      inc_ptr(rb, &tmp);
      ct++;
    }
  if (tmp == rb->wend) {
    log_msg(ERROR, "No line found in ring buffer");
    return (NULL);
  }
  if ((line = (char *)malloc(ct + 1)) == NULL)
    return (NULL);
  ct = 0;
  while (*rb->rend != '\n')
    {
      line[ct++] = *rb->rend;
      inc_ptr(rb, &rb->rend);
    }
  inc_ptr(rb, &rb->rend);
  line[ct] = 0;
  return (line);
}
