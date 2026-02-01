import socket

def main():
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    HOST = '127.0.0.1'
    PORT = 5000
    
    try:
        client_socket.connect((HOST, PORT))
        print(f"Connected to server {HOST}:{PORT}")
        print("Solve quadratic equation: ax^2 + bx + c = 0")
        print("Type 'quit' to exit\n")
        
        while True:
            print("-" * 50)
            a = input("Enter a (or 'quit' to exit): ")
            
            if a.strip().lower() == 'quit':
                client_socket.send('quit'.encode())
                print("Disconnecting...")
                break
            
            b = input("Enter b: ")
            c = input("Enter c: ")
            
            message = f"{a} {b} {c}"
            client_socket.send(message.encode())
            print(f"Sent: {message}")
            
            result = client_socket.recv(1024).decode()
            print(f"\nResult from server:")
            print(result)
            print()
        
    except Exception as e:
        print(f"Error: {e}")
    
    finally:
        client_socket.close()
        print("\nConnection closed")

if __name__ == "__main__":
    main()
