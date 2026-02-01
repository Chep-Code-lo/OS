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
    # Create UDP socket
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    
    # Bind to localhost and port
    HOST = '127.0.0.1'
    PORT = 5001
    server_socket.bind((HOST, PORT))
    
    print(f"UDP Server listening on {HOST}:{PORT}")
    
    while True:
        try:
            # Receive data from client
            data, client_address = server_socket.recvfrom(1024)
            message = data.decode()
            print(f"\nReceived from {client_address}: {message}")
            
            # Parse the coefficients
            a, b, c = map(float, message.split())
            print(f"Solving equation: {a}x^2 + {b}x + {c} = 0")
            
            # Solve the equation
            result = solve_quadratic(a, b, c)
            print(f"Result: {result}")
            
            # Send result back to client
            server_socket.sendto(result.encode(), client_address)
            
        except Exception as e:
            error_msg = f"Error: {str(e)}"
            print(error_msg)
            server_socket.sendto(error_msg.encode(), client_address)

if __name__ == "__main__":
    main()
