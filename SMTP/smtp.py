from socket import *

# Mail content
subject = "I love computer networks!"
contenttype = "text/html"
content = "<img src=\"http://www.people.com.cn/mediafile/200512/09/F2005120913054600000.jpg\"><h1>I love computer networks!</h1>"

# Choose a mail server (e.g. Google mail server) and call it mailserver 
mailserver = 'smtp.126.com'

# Sender and reciever
fromaddress = "chungchenghao@126.com"
toaddress = "1041474530@qq.com"

# Auth information (Encode with base64)
username = "Y2h1bmdjaGVuZ2hhb0AxMjYuY29t"
password = "MzI1NjAwdmFs"

# Create socket called clientSocket and establish a TCP connection with mailserver 
clientSocket = socket(AF_INET,SOCK_STREAM)
clientSocket.connect((mailserver, 25))

recv = clientSocket.recv(1024)
print recv 
if recv[:3] != '220': 
	print '220 reply not received from server.'

# Send EHLO command and print server response. 
heloCommand = 'EHLO Alice\r\n' 
clientSocket.send(heloCommand) 
recv1 = clientSocket.recv(1024) 
print recv1 
if recv1[:3] != '250':     
	print '250 reply not received from server.' 

# Auth
clientSocket.sendall('AUTH LOGIN\r\n')
recv = clientSocket.recv(1024)
print recv
if (recv[:3] != '334'):
	print '334 reply not received from server'
clientSocket.sendall(username + '\r\n')
recv = clientSocket.recv(1024)
print recv
if (recv[:3] != '334'):
	print '334 reply not received from server'
clientSocket.sendall(password + '\r\n')
recv = clientSocket.recv(1024)
print recv
if (recv[:3] != '235'):
	print '235 reply not received from server'

# Send MAIL FROM command and print server response. 
clientSocket.sendall('MAIL FROM: <' + fromaddress + '>\r\n')
recv = clientSocket.recv(1024)
print recv
if (recv[:3] != '250'):
	print '250 reply not received from server'

# Send RCPT TO command and print server response.  
clientSocket.sendall('RCPT TO: <' + toaddress + '>\r\n')
recv = clientSocket.recv(1024)
print recv
if (recv[:3] != '250'):
	print '250 reply not received from server'

# Send DATA command and print server response. 
clientSocket.send('DATA\r\n')
recv = clientSocket.recv(1024)
print recv
if (recv[:3] != '354'):
	print '354 reply not received from server'

# Send message data. 
msg = 'from:' + fromaddress + '\r\n'
msg += 'subject:' + subject + '\r\n'
msg += 'Content-Type:' + contenttype + '\t\n'
msg += '\r\n' + content
clientSocket.sendall(msg)

# Message ends with a single period. 
clientSocket.sendall('\r\n.\r\n')
recv = clientSocket.recv(1024)
print recv
if (recv[:3] != '250'):
	print '250 reply not received from server'

# Send QUIT command and get server response. 
clientSocket.sendall('QUIT\r\n')

# Close connection
clientSocket.close()