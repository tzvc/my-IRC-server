/*
** communication.c for myirc in /home/rootkid/rendu/psu/PSU_2016_myirc
** 
** Made by theo champion
** Login   <theo.champion@epitech.eu>
** 
** Started on  Sun May 28 17:07:03 2017 theo champion
** Last update Sun May 28 17:25:54 2017 theo champion
*/

#include "irc_server.h"

bool		reply(t_handle *hdl, int code, const char *fmt, ...)
{
  char		*text;
  char		*reply;
  va_list	ap;
  size_t	len;

  va_start(ap, fmt);
  len = vsnprintf(NULL, 0, fmt, ap);
  va_start(ap, fmt);
  if ((text = malloc(sizeof(char) * len + 1)) == NULL)
    return (false);
  vsprintf(text, fmt, ap);
  if ((reply = malloc(strlen(text) +
                      strlen((hdl->sender->nick ? hdl->sender->nick : "*")) +
                      12 + strlen(hdl->server_ip))) == NULL)
    return (false);
  sprintf(reply, ":%s %03d %s %s \r\n", hdl->server_ip, code,
          (hdl->sender->nick ? hdl->sender->nick : "*"), text);
  write(hdl->sender->fd, reply, strlen(reply));
  free(text);
  free(reply);
  va_end(ap);
  return (code >= 400 ? false : true);
}

bool		idreply(t_handle *hdl, const char *fmt, ...)
{
  char		*text;
  char		*reply;
  va_list	ap;
  size_t	len;

  va_start(ap, fmt);
  len = vsnprintf(NULL, 0, fmt, ap);
  va_start(ap, fmt);
  if ((text = malloc(sizeof(char) * len + 1)) == NULL)
    return (false);
  vsprintf(text, fmt, ap);
  if ((reply = malloc(strlen(text) + strlen(hdl->sender->nick) +
		      strlen(hdl->sender->username) + 7 +
		      strlen(hdl->sender->hostname))) == NULL)
    return (false);
  sprintf(reply, ":%s!%s@%s %s\r\n", hdl->sender->nick, hdl->sender->username,
          hdl->sender->hostname, text);
  write(hdl->sender->fd, reply, strlen(reply));
  free(text);
  free(reply);
  va_end(ap);
  return (true);
}
