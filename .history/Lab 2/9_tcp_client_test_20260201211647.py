import socket
import sys

DEFAULT_PORT = 8080
HOST = '127.0.0.1'


def main():
    port = DEFAULT_PORT
    if len(sys.argv) > 1:
        try:
            port = int(sys.argv[1])
        except ValueError:
            print(f"Invalid port number. Using default port {DEFAULT_PORT}")
            port = DEFAULT_PORT

    try:
        client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        
        print(f"Connecting to server at {HOST}:{port}...")
        client_socket.connect((HOST, port))
        print("Connected!\n")
        
        response = client_socket.recv(1024).decode('utf-8')
        
        print("Response from server:")
        print("=" * 40)
        print(response)
        print("=" * 40)
        
    except ConnectionRefusedError:
        print(f"Error: Could not connect to server at {HOST}:{port}")
        print("Make sure the server is running first.")
    except Exception as e:
        print(f"Error: {e}")
    finally:
        client_socket.close()
        print("\nConnection closed")


if __name__ == "__main__":
    main()
