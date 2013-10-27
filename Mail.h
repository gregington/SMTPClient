#ifndef MAIL_LIB_H
#define MAIL_LIB_H

#include "Arduino.h"

#ifndef MAIL_LIB_MAX_RECIPIENTS
#define MAIL_LIB_MAX_RECIPIENTS 16
#endif

typedef enum {
  TO = 1,
  CC = 2,
  BCC = 3
} recipient_t;

class Mail {
public:
  Mail();

  void from(char* from);
  void replyTo(char* replyTo);
  void to(char* to);
  void cc(char* cc);
  void bcc(char *bcc);
  void subject(char *subject);
  void body(char *body);

  friend class SmtpClient;
private:
  void addRecipient(recipient_t type, char* recipient);

  char *_from;
  char *_replyTo;
  char *_recipients[MAIL_LIB_MAX_RECIPIENTS];
  recipient_t _recipientTypes[MAIL_LIB_MAX_RECIPIENTS];
  uint8_t _recipientCount;
  char *_subject;
  char *_body;
};

#endif
