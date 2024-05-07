/*
 * Copyright (C) 2000,2001  Florian Sander
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

static int seenflood()
{
  if (!maxseen_thr || !maxseen_time)
    return 0;
  if ((now - seenflood_time) > maxseen_time) {
    seenflood_time = now;
    seenflood_thr = 0;
  }
  seenflood_thr++;
  if (seenflood_thr > maxseen_thr)
    return 1;
  else
    return 0;
}

static int nopub(char *chan)
{
  char buf[121], *b;

  Context;
  strncpy(buf, no_pub, 120);
  buf[120] = 0;
  b = buf;
  while (b[0])
    if (!strcasecmp(chan, newsplit(&b)))
      return 1;
  if (ngetudef("nopubseens", chan))
    return 1;
  return 0;
}

static int quietseen(char *chan)
{
  char buf[121], *b;

  Context;
  strncpy(buf, quiet_seen, 120);
  buf[120] = 0;
  b = buf;
  while (b[0])
    if (!strcasecmp(chan, newsplit(&b)))
      return 1;
  if (ngetudef("quietseens", chan))
    return 1;
  return 0;
}

static int cmd_seen(struct userrec *u, int idx, char *par)
{
  char *query;
  
  Context;
  if (seenflood())
    return 0;
  reset_global_vars();
  //glob_slang = slang_find(coreslangs, default_slang);
  glob_nick = dcc[idx].nick;
  query = newsplit(&par);
  glob_query = query;
  putlog(LOG_CMDS, "*", "#%s# seen %s", dcc[idx].nick, par);
  dprintf(idx, "%s\n", do_seen(query, dcc[idx].nick,
  	  dcc[idx].host, "[partyline]", botnet_seen));
  return 0;
}

static int cmd_seenstats(struct userrec *u, int idx, char *par)
{
  Context;
  if (seenflood())
    return 0;
  reset_global_vars();
  //glob_slang = slang_find(coreslangs, default_slang);
  glob_nick = dcc[idx].nick;
  putlog(LOG_CMDS, "*", "#%s# seenstats", dcc[idx].nick);
  dprintf(idx, "%s\n", do_seenstats());
  return 0;
}

static int cmd_purgeseens(struct userrec *u, int idx, char *par)
{
  Context;
  purge_seens();
  putlog(LOG_CMDS, "*", "#%s# purgeseens", dcc[idx].nick);
  return 0;
}

static int pub_seen(char *nick, char *host, char *hand,
        char *channel, char *text)
{
  char *dest;
  struct chanset_t *chan;

  Context;
  if (seenflood() || nopub(channel))
    return 0;
  reset_global_vars();
  //glob_slang = slang_find(coreslangs, slang_chanlang_get(chanlangs, channel));
  glob_nick = nick;
  putlog(LOG_CMDS, "*", "<<%s>> !%s! seen %s", nick, hand, text);
  if (quietseen(channel)) {
    dprintf(DP_HELP, "NOTICE %s :%s, %s\n", nick, nick,
           do_seen(newsplit(&text), nick, host, channel, botnet_seen));
    return 0;
  }
  chan = findchan_by_dname(channel);
  if (chan)
    dest = chan->name;
  else
    dest = channel;
  dprintf(DP_HELP, "PRIVMSG %s :%s, %s\n", dest, nick,
  	  do_seen(newsplit(&text), nick, host, channel, botnet_seen));
  return 0;
}

static int pub_seenstats(char *nick, char *host, char *hand,
        char *channel, char *text)
{
  char *dest;
  struct chanset_t *chan;

  Context;
  if (seenflood() || nopub(channel))
    return 0;
  reset_global_vars();
  //glob_slang = slang_find(coreslangs, slang_chanlang_get(chanlangs, channel));
  glob_nick = nick;
  putlog(LOG_CMDS, "*", "<<%s>> !%s! seenstats", nick, hand);
  if (quietseen(channel)) {
    dprintf(DP_HELP, "NOTICE %s :%s, %s\n", nick, nick, do_seenstats());
    return 0;
  }
  chan = findchan_by_dname(channel);
  if (chan)
    dest = chan->name;
  else
    dest = channel;
  dprintf(DP_HELP, "PRIVMSG %s :%s, %s\n", dest, nick, do_seenstats());
  return 1;
}

static int msg_seen(char *nick, char *uhost, struct userrec *u, char *text)
{
  Context;
  if (seenflood())
    return 0;
  reset_global_vars();
  //glob_slang = slang_getbynick(coreslangs, nick);
  glob_nick = nick;
  putlog(LOG_CMDS, "*", "(%s!%s) !%s! seen %s", nick, uhost, u ? u->handle : "*", text);
  dprintf(DP_HELP, "PRIVMSG %s :%s\n", nick,
  	  do_seen(newsplit(&text), nick, uhost, "[/msg]", botnet_seen));
  return 1;
}

static int pub_seennick(char *nick, char *host, char *hand,
        char *channel, char *text)
{
  seendat *l;
  char *dest;
  struct chanset_t *chan;

  Context;
  if (seenflood() || nopub(channel))
    return 0;
  putlog(LOG_CMDS, "*", "<<%s>> !%s! seennick %s", nick, hand, text);
  reset_global_vars();
  //glob_slang = slang_find(coreslangs, slang_chanlang_get(chanlangs, channel));
  glob_nick = nick;
  chan = findchan_by_dname(channel);
  if (chan)
    dest = chan->name;
  else
    dest = channel;

  text = newsplit(&text);
  l = findseen(text);
  if (!l) {
    glob_query = text;
    if (quietseen(channel)) {
      char output[100];
      sprintf(output, "NOTICE %s :%s, %s\n", nick, nick, SLNOTSEEN);
      dprintf(DP_HELP, output, text);
    } else {
      char output[100];
      sprintf(output, "PRIVMSG %s :%s, %s\n", dest, nick, SLNOTSEEN);
      dprintf(DP_HELP, output, text);
    }
    return 0;
  }
  if (quietseen(channel)) {
    char output[100];
    sprintf(output, "NOTICE %s :%s, %s\n", nick, nick, do_seennick(l));
    dprintf(DP_HELP, output, nick, text);
  } else {
    char output[100];
    sprintf(output, "PRIVMSG %s :%s, %s\n", dest, nick, do_seennick(l));
    dprintf(DP_HELP, output, nick, text);
  }
  return 0;
}

static int msg_seennick(char *nick, char *uhost, struct userrec *u, char *text)
{
  seendat *l;

  Context;
  if (seenflood())
    return 0;
  putlog(LOG_CMDS, "*", "(%s!%s) !%s! seennick %s", nick, uhost, u ? u->handle : "*", text);
  reset_global_vars();
  //glob_slang = slang_getbynick(coreslangs, nick);
  glob_nick = nick;
  text = newsplit(&text);
  l = findseen(text);
  if (!l) {
    glob_query = text;
    dprintf(DP_HELP, "PRIVMSG %s :%s\n", nick, SLNOTSEEN);
    return 0;
  }
  dprintf(DP_HELP, "PRIVMSG %s :%s\n", nick, do_seennick(l));
  return 0;
}

static int cmd_seennick(struct userrec *u, int idx, char *text)
{
  seendat *l;

  Context;
  if (seenflood())
    return 0;
  putlog(LOG_CMDS, "*", "#%s# seennick %s", dcc[idx].nick, text);
  reset_global_vars();
  //glob_slang = slang_find(coreslangs, default_slang);
  glob_nick = dcc[idx].nick;
  text = newsplit(&text);
  l = findseen(text);
  if (!l) {
    glob_query = text;
    dprintf(idx, "%s\n", SLNOTSEEN);
    return 0;
  }
  dprintf(idx, "%s\n", do_seennick(l));
  return 0;
}

static int bot_gseen_req(char *bot, char *code, char *par)
{
  char *mask, *nick, *uhost, *chan, *reply;
  char tosend[256];
  int i;

  Context;
  if (seenflood())
    return 0;
  i = nextbot(bot);
  if (i < 0) {
    debug1("Couldn't answer botnet-seen-request from %s: no such bot", bot);
    return 0;
  }
  mask = newsplit(&par);
  nick = newsplit(&par);
  uhost = newsplit(&par);
  chan = newsplit(&par);
  reset_global_vars();
  //glob_slang = slang_find(coreslangs, slang_chanlang_get(chanlangs, chan));
  glob_nick = nick;
  reply = do_seen(mask, nick, uhost, chan, -1);
  if (!reply)
    return 0;
  if ((strlen(nick) + strlen(chan) + strlen(reply)) < 255) {
    sprintf(tosend, "gseen_rep %s %s %s", nick, chan, reply);
    botnet_send_zapf(i, botnetnick, bot, tosend);
  }
  return 0;
}

static int bot_gseen_rep(char *bot, char *code, char *par)
{
  char *nick, *chan, *reply;
  int i;

  Context;
  if (seenflood())
    return 0;
  if (!bnsnick || !bnschan) {
    if (bnsnick)
      nfree(bnsnick);
    if (bnschan)
      nfree(bnschan);
    bnsnick = bnschan = NULL;
    return 0;
  }
  nick = newsplit(&par);
  chan = newsplit(&par);
  reset_global_vars();
  glob_remotebot = bot;
  //glob_slang = slang_find(coreslangs, slang_chanlang_get(chanlangs, chan));
  glob_nick = nick;
  reply = par;
  if (strcmp(nick, bnsnick) || strcmp(chan, bnschan))
    return 0; /* unwanted reply */
  if (findchan(chan)) {
    if (nopub(chan)) {
      nfree(bnsnick);
      nfree(bnschan);
      bnsnick = bnschan = NULL;
      debug1("%s is nopub, bns-reply dropped", chan);
      return 0;
    }
    if (quietseen(chan)) {
      dprintf(DP_HELP, "NOTICE %s :%s%s\n", nick, SLRBOTSAYS, reply);
    } else {
      dprintf(DP_HELP, "PRIVMSG %s :%s%s\n", chan, SLRBOTSAYS, reply);
    }
  } else if (!strcmp(chan, "[/msg]")) {
    dprintf(DP_HELP, "PRIVMSG %s :%s%s\n", nick, SLRBOTSAYS, reply);
  } else if (!strcmp(chan, "[partyline]")) {
    for (i = 0; i < dcc_total; i++) {
      if ((!strcasecmp(nick, dcc[i].nick)) &&
         (dcc[i].type->flags & DCT_SIMUL)) {
        dprintf(i, "%s%s\n", SLRBOTSAYS, reply);
        break;
      }
    }
  } else
    debug1("Couldn't send received bns answer, no such chan %s", chan);
  nfree(bnsnick);
  nfree(bnschan);
  bnsnick = bnschan = NULL;
  return 0;
}

static cmd_t mydcc[] =
{
  {"seen", "-|-", cmd_seen, NULL},
  {"seenstats", "-|-", cmd_seenstats, NULL},
  {"purgeseens", "m", cmd_purgeseens, NULL},
  {"seennick", "-|-", cmd_seennick, NULL},
  {0, 0, 0, 0}
};

static cmd_t seen_pub[] =
{
  {"!seen", "", pub_seen, 0},
  {"!seenstats", "", pub_seenstats, 0},
  {"!seennick", "", pub_seennick, 0},
  {0, 0, 0, 0}
};

static cmd_t seen_msg[] =
{
  {"seen", "", msg_seen, 0},
  {"seennick", "", msg_seennick, 0},
  {0, 0, 0, 0}
};

static cmd_t seen_bot[] =
{
  {"gseen_req", "", bot_gseen_req, 0},
  {"gseen_rep", "", bot_gseen_rep, 0},
  {0, 0, 0, 0}
};
