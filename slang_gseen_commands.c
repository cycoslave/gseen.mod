#include <stddef.h>

static void slang_send_botnick()
{
  size_t remaining_space = sizeof(slang_text_buf) - strlen(slang_text_buf);
  strncat(slang_text_buf, botname, remaining_space);
}

static void slang_send_query()
{
  if (glob_query) {
    size_t remaining_space = sizeof(slang_text_buf) - strlen(slang_text_buf);
    strncat(slang_text_buf, glob_query, remaining_space);
  }
}

static void slang_send_laston()
{
  if (glob_laston) {
    size_t remaining_space = sizeof(slang_text_buf) - strlen(slang_text_buf);
    strncat(slang_text_buf, glob_laston, remaining_space);
  }
}

static void slang_send_otherchan()
{
  if (glob_otherchan) {
    size_t remaining_space = sizeof(slang_text_buf) - strlen(slang_text_buf);
    strncat(slang_text_buf, glob_otherchan, remaining_space);
  }
}

static void slang_send_othernick()
{
  if (glob_othernick) {
    size_t remaining_space = sizeof(slang_text_buf) - strlen(slang_text_buf);
    strncat(slang_text_buf, glob_othernick, remaining_space);
  }
}

static void slang_send_remotebot()
{
  if (glob_remotebot) {
    size_t remaining_space = sizeof(slang_text_buf) - strlen(slang_text_buf);
    strncat(slang_text_buf, glob_remotebot, remaining_space);
  }
}

static void slang_send_snick()
{
  if (glob_seendat) {
    size_t remaining_space = sizeof(slang_text_buf) - strlen(slang_text_buf);
    strncat(slang_text_buf, glob_seendat->nick, remaining_space);
  }
}

static void slang_send_shost()
{
  if (glob_seendat) {
    size_t remaining_space = sizeof(slang_text_buf) - strlen(slang_text_buf);
    strncat(slang_text_buf, glob_seendat->host, remaining_space);
  }
}

static void slang_send_schan()
{
  if (glob_seendat) {
    size_t remaining_space = sizeof(slang_text_buf) - strlen(slang_text_buf);
    strncat(slang_text_buf, glob_seendat->chan, remaining_space);
  }
}

static void slang_send_swhen()
{
  char *dur;

  if (glob_seendat) {
    dur = gseen_duration(now - glob_seendat->when);
    size_t remaining_space = sizeof(slang_text_buf) - strlen(slang_text_buf);
    strncat(slang_text_buf, dur, remaining_space);
  }
}

static void slang_send_stime()
{
  time_t tt;
  char t[20];
  
  if (glob_seendat) {
    tt = glob_seendat->when;
    strftime(t, sizeof(t), "%d.%m.%Y %H:%M", localtime(&tt));
    size_t remaining_space = sizeof(slang_text_buf) - strlen(slang_text_buf);
    strncat(slang_text_buf, t, remaining_space);
  }
}

static void slang_send_spent()
{
  char *dur;

  if (glob_seendat) {
    dur = gseen_duration(glob_seendat->spent);
    size_t remaining_space = sizeof(slang_text_buf) - strlen(slang_text_buf);
    strncat(slang_text_buf, dur, remaining_space);
  }
}

static void slang_send_smsg()
{
  if (glob_seendat) {
    size_t remaining_space = sizeof(slang_text_buf) - strlen(slang_text_buf);
    strncat(slang_text_buf, glob_seendat->msg, remaining_space);
  }
}

static void slang_send_numresults()
{
  char buf[7];
  
  snprintf(buf, sizeof(buf), "%d", numresults);
  size_t remaining_space = sizeof(slang_text_buf) - strlen(slang_text_buf);
  strncat(slang_text_buf, buf, remaining_space);
}

static void slang_send_punisher()
{
  char *reason;
  ptrdiff_t len;
  
  if (glob_seendat) {
    reason = strchr(glob_seendat->msg, ' ');
    if (!reason) {
      size_t remaining_space = sizeof(slang_text_buf) - strlen(slang_text_buf);
      strncat(slang_text_buf, glob_seendat->msg, remaining_space);
    } else {
      len = reason - glob_seendat->msg;
      strncat(slang_text_buf, glob_seendat->msg, (sizeof(slang_text_buf) < len) ? sizeof(slang_text_buf) : len);
    }
  }
}

static void slang_send_kickreason()
{
  char *reason;
  
  if (glob_seendat) {
    reason = strchr(glob_seendat->msg, ' ');
    if (reason) {
      size_t remaining_space = sizeof(slang_text_buf) - strlen(slang_text_buf);
      strncat(slang_text_buf, reason, remaining_space);
    }
  }
}

static void slang_send_rnick()
{
  if (glob_seenrequest) {
    Assert(glob_seenrequest->by);
    Assert(glob_seenrequest->by->who);
    size_t remaining_space = sizeof(slang_text_buf) - strlen(slang_text_buf);
    strncat(slang_text_buf, glob_seenrequest->by->who, remaining_space);
  }
}

static void slang_send_rchan()
{
  if (glob_seenrequest) {
    Assert(glob_seenrequest->by);
    Assert(glob_seenrequest->by->chan);
    size_t remaining_space = sizeof(slang_text_buf) - strlen(slang_text_buf);
    strncat(slang_text_buf, glob_seenrequest->by->chan, remaining_space);
  }
}

static void slang_send_rhost()
{
  if (glob_seenrequest) {
    Assert(glob_seenrequest->by);
    Assert(glob_seenrequest->by->host);
    size_t remaining_space = sizeof(slang_text_buf) - strlen(slang_text_buf);
    strncat(slang_text_buf, glob_seenrequest->by->host, remaining_space);
  }
}

static void slang_send_rtime()
{
  time_t tt;
  char t[20];
  
  if (glob_seenrequest) {
    Assert(glob_seenrequest->by);
    tt = glob_seenrequest->by->when;
    strftime(t, sizeof(t), "%d.%m.%Y %H:%M", localtime(&tt));
    size_t remaining_space = sizeof(slang_text_buf) - strlen(slang_text_buf);
    strncat(slang_text_buf, t, remaining_space);
  }
}

static void slang_send_rwhen()
{
  if (glob_seenrequest) {
    Assert(glob_seenrequest->by);
    size_t remaining_space = sizeof(slang_text_buf) - strlen(slang_text_buf);
    strncat(slang_text_buf, gseen_duration(now - glob_seenrequest->by->when), remaining_space);
  }
}

static void slang_send_requests()
{
  char buf[7];
  
  snprintf(buf, sizeof(buf), "%d", glob_seenrequests);
  size_t remaining_space = sizeof(slang_text_buf) - strlen(slang_text_buf);
  strncat(slang_text_buf, buf, remaining_space);
}

static void slang_send_totalnicks()
{
  char buf[7];
  
  snprintf(buf, sizeof(buf), "%d", glob_totalnicks);
  size_t remaining_space = sizeof(slang_text_buf) - strlen(slang_text_buf);
  strncat(slang_text_buf, buf, remaining_space);
}

static void slang_send_totalbytes()
{
  char buf[20];
  
  snprintf(buf, sizeof(buf), "%d", glob_totalbytes);
  size_t remaining_space = sizeof(slang_text_buf) - strlen(slang_text_buf);
  strncat(slang_text_buf, buf, remaining_space);
}

static void slang_send_nick()
{
  if (glob_nick) {
    size_t remaining_space = sizeof(slang_text_buf) - strlen(slang_text_buf);
    strncat(slang_text_buf, glob_nick, remaining_space);
  }
}

struct slang_text_commands slang_text_gseen_command_table[] =
{
  {"botnick", slang_send_botnick},
  {"query", slang_send_query},
  {"laston", slang_send_laston},
  {"otherchan", slang_send_otherchan},
  {"othernick", slang_send_othernick},
  {"remotebot", slang_send_remotebot},
  {"snick", slang_send_snick},
  {"swhen", slang_send_swhen},
  {"stime", slang_send_stime},
  {"shost", slang_send_shost},
  {"schan", slang_send_schan},
  {"spent", slang_send_spent},
  {"smsg", slang_send_smsg},
  {"numresults", slang_send_numresults},
  {"snick2", slang_send_smsg},
  {"bnbot", slang_send_smsg},
  {"punisher", slang_send_punisher},
  {"kickreason", slang_send_kickreason},
  {"rnick", slang_send_rnick},
  {"rchan", slang_send_rchan},
  {"rhost", slang_send_rhost},
  {"rtime", slang_send_rtime},
  {"rwhen", slang_send_rwhen},
  {"requests", slang_send_requests},
  {"totalnicks", slang_send_totalnicks},
  {"totalbytes", slang_send_totalbytes},
  {"nick", slang_send_nick},
  {0, 0}
};
