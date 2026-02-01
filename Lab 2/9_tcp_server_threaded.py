import socket
import threading
import datetime
import sys

DEFAULT_PORT = 8080
HOST = '127.0.0.1'

client_counter = 0
counter_lock = threading.Lock()


def handle_client(client_socket, client_address, client_id):
    try:
        print(f"[Thread {client_id}] Connected to client: {client_address}")
        
        current_time = datetime.datetime.now()
        date_string = current_time.strftime("%Y-%m-%d %H:%M:%S")
        message = f"Server Date/Time: {date_string}\nClient ID: {client_id}\n"
        
        client_socket.send(message.encode('utf-8'))
        print(f"[Thread {client_id}] Sent date/time to {client_address}")
        
    except Exception as e:
        print(f"[Thread {client_id}] Error handling client: {e}")
    finally:
        client_socket.close()
        print(f"[Thread {client_id}] Connection closed with {client_address}")


def main():
    global client_counter
    
    port = DEFAULT_PORT
    if len(sys.argv) > 1:
        try:
            port = int(sys.argv[1])
        except ValueError:
            print(f"Invalid port number. Using default port {DEFAULT_PORT}")
            port = DEFAULT_PORT

    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    
    try:
        server_socket.bind((HOST, port))
        server_socket.listen(5)
        
        print(f"========================================")
        print(f"Multithreaded TCP Date Server")
        print(f"========================================")
        print(f"Server listening on {HOST}:{port}")
        print(f"Press Ctrl+C to stop the server")
        print(f"========================================\n")
        
        while True:
            client_socket, client_address = server_socket.accept()
            
            with counter_lock:
                client_counter += 1
                current_client_id = client_counter
            
            client_thread = threading.Thread(
                target=handle_client,
                args=(client_socket, client_address, current_client_id),
                daemon=True
            )
            
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
