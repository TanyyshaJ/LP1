import socket
import threading

class EchoServer:
    def __init__(self, host='localhost', port=12345):
        self.host = host
        self.port = port
        self.server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.server_socket.bind((self.host, self.port))
        self.server_socket.listen(5)
        print(f"Server listening on {host}:{port}")

    def handle_client(self, client_socket, address):
        """Handle individual client connection"""
        print(f"New connection from {address}")
        try:
            while True:
                # Receive data from client
                data = client_socket.recv(1024).decode('utf-8')
                if not data:
                    break
                
                print(f"Received from {address}: {data}")
                
                # Echo back to client
                response = f"Server received: {data}"
                client_socket.send(response.encode('utf-8'))
        except:
            print(f"Connection with {address} closed unexpectedly")
        finally:
            client_socket.close()
            print(f"Connection with {address} closed")

    def start(self):
        """Start the server and accept connections"""
        try:
            while True:
                # Accept new connection
                client_socket, address = self.server_socket.accept()
                
                # Create new thread to handle client
                client_thread = threading.Thread(
                    target=self.handle_client,
                    args=(client_socket, address)
                )
                client_thread.start()
        except KeyboardInterrupt:
            print("\nShutting down server...")
        finally:
            self.server_socket.close()

# Run the server
if __name__ == "__main__":
    server = EchoServer()
    server.start()