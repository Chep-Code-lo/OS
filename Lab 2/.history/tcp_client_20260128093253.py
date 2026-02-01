import socket

def main():
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    HOST = '127.0.0.1'
    PORT = 5000
    
    try:
        client_socket.connect((HOST, PORT))
        print(f"Connected to server {HOST}:{PORT}")
        print("\nSolve quadratic equation: ax^2 + bx + c = 0")
        a = input("Enter a: ")
        b = input("Enter b: ")
        c = input("Enter c: ")
        
        message = f"{a} {b} {c}"
        client_socket.send(message.encode())
        print(f"Sent: {message}")
        
        result = client_socket.recv(1024).decode()
        print(f"\nResult from server:")
        print(result)
        
    except Exception as e:
        print(f"Error: {e}")
    
    finally:
        client_socket.close()
        print("\nConnection closed")

if __name__ == "__main__":
    main()
