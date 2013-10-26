#ifndef MAIL_LIB_H
#define MAIL_LIB_H

struct Mail {

  char *from;
  char *to;
  char *replyTo;
  char *subject;
  char *body;
};

#endif
