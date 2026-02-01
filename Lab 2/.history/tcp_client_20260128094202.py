import socket

def main():
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    HOST = '127.0.0.1'
    PORT = 5000
    
    try:
        client_socket.connect((HOST, PORT))
        print(f"[+] Connected to server {HOST}:{PORT}")
        print("Type your message and server will reply in UPPERCASE")
        print("Type 'quit' to exit\n")
        
        while True:
            # Get message from user
            message = input("You: ")
            
            # Check if user wants to quit
            if message.strip().lower() == 'quit':
                client_socket.send('quit'.encode())
                print("Disconnecting...")
                break
            
            # Send message to server
            client_socket.send(message.encode())
            
            # Receive response from server
            response = client_socket.recv(1024).decode()
            print(f"Server: {response}\n")
        
    except Exception as e:
        print(f"[!] Error: {e}")
    
    finally:
        client_socket.close()
        print("\nConnection closed")

if __name__ == "__main__":
    main()
