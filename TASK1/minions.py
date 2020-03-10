import socket
import array
import pickle

TYPE = 'minion'
HOST = 'localhost'
PORT = 1234
sum = 0
arr = []


client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect((HOST,PORT))

client.send(pickle.dumps(TYPE))

def recieve_arr():
        global arr
        recv_arr=client.recv(4096)
        arr=pickle.loads(recv_arr)
        print(arr)

recieve_arr()

for num in arr:
    sum=sum+num

client.send(pickle.dumps(sum))
