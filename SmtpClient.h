#ifndef SMTP_CLIENT_H
#define SMTP_CLIENT_H

#import "Arduino.h"
#import "Client.h"
#import "Mail.h"

class SmtpClient {
public:
  SmtpClient(Client *client, char *server);
  SmtpClient(Client *client, char *server, uint16_t port);
  SmtpClient(Client *client, IPAddress serverIP);
  SmtpClient(Client *client, IPAddress serverIP, uint16_t port);
  int send(Mail *mail);

private:
  char *_server;
  IPAddress _serverIP;
  uint16_t _port;
  Client *_client;

  int _send(Mail *mail);
  int connect();
  int helo();
  int mailFrom(char *from);
  int rcptTo(Mail *mail);
  int data();
  void headers(Mail *mail);
  void header(char *header, char* value);
  void recipientHeader(char *header, recipient_t type, Mail *mail);
  void body(char *body);
  int finishBody();
  int readStatus();
  int readLine(char *line, int maxLen);
};

#endif
