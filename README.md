SMTPClient
==========

An Arduino library to send emails to an SMTP server. Supports sending emails from an Arduino to an unauthenticated
SMTP server. This library supports sending email through Ethernet, however it should also work with WiFi, but this
has not been tested.

Usage
-----

`Client.h`, `Mail.h` and `SMTPClient.h` need to be imported.

    #import <Client.h>
    #import <Mail.h>
    #import <SMTPClient.h>
    
An instance of `SMTPClient` needs to be created with the server hostname or IP and the port where the SMTP server is
located as well as the `Client` instance that will be used to make the connection. If the port is omitted, it is 
defaulted to port 25. In the following example, the client is an `EthernetClient`, but a WiFi or other client could 
be specified.

    byte ip[] = { 192, 168, 0, 125 };
    EthernetClient ethClient;
    SmtpClient client(&ethClient, ip);

To send an email, an instance of the `Mail` class needs to be creaed and methods called to populate the object.
At a minimum, at least one recipient and a from address are required. A maximum of 16 destination addresses
are supported, with them being any combination of To, Cc or Bcc.

    Mail mail;
    mail.from("Some Sender <sender@example.com>");
    mail.replyTo("noreply@example.com");
    mail.to("Someone <someone@example.com>");
    mail.to("Someone Else <someoneelse@example.com>");
    mail.cc("Another <another@example.com>");
    mail.bcc("Secret <secret@example.com>");
    mail.subject("Hello there");
    mail.body("I can send email from an Arduino!");
    client.send(&mail);
    
