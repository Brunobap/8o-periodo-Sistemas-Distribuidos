from socket  import * #-

HOST = '194.138.3.4'
PORT = 50007

class Client:
  def run(self):
    s = socket(AF_INET, SOCK_STREAM)
    s.connect((HOST, PORT)) # connect to server (block until accepted)
    print("Conectou!")
    while True:
      entrada = input()
      s.send(str.encode(entrada)) # mandar a string convertida em bytes
      data = s.recv(1024)     # receive the response
      print("Resposta: "+data.decode()+"\n")             # print what you received
      if entrada == 'exit': break
    s.close()               # close the connection

c = Client()
c.run()
