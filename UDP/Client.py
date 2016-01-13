from socket import *
import time
import datetime
# Host
host = ('127.0.0.1', 12000)
# Statistic data
max_rtt = 0.0;
min_rtt = 1.0;
sum_rtt = 0.0;
lost_count = 0.0;
# Create a UDP socket  
# Notice the use of SOCK_DGRAM for UDP packets 
clientSocket = socket(AF_INET, SOCK_DGRAM)
# Assign IP and port for socket
clientSocket.bind(('', 10002))
# Set timeout
clientSocket.settimeout(1)
for x in xrange(1,11):
	# Get start timestamp
	start = time.time()
	# Make Ping message
	message = 'Ping ' + str(x) + ' ' + datetime.datetime.fromtimestamp(start).strftime('%H:%M:%S')
	# Send message
	clientSocket.sendto(message, host)
	try:
		message, address = clientSocket.recvfrom(1024)
		# Get end timestamp
		end = time.time()
		# Caculate RTT
		rtt = end - start
		# Count statis
		max_rtt = max(max_rtt, rtt)
		min_rtt = min(min_rtt, rtt)
		sum_rtt += rtt
		print message + " RTT =", rtt
	except timeout:
		# Timeout
		lost_count += 1.0
		print 'Request timed out'
# Report
print "maximum RTT = " + str(max_rtt) + ", minimum RTT = " + str(min_rtt) + ", average RTT = " + str(sum_rtt/(10-lost_count))
print "Sent = 10, Reciced = " + str(10-lost_count) + ", Lost = " + str(lost_count) + ", Lost rate = " + str(lost_count*10) + "%"