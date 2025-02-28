from socket  import *

HOST = '194.136.15.41'
PORT = 50007

class Server:
  def run(self):
    s = socket(AF_INET, SOCK_STREAM) 
    s.bind((HOST, PORT))  
    s.listen(1)       
    (conn, addr) = s.accept()  # returns new socket and addr. client 
    print("Conectou!")
    while True:                # forever
      data = conn.recv(1024)   # receive data from client
      if not data: break       # stop if client stopped
      print(data)              # mostrar o que foi recebido
      if data == "exit": break # finalizar a conexão caso recebe uma palavra 'exit'
      conn.send(data+b"*")     # return sent data plus an "*"
    conn.close()               # close the connection

s = Server()
s.run()