#include "SmtpClient.h"
#include "Client.h"
#define SMTP_PORT 25
#define ZERO_IP IPAddress(0, 0, 0, 0)
#define TIMEOUT 10000
#define CRLF "\r\n"

SmtpClient::SmtpClient(Client* client, char *server) : _client(client), _server(server), _serverIP(ZERO_IP), _port(SMTP_PORT) {
}

SmtpClient::SmtpClient(Client* client, char *server, uint16_t port) : _client(client), _server(server), _serverIP(ZERO_IP), _port(port) {
}

SmtpClient::SmtpClient(Client* client, char *server, char *username, char *password)
  : _client(client), _server(server), _serverIP(ZERO_IP), _port(SMTP_PORT), _username(username), _password(password) {
}

SmtpClient::SmtpClient(Client* client, char *server, uint16_t port, char *username, char *password)
  : _client(client), _server(server), _serverIP(ZERO_IP), _port(port), _username(username), _password(password) {
}

SmtpClient::SmtpClient(Client* client, IPAddress serverIP) : _client(client), _serverIP(serverIP), _server(""), _port(SMTP_PORT) {
}

SmtpClient::SmtpClient(Client* client, IPAddress serverIP, uint16_t port) : _client(client), _serverIP(serverIP), _server(""), _port(port) {
}

SmtpClient::SmtpClient(Client* client, IPAddress serverIP, char *username, char *password)
  : _client(client), _serverIP(serverIP), _server(""), _port(SMTP_PORT), _username(username), _password(password) {
}

SmtpClient::SmtpClient(Client* client, IPAddress serverIP, uint16_t port, char *username, char *password)
  : _client(client), _serverIP(serverIP), _server(""), _port(port), _username(username), _password(password) {
}


int SmtpClient::send(Mail mail) {
  int result = connect();
  if (!result) {
    _client->stop();
    return 0;
  }
  result = _send(mail);
  _client->stop();
  return result;
}

int SmtpClient::_send(Mail mail) {
  if (readStatus() != 220) {
    return 0;
  }
  if (helo() != 250) {
    return 0;
  }
  if (mailFrom(mail.from) != 250) {
    return 0;
  }
  // TODO CC, BCC
  if (rcptTo(mail.to) != 250) {
    return 0;
  }
  if (data() != 354) {
    return 0;
  }
  headers(mail);
  body(mail.body);
  if (finishBody() != 250) {
    return 0;
  }
  return 1;
}

int SmtpClient::connect() {
  if (strlen(_server) > 0) {
    return _client->connect(_server, _port);
  } else {
    return _client->connect(_serverIP, _port);
  }
}

int SmtpClient::helo() {
  _client->print("EHLO");
  _client->print(CRLF);
  int status = readStatus();
  if (status == 250) {
    return status;
  }
  // IF server doesn't understand EHLO, try HELO
  _client->print("HELO");
  _client->print(CRLF);
  return readStatus();
}

int SmtpClient::mailFrom(char *from) {
  _client->print("MAIL FROM:");
  _client->print(from);
  _client->print(CRLF);
  return readStatus();
}

int SmtpClient::rcptTo(char *to) {
  _client->print("RCPT TO:");
  _client->print(to);
  _client->print(CRLF);
  return readStatus();
}

int SmtpClient::data() {
  _client->print("DATA ");
  _client->print(CRLF);
  return readStatus();
}

void SmtpClient::headers(Mail mail) {
  header("From:", mail.from);
  if (mail.replyTo) {
    header("Reply-To:", mail.replyTo);
  }

  header("To:", mail.to);

  header("Subject:", mail.subject);
}

void SmtpClient::header(char* header, char* value) {
  _client->print(header);
  _client->print(value);
  _client->print(CRLF);
}

void SmtpClient::body(char *body) {
  // TODO Need to escape cr lf .
  int cr = 0;
  int lf = 0;

  while(*body != '\0') {
    if (cr && lf && *body == '.') {
      // Add a second period to escapt the newline/period
      _client->print('.');

    }
    if (cr && *body == '\n') {
      lf = 1;
    } else if (*body == '\r') {
      cr = 1;
      lf = 0;
    } else {
      cr = lf = 0;
    }
    _client->print((char) *body++);
    // TODO use a char buffer as each byte is being sent in its own
    // packet
  }
}

int SmtpClient::finishBody() {
  _client->print(CRLF);
  _client->print('.');
  _client->print(CRLF);
  return readStatus();
}

int SmtpClient::readStatus() {
  char line[4];
  int result;
  while(true) {
    result = readLine(line, 4);
    if (result >= 4 && (line[3] == '-')) {
      // Multiline result
      continue;
    }
    break;
  }

  if (result < 3) {
    return 0;
  }

  char st[4];
  strncpy(st, line, 3);
  st[3] = '\0';
  return atoi(st);
}

int SmtpClient::readLine(char *line, int maxLen) {
  long start = millis();
  int count = 0;
  int cr = 0;
  while (true) {
    long now = millis();
    if (now < start || now - start > TIMEOUT) {
      return 0;
    }
    int c = _client->read();
    if (c != -1) {
      if (count < maxLen) {
        line[count++] = c;
      }
      if (cr && c == '\n') {
        break;
      }
      if (c == '\r') {
        cr = 1;
        continue;
      } else {
        cr = 0;
      }
    }
  }
  if (count == maxLen - 1) {
    line[count - 1] = '\0';
  } else {
    line[count] = '\0';
  }
  return count;
}
