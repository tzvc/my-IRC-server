/*
** connection_registration.c for myirc in /home/rootkid/rendu/psu/PSU_2016_myirc
** 
** Made by theo champion
** Login   <theo.champion@epitech.eu>
** 
** Started on  Thu May 25 13:44:09 2017 theo champion
** Last update Thu May 25 18:12:45 2017 theo champion
*/

#include "myirc.h"

bool	cmd_nick(t_handle *hdl)
{
  return (reply(hdl, 300, "%s %s :Nickname changed.",
		(hdl->sender->nick ? hdl->sender->nick : "*"), hdl->cmd_args[0]));
}
