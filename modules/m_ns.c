/************************************************************************
 *   IRC - Internet Relay Chat, modules/m_ns.c
 *   This file is copyright (C) 2002 Stuart Walsh
 *                                    <stu@ipng.org.uk>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */

#include "stdinc.h"
#include "tools.h"
#include "common.h"
#include "handlers.h"
#include "client.h"
#include "channel.h"
#include "channel_mode.h"
#include "vchannel.h"
#include "hash.h"
#include "ircd.h"
#include "numeric.h"
#include "s_serv.h"
#include "send.h"
#include "list.h"
#include "irc_string.h"
#include "s_conf.h"
#include "msg.h"
#include "parse.h"
#include "modules.h"

#define SERVICES_NAME "services.oftc.net"

static void m_ns(struct Client *, struct Client *, int, char **);

const char* _version = "$Revision 0.1$";

struct Message ns_msgtab = {
      "NS", 0, 0, 0, 0, MFLG_SLOW, 0,
        {m_unregistered, m_ns, m_ns, m_ns}
};

void _modinit(void)
{
      mod_add_cmd(&ns_msgtab);
}

void
_moddeinit(void)
{
      mod_del_cmd(&ns_msgtab);
}

static void m_ns(struct Client *client_p, struct Client *source_p, int parc, char *parv[])
{
    struct Client    *target_p;
    char buf[BUFSIZE];
    int i;

    memset(buf, 0, BUFSIZE);

    if (parc < 2 || *parv[1] == '\0')
    {
        if(MyClient(source_p))
            sendto_one(source_p, form_str(ERR_NOTEXTTOSEND), me.name, parv[0]);
        return;
    }

    /* Ensure full message gets passed on */
    strlcpy(buf, parv[1], BUFSIZE);
    for(i = 2; i < parc; i++)
    {
        strlcat(buf, " ", BUFSIZE);
        strlcat(buf, parv[i], BUFSIZE);
    }
        
    if ((target_p = find_server(SERVICES_NAME)))
        sendto_one(target_p, ":%s PRIVMSG %s :%s",
            parv[0], "nickserv", buf);
    else
        sendto_one(source_p, form_str(ERR_SERVICESDOWN), me.name, 
               parv[0], "nickserv");
    return;
}