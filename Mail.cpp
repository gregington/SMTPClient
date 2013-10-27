#include "Mail.h"

Mail::Mail() {
  _from = NULL;
  _replyTo = NULL;
  memset(_recipients, NULL, sizeof(_recipients));
  memset(_recipientTypes, 0, sizeof(_recipientTypes));
  _recipientCount = 0;
  _subject = NULL;
  _body = NULL;
}

void Mail::from(char *from) {
  _from = from;
}

void Mail::replyTo(char *replyTo) {
  _replyTo = replyTo;
}

void Mail::to(char *to) {
  addRecipient(TO, to);
}

void Mail::cc(char *cc) {
  addRecipient(CC, cc);
}

void Mail::bcc(char *bcc) {
  addRecipient(BCC, bcc);
}

void Mail::subject(char *subject) {
  _subject = subject;
}

void Mail::body(char *body) {
  _body = body;
}

void Mail::addRecipient(recipient_t type, char* recipient) {
  if (_recipientCount == MAIL_LIB_MAX_RECIPIENTS) {
    return;
  }
  _recipientTypes[_recipientCount] = type;
  _recipients[_recipientCount] = recipient;
  _recipientCount++;
}