import smtplib
import ssl
from sys import argv
from email.message import EmailMessage

port = 465; #Outgoing SMTP server port number over SSL (specified by gmail)
smtp_server = "smtp.gmail.com";

def connect(sender, password) :
	#Load default SSL context before handshake for better security
	context = ssl.create_default_context();
	#Attempts handshake
	server = smtplib.SMTP_SSL(smtp_server, port, context=context);

	try :
		server.login(sender, password);
		return server;
	except smtplib.SMTPAuthenticationError :
		server.quit();
		print("Unable to authenticate");



def send_receipt(sender, password, receiver, subject, path_to_receipt):
  receipt = open(path_to_receipt)
  message = EmailMessage()
  message.set_content(receipt.read())
  receipt.close()

  message['Subject'] = subject
  message['From'] = sender
  message['To'] = receiver

  server = connect(sender, password);

  server.send_message(message)
  server.quit()



if (len(argv) == 3) :
	connect(argv[1], argv[2]);
elif (len(argv) == 6):
	send_receipt(argv[1], argv[2], argv[3], argv[4], argv[5]);
