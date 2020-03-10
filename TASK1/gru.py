import socket
import pickle
import array
import threading
import numpy
import time

HOST = 'localhost'
PORT = 1234
minions = 0
arr = array.array('i',[])
recv_arr = array.array('i',[])
user = True
connection_minions = []
address_minions = []
connection_user = []
address_user = []

gru = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
gru.bind((HOST,PORT))
gru.listen(5)

def new_connections():

    global minions
    global arr

    while True:

        client, address = gru.accept()
        gru.setblocking(1)
        print(f"Connection from {address} has been established!")

        if pickle.loads(client.recv(1024)) == 'user':
            connection_user.append(client)
            address_user.append(address)
            print("User connected.")
            recv_arr = client.recv(1024)
            arr = pickle.loads(recv_arr)
            print("Array recieved : ", arr)
            break

        else:
            connection_minions.append(client)
            address_minions.append(address)
            minions=minions+1
            print("Minion connected.")
            print("Total minions : ",minions)

def send_arr(conn,i):
    global arr_split
    conn.send(pickle.dumps(arr_split[i]))

def recieve_arr(conn):
    global recv_arr
    recv_arr.append(pickle.loads(conn.recv(1024)))

def send_thread(connection_minions):
    i = 0
    threads = []
    for conn in connection_minions:
        t = threading.Thread(target = send_arr, args = (conn,i,))
        threads.append(t)
        i=i+1
        t.start()

    for n in range(i):
        threads[n].join()

def recieve_thread(connection_minions):
    i=0
    threads = []
    for conn in connection_minions:
        t = threading.Thread(target = recieve_arr, args = (conn,))
        threads.append(t)
        i=i+1
        t.start()

    for n in range(i):
        threads[n].join()

new_connections()

arr_split = numpy.array_split(arr,minions)

send_thread(connection_minions)
recieve_thread(connection_minions)

print(recv_arr)

sum=0
for num in recv_arr:
    sum=sum+num

for conn in connection_user:
    conn.send(pickle.dumps(sum))
