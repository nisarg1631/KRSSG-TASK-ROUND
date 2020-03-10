import socket
import array
import pickle

TYPE = 'user'
HOST = 'localhost'
PORT = 1234

client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect((HOST,PORT))

client.send(pickle.dumps(TYPE))

n = int(input("Enter array size: "))
arr = array.array('i',[])

for i in range(0,n):
    arr.append(int(input()))

send_arr = pickle.dumps(arr)
client.send(send_arr)

print(pickle.loads(client.recv(4096)))
