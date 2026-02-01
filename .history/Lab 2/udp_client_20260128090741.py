import socket

def main():
    # Create UDP socket
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    
    # Server address
    HOST = '127.0.0.1'
    PORT = 5001
    
    try:
        print(f"UDP Client - Server address: {HOST}:{PORT}")
        
        # Get coefficients from user
        print("\nSolve quadratic equation: ax^2 + bx + c = 0")
        a = input("Enter a: ")
        b = input("Enter b: ")
        c = input("Enter c: ")
        
        # Send data to server
        message = f"{a} {b} {c}"
        client_socket.sendto(message.encode(), (HOST, PORT))
        print(f"Sent: {message}")
        
        # Receive result from server
        data, server_address = client_socket.recvfrom(1024)
        result = data.decode()
        print(f"\nResult from server:")
        print(result)
        
    except Exception as e:
        print(f"Error: {e}")
    
    finally:
        client_socket.close()
        print("\nSocket closed")

if __name__ == "__main__":
    main()
