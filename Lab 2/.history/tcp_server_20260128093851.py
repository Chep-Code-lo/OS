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
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    HOST = '127.0.0.1'
    PORT = 5000
    server_socket.bind((HOST, PORT))

    server_socket.listen(5)
    print(f"TCP Server listening on {HOST}:{PORT}")
    
    while True:
        client_socket, address = server_socket.accept()
        print(f"Connection from {address}")
        
        try:
            while True:
                data = client_socket.recv(1024).decode()
                
                if not data or data.strip().lower() == 'quit':
                    print(f"Client {address} disconnected")
                    break
                
                print(f"Received from {address}: {data}")
                
                try:
                    a, b, c = map(float, data.split())
                    print(f"Solving equation: {a}x^2 + {b}x + {c} = 0")

                    result = solve_quadratic(a, b, c)
                    print(f"Result: {result}")
                    client_socket.send(result.encode())
                    
                except ValueError:
                    error_msg = "Error: Please enter 3 numbers separated by spaces"
                    print(error_msg)
                    client_socket.send(error_msg.encode())
            
        except Exception as e:
            print(f"Connection error: {str(e)}")
        
        finally:
            client_socket.close()
            print(f"Connection with {address} closed\n")

if __name__ == "__main__":
    main()
