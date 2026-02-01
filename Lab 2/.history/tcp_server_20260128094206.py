import socket

def main():
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    HOST = '127.0.0.1'
    PORT = 5000
    server_socket.bind((HOST, PORT))

    server_socket.listen(5)
    print(f"TCP Server listening on {HOST}:{PORT}")
    print("Waiting for client connection...")
    
    while True:
        client_socket, address = server_socket.accept()
        print(f"\n[+] Connection from {address}")
        
        try:
            while True:
                # Receive message from client
                data = client_socket.recv(1024).decode()
                
                # Check if client disconnected or wants to quit
                if not data or data.strip().lower() == 'quit':
                    print(f"[-] Client {address} disconnected")
                    break
                
                print(f"Client: {data}")
                
                # Convert to uppercase and send back
                response = data.upper()
                client_socket.send(response.encode())
                print(f"Server: {response}")
            
        except Exception as e:
            print(f"[!] Connection error: {str(e)}")
        
        finally:
            client_socket.close()
            print(f"Connection with {address} closed\n")
            print("Waiting for new client connection...")

if __name__ == "__main__":
    main()
