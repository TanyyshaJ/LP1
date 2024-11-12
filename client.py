import socket

def run_client():
    # Create client socket
    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    
    # Connect to server
    client.connect(('localhost', 12345))
    
    try:
        while True:
            # Get message from user
            message = input("Enter message (or 'quit' to exit): ")
            if message.lower() == 'quit':
                break
            
            # Send message to server
            client.send(message.encode('utf-8'))
            
            # Receive response
            response = client.recv(1024).decode('utf-8')
            print(f"Server response: {response}")
    finally:
        client.close()

if __name__ == "__main__":
    run_client()