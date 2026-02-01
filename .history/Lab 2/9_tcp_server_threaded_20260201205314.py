"""
Problem 4: Multithreaded TCP Date Server
A TCP server that handles each client request in a separate thread.
Usage: python 9_tcp_server_threaded.py [port]
Example: python 9_tcp_server_threaded.py 8080
"""

import socket
import threading
import datetime
import sys

# Server configuration
DEFAULT_PORT = 8080
HOST = '127.0.0.1'

# Thread counter for logging
client_counter = 0
counter_lock = threading.Lock()


def handle_client(client_socket, client_address, client_id):
    """
    Thread function to handle each client connection
    Sends current date and time to the client
    """
    try:
        print(f"[Thread {client_id}] Connected to client: {client_address}")
        
        # Get current date and time
        current_time = datetime.datetime.now()
        date_string = current_time.strftime("%Y-%m-%d %H:%M:%S")
        
        # Prepare response message
        message = f"Server Date/Time: {date_string}\nClient ID: {client_id}\n"
        
        # Send response to client
        client_socket.send(message.encode('utf-8'))
        
        print(f"[Thread {client_id}] Sent date/time to {client_address}")
        
    except Exception as e:
        print(f"[Thread {client_id}] Error handling client: {e}")
    finally:
        # Close client connection
        client_socket.close()
        print(f"[Thread {client_id}] Connection closed with {client_address}")


def main():
    global client_counter
    
    # Get port from command line or use default
    port = DEFAULT_PORT
    if len(sys.argv) > 1:
        try:
            port = int(sys.argv[1])
        except ValueError:
            print(f"Invalid port number. Using default port {DEFAULT_PORT}")
            port = DEFAULT_PORT

    # Create TCP server socket
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    
    try:
        # Bind socket to address and port
        server_socket.bind((HOST, port))
        
        # Listen for incoming connections (backlog of 5)
        server_socket.listen(5)
        
        print(f"========================================")
        print(f"Multithreaded TCP Date Server")
        print(f"========================================")
        print(f"Server listening on {HOST}:{port}")
        print(f"Press Ctrl+C to stop the server")
        print(f"========================================\n")
        
        # Main server loop
        while True:
            # Accept incoming client connection
            client_socket, client_address = server_socket.accept()
            
            # Increment client counter (thread-safe)
            with counter_lock:
                client_counter += 1
                current_client_id = client_counter
            
            # Create a new thread to handle this client
            client_thread = threading.Thread(
                target=handle_client,
                args=(client_socket, client_address, current_client_id),
                daemon=True  # Daemon thread will exit when main program exits
            )
            
            # Start the thread
            client_thread.start()
            
            print(f"[Main] Created thread {current_client_id} for client {client_address}")
            print(f"[Main] Active threads: {threading.active_count()}\n")
            
    except KeyboardInterrupt:
        print("\n\nServer shutting down...")
    except Exception as e:
        print(f"Server error: {e}")
    finally:
        server_socket.close()
        print("Server socket closed")


if __name__ == "__main__":
    main()
