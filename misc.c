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

/* maskstricthost():
 * basically the same as maskhost() from src/misc.c, but _never_ stripts
 * "~+-^=" off the host
 * maskhost() version: * $Id: misc.c,v 1.1 2005/04/14 10:55:00 Administrator Exp $
 */
static void maskstricthost(const char *s, char *nw)
{
  const char *p, *q, *e, *f;
  int i;

  *nw++ = '*';
  *nw++ = '!';
  p = (q = strchr(s, '!')) ? q + 1 : s;
  /* Strip of any nick, if a username is found, use last 8 chars */
  if ((q = strchr(p, '@'))) {
    int fl = 0;

    if ((q - p) > 9) {
      nw[0] = '*';
      p = q - 7;
      i = 1;
    } else
      i = 0;
    while (*p != '@') {
      if (!fl && strchr("~+-^=", *p)) {
//        if (strict_host)
      nw[i] = '?';
//    else
//      i--;
      } else
    nw[i] = *p;
      fl++;
      p++;
      i++;
    }
    nw[i++] = '@';
    q++;
  } else {
    nw[0] = '*';
    nw[1] = '@';
    i = 2;
    q = s;
  }
  nw += i;
  e = NULL;
  /* Now q points to the hostname, i point to where to put the mask */
  if ((!(p = strchr(q, '.')) || !(e = strchr(p + 1, '.'))) && !strchr(q, ':'))
    /* TLD or 2 part host */
    strcpy(nw, q);
  else {
    if (e == NULL) {        /* IPv6 address?        */
      const char *mask_str;

      f = strrchr(q, ':');
      if (strchr(f, '.')) { /* IPv4 wrapped in an IPv6? */
    f = strrchr(f, '.');
    mask_str = ".*";
      } else            /* ... no, true IPv6.       */
    mask_str = ":*";
      strncpy(nw, q, f - q);
      /* No need to nw[f-q] = 0 here, as the strcpy below will
       * terminate the string for us.
       */
      nw += (f - q);
      strcpy(nw, mask_str);
    } else {
      for (f = e; *f; f++);
      f--;
      if (*f >= '0' && *f <= '9') { /* Numeric IP address */
    while (*f != '.')
      f--;
    strncpy(nw, q, f - q);
    /* No need to nw[f-q] = 0 here, as the strcpy below will
     * terminate the string for us.
     */
    nw += (f - q);
    strcpy(nw, ".*");
      } else {              /* Normal host >= 3 parts */
    /*    a.b.c  -> *.b.c
     *    a.b.c.d ->  *.b.c.d if tld is a country (2 chars)
     *             OR   *.c.d if tld is com/edu/etc (3 chars)
     *    a.b.c.d.e -> *.c.d.e   etc
     */
    const char *x = strchr(e + 1, '.');

    if (!x)
      x = p;
    else if (strchr(x + 1, '.'))
      x = e;
    else if (strlen(x) == 3)
      x = p;
    else
      x = e;
    sprintf(nw, "*%s", x);
      }
    }
  }
}


// From gseen.mod's old slang.c
static char getslang_error[17];
static char *getslang(int id, seendat *l)
{
  time_t tt;
  char t[20];
  char *text = malloc(250); 
  if (text == NULL) {
      return NULL;
  }

  //if (!glob_slang) {
  //  putlog(LOG_MISC, "*", "WARNING! No language selected! (getslang())");
  //  return "NOLANG";
  //}
  char hex_str[10];
  sprintf(hex_str, "0xc%d", id);
  tt = glob_seendat->when;
  strftime(t, sizeof(t), "%d.%m.%Y %H:%M", localtime(&tt));
  switch (id) {
    case 101:
    case 121:
    case 108:
      sprintf(text, get_language(strtol(hex_str, NULL, 16)), l->nick, l->host, l->chan, gseen_duration(now - glob_seendat->when), t, glob_seendat->nick);
      break;
    case 102:
    case 107:
      sprintf(text, get_language(strtol(hex_str, NULL, 16)), l->nick, l->host, l->chan, gseen_duration(now - glob_seendat->when), t, gseen_duration(glob_seendat->spent));
      break;
    case 103:
      sprintf(text, get_language(strtol(hex_str, NULL, 16)), l->nick, l->host, l->chan, gseen_duration(now - glob_seendat->when), t, glob_seendat->msg, gseen_duration(glob_seendat->spent));
      break;
    case 104:
    case 124:
      sprintf(text, get_language(strtol(hex_str, NULL, 16)), l->nick, l->host, glob_seendat->msg, l->chan, gseen_duration(now - glob_seendat->when), t, glob_seendat->msg);
      break;
    case 105:
    case 125:
      sprintf(text, get_language(strtol(hex_str, NULL, 16)), l->nick, l->host, glob_seendat->msg, l->chan, gseen_duration(now - glob_seendat->when), t, l->nick);
      break;
    case 106:
      char data[100];
      strcpy(data, glob_seendat->msg);
      char *punisher = data;
      char *reason = strchr(data, ' ');
      if (reason) {
        *reason = '\0';
        reason++;
      } else {
        reason = "";
      }
      sprintf(text, get_language(strtol(hex_str, NULL, 16)), l->nick, l->host, l->chan, punisher, reason, gseen_duration(now - glob_seendat->when), t, gseen_duration(glob_seendat->spent));
      break;
    case 128:
      sprintf(text, get_language(strtol(hex_str, NULL, 16)), l->nick, l->host, l->chan, gseen_duration(now - glob_seendat->when), t);
      break;
    case 109:
    case 110:
      sprintf(text, get_language(strtol(hex_str, NULL, 16)), l->nick, l->chan, glob_seendat->msg, gseen_duration(now - glob_seendat->when), t);
      break;
    case 129:
    case 130:
      sprintf(text, get_language(strtol(hex_str, NULL, 16)), l->nick, glob_seendat->msg, gseen_duration(now - glob_seendat->when), t);
      break;
    case 140:
      sprintf(text, get_language(strtol(hex_str, NULL, 16)), l->nick, l->host, l->chan, gseen_duration(now - glob_seendat->when), t);
      break;
    default:
      text = get_language(strtol(hex_str, NULL, 16));
  }
  if (!text) {
    snprintf(getslang_error, sizeof(getslang_error), "MSGC%d", id);
    return getslang_error;
  }
  return text;
}