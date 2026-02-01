import socket
import math

def solve_quadratic(a, b, c):
    """Solve quadratic equation ax^2 + bx + c = 0"""
    if a == 0:
        if b == 0:
            if c == 0:
                return "Infinite solutions"
            else:
                return "No solution"
        else:
            x = -c / b
            return f"Linear equation: x = {x}"
    
    # Calculate discriminant
    delta = b * b - 4 * a * c
    
    if delta < 0:
        return "No real solutions"
    elif delta == 0:
        x = -b / (2 * a)
        return f"One solution: x = {x}"
    else:
        x1 = (-b + math.sqrt(delta)) / (2 * a)
        x2 = (-b - math.sqrt(delta)) / (2 * a)
        return f"Two solutions: x1 = {x1}, x2 = {x2}"

def main():
    # Create TCP socket
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    
    # Bind to localhost and port
    HOST = '127.0.0.1'
    PORT = 5000
    server_socket.bind((HOST, PORT))
    
    # Listen for connections
    server_socket.listen(5)
    print(f"TCP Server listening on {HOST}:{PORT}")
    
    while True:
        # Accept connection
        client_socket, address = server_socket.accept()
        print(f"Connection from {address}")
        
        try:
            # Receive data from client
            data = client_socket.recv(1024).decode()
            print(f"Received: {data}")
            
            # Parse the coefficients
            a, b, c = map(float, data.split())
            print(f"Solving equation: {a}x^2 + {b}x + {c} = 0")
            
            # Solve the equation
            result = solve_quadratic(a, b, c)
            print(f"Result: {result}")
            
            # Send result back to client
            client_socket.send(result.encode())
            
        except Exception as e:
            error_msg = f"Error: {str(e)}"
            print(error_msg)
            client_socket.send(error_msg.encode())
        
        finally:
            client_socket.close()
            print("Connection closed\n")

if __name__ == "__main__":
    main()
